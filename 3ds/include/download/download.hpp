/*
*   This file is part of Universal-Updater
*   Copyright (C) 2019-2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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

#ifndef DOWNLOAD_HPP
#define DOWNLOAD_HPP

#include "common.hpp"

#define APP_TITLE "LeafEdit"
#define VERSION_STRING "0.2.0"

enum DownloadError {
	DL_ERROR_NONE = 0,
	DL_ERROR_WRITEFILE,
	DL_ERROR_ALLOC,
	DL_ERROR_STATUSCODE,
	DL_ERROR_GIT,
};

struct NightlyFetch {
	std::string Message;
	std::string Target;
	std::string Committer;
	std::string Author;
};

struct ReleaseFetch {
	std::string Version;
	std::string ReleaseName;
	std::string Body;
	std::string Published;
};

struct ExtraEntry {
	std::string downloadUrl;
	std::string name;
};

Result downloadToFile(std::string url, std::string path);
Result downloadFromRelease(std::string url, std::string asset, std::string path, bool includePrereleases);

void displayProgressBar();

/**
 * Check Wi-Fi status.
 * @return True if Wi-Fi is connected; false if not.
 */
bool checkWifiStatus(void);


ReleaseFetch getLatestRelease();

NightlyFetch getLatestCommit();

// LeafEdit's namespace for Downloads.
namespace Download {
	void downloadAssets(void);
	Result updateApp(bool nightly);

	ReleaseFetch getLatestRelease2();
	NightlyFetch getLatestNightly();

	bool showReleaseInfo(ReleaseFetch RF);

	std::vector<ExtraEntry> getExtraList(std::string category);
	void downloadScripts(void);
}

#endif