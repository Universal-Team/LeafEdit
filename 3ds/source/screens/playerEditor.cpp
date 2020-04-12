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

#include "itemUtils.hpp"
#include "playerEditor.hpp"
#include "Sav.hpp"
#include "stringUtils.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern std::shared_ptr<Sav> save;
// Bring that to other screens too.
extern SaveType savesType;

int selectedPlayer = 0;

std::unique_ptr<Player> player = nullptr; // player pointer which is used at this screen.

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
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - PlayerSelection", 395);
	if (save->player(selectedPlayer)->exist()) {
		Gui::DrawStringCentered(0, 50, 0.9f, WHITE, StringUtils::UTF16toUTF8(save->player(selectedPlayer)->name()), 400);
	}
	GFX::DrawBottom();
	for (int i = 0; i < 4; i++) {
		if (i == selectedPlayer)	GFX::DrawButton(playerPos[i].x, playerPos[i].y, players[i], true);
		else				GFX::DrawButton(playerPos[i].x, playerPos[i].y, players[i]);
	}
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
			// Set selected Player to unique_ptr.
			player = save->player(selectedPlayer);
			ItemUtils::LoadDatabase(1, savesType); // TODO: Handle this different?
			Mode = 1; // Sub Menu.
		}
	}

	if (hDown & KEY_B) {
		player = nullptr; // Set unique_ptr to nullptr again cause out of scope.
		Gui::screenBack();
		return;
	}
}

/*	Sub Menu.	*/
void PlayerEditor::DrawSubMenu(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - Player SubMenu", 395);
	Gui::DrawStringCentered(0, 40, 0.7f, BLACK, "Player Name: " + StringUtils::UTF16toUTF8(player->name()));
	Gui::DrawStringCentered(0, 60, 0.7f, BLACK, "Wallet: " + std::to_string(player->wallet()));
	Gui::DrawStringCentered(0, 90, 0.7f, BLACK, "Bank: " + std::to_string(player->bank()));
	Gui::DrawStringCentered(0, 120, 0.7f, BLACK, "FaceType: " + std::to_string(player->face()));
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		if (i == Selection)	GFX::DrawButton(mainButtons[i].x, mainButtons[i].y, Strings[i], true);
		else				GFX::DrawButton(mainButtons[i].x, mainButtons[i].y, Strings[i]);
	}
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
	if (hDown & KEY_B) {
		Mode = 0;
	}
}