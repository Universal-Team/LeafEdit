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

#include "itemManager.hpp"
#include "itemUtils.hpp"
#include "Sav.hpp"
#include "screenCommon.hpp"
#include "spriteManagement.hpp"
#include "villagerEditorWW.hpp"
#include "villagerSelection.hpp"

extern std::unique_ptr<Overlay> overlay;
extern std::vector<std::string> g_villagerDatabase;
extern std::vector<std::string> g_personality;
extern bool touching(touchPosition touch, ButtonType button);
extern std::vector<std::pair<u16, std::string>> itemDB;
extern std::shared_ptr<Sav> save;

void VillagerEditorWW::Draw(void) const {
	if (villagerMode == 0) {
		DrawSubMenu();
	} else {
		DrawItems();
	}
}

const std::string getPersonality(u8 personality) {
	return g_personality[personality];
}

void VillagerEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (villagerMode == 0) {
		subLogic(hDown, hHeld, touch);
	} else {
		ItemLogic(hDown, hHeld, touch);
	}
}

void VillagerEditorWW::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9, WHITE, "LeafEdit - Villager Editor", 390, 0, font);
	SpriteManagement::DrawVillager(this->villager->id(), 165, 35);
	Gui::DrawStringCentered(0, 100, 0.9f, BLACK, "Villager Name: " + g_villagerDatabase[this->villager->id()], 395, 0, font);
	Gui::DrawStringCentered(0, 130, 0.9f, BLACK, "Personality: " + getPersonality(this->villager->personality()), 395, 0, font);
	Gui::DrawStringCentered(0, 160, 0.9f, BLACK, "Catchphrase: ", 395, 0, font);
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x+100, mainButtons[i].y+30);
	}
}

void VillagerEditorWW::subLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Selection.
	if (hDown & KEY_UP) {
		if (Selection > 0)	Selection--;
	}
	
	if (hDown & KEY_DOWN) {
		if (Selection < 5)	Selection++;
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

	if (hDown & KEY_A) {
		u8 tempSelect = 0;
		switch (Selection) {
			case 0:
				overlay = std::make_unique<VillagerSelection>(this->villager, SaveType::WW);
				break;
			case 1:
				tempSelect = (u8)GFX::ListSelection(this->villager->personality(), g_personality, "Select the wanted personality.");
				this->villager->personality(tempSelect);
				break;
			case 2:
				// Get Furniture Items.
				for (int i = 0; i < 10; i++) {
					this->villagerItems[i] = villager->furniture(i);
				}
				// Get other stuff.
				this->villagerItems[10] = villager->wallpaper();
				this->villagerItems[11] = villager->carpet();
				this->villagerItems[12] = villager->song();
				this->villagerItems[13] = villager->shirt();
				this->villagerItems[14] = villager->umbrella();

				villagerMode = 1;
				break;
		}
	}


	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}

void VillagerEditorWW::DrawItems(void) const {
	std::string itemList;
	GFX::DrawFileBrowseBG(true);
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9, WHITE, "Current Item: " + ItemUtils::getName(this->villagerItems[itemSelection]->id()), 390, 0, font);

	for (int i=(itemIndex<8) ? 0 : (int)itemIndex-8;i<(int)itemDB.size()&&i<(((int)itemIndex<8) ? 9 : (int)itemIndex+1);i++) {
		itemList += itemDB[i].second + "\n";
	}

	for (uint i=0;i<((itemDB.size()<9) ? 9-itemDB.size() : 0);i++) {
		itemList += "\n";
	}

	// Selector Logic.
	if (itemIndex < 9)	GFX::DrawSelector(true, 24 + ((int)itemIndex * 21));
	else				GFX::DrawSelector(true, 24 + (8 * 21));
	Gui::DrawString(5, 25, 0.85f, BLACK, itemList, 360, 0, font);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, std::to_string(itemIndex + 1) + " | " + std::to_string(itemDB.size()), 395, 0, font);


	GFX::DrawBottom();
	for (int i = 0; i < 15; i++) {
		GFX::drawGrid(items[i].x, items[i].y, items[i].w, items[i].h, ItemManager::getColor(this->villagerItems[i]->itemtype()), C2D_Color32(0, 0, 0, 255));
	}

	GFX::DrawGUI(gui_pointer_idx, items[itemSelection].x+15, items[itemSelection].y+15);
}

void VillagerEditorWW::ItemLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (keyRepeatDelay)	keyRepeatDelay--;

	if (hDown & KEY_SELECT) {
		if (isList)	isList = false;
		else		isList = true;
	}

	if (isList) {
		if (hHeld & KEY_DOWN && !keyRepeatDelay) {
			if (itemIndex < itemDB.size()-1)	itemIndex++;
			else	itemIndex = 0;
			keyRepeatDelay = 6;
		}

		if (hHeld & KEY_UP && !keyRepeatDelay) {
			if (itemIndex > 0)	itemIndex--;
			else if (itemIndex == 0)	itemIndex = itemDB.size()-1;
			keyRepeatDelay = 6;
		}


		if (hHeld & KEY_RIGHT && !keyRepeatDelay) {
			if ((itemIndex + 9) > itemDB.size()-1) {
				itemIndex = itemDB.size()-1;
			} else {
				itemIndex += 9;
			}
			keyRepeatDelay = 6;
		}

		if (hDown & KEY_A) {
			this->villagerItems[itemSelection]->id(itemDB[itemIndex].first);
			changes = true;
		}
	} else {
		if (hHeld & KEY_RIGHT && !keyRepeatDelay) {
			if (itemSelection < 14) {
				itemSelection++;
				keyRepeatDelay = 6;
			}
		}

		if (hHeld & KEY_LEFT && !keyRepeatDelay) {
			if (itemSelection > 0) {
				itemSelection--;
				keyRepeatDelay = 6;
			}
		}

		if (hDown & KEY_B) {
			villagerMode = 0;
			// Reset Items to nullptr.
			for (int i = 0; i < 15; i++) {
				this->villagerItems[i] = nullptr;
			}
		}

		if (hDown & KEY_Y) {
			itemIndex = ItemManager::getIndex(this->villagerItems[itemSelection]->id());
		}
	}
}