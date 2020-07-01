/*
*   This file is part of Universal-Updater
*   Copyright (C) 2019-2020 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "cia.hpp"
#include "common.hpp"
#include "download.hpp"
#include "formatting.hpp"
#include "gui.hpp"
#include "init.hpp"
#include "lang.hpp"
#include "screenCommon.hpp"
#include "thread.hpp"

#include <cstdio>
#include <dirent.h>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <curl/curl.h>

#define  USER_AGENT   APP_TITLE "-" VERSION_STRING

static char* result_buf = NULL;
static size_t result_sz = 0;
static size_t result_written = 0;
std::vector<std::string> _topText;
std::string jsonName;

using json = nlohmann::json;

int progressBarType = 0;
char progressBarMsg[128] = "";
bool showProgressBar = false;
extern bool changesMade;
extern std::unique_ptr<Config> config;

#define TIME_IN_US 1
#define TIMETYPE curl_off_t
#define TIMEOPT CURLINFO_TOTAL_TIME_T
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL	3000000

curl_off_t downloadTotal = 1; //Dont initialize with 0 to avoid division by zero later
curl_off_t downloadNow = 0;

static FILE *downfile = NULL;
static size_t file_buffer_pos = 0;
static size_t file_toCommit_size = 0;
static char* g_buffers[2] = { NULL };
static u8 g_index = 0;
static Thread fsCommitThread;
static LightEvent readyToCommit;
static LightEvent waitCommit;
static bool killThread = false;
static bool writeError = false;
#define FILE_ALLOC_SIZE 0x60000

// That are our install Progressbar variables.
extern u64 installSize, installOffset;

static int curlProgress(CURL *hnd,
					curl_off_t dltotal, curl_off_t dlnow,
					curl_off_t ultotal, curl_off_t ulnow)
{
	downloadTotal = dltotal;
	downloadNow = dlnow;
	return 0;
}

bool filecommit() {
	if (!downfile) return false;
	fseek(downfile, 0, SEEK_END);
	u32 byteswritten = fwrite(g_buffers[!g_index], 1, file_toCommit_size, downfile);
	if (byteswritten != file_toCommit_size) return false;
	file_toCommit_size = 0;
	return true;
}

static void commitToFileThreadFunc(void* args) {
	LightEvent_Signal(&waitCommit);
	while (true) {
		LightEvent_Wait(&readyToCommit);
		LightEvent_Clear(&readyToCommit);
		if (killThread) threadExit(0);
		writeError = !filecommit();
		LightEvent_Signal(&waitCommit);
	}
}

static size_t file_handle_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
	(void)userdata;
	const size_t bsz = size * nmemb;
	size_t tofill = 0;
	if (writeError) return 0;
	if (!g_buffers[g_index]) {

		LightEvent_Init(&waitCommit, RESET_STICKY);
		LightEvent_Init(&readyToCommit, RESET_STICKY);

		s32 prio = 0;
		svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
		fsCommitThread = threadCreate(commitToFileThreadFunc, NULL, 0x1000, prio - 1, -2, true);

		g_buffers[0] = (char*)memalign(0x1000, FILE_ALLOC_SIZE);
		g_buffers[1] = (char*)memalign(0x1000, FILE_ALLOC_SIZE);

		if (!fsCommitThread || !g_buffers[0] || !g_buffers[1]) return 0;
	}
	if (file_buffer_pos + bsz >= FILE_ALLOC_SIZE) {
		tofill = FILE_ALLOC_SIZE - file_buffer_pos;
		memcpy(g_buffers[g_index] + file_buffer_pos, ptr, tofill);
		
		LightEvent_Wait(&waitCommit);
		LightEvent_Clear(&waitCommit);
		file_toCommit_size = file_buffer_pos + tofill;
		file_buffer_pos = 0;
		svcFlushProcessDataCache(CUR_PROCESS_HANDLE, g_buffers[g_index], file_toCommit_size);
		g_index = !g_index;
		LightEvent_Signal(&readyToCommit);
	}
	memcpy(g_buffers[g_index] + file_buffer_pos, ptr + tofill, bsz - tofill);
	file_buffer_pos += bsz - tofill;
	return bsz;
}

Result downloadToFile(std::string url, std::string path) {
	Result retcode = 0;
	downloadTotal = 1;
	downloadNow = 0;
	int res;
	CURL *hnd;
	CURLcode cres;

	printf("Downloading from:\n%s\nto:\n%s\n", url.c_str(), path.c_str());
	const char* filepath = path.c_str();

	void *socubuf = memalign(0x1000, 0x100000);
	if (!socubuf) {
		retcode = -1;
		goto exit;
	}
	
	res = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(res)) {
		retcode = res;
		goto exit;
	}

	makeDirs(strdup(filepath));
	downfile = fopen(filepath, "wb");
	if (!downfile) {
		retcode = -2;
		goto exit;
	}

	hnd = curl_easy_init();
	curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, FILE_ALLOC_SIZE);
	curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
	curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 0L); 
	curl_easy_setopt(hnd, CURLOPT_USERAGENT, USER_AGENT);
	curl_easy_setopt(hnd, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(hnd, CURLOPT_FAILONERROR, 1L);
	curl_easy_setopt(hnd, CURLOPT_ACCEPT_ENCODING, "gzip");
	curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
	curl_easy_setopt(hnd, CURLOPT_XFERINFOFUNCTION, curlProgress);
	curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, file_handle_data);
	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(hnd, CURLOPT_STDERR, stdout);

	cres = curl_easy_perform(hnd);
	curl_easy_cleanup(hnd);
	
	if (cres != CURLE_OK) {
		retcode = -cres;
		goto exit;
	}

	LightEvent_Wait(&waitCommit);
	LightEvent_Clear(&waitCommit);

	file_toCommit_size = file_buffer_pos;
	svcFlushProcessDataCache(CUR_PROCESS_HANDLE, g_buffers[g_index], file_toCommit_size);
	g_index = !g_index;
	if (!filecommit()) {
		retcode = -3;
		goto exit;
	}
	fflush(downfile);
	
exit:
	if (fsCommitThread) {
		killThread = true;
		LightEvent_Signal(&readyToCommit);
		threadJoin(fsCommitThread, U64_MAX);
		killThread = false;
		fsCommitThread = NULL;
	}

	socExit();
	
	if (socubuf) {
		free(socubuf);
	}

	if (downfile) {
		fclose(downfile);
		downfile = NULL;
	}

	if (g_buffers[0]) {
		free(g_buffers[0]);
		g_buffers[0] = NULL;
	}

	if (g_buffers[1]) {
		free(g_buffers[1]);
		g_buffers[1] = NULL;
	}

	g_index = 0;
	file_buffer_pos = 0;
	file_toCommit_size = 0;
	writeError = false;
	
	return retcode;
}

// following function is from
// https://github.com/angelsl/libctrfgh/blob/master/curl_test/src/main.c
static size_t handle_data(char* ptr, size_t size, size_t nmemb, void* userdata) {
	(void) userdata;
	const size_t bsz = size*nmemb;

	if (result_sz == 0 || !result_buf) {
		result_sz = 0x1000;
		result_buf = (char*)malloc(result_sz);
	}

	bool need_realloc = false;
	while (result_written + bsz > result_sz) {
		result_sz <<= 1;
		need_realloc = true;
	}

	if (need_realloc) {
		char *new_buf = (char*)realloc(result_buf, result_sz);
		if (!new_buf) {
			return 0;
		}
		result_buf = new_buf;
	}

	if (!result_buf) {
		return 0;
	}

	memcpy(result_buf + result_written, ptr, bsz);
	result_written += bsz;
	return bsz;
}

static Result setupContext(CURL *hnd, const char * url) {
	downloadTotal = 1;
	downloadNow = 0;
	curl_easy_setopt(hnd, CURLOPT_XFERINFOFUNCTION, curlProgress);

	curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
	curl_easy_setopt(hnd, CURLOPT_URL, url);
	curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(hnd, CURLOPT_USERAGENT, USER_AGENT);
	curl_easy_setopt(hnd, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
	curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, handle_data);
	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(hnd, CURLOPT_STDERR, stdout);

	return 0;
}

Result downloadFromRelease(std::string url, std::string asset, std::string path, bool includePrereleases) {
	Result ret = 0;
	void *socubuf = memalign(0x1000, 0x100000);
	if (!socubuf) {
		return -1;
	}

	ret = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(ret)) {
		free(socubuf);
		return ret;
	}

	std::regex parseUrl("github\\.com\\/(.+)\\/(.+)");
	std::smatch result;
	regex_search(url, result, parseUrl);

	std::string repoOwner = result[1].str(), repoName = result[2].str();

	std::stringstream apiurlStream;
	apiurlStream << "https://api.github.com/repos/" << repoOwner << "/" << repoName << (includePrereleases ? "/releases" : "/releases/latest");
	std::string apiurl = apiurlStream.str();

	printf("Downloading latest release from repo:\n%s\nby:\n%s\n", repoName.c_str(), repoOwner.c_str());
	printf("Crafted API url:\n%s\n", apiurl.c_str());

	CURL *hnd = curl_easy_init();

	ret = setupContext(hnd, apiurl.c_str());
	if (ret != 0) {
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return ret;
	}

	CURLcode cres = curl_easy_perform(hnd);
	curl_easy_cleanup(hnd);
	char* newbuf = (char*)realloc(result_buf, result_written + 1);
	result_buf = newbuf;
	result_buf[result_written] = 0; //nullbyte to end it as a proper C style string

	if (cres != CURLE_OK) {
		printf("Error in:\ncurl\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return -1;
	}

	printf("Looking for asset with matching name:\n%s\n", asset.c_str());
	std::string assetUrl;
	json parsedAPI = json::parse(result_buf);
	if(includePrereleases)	parsedAPI = parsedAPI[0];
	if (parsedAPI["assets"].is_array()) {
		for (auto jsonAsset : parsedAPI["assets"]) {
			if (jsonAsset.is_object() && jsonAsset["name"].is_string() && jsonAsset["browser_download_url"].is_string()) {
				std::string assetName = jsonAsset["name"];
				if (Utils::matchPattern(asset, assetName)) {
					assetUrl = jsonAsset["browser_download_url"];
					break;
				}
			}
		}
	}
	socExit();
	free(result_buf);
	free(socubuf);
	result_buf = NULL;
	result_sz = 0;
	result_written = 0;

	if (assetUrl.empty())	ret = DL_ERROR_GIT;
	else	ret = downloadToFile(assetUrl, path);

	return ret;
}

/**
 * Check Wi-Fi status.
 * @return True if Wi-Fi is connected; false if not.
 */
