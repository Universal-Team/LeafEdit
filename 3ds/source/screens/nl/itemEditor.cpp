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

#include "gfx.hpp"
#include "gui.hpp"
#include "itemEditor.hpp"
#include "itemManagement.hpp"
#include "keyboard.hpp"
#include "player.hpp"
#include "save.hpp"
#include "utils.hpp"

#include <3ds.h>
#include <map>

extern Save* SaveFile;
extern std::map<u16, std::string> g_itemDatabase;

#define MAX_DRESSER_ROWS	6
#define MAX_STORAGE_ROWS	9
#define MAX_ISLANDBOX_ROWS	4

extern int selectedPassedPlayer;
#define maxDresser 17 // 17, because it starts with 0, so it would be basically 18.
#define maxIsland 3
#define maxStorage 35 // Is that correct with 36 Boxes? (360)?

extern bool touching(touchPosition touch, Structs::ButtonPos button);

// test: 8471 --99.000 Bells.

// Only load item stuff, when accessing this screen and also unload by exit of that screen.
ItemEditor::ItemEditor() {
	ItemManagement::loadItems();

	// Load Dresser.
	for (int i = 0; i < 180; i++) {
		this->Dresser[i] = std::make_shared<ItemContainer>(Save::Instance()->players[selectedPassedPlayer]->Dresser[i]);
	}

	// Load Pocket.
	for (int i = 0; i < 16; i++) {
		this->Pocket[i] = std::make_shared<ItemContainer>(Save::Instance()->players[selectedPassedPlayer]->Pockets[i]);
	}

	// Load Islandbox.
	for (int i = 0; i < 40; i++) {
		this->islandBox[i] = std::make_shared<ItemContainer>(Save::Instance()->players[selectedPassedPlayer]->IslandBox[i]);
	}

	// Load Storage.
	for (int i = 0; i < 360; i++) {
		this->Storage[i] = std::make_shared<ItemContainer>(Save::Instance()->players[selectedPassedPlayer]->Storage[i]);
	}
}

ItemEditor::~ItemEditor()
{
	ItemManagement::unloadItems();
}

void ItemEditor::Draw(void) const {
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

void ItemEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (itemMode == 0) {
		SubMenuLogic(hDown, hHeld, touch);
	} else if (itemMode == 1) {
		PocketLogic(hDown, hHeld);
	} else if (itemMode == 2) {
		islandBoxLogic(hDown, hHeld, touch);
	} else if (itemMode == 3) {
		DresserLogic(hDown, hHeld, touch);
	} else if (itemMode == 4) {
		storageLogic(hDown, hHeld, touch);
	}
}

// Selection of the Item for IslandBox, Dresser & Storage.
void ItemEditor::DrawSlotSelection(void) const {
	int selectY = 0, selectX = 0;
	if (currentItem < 5)	selectY = 0;	else	selectY = 1;
	if (currentItem > 4)	selectX = currentItem - 5;	else	selectX = currentItem;
	GFX::DrawSpriteBlend(sprites_itemHole_idx, 25 + selectX * 58 - 16, 75 + selectY * 58 - 16, DARKER_COLOR);
}

void ItemEditor::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEMS"), 400);
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, mainButtons[i].x+130, mainButtons[i].y+25);
		}
	}
	// Pocket. (16)
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8f, Lang::get("ITEM_POCKET")))/2-80+17.5, 0.8, WHITE, Lang::get("ITEM_POCKET"), 130, 25);
	// Island. (40)
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("ITEM_ISLAND")))/2-20+17.5, 0.8, WHITE, Lang::get("ITEM_ISLAND"), 130, 25);
	// Dresser. (180)
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("ITEM_DRESSER")))/2+75-17.5, 0.8, WHITE, Lang::get("ITEM_DRESSER"), 130, 25);
	// Storage. (360?)
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("ITEM_STORAGE")))/2-80+17.5, 0.8, WHITE, Lang::get("ITEM_STORAGE"), 130, 25);
}

void ItemEditor::DisplayPocket(void) const {
	int x = 42;
	int y = 63;
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEM_POCKET"), 400);
	GFX::DrawBottom();
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
		GFX::DrawSprite(sprites_itemHole_idx, x - 16, y - 16);
		if (this->Pocket[i]->returnID() > -1)
		{
			ItemManagement::DrawItem(this->Pocket[i]->returnID(), x, y, 1 , 1);
		}
		x += 38;
	}
}

