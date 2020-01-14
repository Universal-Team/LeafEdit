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

#include "common/utils.hpp"

#include "core/management/itemManagement.hpp"

#include "core/save/item.h"
#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"

#include "gui/gui.hpp"
#include "gui/keyboard.hpp"

#include "gui/screens/itemEditor.hpp"
#include "gui/screens/itemViewer.hpp"

#include <3ds.h>
#include <map>

extern Save* SaveFile;
extern std::map<u16, std::string> g_itemDatabase;

static std::vector<std::pair<std::string, s32>> inventoryData; // TODO: I dislike this. Find someother way of doing. Perhaps an item container class?
static std::vector<std::pair<std::string, s32>> dresserData; // Dresser.
static std::vector<std::pair<std::string, s32>> islandBoxData; // Island.
static std::vector<std::pair<std::string, s32>> storageData; // Storage.

std::pair<std::string, s32> passItemData; // Pass the selected Item.

#define MAX_DRESSER_ROWS	6
#define MAX_STORAGE_ROWS	9
#define MAX_ISLANDBOX_ROWS	4


extern int selectedPassedPlayer;
#define maxDresser 17 // 17, because it starts with 0, so it would be basically 18.
#define maxIsland 3
#define maxStorage 35 // Is that correct with 36 Boxes? (360)?

extern bool touching(touchPosition touch, Structs::ButtonPos button);

ItemViewer::ItemViewer() {
	ItemManagement::LoadItemDatabase();
}

void ItemViewer::Draw(void) const {
	if (itemMode == 0) {
		DrawSubMenu();
	} else if (itemMode == 1) {
		DisplayPocket();
	} else if (itemMode == 2) {
		DisplayIslandBox();
	} else if (itemMode == 3) {
		DisplayDresser();
	} else if (itemMode == 4) {
		DisplayStorage();
	}
}

void ItemViewer::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (itemMode == 0) {
		SubMenuLogic(hDown, touch);
	} else if (itemMode == 1) {
		PocketLogic(hDown);
	} else if (itemMode == 2) {
		islandBoxLogic(hDown, touch);
	} else if (itemMode == 3) {
		DresserLogic(hDown, touch);
	} else if (itemMode == 4) {
		storageLogic(hDown, touch);
	}
}

// Selection of the Item for IslandBox, Dresser & Storage.
void ItemViewer::DrawSlotSelection(void) const {
	int selectY = 0, selectX = 0;
	if (currentItem < 5)	selectY = 0;	else	selectY = 1;
	if (currentItem > 4)	selectX = currentItem - 5;	else	selectX = currentItem;
	Gui::Draw_ImageBlend(0, sprites_itemHole_idx, 25 + selectX * 58 - 16, 75 + selectY * 58 - 16, DARKER_COLOR);
}

void ItemViewer::DrawSubMenu(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("ITEMS"), 400);
	Gui::DrawBottom();
	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			Gui::drawAnimatedSelector(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, .030f, SELECTED_COLOR);
		}
	}
	// Pocket. (16)
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.6f, Lang::get("ITEM_POCKET")))/2-80+17.5, 0.6f, WHITE, Lang::get("ITEM_POCKET"), 130, 25);
	// Island. (40)
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.6f, Lang::get("ITEM_ISLAND")))/2-20+17.5, 0.6f, WHITE, Lang::get("ITEM_ISLAND"), 130, 25);
	// Dresser. (180)
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.6f, Lang::get("ITEM_DRESSER")))/2+75-17.5, 0.6f, WHITE, Lang::get("ITEM_DRESSER"), 130, 25);
	// Storage. (360?)
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.6f, Lang::get("ITEM_STORAGE")))/2-80+17.5, 0.6f, WHITE, Lang::get("ITEM_STORAGE"), 130, 25);
}

