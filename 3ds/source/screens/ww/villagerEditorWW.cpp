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
#include "spriteManagement.hpp"
#include "villagerEditorWW.hpp"

extern std::vector<std::tuple<u16, std::string, std::string>> villagerDB;
extern std::vector<std::string> g_personality;

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

void VillagerEditorWW::Draw(void) const {
	switch(this->villagerMode) {
		case 0:
			this->DrawSubMenu();
			break;

		case 1:
			this->DrawItems();
			break;
	}
}

void VillagerEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	switch(this->villagerMode) {
		case 0:
			this->subLogic(hDown, hHeld, touch);
			break;

		case 1:
			this->ItemLogic(hDown, hHeld, touch);
			break;
	}
}

const std::string getPersonality(u8 personality) {
	return g_personality[personality];
}

void VillagerEditorWW::DrawSubMenu(void) const {
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
		if (i == this->Selection) GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x+100, mainButtons[i].y+30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void VillagerEditorWW::subLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}
	
	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 5) this->Selection++;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->Selection < 3) this->Selection += 3;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->Selection < 6 && this->Selection > 2) this->Selection -= 3;
	}

	if (hDown & KEY_A) {
		u8 tempSelect = 0;
		switch (this->Selection) {
			case 0:
				this->villager->id((u8)Overlays::SelectVillager(this->villager->id(), SaveType::WW));
				break;

			case 1:
				tempSelect = (u8)GFX::ListSelection(this->villager->personality(), g_personality, Lang::get("VILLAGER_PERSONALITY_SELECT"));
				this->villager->personality(tempSelect);
				break;

			case 2:
				/* Get Furniture Items. */
				for (int i = 0; i < 10; i++) {
					this->villagerItems[i] = this->villager->furniture(i);
				}

				/* Get other item stuff. */
				this->villagerItems[10] = this->villager->shirt();

				this->miscItems[0] = this->villager->songWW();
				this->miscItems[1] = this->villager->wallpaperWW();
				this->miscItems[2] = this->villager->carpetWW();
				this->miscItems[3] = this->villager->umbrellaWW();

				this->villagerMode = 1;
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, this->mainButtons[0])) {
			this->villager->id((u8)Overlays::SelectVillager(this->villager->id(), SaveType::WW));


		} else if (touching(touch, this->mainButtons[1])) {
			u8 tempSelect = (u8)GFX::ListSelection(this->villager->personality(), g_personality, Lang::get("VILLAGER_PERSONALITY_SELECT"));
			this->villager->personality(tempSelect);


		} else if (touching(touch, this->mainButtons[2])) {
			/* Get Furniture Items. */
			for (int i = 0; i < 10; i++) {
				this->villagerItems[i] = this->villager->furniture(i);
			}

			/* Get other item stuff. */
			this->villagerItems[10] = this->villager->shirt();

			this->miscItems[0] = this->villager->songWW();
			this->miscItems[1] = this->villager->wallpaperWW();
			this->miscItems[2] = this->villager->carpetWW();
			this->miscItems[3] = this->villager->umbrellaWW();

			this->villagerMode = 1;
		}
	}


	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}
}

