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
#include "overlay.hpp"
#include <dirent.h>
#include <unistd.h>

extern std::unique_ptr<Config> config;

/* Draw the Top (File) browse. */
static void DrawTop(uint Selection, std::vector<DirEntry> dirContents, bool romfs, const std::string Text) {
	std::string files;

	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);

	GFX::DrawFileBrowseBG();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, Text + " " + std::string(romfs ? "[RomFS]" : "[" + Lang::get("SD_CARD") + "]"), 395, 0, font);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, Lang::get("REFRESH"), 395, 0, font);

	for (uint i = (Selection < 8) ? 0 : Selection - 8; i < dirContents.size() && i < ((Selection < 8) ? 9 : Selection + 1); i++) {
		files += dirContents[i].name + "\n";
	}

	for (uint i = 0; i < ((dirContents.size() < 9) ? 9 - dirContents.size() : 0); i++) {
		files += "\n";
	}

	if (Selection < 9) GFX::DrawSelector(true, 24 + ((int)Selection * 21));
	else GFX::DrawSelector(true, 24 + (8 * 21));
	
	Gui::DrawString(5, 25, 0.85f, BLACK, files, 360, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

static void DrawBottom() {
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, Lang::get("CHANGE_LOCATION"), 310, 0, font);
	C3D_FrameEnd(0);
}

std::string Overlays::RomfsSDOverlay(std::vector<std::string> extensions, std::string SDPath, std::string romfsPath, std::string Text) {
	s32 selectedFile = 0;
	bool romfsMode = true;
	std::vector<DirEntry> dirContents;
	bool dirChanged = true;

	/* Initial directory change. */
	dirContents.clear();

	chdir(romfsPath.c_str());
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, extensions);

	for(uint i = 0; i < dirContentsTemp.size(); i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	while (1) {
		/* Screen draw part. */
		DrawTop(selectedFile, dirContents, romfsMode, Text);
		DrawBottom();

		/* The input part. */
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();

		/* if directory changed -> Refresh it. */
		if (dirChanged) {
			dirContents.clear();
			std::vector<DirEntry> dirContentsTemp;
			getDirectoryContents(dirContentsTemp, extensions);

			for(uint i = 0; i < dirContentsTemp.size(); i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}

			dirChanged = false;
		}

		if ((hRepeat & KEY_LEFT) || (hRepeat & KEY_L)) {
			if ((selectedFile - 9) < 0) {
				selectedFile = 0;

			} else {
				selectedFile -= 9;
			}
		}

		if ((hRepeat & KEY_RIGHT) || (hRepeat & KEY_R)) {
			if ((selectedFile + 9) > (int)dirContents.size()-1) {
				selectedFile = (int)dirContents.size()-1;

			} else {
				selectedFile += 9;
			}
		}
		
		if (hRepeat & KEY_UP) {
			if (selectedFile > 0) {
				selectedFile--;
			}
		}
		

		if (hRepeat & KEY_DOWN) {
			if ((uint)selectedFile < dirContents.size()-1) {
				selectedFile++;
			}
		}
		
		if (hDown & KEY_A) {
			if (dirContents.size() > 0) {
				if (dirContents[selectedFile].isDirectory) {
					chdir(dirContents[selectedFile].name.c_str());
					selectedFile = 0;
					dirChanged = true;

				} else {
					char path[PATH_MAX];
					getcwd(path, PATH_MAX);
					return path + dirContents[selectedFile].name;
				}
			}
		}

		if (hDown & KEY_B) {
			char path[PATH_MAX];
			getcwd(path, PATH_MAX);
			if (strcmp(path, SDPath.c_str()) == 0 || strcmp(path, "/") == 0 || strcmp(path, romfsPath.c_str()) == 0) {
				return "";

			} else {
				chdir("..");
				selectedFile = 0;
				dirChanged = true;
			}
		}

		if (hDown & KEY_X) {
			if (romfsMode) {
				romfsMode = false;
				chdir(SDPath.c_str());
				
			} else {
				romfsMode = true;
				chdir(romfsPath.c_str());
			}
			
			selectedFile = 0;
			dirChanged = true;
		}
		
		if (hDown & KEY_START) {
			selectedFile = 0;
			dirChanged = true;
		}
	}
}