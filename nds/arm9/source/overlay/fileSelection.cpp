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

#include "gui.hpp"
#include "filebrowse.hpp"
#include "flashcard.hpp"
#include "lang.hpp"
#include "overlay.hpp"

#include <dirent.h>

static void Draw(std::vector<DirEntry> dir, int index, bool background) {
	drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, true, true);
	printTextCentered(Lang::get("SELECT_A_FILE"), 0, 0, true, true);
	// Clear text.
	drawRectangle(0, 0, 256, 192, CLEAR, false, true);
	// Print list.
	for (unsigned i = 0; i < std::min(9u, dir.size()-index); i++) {
		printText(dir[index+i].name, 4, 4 + (i * 20), false, true);
	}
}

// Select an Item.
std::string Overlays::SelectFile(const std::vector<std::string> fileType, const std::string initialPath, bool showPointer) {
	int hHeld, hDown, index = 0, screenPos = 0, entriesPerScreen = 9;
	std::vector<DirEntry> dirContents;
	bool dirChanged = true;

	// Initial dir change.
	dirContents.clear();
	chdir(initialPath.c_str());
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, fileType);
	for(uint i = 0; i < dirContentsTemp.size(); i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	// Set pointer position.
	setSpriteVisibility(Gui::pointerID, false, true);
	setSpritePosition(Gui::pointerID, false, 4 + getTextWidth(dirContents[index].name), -2);
	updateOam();

	Draw(dirContents, index, true);

	while(1) {
		do {
			swiWaitForVBlank();
			scanKeys();
			hDown = keysDown();
			hHeld = keysDownRepeat();
		} while(!hHeld);

		// if directory changed -> Refresh it.
		if (dirChanged) {
			dirContents.clear();
			std::vector<DirEntry> dirContentsTemp;
			getDirectoryContents(dirContentsTemp, fileType);
			for(uint i = 0; i < dirContentsTemp.size(); i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}

			Draw(dirContents, 0, true);
			dirChanged = false;
		}


		if (hHeld & KEY_DOWN) {
			if (index < (int)dirContents.size()-1) index++;
		}

		if (hHeld & KEY_UP) {
			if (index > 0) index--;
		}

		if (hDown & KEY_A) {
			if (dirContents[index].isDirectory) {
				chdir(dirContents[index].name.c_str());
				index = 0;
				dirChanged = true;
			} else {
				char path[PATH_MAX];
				getcwd(path, PATH_MAX);
				std::string output = path + dirContents[index].name;
				if (!showPointer) Gui::hidePointer();
				Gui::DrawScreen();
				return output;
			}
		}

		if (hDown & KEY_B) {
			char path[PATH_MAX];
			getcwd(path, PATH_MAX);
			if (strcmp(path, sdFound() ? "sd:/" : "fat:/") == 0 || strcmp(path, "/") == 0) {
				if (!showPointer) Gui::hidePointer();
				Gui::DrawScreen();
				return "";
			} else {
				chdir("..");
				index = 0;
				dirChanged = true;
			}
		}

		// Scroll screen if needed.
		if (index < screenPos) {
			screenPos = index;
			Draw(dirContents, screenPos, false);
		} else if (index > screenPos + entriesPerScreen - 1) {
			screenPos = index - entriesPerScreen + 1;
			Draw(dirContents, screenPos, false);
		}

		// Move pointer.
		setSpritePosition(Gui::pointerID, false, 3 + getTextWidth(dirContents[index].name), (20 * (index-screenPos) - 2) + 10);
		updateOam();
	}
}