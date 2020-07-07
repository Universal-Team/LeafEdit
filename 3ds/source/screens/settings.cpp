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

#include "common.hpp"
#include "screenCommon.hpp"
#include "settings.hpp"

extern std::unique_ptr<Config> config;
extern bool changesMade;
extern bool touching(touchPosition touch, ButtonType button);

void Settings::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9, WHITE, "LeafEdit - Settings", 390, 0, font);
	GFX::DrawBottom();
	for (int i = 0; i < 3; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x+100, mainButtons[i].y+30);
	}
}


void Settings::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
	if (hDown & KEY_DOWN) {
		if (Selection < 2)	Selection++;
	}

	if (hDown & KEY_UP) {
		if (Selection > 0)	Selection--;
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
	}
	
	if (hDown & KEY_A) {
		if (Selection == 0) {
			if (config->newStyle()) {
				config->newStyle(false);
				barOffset = 2;
			} else {
				config->newStyle(true);
				barOffset = 0;
			}
			
			changesMade = true;
		} else if (Selection == 1) {
			if (Msg::promptMsg("Would you like to toggle Backups?")) {
				config->createBackups(config->createBackups() ? false : true);
			}
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			if (config->newStyle()) {
				config->newStyle(false);
				barOffset = 2;
			} else {
				config->newStyle(true);
				barOffset = 0;
			}

			changesMade = true;
		} else if (touching(touch, mainButtons[1])) {
			if (Msg::promptMsg("Would you like to toggle Backups?")) {
				config->createBackups(config->createBackups() ? false : true);
			}
		}
	}
}