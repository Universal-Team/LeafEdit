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

#include <3ds.h>

extern Save* SaveFile;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

extern std::pair<std::string, s32> passItemData;

void ItemEditor::Draw(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("ITEM_EDITOR"), 390);
	ItemManagement::DrawItem(passItemData.second, 200, 100, 1 , 1);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, passItemData.first, 390);
	Gui::DrawBottom();
	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			Gui::drawAnimatedSelector(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, .030f, SELECTED_COLOR);
		}
	}
	// Replace Item.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.6f, Lang::get("REPLACE")))/2-80+17.5, 0.6f, WHITE, Lang::get("REPLACE"), 130, 25);
	// Remove Item.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.6f, Lang::get("REMOVE")))/2-20+17.5, 0.6f, WHITE, Lang::get("REMOVE"), 130, 25);
}

void ItemEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
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

	if (hDown & KEY_A) {
		switch (Selection) {
			case 0:
				Msg::NotImplementedYet();
				break;
			case 1:
				Msg::NotImplementedYet();
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			Msg::NotImplementedYet();
		} else if (touching(touch, mainButtons[1])) {
			Msg::NotImplementedYet();
		}
	}
}