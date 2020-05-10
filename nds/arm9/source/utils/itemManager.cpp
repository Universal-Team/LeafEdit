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

#include "gui.hpp"
#include "itemManager.hpp"

#include <vector>

extern std::vector<std::pair<u16, std::string>> itemDB;

// Return color index of the ItemPalette array.
u8 ItemManager::getColor(ItemType item) {
	switch(item) {
		case ItemType::Empty:
			return 0x0; // Transparent.
		case ItemType::Furniture:
			return 0xF5;
		case ItemType::Gyroid:
			return 0xF6;
		case ItemType::Diary:
			return 0x0; // Isn't supported in LeafEdit.
		case ItemType::Clothes:
			return 0xEC;
		case ItemType::Song:
			return 0xEE;
		case ItemType::Paper:
			return 0xEA;
		case ItemType::Trash:
			return 0xF8; // Currently not in core for WW? Research.
		case ItemType::Shell:
			return 19;
		case ItemType::Fruit:
			return 0xF9; // Currently not in core for WW? Research.
		case ItemType::Turnip:
			return 0xF2;
		case ItemType::Catchable:
			return 0xED;
		case ItemType::QuestItem:
			return 0x0; // Isn't supported in LeafEdit.
		case ItemType::Item:
			return 0xF1;
		case ItemType::RaffleTicket:
			return 0x0; // Isn't supported in LeafEdit.
		case ItemType::WallpaperCarpet:
			return 0xEB;
		case ItemType::Fossil:
			return 0xF3;
		case ItemType::Tool:
			return 0xEF;
		case ItemType::Tree:
			return 0xE4;
		case ItemType::Weed:
			return 0xE3;
		case ItemType::Flower:
			return 0xE2;
		case ItemType::Rock:
			return 0xE8;
		case ItemType::MoneyRock:
			return 0xE9;
		case ItemType::Signboard:
			return 0x0; // Isn't supported in LeafEdit.
		case ItemType::Money:
			return 0xF0;
		case ItemType::HouseObject:
			return 0x0; // Isn't supported in LeafEdit.
		case ItemType::Building:
			return 0xF7;
		case ItemType::ParchedFlower:
			return 0xE5;
		case ItemType::WateredFlower:
			return 0xE6;
		case ItemType::Pattern:
			return 0xE7;
		case ItemType::WiltedFlower:
			return 0x0; // Not in the WW Core rn.
		case ItemType::Occupied:
			return 0xE1;
		case ItemType::Invalid:
			return 0xFA;
	}
	return 0x0; // Should not happen.
}

// Get the index of the current Item for the selection.
int getIndex(std::vector<std::pair<u16, std::string>>& search, const u16& v) {
	if (v == search[0].first || v >= 0xFFF1)
	{
		return 0;
	}
	int index = -1, min = 0, mid = 0, max = search.size();
	while (min <= max)
	{
		mid = min + (max - min) / 2;
		if (search[mid].first == v)
		{
			index = mid;
			break;
		}
		if (search[mid].first < v)
		{
			min = mid + 1;
		}
		else
		{
			max = mid - 1;
		}
	}
	return index >= 0 ? index : 0;
}

// Draw's the List for the Item Selection.
static void drawItemList(int screenPos, bool background, const std::string Text) {
	drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, true, false);
	printTextCentered(Text, 0, 0, true, true);
	if (background) {
		// Clear screen.
		drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, false, false);
	}
	// Clear text.
	drawRectangle(0, 0, 256, 192, CLEAR, false, true);
	// Print list.
	for (unsigned i=0;i<std::min(9u, itemDB.size()-screenPos);i++) {
		printText(itemDB[screenPos+i].second, 4, 4+(i*20), false, true);
	}
}

/*
*	Select an Item from the Item List.
*	current for the current Item ID.
*	Text for the Text which should be displayed.
*/

u16 ItemManager::selectItem(u16 current, const std::string Text) {
	int index = getIndex(itemDB, current); // Get the current Index of the Item.
	// Set pointer position.
	setSpriteVisibility(Gui::pointerID, false, true);
	setSpritePosition(Gui::pointerID, false, 4+getTextWidth(itemDB[index].second), -2);
	updateOam();

	// Initial list draw.
	drawItemList(index, true, Text);

	// Get min & max ID's.
	u16 minID = 0, maxID = itemDB.size();

	// Used variables.
	int held, pressed, screenPos = index, newSelection = index, entriesPerScreen = 9;
	while(1) {
		do {
			swiWaitForVBlank();
			scanKeys();
			pressed = keysDown();
			held = keysDownRepeat();
		} while(!held);

		if (held & KEY_UP) {
			if (newSelection > minID)	newSelection--;
			else	newSelection = maxID-1;
		} else if (held & KEY_DOWN) {
			if (newSelection < (int)maxID-1)	newSelection++;
			else newSelection = minID;
		} else if (held & KEY_LEFT) {
			newSelection -= entriesPerScreen;
			if (newSelection < minID)	newSelection = minID;
		} else if (held & KEY_RIGHT) {
			newSelection += entriesPerScreen;
			if (newSelection > (int)maxID-1)	newSelection = maxID-1;
		} else if (pressed & KEY_A) {
			Gui::hidePointer();
			return itemDB[newSelection].first;

		} if (pressed & KEY_B) {
			Gui::hidePointer();
			return current;
		}

		// Scroll screen if needed.
		if (newSelection < screenPos) {
			screenPos = newSelection;
			drawItemList(screenPos, false, Text);
		} else if (newSelection > screenPos + entriesPerScreen - 1) {
			screenPos = newSelection - entriesPerScreen + 1;
			drawItemList(screenPos, false, Text);
		}

		// Move pointer.
		setSpritePosition(Gui::pointerID, false, 3+getTextWidth(itemDB[newSelection].second), (20*(newSelection-screenPos)-2)+10);
		updateOam();
	}
}