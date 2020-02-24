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

#include "input.hpp"
#include "msg.hpp"
#include "playerEditor.hpp"
#include "playerManagement.hpp"
#include "stringUtils.hpp"
#include "wwoffsets.hpp"
#include "wwPlayer.hpp"
#include "wwsave.hpp"
#include "wwStringUtils.hpp"
#include "wwVillager.hpp"
#include "villagerManagement.hpp"

extern WWSave* SaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
int selectedPassedPlayer;

void PlayerEditor::DrawPlayerBoxes(void) const {
	for (u32 y = 0; y < 2; y++) {
		for (u32 x = 0; x < 2; x++) {
			drawRectangle(20 + x*120, 35 + y*70, 100, 50, DARK_GREEN, true, true);
		}
	}
}

void PlayerEditor::Draw(void) const {
	if (screen == 0) {
		DrawPlayerSelection();
	} else if (screen == 1) {
		DrawSubMenu();
	} else if (screen == 2) {
		DrawPlayerScreen();
	}
}

void PlayerEditor::DrawPlayerSelection(void) const {
	Gui::DrawTop();
	printTextCentered("LeafEdit - Player Selection", 0, 0, true, true);
	DrawPlayerBoxes();
	printTextCentered("Current Player: " + std::to_string(selectedPlayer+1), 0, 172, true, true);

	//VillagerManagement::DrawVillager(SaveFile->villagers[0]->GetId(), 100, 100); // That was my test.
	Gui::DrawBottom();
}

void PlayerEditor::Logic(u16 hDown, touchPosition touch) {
	if (screen == 0) {
		PlayerSelectionLogic(hDown, touch);
	} else if (screen == 1) {
		SubMenuLogic(hDown, touch);
	} else if (screen == 2) {
		PlayerLogic(hDown, touch);
	}
}

void PlayerEditor::DrawSubMenu(void) const {
	Gui::DrawTop();
	printTextCentered("LeafEdit - Player Sub Menu", 0, 0, true, true); 
	Gui::DrawBottom();
	for (int i = 0; i < 3; i++) {
		if (selection == i) {
			drawRectangle(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, LIGHT_GREEN, LIGHT_GREEN, false, true);
		} else {
			drawRectangle(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, DARK_GREEN, DARK_GREEN, false, true);
		}
	}
	printTextCentered("Player", 0, 40, false, true);
	printTextCentered("Items", 0, 90, false, true);
	printTextCentered("Appearance", 0, 140, false, true);
}

void PlayerEditor::DrawPlayerScreen(void) const {
	Gui::DrawTop();
	printTextCentered("LeafEdit - Player Editor", 0, 0, true, true);

	drawRectangle(20, 35, 216, 30, DARK_GREEN, true, true);
	printTextCentered(SaveFile->players[cp]->Name, 0, 40, true, true);
	printTextCentered("Bells: " + std::to_string(SaveFile->players[cp]->Bells), 0, 80, true, true);

	Gui::DrawBottom();
	for (int i = 0; i < 6; i++) {
		if (selection == i) {
			drawRectangle(playerButtons[i].x, playerButtons[i].y, playerButtons[i].w, playerButtons[i].h, LIGHT_GREEN, LIGHT_GREEN, false, true);
		} else {
			drawRectangle(playerButtons[i].x, playerButtons[i].y, playerButtons[i].w, playerButtons[i].h, DARK_GREEN, DARK_GREEN, false, true);
		}
	}
	printTextCentered("Bells", -64, 40, false, true);
}

void PlayerEditor::PlayerLogic(u16 hDown, touchPosition touch) {
	if (hDown & KEY_A) {
		if (selection == 0) {
			PlayerManagement::setBells(cp);
		}
	}

	if (hDown & KEY_X) {
		std::string test = Input::getLine("Enter the Playername.", 8);
		if (test != "") {
			SaveFile->players[cp]->Name = StringUtils::utf8to16(test);
		}
	}

	if (hDown & KEY_B) {
		Gui::clearScreen(true, true);
		Gui::clearScreen(false, true);
		selection = 0;
		screen = 1;
	}
}


void PlayerEditor::SubMenuLogic(u16 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::clearScreen(true, true);
		Gui::clearScreen(false, true);
		selection = 0;
		screen = 0;
	}

	if (hDown & KEY_DOWN) {
		if (selection < 2)	selection++;
	}

	if (hDown & KEY_UP) {
		if (selection > 0)	selection--;
	}

	if (hDown & KEY_A) {
			// Player.
		if (selection == 0) {
			Gui::clearScreen(true, true);
			Gui::clearScreen(false, true);
			selection = 0;
			screen = 2;
		}
	}
}

void PlayerEditor::PlayerSelectionLogic(u16 hDown, touchPosition touch) {
	for (int player = 0; player < 4; player++) {
		if (SaveFile->players[player]->Exists())	maxPlayer = player;
	}

	if (hDown & KEY_RIGHT) {
		Gui::clearScreen(true, true);
		selectedPlayer++;
		if(selectedPlayer > maxPlayer)	selectedPlayer = 0;
	} else if (hDown & KEY_LEFT) {
		Gui::clearScreen(true, true);
		selectedPlayer--;
		if(selectedPlayer < 0)	selectedPlayer = maxPlayer;
	}

	if (hDown & KEY_A) {
		Gui::clearScreen(true, true);
		Gui::clearScreen(false, true);
		cp = selectedPlayer;
		selection = 0;
		screen = 1;
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}