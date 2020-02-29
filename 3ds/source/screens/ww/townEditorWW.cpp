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

#include "screenCommon.hpp"
#include "townEditorWW.hpp"
#include "utils.hpp"
#include "wwsave.hpp"
#include "wwTown.hpp"
#include "wwTownManagement.hpp"

#include <3ds.h>

extern WWSave* WWSaveFile;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void TownEditorWW::Draw(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("TOWN_EDITOR"), 400);
	Gui::DrawStringCentered(0, 45, 0.9f, WHITE, "Town Name: " + StringUtils::UTF16toUTF8(WWSaveFile->town->Name), 380);
	Gui::DrawStringCentered(0, 70, 0.9f, WHITE, "Debt: " + std::to_string(WWSaveFile->town->Debt), 380);
	GFX::DrawBottom();

	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h, UNSELECTED_COLOR);
		if (selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, buttons[i].x+130, buttons[i].y+25);
		}
	}

	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, "Town Name"))/2-80+17.5, 0.8, WHITE, "Town Name", 130, 25);
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, "Debt"))/2-20+17.5, 0.8, WHITE, "Debt", 130, 25);
}



void TownEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		switch (selection) {
			case 0:
				WWTownManagement::setTownName();
				break;
			case 1:
				WWTownManagement::setDebt();
				break;
			default:
				break;
		}
	}

	// Selection.
	if (hDown & KEY_UP) {
		if(selection > 0)	selection--;
	}
	
	if (hDown & KEY_DOWN) {
			if(selection < 5)	selection++;
	}

	if (hDown & KEY_RIGHT) {
		if (selection < 3) {
			selection += 3;
		}
	}

	if (hDown & KEY_LEFT) {
		if (selection < 6 && selection > 2) {
			selection -= 3;
		}
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}
}