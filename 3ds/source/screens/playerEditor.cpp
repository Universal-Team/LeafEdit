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

#include "playerEditor.hpp"
#include "Sav.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern std::shared_ptr<Sav> save;
// Bring that to other screens too.
extern SaveType savesType;
int selectedPlayer = 0;

std::shared_ptr<Player> player = nullptr;

const std::vector<std::string> players = {
	"Player 1",
	"Player 2",
	"Player 3",
	"Player 4"
};

const std::vector<std::string> Strings = {
	"Appearance",
	"",
	"",
	"",
	"",
	""
};

void PlayerEditor::Draw(void) const {
	if (Mode == 0)	DrawPlayerSelection();
	else if (Mode == 1)	DrawSubMenu();
}

void PlayerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0)	PlayerSelectionLogic(hDown, hHeld, touch);
	else if (Mode == 1)	SubMenuLogic(hDown, hHeld, touch);
}

/*	Player Selection	*/
void PlayerEditor::DrawPlayerSelection(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - PlayerSelection", 400);
	if (save->player(selectedPlayer)->exist()) {
		Gui::DrawStringCentered(0, 50, 0.9f, WHITE, "Player " + std::to_string(selectedPlayer + 1) + "Exist!", 400);
	}
	GFX::DrawBottom();
	for (int i = 0; i < 4; i++) {
		GFX::DrawButton(playerPos[i].x, playerPos[i].y, players[i]);
	}
	GFX::DrawSprite(sprites_selector_idx, playerPos[selectedPlayer].x+4, playerPos[selectedPlayer].y+4);
}

void PlayerEditor::PlayerSelectionLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
	if (hDown & KEY_UP) {
		if (selectedPlayer > 1)	selectedPlayer -= 2;
	}
	if (hDown & KEY_DOWN) {
		if (selectedPlayer < 2)	selectedPlayer += 2;
	}
	if (hDown & KEY_RIGHT) {
		if (selectedPlayer < 3)	selectedPlayer++;
	}
	if (hDown & KEY_LEFT) {
		if (selectedPlayer > 0)	selectedPlayer--;
	}

	if (hDown & KEY_A) {
		// Check if player exist.
		if (save->player(selectedPlayer)->exist()) {
			// Set selected Player to shared_ptr.
			player = save->player(selectedPlayer);
			Mode = 1; // Sub Menu.
		}
	}

	if (hDown & KEY_B) {
		player = nullptr; // Set shared_ptr to nullptr again cause out of scope.
		Gui::screenBack();
		return;
	}
}

/*	Sub Menu.	*/
void PlayerEditor::DrawSubMenu(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - Player SubMenu", 400);
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(mainButtons[i].x, mainButtons[i].y, Strings[i]);
	}
	GFX::DrawSprite(sprites_selector_idx, mainButtons[Selection].x+4, mainButtons[Selection].y+4);
}

void PlayerEditor::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
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

	if (hDown & KEY_A) {
		// if (Selection == 0)	Mode = 2;
	}

	if (hDown & KEY_B) {
		save->player(selectedPlayer) = player; // Set shared_ptr stuff to selected Player.
		Mode = 0;
	}
}