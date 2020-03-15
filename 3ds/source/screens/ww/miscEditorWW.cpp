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

#include "acresEditorWW.hpp"
#include "miscEditorWW.hpp"
#include "screenCommon.hpp"
#include "townEditorWW.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void MiscEditorWW::Draw(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("MISC_EDITOR"), 400);
	GFX::DrawBottom();

	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, mainButtons[i].x+130, mainButtons[i].y+25);
		}
	}
	Gui::DrawStringCentered(0, mainButtons[0].y+10, 0.8f, WHITE, Lang::get("TOWN_EDITOR"), 130);
	Gui::DrawStringCentered(0, mainButtons[1].y+10, 0.8f, WHITE, Lang::get("ACRE_EDITOR"), 130);
}



void MiscEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}
	
	if (hDown & KEY_A) {
		if (Selection == 0) {
			Gui::setScreen(std::make_unique<TownEditorWW>());
		} else if (Selection == 1) {
			Gui::setScreen(std::make_unique<AcresEditorWW>());
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			Gui::setScreen(std::make_unique<TownEditorWW>());
		} else if (touching(touch, mainButtons[1])) {
			Gui::setScreen(std::make_unique<AcresEditorWW>());
		}
	}
}