void ItemViewer::DisplayPocket(void) const {
	int x = 42;
	int y = 63;
	inventoryData = EditorUtils::load_player_invitems(selectedPassedPlayer);
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("ITEM_POCKET"), 400);
	Gui::DrawBottom();
	for (int i = 0; i < 16; ++i)
	{
		if (i == 2)
		{
			x += 38 * 2;
		}
		if (i > 0 && (i == 4 || i % 10 == 0))
		{
			y += 38;
			x = 42;
		}
		Gui::sprite(0, sprites_itemHole_idx, x - 16, y - 16);
		if (inventoryData[i].second > -1)
		{
			ItemManagement::DrawItem(inventoryData[i].second, x, y, 1 , 1);
		}
		x += 38;
	}
}

void ItemViewer::DisplayIslandBox(void) const {
	islandBoxData = EditorUtils::load_player_islandbox(selectedPassedPlayer, currentBox);
	std::string itemName = islandBoxData[currentItem].first;
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("ITEM_ISLAND") + std::to_string(currentBox+1), 400);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, itemName, 390);
	Gui::DrawBottom();
	for (int i = 0; i < 10; ++i) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				Gui::sprite(0, sprites_itemHole_idx, 25 + x * 58 - 16, 75 + y * 58 - 16);
				if (islandBoxData[i].second > -1)
				{
					ItemManagement::DrawItem(islandBoxData[i].second, 25 + x * 58, 75 + y * 58, 1 , 1);
				}
			}
		}
	}

	DrawSlotSelection();
	Gui::DrawString(305, 85, 0.8f, WHITE, std::to_string(currentRow + 1) + "\n-\n" + std::to_string(MAX_ISLANDBOX_ROWS), 390);
	Gui::DrawArrow(295, 5);
	Gui::DrawArrow(315, 235, 180.0);
}

void ItemViewer::DisplayDresser(void) const {
	dresserData = EditorUtils::load_player_dresseritems(selectedPassedPlayer, currentBox);
	std::string itemName = dresserData[currentItem].first;
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("ITEM_DRESSER") + std::to_string(currentBox+1), 400);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, itemName, 390);
	Gui::DrawBottom();
	for (int i = 0; i < 10; ++i) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				Gui::sprite(0, sprites_itemHole_idx, 25 + x * 58 - 16, 75 + y * 58 - 16);
				if (dresserData[i].second > -1)
				{
					ItemManagement::DrawItem(dresserData[i].second, 25 + x * 58, 75 + y * 58, 1 , 1);
				}
			}
		}
	}

	DrawSlotSelection();
	Gui::DrawString(305, 85, 0.8f, WHITE, std::to_string(currentRow + 1) + "\n-\n" + std::to_string(MAX_DRESSER_ROWS), 390);
	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(catPos[i].x, catPos[i].y, catPos[i].w, catPos[i].h, UNSELECTED_COLOR);
		if (selectedCategory == i) {
			Gui::Draw_Rect(catPos[i].x, catPos[i].y, catPos[i].w, catPos[i].h, LIGHTER_COLOR);
		}
	}
	Gui::DrawArrow(295, 5);
	Gui::DrawArrow(315, 235, 180.0);
}

void ItemViewer::DisplayStorage(void) const {
	storageData = EditorUtils::load_player_storageitems(selectedPassedPlayer, currentBox);
	std::string itemName = storageData[currentItem].first;
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("ITEM_STORAGE") + std::to_string(currentBox+1), 400);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, itemName, 390);
	Gui::DrawBottom();
	for (int i = 0; i < 10; ++i) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				Gui::sprite(0, sprites_itemHole_idx, 25 + x * 58 - 16, 75 + y * 58 - 16);
				if (storageData[i].second > -1)
				{
					ItemManagement::DrawItem(storageData[i].second, 25 + x * 58, 75 + y * 58, 1 , 1);
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		Gui::Draw_Rect(catPos[i].x, catPos[i].y, catPos[i].w, catPos[i].h, UNSELECTED_COLOR);
		if (selectedCategory == i) {
			Gui::Draw_Rect(catPos[i].x, catPos[i].y, catPos[i].w, catPos[i].h, LIGHTER_COLOR);
		}
	}

	DrawSlotSelection();
	Gui::DrawString(305, 85, 0.8f, WHITE, std::to_string(currentRow + 1) + "\n-\n" + std::to_string(MAX_STORAGE_ROWS), 390);
	Gui::DrawArrow(295, 5);
	Gui::DrawArrow(315, 235, 180.0);
}