void ItemEditor::DisplayIslandBox(void) const {
	std::string itemName = this->islandBox[currentItem]->returnName();
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEM_ISLAND") + std::to_string(currentBox+1), 400);
	Gui::DrawStringCentered(0, 214, 0.9f, WHITE, itemName, 390);
	GFX::DrawBottom();
	for (int i = 0; i < 10; ++i) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				GFX::DrawSprite(sprites_itemHole_idx, 25 + x * 58 - 16, 75 + y * 58 - 16);
				if (this->islandBox[i]->returnID() > -1)
				{
					ItemManagement::DrawItem(this->islandBox[i]->returnID(), 25 + x * 58, 75 + y * 58, 1 , 1);
				}
			}
		}
	}

	DrawSlotSelection();
	Gui::DrawString(305, 85, 0.9f, WHITE, std::to_string(currentRow + 1) + "\n-\n" + std::to_string(MAX_ISLANDBOX_ROWS), 390);
	GFX::DrawArrow(295, 5);
	GFX::DrawArrow(315, 235, 180.0);
}

void ItemEditor::DisplayDresser(void) const {
	std::string itemName = this->Dresser[currentItem]->returnName();
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEM_DRESSER") + std::to_string(currentBox+1), 400);
	Gui::DrawStringCentered(0, 214, 0.9f, WHITE, itemName, 390);
	GFX::DrawBottom();
	for (int i = 0; i < 10; ++i) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				GFX::DrawSprite(sprites_itemHole_idx, 25 + x * 58 - 16, 75 + y * 58 - 16);
				if (this->Dresser[i]->returnID() > -1)
				{
					ItemManagement::DrawItem(this->Dresser[i]->returnID(), 25 + x * 58, 75 + y * 58, 1 , 1);
				}
			}
		}
	}

	DrawSlotSelection();
	Gui::DrawString(305, 85, 0.9f, WHITE, std::to_string(currentRow + 1) + "\n-\n" + std::to_string(MAX_DRESSER_ROWS), 390);
	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(catPos[i].x, catPos[i].y, catPos[i].w, catPos[i].h, UNSELECTED_COLOR);
		if (selectedCategory == i) {
			Gui::Draw_Rect(catPos[i].x, catPos[i].y, catPos[i].w, catPos[i].h, LIGHTER_COLOR);
		}
	}
	GFX::DrawArrow(295, 5);
	GFX::DrawArrow(315, 235, 180.0);
}

void ItemEditor::DisplayStorage(void) const {
	std::string itemName = this->Storage[currentItem]->returnName();
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEM_STORAGE") + std::to_string(currentBox+1), 400);
	Gui::DrawStringCentered(0, 214, 0.9f, WHITE, itemName, 390);
	GFX::DrawBottom();
	for (int i = 0; i < 10; ++i) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				GFX::DrawSprite(sprites_itemHole_idx, 25 + x * 58 - 16, 75 + y * 58 - 16);
				if (this->Storage[i]->returnID() > -1)
				{
					ItemManagement::DrawItem(this->Storage[i]->returnID(), 25 + x * 58, 75 + y * 58, 1 , 1);
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
	Gui::DrawString(305, 85, 0.9f, WHITE, std::to_string(currentRow + 1) + "\n-\n" + std::to_string(MAX_STORAGE_ROWS), 390);
	GFX::DrawArrow(295, 5);
	GFX::DrawArrow(315, 235, 180.0);
}

void ItemEditor::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
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

void ItemEditor::PocketLogic(u32 hDown, u32 hHeld) {
	if (hDown & KEY_B) {
		itemMode = 0;
	}

	if (hDown & KEY_X) {
		ItemManagement::SetID(SaveFile->players[selectedPassedPlayer]->Pockets[0], 8471);
		ItemManagement::SetFlag(SaveFile->players[selectedPassedPlayer]->Pockets[0], 0);
		SaveFile->players[selectedPassedPlayer]->Pockets[0]->Write();
		ItemManagement::RefreshItem(this->Pocket[0]);
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("B_BACK"));
	}
}

void ItemEditor::islandBoxLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		currentBox = 0;
		currentRow = 0;
		selectedCategory = 0;
		currentItem = 0;
		itemMode = 0;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("B_BACK") + "\n" + Lang::get("LR_SWITCH_PAGE"));
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

void ItemEditor::DresserLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (currentBox < 6) {
		selectedCategory = 0;
	} else if (currentBox > 5 && currentBox < 12) {
		selectedCategory = 1;
	} else {
		selectedCategory = 2;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("B_BACK") + "\n" + Lang::get("LR_SWITCH_PAGE"));
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


void ItemEditor::storageLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		currentBox = 0;
		currentRow = 0;
		selectedCategory = 0;
		currentItem = 0;
		itemMode = 0;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("B_BACK") + "\n" + Lang::get("LR_SWITCH_PAGE"));
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