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

#include "gui/keyboard.hpp"

#include "gui/screens/editor.hpp"
#include "gui/screens/mainMenu.hpp"
#include "gui/screens/screenCommon.hpp"
#include "gui/screens/settings.hpp"
#include "gui/screens/townManager.hpp"

extern bool titleIsLoaded;

void MainMenu::Draw(void) const
{
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, "LeafEdit"))/2, 2, 0.8f, Config::barText, "LeafEdit", 400);

	Gui::DrawBottom();

	if (Selection == 0) {
		Gui::Draw_ImageBlend(0, sprites_button_idx, mainButtons[0].x, mainButtons[0].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, mainButtons[1].x, mainButtons[1].y);
		Gui::sprite(0, sprites_button_idx, mainButtons[2].x, mainButtons[2].y);

	} else if (Selection == 1) {
		Gui::sprite(0, sprites_button_idx, mainButtons[0].x, mainButtons[0].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, mainButtons[1].x, mainButtons[1].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, mainButtons[2].x, mainButtons[2].y);

	} else if (Selection == 2) {
		Gui::sprite(0, sprites_button_idx, mainButtons[0].x, mainButtons[0].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, mainButtons[2].x, mainButtons[2].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, mainButtons[1].x, mainButtons[1].y);
	}

	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::get("TOWN_MANAGER")))/2, mainButtons[0].y+10, 0.6f, Config::buttonText, Lang::get("TOWN_MANAGER"), 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::get("EDITOR")))/2, mainButtons[1].y+10, 0.6f, Config::buttonText, Lang::get("EDITOR"), 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::get("SETTINGS")))/2, mainButtons[2].y+10, 0.6f, Config::buttonText, Lang::get("SETTINGS"), 140);
}

void MainMenu::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	SelectionLogic(hDown, hHeld);

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		switch(Selection) {
			case 0:
				if (titleIsLoaded == false) {
					Msg::DisplayWarnMsg("You haven't loaded a Title!!");
				} else if (titleIsLoaded == true) {
					Gui::setScreen(std::make_unique<TownManager>());
				}
				break;
			case 1:
				Gui::setScreen(std::make_unique<Editor>());
				break;
			case 2:
				Gui::setScreen(std::make_unique<Settings>());
				break;
		}
	}
}

void MainMenu::SelectionLogic(u32 hDown, u32 hHeld)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	}  else if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Select Town Manager, to manage your Towns.\nSelect Editor to edit a Save from your SD Card.\nPress Settings for the Settings.\nPress B to exit from this Screen.");
	}
}