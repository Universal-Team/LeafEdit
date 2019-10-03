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

#include "gui/screens/miscEditor.hpp"
#include "gui/screens/screenCommon.hpp"

#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"

extern Save* SaveFile;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void MiscEditor::Draw(void) const
{
	Gui::DrawTop();
	std::string townName = StringUtils::UTF16toUTF8(SaveFile->players[0]->TownName).c_str();
	Gui::sprite(0, sprites_topbox_idx, 40, 37);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, townName.c_str()))/2, 35, 0.8f, Config::boxText, townName.c_str(), 400);

	Gui::DrawBottom();
}



void MiscEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if (hDown & KEY_X) {
		SaveFile->players[0]->TownName = Input::getu16String(8, "Please type in the new Town Name.");
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Press X to edit the Town Name. \nPress B to exit from this Screen.");
	}
}