void ItemViewer::SubMenuLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		ItemManagement::UnloadItemDatabase();
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		switch(Selection) {
			case 0:
				itemMode = 1;
				break;
			case 1:
				itemMode = 2;
				break;
			case 2:
				itemMode = 3;
				break;
			case 3:
				itemMode = 4;
				break;
		}
	}

	// Selection.
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	}
	if (hDown & KEY_DOWN) {
			if(Selection < 5)	Selection++;
	}
	if (hDown & KEY_RIGHT) {
		if (Selection < 3) {
			Selection += 3;
		}
	}
	if (hDown & KEY_LEFT) {
		if (Selection < 6 && Selection > 2) {
			Selection -= 3;
		}
	}
	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			itemMode = 1;
		} else if (touching(touch, mainButtons[1])) {
			itemMode = 2;
		} else if (touching(touch, mainButtons[2])) {
			itemMode = 3;
		} else if (touching(touch, mainButtons[3])) {
			itemMode = 4;
		}
	}
}

void ItemViewer::PocketLogic(u32 hDown) {
	if (hDown & KEY_B) {
		itemMode = 0;
	}
}

void ItemViewer::islandBoxLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		currentBox = 0;
		currentRow = 0;
		selectedCategory = 0;
		currentItem = 0;
		itemMode = 0;
	}

	if (hDown & KEY_A) {
		Item* item = &Save::Instance()->players[selectedPassedPlayer]->IslandBox[currentItem];
		passItemData = std::make_pair(item->GetName(), item->GetSpritesheetID());
		Gui::setScreen(std::make_unique<ItemEditor>());
	}

	if (currentBox == 0) {
		currentRow = 0;
	} else if (currentBox == 1) {
		currentRow = 1;
	} else if (currentBox == 2) {
		currentRow = 2;
	} else if (currentBox == 3) {
		currentRow = 3;
	}

	if (hDown & KEY_R) {
		if(currentBox < maxIsland)	currentBox++;
	} else if (hDown & KEY_L) {
		if (currentBox > 0) currentBox--;
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, arrowPos[0])) {
			if (currentBox > 0)	currentBox--;
		} else if (touching(touch, arrowPos[1])) {
			if(currentBox < maxIsland)	currentBox++;
		}
	}

	if (hDown & KEY_RIGHT) {
		if(currentItem < 9)	currentItem++;
	} else if (hDown & KEY_UP) {
		if (currentItem > 4)	currentItem -= 5;
	} else if (hDown & KEY_DOWN) {
		if (currentItem < 5)	currentItem += 5;
	} else if (hDown & KEY_LEFT) {
		if (currentItem > 0) currentItem--;
	}
}

void ItemViewer::DresserLogic(u32 hDown, touchPosition touch) {
	if (currentBox < 6) {
		selectedCategory = 0;
	} else if (currentBox > 5 && currentBox < 12) {
		selectedCategory = 1;
	} else {
		selectedCategory = 2;
	}

	if (currentBox == 0 || currentBox == 6 || currentBox == 12) {
		currentRow = 0;
	} else if (currentBox == 1 || currentBox == 7 || currentBox == 13) {
		currentRow = 1;
	} else if (currentBox == 2 || currentBox == 8 || currentBox == 14) {
		currentRow = 2;
	} else if (currentBox == 3 || currentBox == 9 || currentBox == 15) {
		currentRow = 3;
	} else if (currentBox == 4 || currentBox == 10 || currentBox == 16) {
		currentRow = 4;
	} else if (currentBox == 5 || currentBox == 11 || currentBox == 17) {
		currentRow = 5;
	}

	if (hDown & KEY_B) {
		currentBox = 0;
		currentRow = 0;
		selectedCategory = 0;
		currentItem = 0;
		itemMode = 0;
	}

	if (hDown & KEY_A) {
		Item* item = &Save::Instance()->players[selectedPassedPlayer]->Dresser[currentItem];
		passItemData = std::make_pair(item->GetName(), item->GetSpritesheetID());
		Gui::setScreen(std::make_unique<ItemEditor>());
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 3; i++) {
			if (touching(touch, catPos[i])) {
				selectedCategory = i;
				currentBox = i*6;
			}
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, arrowPos[0])) {
			if (currentBox > 0)	currentBox--;
		} else if (touching(touch, arrowPos[1])) {
			if(currentBox < maxDresser)	currentBox++;
		}
	}

	if (hDown & KEY_R) {
		if(currentBox < maxDresser)	currentBox++;
	} else if (hDown & KEY_L) {
		if (currentBox > 0) currentBox--;
	}

	if (hDown & KEY_RIGHT) {
		if(currentItem < 9)	currentItem++;
	} else if (hDown & KEY_UP) {
		if (currentItem > 4)	currentItem -= 5;
	} else if (hDown & KEY_DOWN) {
		if (currentItem < 5)	currentItem += 5;
	} else if (hDown & KEY_LEFT) {
		if (currentItem > 0) currentItem--;
	}
}


