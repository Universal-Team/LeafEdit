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

#include "itemManager.hpp"
#include "overlay.hpp"
#include "screenCommon.hpp"
#include "spriteManagement.hpp"
#include "villagerEditorNL.hpp"

extern std::vector<std::tuple<u16, std::string, std::string>> villagerDB;
extern std::vector<std::string> g_personality;
extern const std::string getPersonality(u8 personality);
extern bool touching(touchPosition touch, ButtonType button);

/* Bring that to other screens too. */
extern SaveType savesType;
extern const std::string getVillagerName(int index);

void VillagerEditorNL::Draw(void) const {
	if (this->villagerMode == 0) this->DrawSubMenu();
	else if (this->villagerMode == 1) this->DrawItems();
}

void VillagerEditorNL::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->villagerMode == 0) this->subLogic(hDown, hHeld, touch);
	else if (this->villagerMode == 1) this->ItemLogic(hDown, hHeld, touch);
}

void VillagerEditorNL::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9, WHITE, "LeafEdit - " + Lang::get("VILLAGER_EDITOR"), 390, 0, font);
	SpriteManagement::DrawVillager(this->villager->id(), 165, 35);

	Gui::DrawStringCentered(0, 100, 0.9f, BLACK, Lang::get("VILLAGER_NAME") + std::get<1>(villagerDB[this->villager->id()]), 395, 0, font);

	Gui::DrawStringCentered(0, 130, 0.9f, BLACK, Lang::get("VILLAGER_PERSONALITY") + ": " + getPersonality(this->villager->personality()), 395, 0, font);
	Gui::DrawStringCentered(0, 160, 0.9f, BLACK, Lang::get("VILLAGER_CATCHPHRASE") + ": ", 395, 0, font);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == this->Selection) GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x + 100, mainButtons[i].y + 30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void VillagerEditorNL::subLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	/* Selection. */
	if (hRepeat & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}
	
	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 5) this->Selection++;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->Selection < 3) {
			this->Selection += 3;
		}
	}

	if (hRepeat & KEY_LEFT) {
		if (this->Selection < 6 && this->Selection > 2) {
			this->Selection -= 3;
		}
	}

	if (hDown & KEY_A) {
		u16 tempSelect = 0;
		switch (Selection) {
			case 0:
				this->villager->id(Overlays::SelectVillager(this->villager->id(), savesType));
				break;
			case 1:
				tempSelect = (u8)GFX::ListSelection(this->villager->personality(), g_personality, Lang::get("VILLAGER_PERSONALITY_SELECT"));
				this->villager->personality((u8)tempSelect);
				break;
			case 2:
				/* Get Furniture Items. */
				for (int i = 0; i < 15; i++) {
					this->villagerItems[i] = this->villager->furniture(i);
				}

				/* Get other stuff. */
				this->villagerItems[15] = this->villager->wallpaper();
				this->villagerItems[16] = this->villager->carpet();
				this->villagerItems[17] = this->villager->song();
				this->villagerItems[18] = this->villager->shirt();
				this->villagerItems[19] = this->villager->umbrella();

				this->villagerMode = 1;
				break;
		}
	}


	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}
}

void VillagerEditorNL::DrawItems(void) const {
	GFX::DrawTop();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	for (int i = 0; i < 20; i++) {
		GFX::drawGrid(items[i].x, items[i].y, items[i].w, items[i].h, ItemManager::getColor(this->villagerItems[i]->itemtype()), C2D_Color32(0, 0, 0, 255));
	}

	GFX::DrawGUI(gui_pointer_idx, items[this->itemSelection].x + 15, items[this->itemSelection].y + 15);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void VillagerEditorNL::ItemLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hDown & KEY_A) {
		if (this->itemSelection < 15) {
			/* Villager Furniture. */
			this->villagerItems[this->itemSelection]->id(Overlays::SelectItemCategory(this->villagerItems[this->itemSelection]->id(), savesType, false, {37}, "Select a Furniture."));
		} else if (this->itemSelection == 15) {
			/* Villager Wallpaper. */
			this->villagerItems[15]->id(Overlays::SelectItemCategory(this->villagerItems[15]->id(), savesType, false, {25}, "Select a Wallpaper."));
		} else if (this->itemSelection == 16) {
			/* Villager Carpet. */
			this->villagerItems[16]->id(Overlays::SelectItemCategory(this->villagerItems[16]->id(), savesType, false, {26}, "Select a Carpet."));
		} else if (this->itemSelection == 17) {
			/* Villager Song. */
			this->villagerItems[17]->id(Overlays::SelectItemCategory(this->villagerItems[17]->id(), savesType, false, {17}, "Select a Song."));
		} else if (this->itemSelection == 18) {
			/* Villager Shirt. */
			this->villagerItems[18]->id(Overlays::SelectItemCategory(this->villagerItems[18]->id(), savesType, false, {27}, "Select a Shirt."));
		} else if (this->itemSelection == 19) {
			/* Villager Umbrella. */
			this->villagerItems[19]->id(Overlays::SelectItemCategory(this->villagerItems[19]->id(), savesType, false, {33}, "Select a Umbrella."));
		}
	}
		
	if (hRepeat & KEY_RIGHT) {
		if (this->itemSelection < 19) {
			this->itemSelection++;
		}
	}

	if (hRepeat & KEY_LEFT) {
		if (this->itemSelection > 0) {
			this->itemSelection--;
		}
	}

	if (hDown & KEY_B) {
		this->villagerMode = 0;
		/* Reset Items to nullptr. */
		for (int i = 0; i < 20; i++) {
			this->villagerItems[i] = nullptr;
		}
	}
}