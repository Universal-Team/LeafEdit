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

#include "gui/screens/screenCommon.hpp"
#include "gui/screens/mainMenu.hpp"
#include "gui/keyboard.hpp"
#include "gui/screens/townManager.hpp"

void MainMenu::Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
	Gui::DrawString((400-Gui::Draw_GetStringWidth(0.8f, Lang::title))/2, 2, 0.8f, WHITE, Lang::title);

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);

	if (Selection == 0) {
		Gui::Draw_Rect(mainButtons[0].x, mainButtons[0].y, mainButtons[0].w, mainButtons[0].h, RED);
		Gui::Draw_Rect(mainButtons[1].x, mainButtons[1].y, mainButtons[1].w, mainButtons[1].h, GREEN);
		Gui::Draw_Rect(mainButtons[2].x, mainButtons[2].y, mainButtons[2].w, mainButtons[2].h, GREEN);

	} else if (Selection == 1) {
		Gui::Draw_Rect(mainButtons[0].x, mainButtons[0].y, mainButtons[0].w, mainButtons[0].h, GREEN);
		Gui::Draw_Rect(mainButtons[1].x, mainButtons[1].y, mainButtons[1].w, mainButtons[1].h, RED);
		Gui::Draw_Rect(mainButtons[2].x, mainButtons[2].y, mainButtons[2].w, mainButtons[2].h, GREEN);

	} else if (Selection == 2) {
		Gui::Draw_Rect(mainButtons[0].x, mainButtons[0].y, mainButtons[0].w, mainButtons[0].h, GREEN);
		Gui::Draw_Rect(mainButtons[1].x, mainButtons[1].y, mainButtons[1].w, mainButtons[1].h, GREEN);
		Gui::Draw_Rect(mainButtons[2].x, mainButtons[2].y, mainButtons[2].w, mainButtons[2].h, RED);
	}

	Gui::DrawString((320-Gui::Draw_GetStringWidth(0.6f, Lang::mainMenu[0]))/2, mainButtons[0].y+10, 0.6f, WHITE, Lang::mainMenu[0]);
	Gui::DrawString((320-Gui::Draw_GetStringWidth(0.6f, Lang::mainMenu[1]))/2, mainButtons[1].y+10, 0.6f, WHITE, Lang::mainMenu[1]);
	Gui::DrawString((320-Gui::Draw_GetStringWidth(0.6f, Lang::mainMenu[2]))/2, mainButtons[2].y+10, 0.6f, WHITE, Lang::mainMenu[2]);
}

void MainMenu::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	SelectionLogic(hDown, hHeld);

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						Gui::setScreen(std::make_unique<TownManager>());
						break;
				}   case 1:
						break;
				 	case 2: {
						break;
					 }
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