void ItemViewer::storageLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		currentBox = 0;
		currentRow = 0;
		selectedCategory = 0;
		currentItem = 0;
		itemMode = 0;
	}

	if (currentBox < 9) {
		selectedCategory = 0;
	} else if (currentBox > 8 && currentBox < 18) {
		selectedCategory = 1;
	} else if (currentBox > 17 && currentBox < 27) {
		selectedCategory = 2;
	} else {
		selectedCategory = 3;
	}

	if (currentBox == 0 || currentBox == 9 || currentBox == 18 || currentBox == 27 || currentBox == 36) {
		currentRow = 0;
	} else if (currentBox == 1 || currentBox == 10 || currentBox == 19 || currentBox == 28) {
		currentRow = 1;
	} else if (currentBox == 2 || currentBox == 11 || currentBox == 20 || currentBox == 29) {
		currentRow = 2;
	} else if (currentBox == 3 || currentBox == 12 || currentBox == 21 || currentBox == 30) {
		currentRow = 3;
	} else if (currentBox == 4 || currentBox == 13 || currentBox == 22 || currentBox == 31) {
		currentRow = 4;
	} else if (currentBox == 5 || currentBox == 14 || currentBox == 23 || currentBox == 32) {
		currentRow = 5;
	} else if (currentBox == 6 || currentBox == 15 || currentBox == 24 || currentBox == 33) {
		currentRow = 6;
	} else if (currentBox == 7 || currentBox == 16 || currentBox == 25 || currentBox == 34) {
		currentRow = 7;
	} else if (currentBox == 8 || currentBox == 17 || currentBox == 26 || currentBox == 35) {
		currentRow = 8;
	}

	if (hDown & KEY_R) {
		if(currentBox < maxStorage)	currentBox++;
	} else if (hDown & KEY_L) {
		if (currentBox > 0) currentBox--;
	}

	if (hDown & KEY_A) {
		Item* item = &Save::Instance()->players[selectedPassedPlayer]->Storage[currentItem];
		passItemData = std::make_pair(item->GetName(), item->GetSpritesheetID());
		Gui::setScreen(std::make_unique<ItemEditor>());
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 4; i++) {
			if (touching(touch, catPos[i])) {
				selectedCategory = i;
				currentBox = i*9;
			}
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, arrowPos[0])) {
			if (currentBox > 0)	currentBox--;
		} else if (touching(touch, arrowPos[1])) {
			if(currentBox < maxStorage)	currentBox++;
		}
	}

	if (hDown & KEY_RIGHT) {
		if(currentItem < 9)	currentItem++;
	} else if (hDown & KEY_UP) {
		if (currentItem > 4)	currentItem -= 5;
	} else if (hDown & KEY_DOWN) {
		if (currentItem < 5)	currentItem += 5;
	} else if (hDown & KEY_LEFT) {
		if (currentItem > 0) currentItem--;
	}
}