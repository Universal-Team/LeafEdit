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

#include "gui/keyboard.hpp"

#include "gui/screens/credits.hpp"
#include "gui/screens/settings.hpp"
#include "gui/screens/screenCommon.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Settings::Draw(void) const
{
	if(screenMode == 0) {
		Gui::DrawTop();
		Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("SETTINGS"), 400);
		Gui::DrawBottom();
		for (int i = 0; i < 3; i++) {
			Gui::Draw_Rect(settingsButtons[i].x, settingsButtons[i].y, settingsButtons[i].w, settingsButtons[i].h, UNSELECTED_COLOR);
			if (Selection == i) {
				Gui::drawAnimatedSelector(settingsButtons[i].x, settingsButtons[i].y, settingsButtons[i].w, settingsButtons[i].h, .030f, SELECTED_COLOR);
			}
		}
		Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.6f, Lang::get("LANGUAGE")))/2-80+17.5, 0.6f, WHITE, Lang::get("LANGUAGE"), 130, 25);
		Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.6f, Lang::get("CREDITS")))/2-20+17.5, 0.6f, WHITE, Lang::get("CREDITS"), 130, 25);
		Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.6f, Lang::get("COLORMODE")))/2+75-17.5, 0.6f, WHITE, Lang::get("COLORMODE"), 130, 25);
	} else if(screenMode == 1) {
		DrawLangScreen();
	}
}

void Settings::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if(screenMode == 0) {
		SelectionLogic(hDown, hHeld);

		if (hDown & KEY_B) {
			Gui::screenBack();
			return;
		}

		if (hDown & KEY_A) {
			switch(Selection) {
				case 0:
					screenMode = 1;
					break;
				case 1:
					Gui::setScreen(std::make_unique<Credits>());
					break;
				case 2:
					Utils::colorLogic(Config::colorMode);
					break;
			}
		}

		if (hDown & KEY_TOUCH) {
			if (touching(touch, settingsButtons[0])) {
				screenMode = 1;
			} else if (touching(touch, settingsButtons[1])) {
				Gui::setScreen(std::make_unique<Credits>());
			} else if (touching(touch, settingsButtons[2])) {
				Utils::colorLogic(Config::colorMode);
			}
		}

	} else if(screenMode == 1) {
		langScreenLogic(hDown, hHeld, touch);
	}
}

void Settings::SelectionLogic(u32 hDown, u32 hHeld)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	}
}


void Settings::DrawLangScreen(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, Lang::get("SELECT_LANG"), 398);
	Gui::DrawBottom();

	for (int i = 0; i < 8; i++) {
		Gui::Draw_Rect(langBlocks[i].x, langBlocks[i].y, langBlocks[i].w, langBlocks[i].h, UNSELECTED_COLOR);
		if (Config::lang == i) {
			Gui::drawAnimatedSelector(langBlocks[i].x, langBlocks[i].y, langBlocks[i].w, langBlocks[i].h, .030f, SELECTED_COLOR);
		}
	}
	Gui::DrawString(langBlocks[0].x+25, langBlocks[0].y-2, 0.7f, WHITE, "Deutsch", 320);
	Gui::DrawString(langBlocks[1].x+25, langBlocks[1].y-2, 0.7f, WHITE, "English", 320);
	Gui::DrawString(langBlocks[2].x+25, langBlocks[2].y-2, 0.7f, WHITE, "Español", 320);
	Gui::DrawString(langBlocks[3].x+25, langBlocks[3].y-2, 0.7f, WHITE, "Français", 320);

	Gui::DrawString(langBlocks[4].x+25, langBlocks[4].y-2, 0.7f, WHITE, "Italiano", 320);
	Gui::DrawString(langBlocks[5].x+25, langBlocks[5].y-2, 0.7f, WHITE, "Lietuvių", 320);
	Gui::DrawString(langBlocks[6].x+25, langBlocks[6].y-2, 0.7f, WHITE, "Português", 320);
	Gui::DrawString(langBlocks[7].x+25, langBlocks[7].y-2, 0.7f, WHITE, "日本語", 320);

	Gui::sprite(0, sprites_back_idx, langBlocks[8].x, langBlocks[8].y);
}

void Settings::langScreenLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = 0;
	}

	if (hDown & KEY_TOUCH && touching(touch, langBlocks[8])) {
		screenMode = 0;
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 8; i++) {
			if (touching(touch, langBlocks[i])) {
				Config::lang = i;
				ItemManagement::LoadDatabase(Config::lang);
				Lang::load(Config::lang);
			}
		}
	}
}