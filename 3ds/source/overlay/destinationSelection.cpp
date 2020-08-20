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

/* Draw the Top (file) browse. */
static void DrawTop(uint Selection, std::vector<DirEntry> dirContents, const std::string Text) {
	std::string files;

	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);

	GFX::DrawFileBrowseBG();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, Text, 395, 0, font);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, Lang::get("REFRESH"), 395, 0, font);
	for (uint i = (Selection < 8) ? 0 : Selection - 8; i < dirContents.size() && i < ((Selection < 8) ? 9 : Selection + 1); i++) {
		files += dirContents[i].name + "\n";
	}

	for (uint i = 0; i < ((dirContents.size() < 9) ? 9 - dirContents.size() : 0); i++) {
		files += "\n";
	}

	if (Selection < 9) GFX::DrawSelector(true, 24 + ((int)Selection * 21));
	else GFX::DrawSelector(true, 24 + (8 * 21));
	config->newStyle() ? Gui::DrawString(5, 25, 0.85f, BLACK, files, 360, 0, font) : Gui::DrawString(5, 23, 0.85f, BLACK, files, 360, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

static void DrawBottom() {
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, Lang::get("DESTINATION"), 310, 0, font);
	C3D_FrameEnd(0);
}

std::string Overlays::SelectDestination(std::string Text, std::string initialPath, std::string defaultDest) {
	s32 selectedDir = 0;
	std::vector<DirEntry> dirContents;
	bool dirChanged = true;

	/* Initial directory change. */
	dirContents.clear();

	chdir(initialPath.c_str());
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, {});
	for(uint i = 0; i < dirContentsTemp.size(); i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	while (1) {
		/* Screen draw part. */
		DrawTop(selectedDir, dirContents, Text);
		DrawBottom();

		/* The input part. */
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();

		/* if directory changed -> Refresh it. */
		if (dirChanged) {
			dirContents.clear();
			std::vector<DirEntry> dirContentsTemp;
			getDirectoryContents(dirContentsTemp, {});
			for(uint i = 0; i < dirContentsTemp.size(); i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}

			dirChanged = false;
		}

		if (hDown & KEY_A) {
			if (dirContents.size() > 0) {
				if (dirContents[selectedDir].isDirectory) {
					chdir(dirContents[selectedDir].name.c_str());
					selectedDir = 0;
					dirChanged = true;
				}
			}
		}

		if (hDown & KEY_Y) {
			char path[PATH_MAX];
			getcwd(path, PATH_MAX);
			return path;
		}

		if (hRepeat & KEY_UP) {
			if (selectedDir > 0) {
				selectedDir--;
			}
		}
		
		if (hRepeat & KEY_DOWN) {
			if ((uint)selectedDir < dirContents.size()-1) {
				selectedDir++;
			}
		}
		
		if (hDown & KEY_B) {
			char path[PATH_MAX];
			getcwd(path, PATH_MAX);
			if (strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
				return defaultDest;
			} else {
				chdir("..");
				selectedDir = 0;
				dirChanged = true;
			}
		}
		
		if (hDown & KEY_START) {
			selectedDir = 0;
			dirChanged = true;
		}
	}
}