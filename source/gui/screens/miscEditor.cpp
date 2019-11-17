	/*
*   This file is part of LeafEdit
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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

#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"

extern Save* SaveFile;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void MiscEditor::Draw(void) const
{
	std::string title = "LeafEdit - ";
	title += Lang::get("MISC_EDITOR");
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, title.c_str()))/2, 2, 0.8f, Config::TxtColor, title.c_str(), 400);
	Gui::DrawBottom();

	Gui::Draw_ImageBlend(0, sprites_button_idx, mainButtons[0].x, mainButtons[0].y, selectedColor);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::get("ACRES")))/2, mainButtons[0].y+10, 0.6f, Config::TxtColor, Lang::get("ACRES"), 140);
}



void MiscEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		Gui::setScreen(std::make_unique<AcresEditor>());
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Press A to access the Acres Editor. \nPress B to exit from this Screen.");
	}
}