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

#include "gui/keyboard.hpp"

#include "gui/screens/acresEditor.hpp"
#include "gui/screens/miscEditor.hpp"
#include "gui/screens/screenCommon.hpp"
#include "gui/screens/scripts.hpp"

#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"

extern Save* SaveFile;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void MiscEditor::Draw(void) const
{
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("MISC_EDITOR"), 400);
	Gui::DrawBottom();

	for (int i = 0; i < 2; i++) {
		if (Selection == i) {
			Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, selectedColor);
		} else {
			Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, unselectedColor);
		}
	}

	Gui::DrawStringCentered(0, mainButtons[0].y+10, 0.6f, WHITE, Lang::get("ACRES"), 130);
	Gui::DrawStringCentered(0, mainButtons[1].y+10, 0.6f, WHITE, Lang::get("SCRIPTS"), 130);
}



void MiscEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 1)	Selection++;
	}

	if (hDown & KEY_A) {
		if (Selection == 0) {
			if (Config::getBool("Debug") == true) {
				Gui::setScreen(std::make_unique<AcresEditor>());
			} else {
				Msg::DisplayWarnMsg2(Lang::get("NOT_SAVE_TO_USE"));
			}
		} else if (Selection == 1) {
			Gui::setScreen(std::make_unique<Scripts>());
		}
	}
}