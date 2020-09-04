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

#include "itemEditorNL.hpp"
#include "itemManager.hpp"
#include "itemUtils.hpp"
#include "gfx.hpp"
#include "keyboard.hpp"
#include "overlay.hpp"
#include "spriteManagement.hpp"

extern std::unique_ptr<Config> config;
extern SaveType savesType;

#define MAX_DRESSER 17
#define MAX_ISLAND	3
#define MAX_STORAGE 35

extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

/* Only load item stuff, when accessing this screen and also unload by exit of that screen. */
ItemEditorNL::ItemEditorNL(std::unique_ptr<Player> &refPlayer) : player(refPlayer) {
	ItemUtils::loadItemBins();

	/* Loading Pocket here. */
	for (int i = 0; i < 16; i++) {
		this->pockets[i] = this->player->pocket(i);
	}

	/* Loading Dresser here. */
	for (int i = 0; i < 180; i++) {
		this->dresser[i] = this->player->dresser(i);
	}

	/* Loading Island Box here. */
	for (int i = 0; i < 40; i++) {
		this->islandBox[i] = this->player->islandbox(i);
	}

	/* Loading Storage here. WA only. */
	if (savesType == SaveType::WA) {
		for (int i = 0; i < 360; i++) {
			this->storage[i] = this->player->storage(i);
		}
	}
}

ItemEditorNL::~ItemEditorNL() { ItemUtils::closeItemBins(); }

void ItemEditorNL::Draw(void) const {
	switch(this->Mode) {
		case 0:
			this->DrawSubMenu();
			break;
		case 1:
			this->DrawPocket();
			break;
		case 2:
			this->DrawIslandBox();
			break;
		case 3:
			this->DrawDresser();
			break;
		case 4:
			this->DrawStorage();
			break;
	}
}

void ItemEditorNL::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	switch(this->Mode) {
		case 0:
			this->SubMenuLogic(hDown, hHeld, touch);
			break;
		case 1:
			this->PocketLogic(hDown, hHeld, touch);
			break;
		case 2:
			this->IslandBoxLogic(hDown, hHeld, touch);
			break;
		case 3:
			this->DresserLogic(hDown, hHeld, touch);
			break;
		case 4:
			this->StorageLogic(hDown, hHeld, touch);
			break;
	}
}

/* Selection of the Item for IslandBox, Dresser & Storage. */
void ItemEditorNL::DrawSlotSelection(void) const {
	if (this->Mode != 1) {
		GFX::DrawGUIBlend(gui_itemHole_idx, this->iconSlots[this->selectedItem].x, this->iconSlots[this->selectedItem].y, 1, 1, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
	} else {
		GFX::DrawGUIBlend(gui_itemHole_idx, this->pocketSlots[this->selectedItem].x, this->pocketSlots[this->selectedItem].y, 1, 1, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
	}
}


void ItemEditorNL::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEMS"), 395, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(this->mainButtons[i]);
		if (i == this->Selection) GFX::DrawGUI(gui_pointer_idx, this->mainButtons[i].x + 100, this->mainButtons[i].y + 30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void ItemEditorNL::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	/* Navigation. */
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

	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
		return;
	}

	
	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				this->Mode = 1;
				break;
			case 1:
				this->Mode = 2;
				break;
			case 2:
				this->Mode = 3;
				break;
			case 3:
				if (savesType == SaveType::WA) this->Mode = 4; // WA only.
				break;
		}
	}
}


void ItemEditorNL::DrawPocket(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEM_POCKET"), 395, 0, font);
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, Lang::get("CURRENT_ITEM") + this->pockets[this->selectedItem]->name(), 400, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	for (int i = 0; i < 16; i++) {
		GFX::DrawGUI(gui_itemHole_idx, this->pocketSlots[i].x, this->pocketSlots[i].y); // Draw Item Slots.
		if (ItemUtils::GetSpritesheetID(this->pockets[i]->id(), this->pockets[i]->flags()) > -1) {
			SpriteManagement::DrawItem(ItemUtils::GetSpritesheetID(this->pockets[i]->id(), this->pockets[i]->flags()), this->pocketSlots[i].x + 16, this->pocketSlots[i].y + 16, 1, 1);
		}
	}

	this->DrawSlotSelection();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void ItemEditorNL::PocketLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_RIGHT) {
		if (this->selectedItem < 15) this->selectedItem++;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->selectedItem > 0) this->selectedItem--;
	}

	if (hDown & KEY_B) {
		this->currentBox = 0;
		this->selectedItem = 0;
		this->Mode = 0;
	}


	if (hDown & KEY_A) {
		this->pockets[this->selectedItem]->id(Overlays::SelectItem(this->pockets[this->selectedItem]->id(), savesType, false));
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 16; i++) {
			if (iconTouch(touch, this->pocketSlots[i])) {
				this->pockets[i]->id(Overlays::SelectItem(this->pockets[i]->id(), savesType, false));
			}
		}
	}
}


void ItemEditorNL::DrawIslandBox(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEM_ISLAND") + std::to_string(currentBox + 1), 395, 0, font);
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, Lang::get("CURRENT_ITEM") + this->islandBox[this->selectedItem + (this->currentBox * 10)]->name(), 400, 0, font);

	GFX::DrawBottom();
	for (int i = 0 + (10 * this->currentBox); i < 10 + (10 * this->currentBox); i++) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				GFX::DrawGUI(gui_itemHole_idx, 25 + x * 58 - 16, 75 + y * 58 - 16); // Draw Item Slots.
				if (ItemUtils::GetSpritesheetID(this->islandBox[i]->id(), this->islandBox[i]->flags()) > -1) {
					SpriteManagement::DrawItem(ItemUtils::GetSpritesheetID(this->islandBox[i]->id(), this->islandBox[i]->flags()), 25 + x * 58, 75 + y * 58, 1 , 1);
				}
			}
		}
	}

	this->DrawSlotSelection();
}

