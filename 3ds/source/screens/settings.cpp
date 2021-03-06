/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2021 Universal-Team
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
	Gui::DrawStringCentered(0, -2, 0.9, WHITE, "LeafEdit - " + Lang::get("SETTINGS"), 390, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();

	for (int i = 0; i < 1; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x + 100, mainButtons[i].y + 30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void Settings::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) Gui::screenBack(doFade);

	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				if (Msg::promptMsg(Lang::get("TOGGLE_BACKUPS"))) {
					config->createBackups(config->createBackups() ? false : true);
					Msg::DisplayWaitMsg(config->createBackups() ? Lang::get("BACKUP_ENABLED") : Lang::get("BACKUP_DISABLED"));
				}
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, this->mainButtons[0])) {
			if (Msg::promptMsg(Lang::get("TOGGLE_BACKUPS"))) {
				config->createBackups(config->createBackups() ? false : true);
				Msg::DisplayWaitMsg(config->createBackups() ? Lang::get("BACKUP_ENABLED") : Lang::get("BACKUP_DISABLED"));
			}
		}
	}
}