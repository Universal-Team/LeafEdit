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

#include "editor.hpp"
#include "mainMenu.hpp"

extern bool exiting;

void MainMenu::Draw(void) const {
	Gui::DrawTop();
	printTextCentered("LeafEdit - MainMenu", 0, 1, true, true);
	Gui::DrawBottom();
	for (int i = 0; i < 2; i++) {
		drawRectangle(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, DARK_GREEN, DARK_GREEN, false, true);
		if (selection == i) {
			drawRectangle(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, LIGHT_GREEN, LIGHT_GREEN, false, true);
		}
	}
	printTextCentered("Editor", -64, 88, false, true);
	printTextCentered("Settings", 64, 88, false, true);
}

void MainMenu::Logic(u16 hDown, touchPosition touch) {
	if (hDown & KEY_START) {
		exiting = true;
	}
	if (hDown & KEY_RIGHT) {
		if (selection < 1)	selection++;
	}
	if (hDown & KEY_LEFT) {
		if (selection > 0)	selection--;
	}

	if (hDown & KEY_A) {
		if (selection == 0)	Gui::setScreen(std::make_unique<Editor>());
	}
}