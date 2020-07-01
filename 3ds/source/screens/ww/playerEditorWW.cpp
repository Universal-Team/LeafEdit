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

#include "playerEditorWW.hpp"
#include "Sav.hpp"
#include "stringUtils.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern std::shared_ptr<Sav> save;

PlayerEditorWW::PlayerEditorWW(std::shared_ptr<Player> p): player(p) { }

void PlayerEditorWW::Draw(void) const {
	if (Mode == 0)	DrawSubMenu();
}

void PlayerEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0)	SubMenuLogic(hDown, hHeld, touch);
}

/*	Sub Menu.	*/
void PlayerEditorWW::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - Player SubMenu", 395, 0, font);
	Gui::DrawStringCentered(0, 40, 0.7f, BLACK, "Player Name: " + StringUtils::UTF16toUTF8(player->name()), 0, 0, font);
	Gui::DrawStringCentered(0, 60, 0.7f, BLACK, "Wallet: " + std::to_string(player->wallet()), 0, 0, font);
	Gui::DrawStringCentered(0, 90, 0.7f, BLACK, "Bank: " + std::to_string(player->bank()), 0, 0, font);
	Gui::DrawStringCentered(0, 120, 0.7f, BLACK, "FaceType: " + std::to_string(player->face()), 0, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x+100, mainButtons[i].y+30);
	}
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void PlayerEditorWW::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	}

	if (hDown & KEY_DOWN) {
		if(Selection < 5)	Selection++;
	}

	if (hDown & KEY_RIGHT) {
		if (Selection < 3) {
			Selection += 3;
		}
	}
	
	if (hDown & KEY_LEFT) {
		if (Selection < 6 && Selection > 2) {
			Selection -= 3;
		}
	}
	
	if (hDown & KEY_B) {
		Gui::screenBack(true);
	}
}