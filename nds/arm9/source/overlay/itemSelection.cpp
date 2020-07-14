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
#include "input.hpp"
#include "itemManager.hpp"
#include "itemUtils.hpp"
#include "lang.hpp"
#include "overlay.hpp"

extern std::vector<std::pair<u16, std::string>> itemDB;

static std::vector<std::pair<u16, std::string>> search() {
	Gui::hidePointer();
	const std::string searchResult = Input::getLine2(Lang::get("ENTER_SEARCH"), true);

	Gui::showPointer();
	std::vector<std::pair<u16, std::string>> temp;

	for (int i = 0; i < (int)itemDB.size(); i++) {
		if (itemDB[i].second.find(searchResult) != std::string::npos) {
			temp.push_back({itemDB[i]});
		}
	}

	return temp;
}

// Get the index of the current Item for the selection.
static int getIndex(const u16 &v) {
	if (v == itemDB[0].first || v >= 0xFFF1) {
		return 0;
	}

	int index = -1, min = 0, mid = 0, max = itemDB.size();
	while (min <= max) {
		mid = min + (max - min) / 2;
		if (itemDB[mid].first == v) {
			index = mid;
			break;
		}

		if (itemDB[mid].first < v) {
			min = mid + 1;
		} else {
			max = mid - 1;
		}
	}
	
	return index >= 0 ? index : 0;
}

static void Draw(std::vector<std::pair<u16, std::string>> temp, int itemIndex, bool background) {
	drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, true, true);
	printTextCentered(Lang::get("SELECT_AN_ITEM"), 0, 0, true, true);
	// Clear text.
	drawRectangle(0, 0, 256, 192, CLEAR, false, true);
	drawImage(256-Gui::searchImg.width, 0, Gui::searchImg, false, false);
	// Print list.
	for (unsigned i = 0; i < std::min(9u, temp.size()-itemIndex); i++) {
		printText(temp[itemIndex+i].second, 4, 4 + (i * 20), false, true);
	}
}

// Select an Item.
u16 Overlays::SelectItem(u16 oldID, bool showPointer) {
	std::vector<std::pair<u16, std::string>> usedList = itemDB;

	int itemIndex = getIndex(oldID);
	// Set pointer position.
	setSpriteVisibility(Gui::pointerID, false, true);
	setSpritePosition(Gui::pointerID, false, 4 + getTextWidth(usedList[itemIndex].second), -2);
	updateOam();

	Draw(usedList, itemIndex, true);

	int hHeld, hDown, screenPos = itemIndex, entriesPerScreen = 9;
	touchPosition touch;
	while(1) {
		do {
			swiWaitForVBlank();
			scanKeys();
			hDown = keysDown();
			hHeld = keysDownRepeat();
		} while(!hHeld);

		if (usedList.size() > 0) {
			if (hHeld & KEY_UP) {
				if (itemIndex > 0) itemIndex--;
				else itemIndex = (int)usedList.size()-1;
			}
		
			if (hHeld & KEY_DOWN) {
				if (itemIndex < (int)usedList.size()-1) itemIndex++;
				else itemIndex = 0;
			}
			
			if (hHeld & KEY_LEFT) {
				itemIndex -= entriesPerScreen;
				if (itemIndex < 0) itemIndex = 0;
			}
			
			if (hHeld & KEY_RIGHT) {
				itemIndex += entriesPerScreen;
				if (itemIndex > (int)itemDB.size()-1) itemIndex = (int)usedList.size()-1;
			}
		
			if (hDown & KEY_A) {
				for (unsigned int i = 0; i < usedList.size(); i++) {
					if (!showPointer) Gui::hidePointer();
					Gui::DrawScreen();
					return usedList[itemIndex].first;
				}
			}

		
			if (hDown & KEY_TOUCH) {
				touchRead(&touch);
				if (touch.px >= 256 - Gui::searchImg.width && touch.py <= Gui::searchImg.height) {
					itemIndex = 0;
					screenPos = itemIndex;
					usedList = search();
					Draw(usedList, screenPos, false);
				}

				for (int i = 0; i < entriesPerScreen; i++) {
					if (touch.px >= 4 && touch.px <= 4 + getTextWidth(usedList[screenPos+i].second) && touch.py >= 4 + (i * 20) && touch.py <= 4 + ((i + 1) * 20)) {
						drawRectangle(0, 0, 256, 192, CLEAR, false, true);
						if (!showPointer) Gui::hidePointer();
						Gui::DrawScreen();
						return usedList[screenPos+i].first;
						break;
					}
				}
			}

			// Scroll screen if needed.
			if (itemIndex < screenPos) {
				screenPos = itemIndex;
				Draw(usedList, screenPos, false);
			} else if (itemIndex > screenPos + entriesPerScreen - 1) {
				screenPos = itemIndex - entriesPerScreen + 1;
				Draw(usedList, screenPos, false);
			}

			// Move pointer.
			setSpritePosition(Gui::pointerID, false, 3 + getTextWidth(usedList[itemIndex].second), (20 * (itemIndex-screenPos) - 2) + 10);
			updateOam();
		}
		
		if (hDown & KEY_B) {
			if (!showPointer) Gui::hidePointer();
			Gui::DrawScreen();
			return oldID;
		}

		if (hDown & KEY_X) {
			itemIndex = 0;
			screenPos = itemIndex;
			usedList = search();
			Draw(usedList, screenPos, false);
		}

		if (hDown & KEY_Y) {
			itemIndex = 0;
			screenPos = itemIndex;
			usedList = itemDB;
			Draw(usedList, screenPos, false);
		}
	}
}