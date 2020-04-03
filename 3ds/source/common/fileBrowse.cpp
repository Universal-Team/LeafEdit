/*
*   This file is part of LeafEdit
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

#include "common.hpp"
#include "fileBrowse.hpp"

#include <3ds.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include <algorithm>
#include <functional>
#include <array>
#include <iostream>

extern touchPosition touch;

off_t getFileSize(const char *fileName)
{
	FILE* fp = fopen(fileName, "rb");
	off_t fsize = 0;
	if (fp) {
		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);			// Get source file's size
		fseek(fp, 0, SEEK_SET);
	}
	fclose(fp);

	return fsize;
}

bool nameEndsWith(const std::string& name, const std::vector<std::string> extensionList) {
	if(name.substr(0, 2) == "._") return false;

	if(name.size() == 0) return false;

	if(extensionList.size() == 0) return true;

	for(int i = 0; i <(int)extensionList.size(); i++) {
		const std::string ext = extensionList.at(i);
		if(strcasecmp(name.c_str() + name.size() - ext.size(), ext.c_str()) == 0) return true;
	}
	return false;
}

bool dirEntryPredicate(const DirEntry& lhs, const DirEntry& rhs) {
	if(!lhs.isDirectory && rhs.isDirectory) {
		return false;
	}
	if(lhs.isDirectory && !rhs.isDirectory) {
		return true;
	}
	return strcasecmp(lhs.name.c_str(), rhs.name.c_str()) < 0;
}

void getDirectoryContents(std::vector<DirEntry>& dirContents, const std::vector<std::string> extensionList) {
	struct stat st;

	dirContents.clear();

	DIR *pdir = opendir(".");

	if(pdir == NULL) {
		Msg::DisplayMsg("Unable to open the directory.");
		for(int i=0;i<120;i++)	gspWaitForVBlank();
	} else {
		while(true) {
			DirEntry dirEntry;

			struct dirent* pent = readdir(pdir);
			if(pent == NULL) break;

			stat(pent->d_name, &st);
			dirEntry.name = pent->d_name;
			dirEntry.isDirectory = (st.st_mode & S_IFDIR) ? true : false;

			if(dirEntry.name.compare(".") != 0 && (dirEntry.isDirectory || nameEndsWith(dirEntry.name, extensionList))) {
				dirContents.push_back(dirEntry);
			}
		}
		closedir(pdir);
	}
	sort(dirContents.begin(), dirContents.end(), dirEntryPredicate);
}

void getDirectoryContents(std::vector<DirEntry>& dirContents) {
	getDirectoryContents(dirContents, {});
}

std::vector<std::string> getContents(const std::string &name, const std::vector<std::string> &extensionList) {
	std::vector<std::string> dirContents;
	DIR* pdir = opendir(name.c_str());
	struct dirent *pent;
	while ((pent = readdir(pdir)) != NULL) {
		if(nameEndsWith(pent->d_name, extensionList))
			dirContents.push_back(pent->d_name);
	}
	closedir(pdir);
	return dirContents;
}

// Draw the Browse.
static void DrawBrowse(uint Selection, std::vector<DirEntry> dirContents, const std::string Text) {
	std::string dirs;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawFileBrowseBG();
	GFX::DrawTitle(Text);
	GFX::DrawTitle(Lang::get("REFRESH"), false);
	for (uint i=(Selection<5) ? 0 : (uint)Selection-5;i<dirContents.size()&&i<((Selection<5) ? 6 : Selection+1);i++) {
		if (i == Selection) {
			dirs += "> " + dirContents[i].name + "\n\n";
		} else {
			dirs += dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}
	Gui::DrawString(26, 32, 0.65f, BLACK, dirs.c_str(), 360);
	GFX::DrawFileBrowseBG(false);
	C3D_FrameEnd(0);
}


std::string SaveBrowse::searchForSave(const std::vector<std::string> SaveType, const std::string initialPath, const std::string Text) {
	s32 selectedSave = 0;
	static int keyRepeatDelay = 4;
	static bool dirChanged = false;
	std::vector<DirEntry> dirContents;

	// Initial dir change.
	dirContents.clear();
	chdir(initialPath.c_str());
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, SaveType);
	for(uint i=0;i<dirContentsTemp.size();i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	while (1) {
		// Screen draw part.
		DrawBrowse(selectedSave, dirContents, Text);

		// The input part.
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hHeld = hidKeysHeld();
		hidTouchRead(&touch);
		if (keyRepeatDelay)	keyRepeatDelay--;

		// if directory changed -> Refresh it.
		if (dirChanged) {
			dirContents.clear();
			std::vector<DirEntry> dirContentsTemp;
			getDirectoryContents(dirContentsTemp, SaveType);
			for(uint i=0;i<dirContentsTemp.size();i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}
			dirChanged = false;
		}

		if (hDown & KEY_A) {
			if (dirContents[selectedSave].isDirectory) {
				chdir(dirContents[selectedSave].name.c_str());
				selectedSave = 0;
				dirChanged = true;
			} else {
				return dirContents[selectedSave].name;
			}
		}

		if (hHeld & KEY_UP) {
			if (selectedSave > 0 && !keyRepeatDelay) {
				selectedSave--;
				keyRepeatDelay = 6;
			}
		} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
			if ((uint)selectedSave < dirContents.size()-1) {
				selectedSave++;
				keyRepeatDelay = 6;
			}
		} else if (hDown & KEY_B) {
			char path[PATH_MAX];
			getcwd(path, PATH_MAX);
			if(strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
				if(Msg::promptMsg(Lang::get("CANCEL_SAVE_SELECTION"))) {
					return "";
				}
			} else {
				chdir("..");
				selectedSave = 0;
				dirChanged = true;
			}
		} else if (hDown & KEY_START) {
			dirChanged = true;
		}
	}
}