bool checkWifiStatus(void) {
	u32 wifiStatus;
	bool res = false;

	if (R_SUCCEEDED(ACU_GetWifiStatus(&wifiStatus)) && wifiStatus) {
		res = true;
	}

	return res;
}

void downloadFailed(void) {
	Msg::DisplayMsg(Lang::get("DOWNLOAD_FAILED"));
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

void notImplemented(void) {
	Msg::DisplayMsg(Lang::get("NOT_IMPLEMENTED"));
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

void doneMsg(void) {
	Msg::DisplayMsg(Lang::get("DONE"));
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

void notConnectedMsg(void) {
	Msg::DisplayMsg(Lang::get("CONNECT_WIFI"));
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

ReleaseFetch getLatestRelease() {
	Result ret = 0;
	void *socubuf = memalign(0x1000, 0x100000);
	if (!socubuf) {
		return {""};
	}

	ret = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(ret)) {
		free(socubuf);
		return {""};
	}

	std::stringstream apiurlStream;
	apiurlStream << "https://api.github.com/repos/Universal-Team/LeafEdit/releases/latest";
	std::string apiurl = apiurlStream.str();

	CURL *hnd = curl_easy_init();
	ret = setupContext(hnd, apiurl.c_str());
	if (ret != 0) {
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return {""};
	}

	CURLcode cres = curl_easy_perform(hnd);
	curl_easy_cleanup(hnd);
	char* newbuf = (char*)realloc(result_buf, result_written + 1);
	result_buf = newbuf;
	result_buf[result_written] = 0; //nullbyte to end it as a proper C style string

	if (cres != CURLE_OK) {
		printf("Error in:\ncurl\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return {""};
	}

	ReleaseFetch RF;
	json parsedAPI = json::parse(result_buf);

	if (parsedAPI["name"].is_string()) {
		RF.ReleaseName = parsedAPI["name"];
	} else {
		RF.ReleaseName = "";
	}
	
	if (parsedAPI["tag_name"].is_string()) {
		RF.Version = parsedAPI["tag_name"];
	} else {
		RF.Version = "";
	}

	if (parsedAPI["body"].is_string()) {
		RF.Body = parsedAPI["body"];
	} else {
		RF.Body = "";
	}

	if (parsedAPI["published_at"].is_string()) {
		std::string Test = parsedAPI["published_at"];
		RF.Published = Test.substr(0, 10) + "  " + Test.substr(11, 8);
	} else {
		RF.Published = "";
	}


	socExit();
	free(result_buf);
	free(socubuf);
	result_buf = NULL;
	result_sz = 0;
	result_written = 0;

	return RF;
}

NightlyFetch getLatestCommit() {
	Result ret = 0;
	void *socubuf = memalign(0x1000, 0x100000);
	if (!socubuf) {
		return {""};
	}

	ret = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(ret)) {
		free(socubuf);
		return {""};
	}

	std::stringstream apiurlStream;
	apiurlStream << "https://api.github.com/repos/Universal-Team/LeafEdit/commits/master";
	std::string apiurl = apiurlStream.str();

	CURL *hnd = curl_easy_init();
	ret = setupContext(hnd, apiurl.c_str());
	if (ret != 0) {
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return {""};
	}

	CURLcode cres = curl_easy_perform(hnd);
	curl_easy_cleanup(hnd);
	char* newbuf = (char*)realloc(result_buf, result_written + 1);
	result_buf = newbuf;
	result_buf[result_written] = 0; //nullbyte to end it as a proper C style string

	if (cres != CURLE_OK) {
		printf("Error in:\ncurl\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return {""};
	}

	NightlyFetch NF;
	json parsedAPI = json::parse(result_buf);

	if (parsedAPI["commit"]["message"].is_string()) {
		NF.Message = parsedAPI["commit"]["message"];
	} else {
		NF.Message = "";
	}

	if (parsedAPI["committer"]["login"].is_string()) {
		NF.Committer = parsedAPI["committer"]["login"];
	} else {
		NF.Committer = "";
	}

	if (parsedAPI["author"]["login"].is_string()) {
		NF.Author = parsedAPI["author"]["login"];
	} else {
		NF.Author = "";
	}

	if (parsedAPI["sha"].is_string()) {
		std::string Temp = parsedAPI["sha"];
		NF.Target = Temp.substr(0,7);
	} else {
		NF.Target = "";
	}

	socExit();
	free(result_buf);
	free(socubuf);
	result_buf = NULL;
	result_sz = 0;
	result_written = 0;

	return NF;
}

extern bool is3dsx;
extern bool Is3dsxUpdated;
extern std::string path3dsx;

void setMessageText(const std::string &text) {
	std::string _topTextStr(text);
	std::vector<std::string> words;
	std::size_t pos;
	// std::replace( _topTextStr.begin(), _topTextStr.end(), '\n', ' ');
	_topTextStr.erase(std::remove(_topTextStr.begin(), _topTextStr.end(), '\r'), _topTextStr.end());
	while((pos = _topTextStr.find(' ')) != std::string::npos) {
		words.push_back(_topTextStr.substr(0, pos));
		_topTextStr = _topTextStr.substr(pos + 1);
	}

	if (_topTextStr.size())	words.push_back(_topTextStr);
	std::string temp;
	_topText.clear();

	for(auto word : words) {
		int width = Gui::GetStringWidth(0.7f, (temp + " " + word).c_str());

		if (word.find('\n') != -1u) {
			word.erase(std::remove(word.begin(), word.end(), '\n'), word.end());
			temp += " " + word;
			_topText.push_back(temp);
			temp = "";
		} else if (width > 350) {
			_topText.push_back(temp);
			temp = word;
		} else {
			temp += " " + word;
		}
	}

	if(temp.size())	_topText.push_back(temp);
}

void drawMessageText(int position) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, TRANSPARENT);
	Gui::ScreenDraw(Top);
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - Release Notes", 395);
	Gui::DrawString(0, 25, 0.7, BLACK, jsonName.c_str(), 395);
	for (int i = 0; i < (int)_topText.size() && i < (10); i++) {
		Gui::DrawString(0, ((i * 16) + 40), 0.7f, BLACK, _topText[i+position].c_str(), 395);
	}
	C3D_FrameEnd(0);
}

bool Download::showReleaseInfo(ReleaseFetch RF) {
	setMessageText(RF.Body);
	int textPosition = 0;
	bool redrawText = true;

	while(1) {
		if(redrawText) {
			drawMessageText(textPosition);
			redrawText = false;
		}

		gspWaitForVBlank();
		hidScanInput();
		const u32 hDown = hidKeysDown();
		const u32 hHeld = hidKeysHeld();

		if(hHeld & KEY_UP || hHeld & KEY_DOWN) {
			for(int i=0;i<10;i++)
				gspWaitForVBlank();
		}

		if(hDown & KEY_A) {
			return true;
		} else if(hDown & KEY_B || hDown & KEY_Y || hDown & KEY_TOUCH) {
			return false;
		} else if(hHeld & KEY_UP) {
			if(textPosition > 0) {
				textPosition--;
				redrawText = true;
			}
		} else if(hHeld & KEY_DOWN) {
			if(textPosition < (int)(_topText.size() - 10)) {
				textPosition++;
				redrawText = true;
			}
		}
	}
}

// Return latest Release & Nightly states.
ReleaseFetch Download::getLatestRelease2(void) {
	ReleaseFetch RF;
	RF = getLatestRelease();
	return RF;
}

NightlyFetch Download::getLatestNightly(void) {
	NightlyFetch NF;
	NF = getLatestCommit();
	return NF;
}

Result Download::updateApp(bool nightly, const std::string &version) {
	bool success = false;
	if(nightly) {
		if (is3dsx == false) {
			// Download CIA Nightly.
			snprintf(progressBarMsg, sizeof(progressBarMsg), (Lang::get("DOWNLOADING_LATEST_NIGHTLY") + " (CIA)").c_str());
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/LeafEdit/LeafEdit.cia?raw=true", "sdmc:/LeafEdit/LeafEdit.cia") != 0) {
				showProgressBar = false;
				Msg::DisplayWarnMsg(Lang::get("DOWNLOAD_FAILED"));
				return -1;
			}
			// Install and delete.
			snprintf(progressBarMsg, sizeof(progressBarMsg), (Lang::get("INSTALLING_CIA")).c_str());
			progressBarType = 1;
			if (version != "")	config->currentNightly(version);
			config->save(); // Needed to do that here.
			installCia("sdmc:/LeafEdit/LeafEdit.cia", true);
			showProgressBar = false;
			deleteFile("sdmc:/LeafEdit/LeafEdit.cia");
		} else if (is3dsx == true) {
			// Download 3DSX Nightly.
			snprintf(progressBarMsg, sizeof(progressBarMsg), (Lang::get("DOWNLOADING_LATEST_NIGHTLY") + " (3DSX)").c_str());
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/LeafEdit/LeafEdit.3dsx?raw=true", path3dsx + "LeafEdit.3dsx") != 0) {
				showProgressBar = false;
				Msg::DisplayWarnMsg(Lang::get("DOWNLOAD_FAILED"));
				return -1;
			}
			showProgressBar = false;
			if (version != "")	config->currentNightly(version);
			config->save(); // Needed to do that here.
			success = true;
		}
	} else {
		if (is3dsx == false) {
			// Download CIA Release.
			snprintf(progressBarMsg, sizeof(progressBarMsg), (Lang::get("DOWNLOADING_LATEST_RELEASE") + " (CIA)").c_str());
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadFromRelease("https://github.com/Universal-Team/LeafEdit", "LeafEdit.cia", "sdmc:/LeafEdit/LeafEdit.cia", false) != 0) {
				showProgressBar = false;
				Msg::DisplayWarnMsg(Lang::get("DOWNLOAD_FAILED"));
				return -1;
			}
			// Install and delete.
			snprintf(progressBarMsg, sizeof(progressBarMsg), (Lang::get("INSTALLING_CIA")).c_str());
			progressBarType = 1;
			if (version != "")	config->currentRelease(version);
			config->save(); // Needed to do that here.
			installCia("sdmc:/LeafEdit/LeafEdit.cia", true);
			showProgressBar = false;
			deleteFile("sdmc:/LeafEdit/LeafEdit.cia");
		} else if (is3dsx == true) {
			// Download 3DSX Release.
			snprintf(progressBarMsg, sizeof(progressBarMsg), (Lang::get("DOWNLOADING_LATEST_RELEASE") + " (3DSX)").c_str());
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadFromRelease("https://github.com/Universal-Team/LeafEdit", "LeafEdit.3dsx", path3dsx + "LeafEdit.3dsx", false) != 0) {
				showProgressBar = false;
				Msg::DisplayWarnMsg(Lang::get("DOWNLOAD_FAILED"));
				return -1;
			}
			showProgressBar = false;
			if (version != "")	config->currentRelease(version);
			config->save(); // Needed to do that here.
			success = true;
		}
	}
	Msg::DisplayWarnMsg(Lang::get("DONE"));
	if (version != "")
	if (success == true) {
		if (is3dsx == true) {
			Is3dsxUpdated = true;
		}
	}
	success = false;
	return 0;
}

