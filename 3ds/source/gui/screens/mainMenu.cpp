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

#include "core/management/itemManagement.hpp"
#include "core/management/playerManagement.hpp"
#include "core/management/villagerManagement.hpp"

#include "gui/screens/mainMenu.hpp"
#include "gui/screens/screenCommon.hpp"
#include "gui/screens/settings.hpp"
#include "gui/screens/townManager.hpp"

#include "gui/screensnl/editor.hpp"

#include "gui/screensww/editorWW.hpp"

extern bool isACWW;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool isROMHack;

void MainMenu::Draw(void) const
{
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit", 400);

	Gui::DrawBottom();

	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			Gui::drawAnimatedSelector(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, .030f, SELECTED_COLOR);
		}
	}

	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("TOWN_MANAGER")))/2-80+17.5, 0.8, WHITE, Lang::get("TOWN_MANAGER"), 130, 25);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("EDITOR")))/2-20+17.5, 0.8, WHITE, Lang::get("EDITOR"), 130, 25);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("SETTINGS")))/2+75-17.5, 0.8, WHITE, Lang::get("SETTINGS"), 130, 25);
}


void MainMenu::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	SelectionLogic(hDown, hHeld);

	if (hDown & KEY_B) {
		// Reset the status.
		isACWW = false; 
		isROMHack = false;
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		switch(Selection) {
			case 0:
				if (!isACWW) {
					Gui::setScreen(std::make_unique<TownManager>());
				} else {
					Msg::NotImplementedYet();
				}
				break;
			case 1:
				if (!isACWW) {
					if (Msg::promptMsg2(Lang::get("EXPERIMENTAL_EDITOR"))) {
						Msg::DisplayMsg(Lang::get("PREPARING_EDITOR"));
						Gui::loadSheets();
						ItemManagement::LoadDatabase(Config::getLang("Lang"));
						Gui::setScreen(std::make_unique<Editor>());
					}
				} else {
					if (Msg::promptMsg2(Lang::get("EXPERIMENTAL_EDITOR"))) {
						Gui::setScreen(std::make_unique<EditorWW>());
					}
				}
				break;
			case 2:
				Gui::setScreen(std::make_unique<Settings>());
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			if (!isACWW) {
				Gui::setScreen(std::make_unique<TownManager>());
			} else {
				Msg::NotImplementedYet();
			}
		} else if (touching(touch, mainButtons[1])) {
			if (!isACWW) {
				if (Msg::promptMsg2(Lang::get("EXPERIMENTAL_EDITOR"))) {
					Msg::DisplayMsg(Lang::get("PREPARING_EDITOR"));
					Gui::loadSheets();
					ItemManagement::LoadDatabase(Config::getLang("Lang"));
					Gui::setScreen(std::make_unique<Editor>());
				}
			} else {
				if (Msg::promptMsg2(Lang::get("EXPERIMENTAL_EDITOR"))) {
					Gui::setScreen(std::make_unique<EditorWW>());
				}
			}
		} else if (touching(touch, mainButtons[2])) {
			Gui::setScreen(std::make_unique<Settings>());
		}
	}
}

void MainMenu::SelectionLogic(u32 hDown, u32 hHeld)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	}
}