void ItemEditorNL::IslandBoxLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_R) {
		if (this->currentBox < MAX_ISLAND) this->currentBox++;
	}

	if (hRepeat & KEY_L) {
		if (this->currentBox > 0) this->currentBox--;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->selectedItem < 9) this->selectedItem++;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->selectedItem > 0) this->selectedItem--;
	}

	if (hRepeat & KEY_UP) {
		if (this->selectedItem > 4) this->selectedItem -= 5;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->selectedItem < 5) this->selectedItem += 5;
	}

	if (hDown & KEY_B) {
		this->currentBox = 0;
		this->selectedItem = 0;
		this->Mode = 0;
	}

	if (hDown & KEY_A) {
		this->islandBox[this->selectedItem + (this->currentBox * 10)]->id(Overlays::SelectItem(this->islandBox[this->selectedItem + (this->currentBox * 10)]->id(), savesType));
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 10; i++) {
			if (iconTouch(touch, this->iconSlots[i])) {
				this->islandBox[i + (this->currentBox * 10)]->id(Overlays::SelectItem(this->islandBox[i + (this->currentBox * 10)]->id(), savesType));
			}
		}
	}
}


void ItemEditorNL::DrawDresser(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEM_DRESSER") + std::to_string(currentBox + 1), 395, 0, font);
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, Lang::get("CURRENT_ITEM") + this->dresser[this->selectedItem + (this->currentBox * 10)]->name(), 400, 0, font);

	GFX::DrawBottom();
	for (int i = 0 + (10 * this->currentBox); i < 10 + (10 * this->currentBox); i++) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				GFX::DrawGUI(gui_itemHole_idx, 25 + x * 58 - 16, 75 + y * 58 - 16); // Draw Item Slots.
				if (ItemUtils::GetSpritesheetID(this->dresser[i]->id(), this->dresser[i]->flags()) > -1) {
					SpriteManagement::DrawItem(ItemUtils::GetSpritesheetID(this->dresser[i]->id(), this->dresser[i]->flags()), 25 + x * 58, 75 + y * 58, 1 , 1);
				}
			}
		}
	}

	this->DrawSlotSelection();
}

void ItemEditorNL::DresserLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_R) {
		if (this->currentBox < MAX_DRESSER) this->currentBox++;
	}

	if (hRepeat & KEY_L) {
		if (this->currentBox > 0) this->currentBox--;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->selectedItem < 9) this->selectedItem++;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->selectedItem > 0) this->selectedItem--;
	}

	if (hRepeat & KEY_UP) {
		if (this->selectedItem > 4) this->selectedItem -= 5;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->selectedItem < 5) this->selectedItem += 5;
	}

	if (hDown & KEY_B) {
		this->currentBox = 0;
		this->selectedItem = 0;
		this->Mode = 0;
	}

	if (hDown & KEY_A) {
		this->dresser[this->selectedItem + (this->currentBox * 10)]->id(Overlays::SelectItem(this->dresser[this->selectedItem + (this->currentBox * 10)]->id(), savesType));
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 10; i++) {
			if (iconTouch(touch, this->iconSlots[i])) {
				this->dresser[i + (this->currentBox * 10)]->id(Overlays::SelectItem(this->dresser[i + (this->currentBox * 10)]->id(), savesType));
			}
		}
	}
}


void ItemEditorNL::DrawStorage(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEM_STORAGE") + std::to_string(currentBox + 1), 395, 0, font);
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, Lang::get("CURRENT_ITEM") + this->storage[this->selectedItem + (this->currentBox * 10)]->name(), 400, 0, font);

	GFX::DrawBottom();
	for (int i = 0 + (10 * this->currentBox); i < 10 + (10 * this->currentBox); i++) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				GFX::DrawGUI(gui_itemHole_idx, 25 + x * 58 - 16, 75 + y * 58 - 16); // Draw Item Slots.
				if (ItemUtils::GetSpritesheetID(this->storage[i]->id(), this->storage[i]->flags()) > -1) {
					SpriteManagement::DrawItem(ItemUtils::GetSpritesheetID(this->storage[i]->id(), this->storage[i]->flags()), 25 + x * 58, 75 + y * 58, 1 , 1);
				}
			}
		}
	}

	this->DrawSlotSelection();
}

void ItemEditorNL::StorageLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_R) {
		if (this->currentBox < MAX_STORAGE) this->currentBox++;
	}

	if (hRepeat & KEY_L) {
		if (this->currentBox > 0) this->currentBox--;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->selectedItem < 9) this->selectedItem++;
	}

	if (hDown & KEY_LEFT) {
		if (this->selectedItem > 0) this->selectedItem--;
	}

	if (hRepeat & KEY_UP) {
		if (this->selectedItem > 4) this->selectedItem -= 5;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->selectedItem < 5) this->selectedItem += 5;
	}

	if (hDown & KEY_B) {
		this->currentBox = 0;
		this->selectedItem = 0;
		this->Mode = 0;
	}

	if (hDown & KEY_A) {
		this->storage[this->selectedItem + (this->currentBox * 10)]->id(Overlays::SelectItem(this->storage[this->selectedItem + (this->currentBox * 10)]->id(), savesType));
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 10; i++) {
			if (iconTouch(touch, this->iconSlots[i])) {
				this->storage[i + (this->currentBox * 10)]->id(Overlays::SelectItem(this->storage[i + (this->currentBox * 10)]->id(), savesType));
			}
		}
	}
}