void Download::downloadAssets(void) {
	// Acres AC:WW & AC:NL.
	snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Assets... 1 / 6");
	showProgressBar = true;
	progressBarType = 0;
	Threads::create((ThreadFunc)displayProgressBar);
	if (downloadToFile("https://github.com/Universal-Team/LeafEdit-Extras/blob/master/assets/acres.t3x?raw=true", "sdmc:/3ds/LeafEdit/assets/acres.t3x") != 0) {
		showProgressBar = false;
		Msg::DisplayWarnMsg("Download Failed!");
		return;
	}
	// Items & Badges.
	snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Assets... 2 / 6");
	if (downloadToFile("https://github.com/Universal-Team/LeafEdit-Extras/blob/master/assets/items.t3x?raw=true", "sdmc:/3ds/LeafEdit/assets/items.t3x") != 0) {
		showProgressBar = false;
		Msg::DisplayWarnMsg("Download Failed!");
		return;
	}
	// Faces & Hair.
	snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Assets... 3 / 6");
	if (downloadToFile("https://github.com/Universal-Team/LeafEdit-Extras/blob/master/assets/players.t3x?raw=true", "sdmc:/3ds/LeafEdit/assets/players.t3x") != 0) {
		showProgressBar = false;
		Msg::DisplayWarnMsg("Download Failed!");
		return;
	}
	// Font.
	snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Assets... 4 / 6");
	if (downloadToFile("https://github.com/Universal-Team/LeafEdit-Extras/blob/master/assets/font.bcfnt?raw=true", "sdmc:/3ds/LeafEdit/assets/font.bcfnt") != 0) {
		showProgressBar = false;
		Msg::DisplayWarnMsg("Download Failed!");
		return;
	}
	// First Villager Sprite.
	snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Assets... 5 / 6");
	if (downloadToFile("https://github.com/Universal-Team/LeafEdit-Extras/blob/master/assets/villagers.t3x?raw=true", "sdmc:/3ds/LeafEdit/assets/villagers.t3x") != 0) {
		showProgressBar = false;
		Msg::DisplayWarnMsg("Download Failed!");
		return;
	}
	// Second Villager Sprite.
	snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Assets... 6 / 6");
	if (downloadToFile("https://github.com/Universal-Team/LeafEdit-Extras/blob/master/assets/villagers2.t3x?raw=true", "sdmc:/3ds/LeafEdit/assets/villagers2.t3x") != 0) {
		showProgressBar = false;
		Msg::DisplayWarnMsg("Download Failed!");
		return;
	}

	showProgressBar = false;
	// Load the Font, cause it's downloaded in that function.
	Init::loadFont();
	Msg::DisplayWarnMsg("Done!");
}