void VillagerEditorWW::DrawItems(void) const {
	GFX::DrawTop();
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209);

	/* Item Names. */
	switch(this->itemSelection) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			Gui::DrawStringCentered(0, 218, 0.8f, WHITE, Lang::get("CURRENT_ITEM") + this->villagerItems[this->itemSelection]->name(), 390, 0, font);
			break;

		case 11:
			Gui::DrawStringCentered(0, 218, 0.8f, WHITE, "Song hasn't been researched yet.", 390, 0, font);
			break;
		
		case 12:
			Gui::DrawStringCentered(0, 218, 0.8f, WHITE, Lang::get("CURRENT_ITEM") + ItemUtils::getWWName(17, this->miscItems[1]), 390, 0, font);
			break;
		
		case 13:
			Gui::DrawStringCentered(0, 218, 0.8f, WHITE, Lang::get("CURRENT_ITEM") + ItemUtils::getWWName(18, this->miscItems[2]), 390, 0, font);
			break;

		case 14:
			Gui::DrawStringCentered(0, 218, 0.8f, WHITE, Lang::get("CURRENT_ITEM") + ItemUtils::getWWName(26, this->miscItems[3]), 390, 0, font);
			break;
	}
		
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();
	for (int i = 0; i < 10; i++) {
		GFX::drawGrid(items[i].x, items[i].y, items[i].w, items[i].h, ItemManager::getColor(this->villagerItems[i]->itemtype()), C2D_Color32(0, 0, 0, 255));
	}

	GFX::drawGrid(items[10].x, items[10].y, items[10].w, items[10].h, ItemManager::getColor(ItemType::Clothes), C2D_Color32(0, 0, 0, 255)); // Shirt.
	GFX::drawGrid(items[11].x, items[11].y, items[11].w, items[11].h, ItemManager::getColor(ItemType::Song), C2D_Color32(0, 0, 0, 255)); // Song.
	GFX::drawGrid(items[12].x, items[12].y, items[12].w, items[12].h, ItemManager::getColor(ItemType::WallpaperCarpet), C2D_Color32(0, 0, 0, 255)); // Wallpaper.
	GFX::drawGrid(items[13].x, items[13].y, items[13].w, items[13].h, ItemManager::getColor(ItemType::WallpaperCarpet), C2D_Color32(0, 0, 0, 255)); // Carpet.
	GFX::drawGrid(items[14].x, items[14].y, items[14].w, items[14].h, ItemManager::getColor(ItemType::Clothes), C2D_Color32(0, 0, 0, 255)); // Umbrella.

	GFX::DrawGUI(gui_pointer_idx, items[this->itemSelection].x + 15, items[this->itemSelection].y + 15);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void VillagerEditorWW::ItemLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hDown & KEY_A) {
		switch(this->itemSelection) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				/* Villager furniture. */
				this->villagerItems[this->itemSelection]->id(Overlays::SelectItemCategory(this->villagerItems[this->itemSelection]->id(), SaveType::WW, false, {46}, "Select a Furniture."));
				break;

			case 10:
				/* Villager shirt. */
				this->villagerItems[10]->id(Overlays::SelectItemCategory(this->villagerItems[10]->id(), SaveType::WW, false, {19}, "Select a Shirt."));
				break;

			case 11:
				/* Song: TODO. */
				Msg::DisplayWaitMsg("Song hasn't been researched yet.");
				break;
			
			case 12:
				/* Villager Wallpaper. */
				this->miscItems[1] = Overlays::SelectWWCategory(this->miscItems[1], {17}, "Select a Wallpaper.");
				break;

			case 13:
				/* Villager Carpet. */
				this->miscItems[2] = Overlays::SelectWWCategory(this->miscItems[2], {18}, "Select a Carpet.");
				break;

			case 14:
				/* Villager Umbrella. */
				this->miscItems[3] = Overlays::SelectWWCategory(this->miscItems[3], {26}, "Select a Umbrella.");
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 15; i++) {
			if (iconTouch(touch, this->items[i])) {
				switch(i) {
					case 0:
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
					case 9:
						/* Villager furniture. */
						this->villagerItems[i]->id(Overlays::SelectItemCategory(this->villagerItems[i]->id(), SaveType::WW, false, {46}, "Select a Furniture."));
						break;

					case 10:
						/* Villager shirt. */
						this->villagerItems[10]->id(Overlays::SelectItemCategory(this->villagerItems[10]->id(), SaveType::WW, false, {19}, "Select a Shirt."));
						break;

					case 11:
						/* Song: TODO. */
						Msg::DisplayWaitMsg("Song hasn't been researched yet.");
						break;
			
					case 12:
						/* Villager Wallpaper. */
						this->miscItems[1] = Overlays::SelectWWCategory(this->miscItems[1], {17}, "Select a Wallpaper.");
						break;

					case 13:
						/* Villager Carpet. */
						this->miscItems[2] = Overlays::SelectWWCategory(this->miscItems[2], {18}, "Select a Carpet.");
						break;

					case 14:
						/* Villager Umbrella. */
						this->miscItems[3] = Overlays::SelectWWCategory(this->miscItems[3], {26}, "Select a Umbrella.");
						break;
				}
			}
		}
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->itemSelection < 14) this->itemSelection++;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->itemSelection > 0) this->itemSelection--;
	}

	if (hDown & KEY_B) {
		this->villagerMode = 0;
		/* Reset Items to nullptr. */
		for (int i = 0; i < 11; i++) {
			this->villagerItems[i] = nullptr;
		}

		for (int i = 0; i < 4; i++) {
			this->miscItems[i] = 0;
		}
	}
}