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
#include "screenCommon.hpp"

extern int fadealpha;
extern bool fadein;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool exiting;

const std::vector<std::string> Strings = {
	"Editor",
	"Settings",
	"",
	"",
	"",
	"",
};

void MainMenu::Draw(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9, WHITE, "LeafEdit - MainMenu", 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		if (i == Selection)	GFX::DrawButton(mainButtons[i].x, mainButtons[i].y, Strings[i], true);
		else 				GFX::DrawButton(mainButtons[i].x, mainButtons[i].y, Strings[i]);
	}
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect
}


void MainMenu::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
	if(hDown & KEY_UP) {
		if(Selection > 1)	Selection -= 2;
	} else if(hDown & KEY_DOWN) {
		if(Selection < 3 && Selection != 2 && Selection != 3)	Selection += 2;
	} else if (hDown & KEY_LEFT) {
		if (Selection%2) Selection--;
	} else if (hDown & KEY_RIGHT) {
		if (!(Selection%2)) Selection++;
	}

	if (hDown & KEY_START) {
		exiting = true;
	}

	if (hDown & KEY_A) {
		if (Selection == 0)	Gui::setScreen(std::make_unique<Editor>());
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}

	if (hDown & KEY_X) {
		Msg::DisplayWarnMsg("Look! I'm a TextBox!!!");
	}
}