void displayProgressBar() {
	char str[256];
	while(showProgressBar) {
		if (downloadTotal < 1.0f) {
			downloadTotal = 1.0f;
		}
		if (downloadTotal < downloadNow) {
			downloadTotal = downloadNow;
		}

		if (progressBarType == 0) {
			snprintf(str, sizeof(str), "%s / %s (%.2f%%)",
					formatBytes(downloadNow).c_str(),
					formatBytes(downloadTotal).c_str(),
					((float)downloadNow/(float)downloadTotal) * 100.0f
			);
		} else {
			snprintf(str, sizeof(str), "%s / %s (%.2f%%)",
					formatBytes(installOffset).c_str(),
					formatBytes(installSize).c_str(),
					((float)installOffset/(float)installSize) * 100.0f);
		};

		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, BLACK);
		C2D_TargetClear(Bottom, BLACK);
		GFX::DrawTop();

		Gui::DrawStringCentered(0, 1, 0.7f, WHITE, progressBarMsg, 400, 0, font);
		Gui::DrawStringCentered(0, 80, 0.7f, WHITE, str, 400, 0, font);
		Gui::Draw_Rect(30, 120, 340, 30, BLACK);

			// Download.
		if (progressBarType == 0) {
			Gui::Draw_Rect(31, 121, (int)(((float)downloadNow/(float)downloadTotal) * 338.0f), 28, DARKER_COLOR);
			// Install.
		} else {
			Gui::Draw_Rect(31, 121, (int)(((float)installOffset/(float)installSize) * 338.0f), 28, DARKER_COLOR);
		}

		GFX::DrawBottom();
		C3D_FrameEnd(0);
		gspWaitForVBlank();
	}
}

