/*
*   This file is part of LeafEdit
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

#include "common.hpp"
#include "fileBrowse.hpp"
#include "keyboard.hpp"

#include <3ds.h>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include <algorithm>
#include <functional>
#include <array>
#include <iostream>

extern touchPosition touch;
std::vector<FavSave> favDatabase;
extern std::unique_ptr<Config> config;

off_t getFileSize(const char *fileName) {
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
	if (name.substr(0, 2) == "._") return false;

	if (name.size() == 0) return false;

	if (extensionList.size() == 0) return true;

	for(int i = 0; i <(int)extensionList.size(); i++) {
		const std::string ext = extensionList.at(i);
		if (strcasecmp(name.c_str() + name.size() - ext.size(), ext.c_str()) == 0) return true;
	}

	return false;
}

bool dirEntryPredicate(const DirEntry& lhs, const DirEntry& rhs) {
	if (!lhs.isDirectory && rhs.isDirectory) {
		return false;
	}

	if (lhs.isDirectory && !rhs.isDirectory) {
		return true;
	}

	return strcasecmp(lhs.name.c_str(), rhs.name.c_str()) < 0;
}

void getDirectoryContents(std::vector<DirEntry>& dirContents, const std::vector<std::string> extensionList) {
	struct stat st;

	dirContents.clear();

	DIR *pdir = opendir(".");

	if (pdir == NULL) {
		Msg::DisplayMsg("Unable to open the directory.");
		for(int i=0;i<120;i++)	gspWaitForVBlank();
	} else {
		while(true) {
			DirEntry dirEntry;

			struct dirent* pent = readdir(pdir);
			if (pent == NULL) break;

			stat(pent->d_name, &st);
			dirEntry.name = pent->d_name;
			dirEntry.isDirectory = (st.st_mode & S_IFDIR) ? true : false;

			if (dirEntry.name.compare(".") != 0 && (dirEntry.isDirectory || nameEndsWith(dirEntry.name, extensionList))) {
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
		if (nameEndsWith(pent->d_name, extensionList))
			dirContents.push_back(pent->d_name);
	}

	closedir(pdir);
	return dirContents;
}

// Draw the Browse.
static void DrawBrowseTop(uint Selection, std::vector<DirEntry> dirContents, const std::string Text) {
	std::string dirs;
	GFX::DrawFileBrowseBG();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, Text, 395, 0, font);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, Lang::get("REFRESH"), 395, 0, font);
	for (uint i=(Selection<8) ? 0 : (uint)Selection-8;i<dirContents.size()&&i<((Selection<8) ? 9 : Selection+1);i++) {
		dirs += dirContents[i].name + "\n";
	}

	for (uint i=0;i<((dirContents.size()<9) ? 9-dirContents.size() : 0);i++) {
		dirs += "\n";
	}

	if (Selection < 9)	GFX::DrawSelector(true, 24 + ((int)Selection * 21));
	else				GFX::DrawSelector(true, 24 + (8 * 21));
	if (!config->newStyle()) {
		Gui::DrawString(5, 23, 0.85f, BLACK, dirs, 360, 0, font);
	} else {
		Gui::DrawString(5, 25, 0.85f, BLACK, dirs, 360, 0, font);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

// Draw Fav Browse.
void DrawFavSaves(uint Selection) {
	std::string saves;
	GFX::DrawFileBrowseBG(false);
	for (uint i=(Selection<8) ? 0 : (uint)Selection-8;i<favDatabase.size()&&i<((Selection<8) ? 9 : Selection+1);i++) {
		saves += favDatabase[i].Name + "\n";
	}

	for (uint i=0;i<((favDatabase.size()<9) ? 9-favDatabase.size() : 0);i++) {
		saves += "\n";
	}

	if (!config->newStyle()) {
		if (Selection < 9)	GFX::DrawSelector(false, 24 + ((int)Selection * 21));
		else				GFX::DrawSelector(false, 24 + (8 * 21));
	} else {
		if (Selection < 9)	GFX::DrawSelector(false, 26 + ((int)Selection * 21));
		else				GFX::DrawSelector(false, 26 + (8 * 21));
	}

	if (!config->newStyle()) {
		Gui::DrawString(5, 23, 0.85f, BLACK, saves, 360, 0, font);
	} else {
		Gui::DrawString(5, 25, 0.85f, BLACK, saves, 360, 0, font);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

FavSave getFavSave(std::string line) {
	FavSave FS;
	if (line.size() != 0) {
		FS.Name = line.substr(0, line.find(", "));
		line = line.substr(line.find(", ")+1);
	
		FS.Path = line.substr(0, std::min(line.find(", "), line.length()-1));
		line = line.substr(line.find(";"));
	} else {
		FS.Name = "-";
		FS.Path = "-";
	}

	return FS;
}

// Implementation of favorite saves.
void FavSaves::Parse() {
	favDatabase.clear(); // Clear!

	// if File not found -> Create!
	if ((access("sdmc:/LeafEdit/FavSaves.fs", F_OK) != 0)) {
		std::ofstream outfile("sdmc:/LeafEdit/FavSaves.fs");
		outfile.close();
	}

	std::ifstream in("sdmc:/LeafEdit/FavSaves.fs");
	if (in.good()) {
		std::string line;
		while(std::getline(in, line)) {
			favDatabase.push_back(getFavSave(line)); // PUSH!
		}
	}
}

// Add a favorite Save.
void FavSaves::add(std::string name, std::string path) {
	std::ofstream out;
	out.open("sdmc:/LeafEdit/FavSaves.fs", std::ofstream::app);
	out << name << ", " << path << ";" << std::endl;
	out.close();
	Parse(); // Reparse after it.
}

std::string SaveBrowse::searchForSave(const std::vector<std::string> SaveType, const std::string initialPath, const std::string Text) {
	s32 selectedSave = 0;
	s32 selectedFavSave = 0;
	int keyRepeatDelay = 4;
	bool dirChanged = false;
	int Mode = 0;
	std::vector<DirEntry> dirContents;
	FavSaves::Parse(); // Parse.

	// Initial dir change.
	dirContents.clear();
	chdir(initialPath.c_str());
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, SaveType);
	for(uint i=0;i<dirContentsTemp.size();i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	while (1) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, BLACK);
		C2D_TargetClear(Bottom, BLACK);
		// Screen draw part.
		DrawBrowseTop(selectedSave, dirContents, Text);
		DrawFavSaves(selectedFavSave);
		C3D_FrameEnd(0);

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

		if (hDown & KEY_X) {
			if (Mode == 0) {
				if (favDatabase.size() > 0)	Mode = 1;
			}
			else			Mode = 0;
		}

		if (hDown & KEY_A) {
			if (Mode == 0) {
				if (dirContents[selectedSave].isDirectory) {
					chdir(dirContents[selectedSave].name.c_str());
					selectedSave = 0;
					dirChanged = true;
				} else {
					char path[PATH_MAX];
					getcwd(path, PATH_MAX);
					std::string output = path + dirContents[selectedSave].name;
					return output;
				}
			} else if (Mode == 1) {
				std::string file;
				if (favDatabase[selectedFavSave].Path.size() > 3 || favDatabase[selectedFavSave].Path != "" || favDatabase[selectedFavSave].Path != "-") {
					file = favDatabase[selectedFavSave].Path.substr(1, favDatabase[selectedFavSave].Path.size() -1);
					if((access(file.c_str(), F_OK) == 0)) {
						return file;
					}
				}
			}
		}

		if (hHeld & KEY_UP) {
			if (Mode == 0) {
				if (selectedSave > 0 && !keyRepeatDelay) {
					selectedSave--;
					keyRepeatDelay = 6;
				}
			} else {
				if (selectedFavSave > 0 && !keyRepeatDelay) {
					selectedFavSave--;
					keyRepeatDelay = 6;
				}
			}
		} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
			if (Mode == 0) {
				if ((uint)selectedSave < dirContents.size()-1) {
					selectedSave++;
					keyRepeatDelay = 6;
				}
			} else {
				if ((uint)selectedFavSave < favDatabase.size()-1) {
					selectedFavSave++;
					keyRepeatDelay = 6;
				}
			}
		} else if (hDown & KEY_B) {
			if (Mode == 0) {
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
			} else {
				if(Msg::promptMsg(Lang::get("CANCEL_SAVE_SELECTION"))) {
					return "";
				}
			}
		} else if (hDown & KEY_START) {
			dirChanged = true;
		}

		if (hDown & KEY_Y) {
			if (Mode == 0) {
				if (!dirContents[selectedSave].isDirectory) {
					if (Msg::promptMsg("Do you like to add this to a Favorite Save?")) {
						std::string name = Input::getString("Enter the favorite savename.");
						char path[PATH_MAX];
						getcwd(path, PATH_MAX);
						std::string output = path + dirContents[selectedSave].name;
						FavSaves::add(name, output);
					}
				}
			}
		}
	}
}

// Script File Selection.
static void Drawbrowse(uint Selection, std::vector<DirEntry> dirContents, const std::string Text) {
	std::string dirs;
	GFX::DrawFileBrowseBG();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, Text, 395, 0, font);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, Lang::get("REFRESH"), 395, 0, font);
	for (uint i=(Selection<8) ? 0 : (uint)Selection-8;i<dirContents.size()&&i<((Selection<8) ? 9 : Selection+1);i++) {
		dirs += dirContents[i].name + "\n";
	}

	for (uint i=0;i<((dirContents.size()<9) ? 9-dirContents.size() : 0);i++) {
		dirs += "\n";
	}

	if (Selection < 9)	GFX::DrawSelector(true, 24 + ((int)Selection * 21));
	else				GFX::DrawSelector(true, 24 + (8 * 21));
	Gui::DrawString(5, 25, 0.85f, BLACK, dirs, 360, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawFileBrowseBG(false);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

std::string searchForFile(char *path, char *Text) {
	s32 selectedFile = 0;
	int keyRepeatDelay = 4;
	bool refreshed = false;
	std::vector<DirEntry> dirContents;

	// Initial dir change.
	dirContents.clear();
	chdir(path);
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, {});
	for(uint i=0;i<dirContentsTemp.size();i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	while (1) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, BLACK);
		C2D_TargetClear(Bottom, BLACK);
		// Screen draw part.
		Drawbrowse(selectedFile, dirContents, Text);
		C3D_FrameEnd(0);

		// The input part.
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hHeld = hidKeysHeld();
		hidTouchRead(&touch);
		if (keyRepeatDelay)	keyRepeatDelay--;

		// if directory changed -> Refresh it.
		if (refreshed) {
			dirContents.clear();
			std::vector<DirEntry> dirContentsTemp;
			getDirectoryContents(dirContentsTemp);
			for(uint i=0;i<dirContentsTemp.size();i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}

			refreshed = false;
		}

		if (hDown & KEY_A) {
			char path[PATH_MAX];
			getcwd(path, PATH_MAX);
			return path + dirContents[selectedFile].name;
		}

		if (hHeld & KEY_UP) {
			if (selectedFile > 0 && !keyRepeatDelay) {
				selectedFile--;
				keyRepeatDelay = 6;
			}
		} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
			if ((uint)selectedFile < dirContents.size()-1) {
				selectedFile++;
				keyRepeatDelay = 6;
			}
		} else if (hDown & KEY_B) {
			if (Msg::promptMsg("Cancel File Selection?\nThis returns ''.")) {
				return "?";
			}
		} else if (hDown & KEY_START) {
			refreshed = true;
		}
	}
}