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

#include "colors.hpp"
#include "gui.hpp"
#include "msg.hpp"

void Msg::DisplayWarnMsg(const std::string &Text) {
	// Clearing Layer 2 first, so we can draw perfectly fine on this Layer.
	Gui::clearScreen(true, true);
	drawRectangle(0, 70, 256, 60, DARKER_GRAY, true, true);
	printTextCentered(Text, 0, 80, true, true);
	for (int i = 0; i < 60*2; i++) {
		swiWaitForVBlank();
	}
	// Clear Layer 2 again.
	Gui::DrawScreen();
}

// Display a Message, which can be skipped with A.
void Msg::DisplayWaitMsg(std::string waitMsg, ...) {
	// Clearing Layer 2 first, so we can draw perfectly fine on this Layer.
	Gui::clearScreen(true, true);
	drawRectangle(0, 70, 256, 60, DARKER_GRAY, true, true);
	printTextCentered(waitMsg, 0, 80, true, true);
	printTextCentered("Press A to continue.", 0, 175, true, true);
	while(1)
	{
		scanKeys();
		if(keysDown() & KEY_A)
			break;
	}
	Gui::DrawScreen();
}