// Get the contents from `Universal-Team/LeafEdit-Extras`.
std::vector<ExtraEntry> Download::getExtraList(std::string category) {
	Result ret = 0;
	void *socubuf = memalign(0x1000, 0x100000);
	std::vector<ExtraEntry> emptyVector;
	if(!socubuf) {
		return emptyVector;
	}

	ret = socInit((u32*)socubuf, 0x100000);
	if(R_FAILED(ret)) {
		free(socubuf);
		return emptyVector;
	}

	std::stringstream apiurlStream;
	apiurlStream << "https://api.github.com/repos/Universal-Team/LeafEdit-Extras/contents/" << category;
	std::string apiurl = apiurlStream.str();

	CURL *hnd = curl_easy_init();
	ret = setupContext(hnd, apiurl.c_str());
	if(ret != 0) {
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return emptyVector;
	}

	CURLcode cres = curl_easy_perform(hnd);
	curl_easy_cleanup(hnd);
	char* newbuf = (char*)realloc(result_buf, result_written + 1);
	result_buf = newbuf;
	result_buf[result_written] = 0; //nullbyte to end it as a proper C style string

	if (cres != CURLE_OK) {
		printf("Error in:\ncurl\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return emptyVector;
	}

	std::vector<ExtraEntry> jsonItems;
	json parsedAPI = json::parse(result_buf);
	for(uint i=0;i<parsedAPI.size();i++) {
		ExtraEntry extraEntry;
		if(parsedAPI[i]["name"].is_string()) {
			extraEntry.name = parsedAPI[i]["name"];
		}
		if(parsedAPI[i]["download_url"].is_string()) {
			extraEntry.downloadUrl = parsedAPI[i]["download_url"];
		}
		jsonItems.push_back(extraEntry);
	}

	socExit();
	free(result_buf);
	free(socubuf);
	result_buf = NULL;
	result_sz = 0;
	result_written = 0;

	return jsonItems;
}