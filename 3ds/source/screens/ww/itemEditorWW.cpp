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

#include "itemEditorWW.hpp"
#include "itemManager.hpp"
#include "itemUtils.hpp"
#include "gfx.hpp"
#include "keyboard.hpp"
#include "overlay.hpp"

#define MAX_DRESSER 8

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

ItemEditorWW::ItemEditorWW(std::unique_ptr<Player> &refPlayer) : player(refPlayer) {
	/* Loading Pocket here. */
	for (int i = 0; i < 15; i++) {
		this->pockets[i] = this->player->pocket(i);
	}

	/* Loading Dresser here. */
	for (int i = 0; i < 90; i++) {
		this->dresser[i] = this->player->dresser(i);
	}
}

ItemEditorWW::~ItemEditorWW() { }

void ItemEditorWW::Draw(void) const {
	switch(this->Mode) {
		case 0:
			this->DrawSubMenu();
			break;

		case 1:
			this->DrawPocket();
			break;

		case 2:
			this->DrawDresser();
			break;
	}
}

void ItemEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	switch(this->Mode) {
		case 0:
			this->SubMenuLogic(hDown, hHeld, touch);
			break;

		case 1:
			this->PocketLogic(hDown, hHeld, touch);
			break;

		case 2:
			this->DresserLogic(hDown, hHeld, touch);
			break;
	}
}


void ItemEditorWW::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEMS"), 395, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();

	for (int i = 0; i < 2; i++) {
		GFX::DrawButton(this->mainButtons[i]);
		if (i == this->Selection) GFX::DrawGUI(gui_pointer_idx, this->mainButtons[i].x + 100, this->mainButtons[i].y + 30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void ItemEditorWW::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 1) this->Selection++;
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
		}
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 2; i++) {
			if (touching(touch, this->mainButtons[i])) {
				this->Selection = 0;
				this->Mode = i + 1;
			}
		}
	}
}


void ItemEditorWW::DrawPocket(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEM_POCKET"), 395, 0, font);
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 218, 0.8f, WHITE, Lang::get("CURRENT_ITEM") + this->pockets[this->selectedItem]->name(), 390, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();

	for (int i = 0; i < 15; i++) {
		GFX::drawGrid(this->pocketSlots[i].x, this->pocketSlots[i].y, this->pocketSlots[i].w, this->pocketSlots[i].h, ItemManager::getColor(this->pockets[i]->itemtype()), C2D_Color32(0, 0, 0, 255));
	}

	GFX::DrawGUI(gui_pointer_idx, this->pocketSlots[this->selectedItem].x + 10, this->pocketSlots[this->selectedItem].y + 10);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void ItemEditorWW::PocketLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_RIGHT) {
		if (this->selectedItem < 14) this->selectedItem++;
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
		this->pockets[this->selectedItem]->id(Overlays::SelectItem(this->pockets[this->selectedItem]->id(), SaveType::WW, false));
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 15; i++) {
			if (iconTouch(touch, this->pocketSlots[i])) {
				this->pockets[i]->id(Overlays::SelectItem(this->pockets[i]->id(), SaveType::WW, false));
			}
		}
	}
}

void ItemEditorWW::DrawDresser(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ITEM_DRESSER") + std::to_string(currentBox + 1), 395, 0, font);
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 218, 0.8f, WHITE, Lang::get("CURRENT_ITEM") + this->dresser[this->selectedItem + (this->currentBox * 10)]->name(), 400, 0, font);
	
	GFX::DrawBottom();

	for (int i = 0 + (10 * this->currentBox), i2 = 0; i < 10 + (10 * this->currentBox); i++, i2++) {
		GFX::drawGrid(this->iconSlots[i2].x, this->iconSlots[i2].y, this->iconSlots[i2].w, this->iconSlots[i2].h, ItemManager::getColor(this->dresser[i]->itemtype()), C2D_Color32(0, 0, 0, 255));
	}

	GFX::DrawGUI(gui_pointer_idx, this->iconSlots[this->selectedItem].x + 10, this->iconSlots[this->selectedItem].y + 10);
}

void ItemEditorWW::DresserLogic(u32 hDown, u32 hHeld, touchPosition touch) {
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
		this->dresser[this->selectedItem + (this->currentBox * 10)]->id(Overlays::SelectItem(this->dresser[this->selectedItem + (this->currentBox * 10)]->id(), SaveType::WW));
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 10; i++) {
			if (iconTouch(touch, this->iconSlots[i])) {
				this->dresser[i + (this->currentBox * 10)]->id(Overlays::SelectItem(this->dresser[i + (this->currentBox * 10)]->id(), SaveType::WW));
			}
		}
	}
}