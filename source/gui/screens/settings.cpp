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

#include "common/config.hpp"

#include "gui/keyboard.hpp"

#include "gui/screens/settings.hpp"
#include "gui/screens/screenCommon.hpp"

void Settings::Draw(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += Lang::mainMenu[2];
	
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str(), 400);

	Gui::DrawBottom();

	if (Selection == 0) {
		Gui::Draw_ImageBlend(0, sprites_button_idx, settingsButtons[0].x, settingsButtons[0].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, settingsButtons[1].x, settingsButtons[1].y);
		Gui::sprite(0, sprites_button_idx, settingsButtons[2].x, settingsButtons[2].y);

	} else if (Selection == 1) {
		Gui::sprite(0, sprites_button_idx, settingsButtons[0].x, settingsButtons[0].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, settingsButtons[1].x, settingsButtons[1].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, settingsButtons[2].x, settingsButtons[2].y);

	} else if (Selection == 2) {
		Gui::sprite(0, sprites_button_idx, settingsButtons[0].x, settingsButtons[0].y);
		Gui::sprite(0, sprites_button_idx, settingsButtons[1].x, settingsButtons[1].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, settingsButtons[2].x, settingsButtons[2].y, selectedColor);
	}

	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::settings[0]))/2, settingsButtons[0].y+10, 0.6f, WHITE, Lang::settings[0], 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::settings[1]))/2, settingsButtons[1].y+10, 0.6f, WHITE, Lang::settings[1], 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::settings[2]))/2, settingsButtons[2].y+10, 0.6f, WHITE, Lang::settings[2], 140);
}

void Settings::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	SelectionLogic(hDown, hHeld);

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						Config::setLanguage();
						break;
				}   case 1:
						Config::setSelector();
						break;
				 	case 2: {
						Msg::NotImplementedYet();
						break;
					 }
			}
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