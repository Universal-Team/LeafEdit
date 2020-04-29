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

#include "coreUtils.hpp"
#include "itemUtils.hpp"
#include "playerEditorWW.hpp"
#include "Sav.hpp"
#include "stringUtils.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern std::shared_ptr<Sav> save;
// Bring that to other screens too.
extern SaveType savesType;

int selectedPlayerWW = 0;

std::unique_ptr<Player> playerWW = nullptr; // player pointer which is used at this screen.

void PlayerEditorWW::Draw(void) const {
	if (Mode == 0)	DrawPlayerSelection();
	else if (Mode == 1)	DrawSubMenu();
}

void PlayerEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0)	PlayerSelectionLogic(hDown, hHeld, touch);
	else if (Mode == 1)	SubMenuLogic(hDown, hHeld, touch);
}

/*	Player Selection	*/
void PlayerEditorWW::DrawPlayerSelection(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - PlayerSelection", 395);
	if (save->player(selectedPlayerWW)->exist()) {
		Gui::DrawStringCentered(0, 50, 0.9f, BLACK, StringUtils::UTF16toUTF8(save->player(selectedPlayerWW)->name()), 400);
	}
	GFX::DrawBottom();
	for (int i = 0; i < 4; i++) {
		GFX::DrawButton(playerPos[i]);
		if (i == selectedPlayerWW)	GFX::DrawGUI(gui_pointer_idx, playerPos[i].x+100, playerPos[i].y+30);
	}
}

void PlayerEditorWW::PlayerSelectionLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
	if (hDown & KEY_UP) {
		if (selectedPlayerWW > 1)	selectedPlayerWW -= 2;
	}
	if (hDown & KEY_DOWN) {
		if (selectedPlayerWW < 2)	selectedPlayerWW += 2;
	}
	if (hDown & KEY_RIGHT) {
		if (selectedPlayerWW < 3)	selectedPlayerWW++;
	}
	if (hDown & KEY_LEFT) {
		if (selectedPlayerWW > 0)	selectedPlayerWW--;
	}

	if (hDown & KEY_A) {
		// Check if player exist.
		if (save->player(selectedPlayerWW)->exist()) {
			// Set selected Player to unique_ptr.
			playerWW = save->player(selectedPlayerWW);
			Mode = 1; // Sub Menu.
		}
	}

	if (hDown & KEY_B) {
		playerWW = nullptr; // Set unique_ptr to nullptr again cause out of scope.
		Gui::screenBack();
		return;
	}
}

/*	Sub Menu.	*/
void PlayerEditorWW::DrawSubMenu(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - Player SubMenu", 395);
	Gui::DrawStringCentered(0, 40, 0.7f, BLACK, "Player Name: " + StringUtils::UTF16toUTF8(playerWW->name()));
	Gui::DrawStringCentered(0, 60, 0.7f, BLACK, "Wallet: " + std::to_string(playerWW->wallet()));
	Gui::DrawStringCentered(0, 90, 0.7f, BLACK, "Bank: " + std::to_string(playerWW->bank()));
	Gui::DrawStringCentered(0, 120, 0.7f, BLACK, "FaceType: " + std::to_string(playerWW->face()));

	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x+100, mainButtons[i].y+30);
	}
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
		Mode = 0;
	}
}