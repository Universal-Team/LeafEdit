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
#include "Sav.hpp"

extern std::shared_ptr<Sav> save;

extern bool touching(touchPosition touch, Structs::ButtonPos button);
std::unique_ptr<Player> player = nullptr;

extern std::vector<std::string> g_hairStyle;
extern std::vector<std::string> g_hairColor;
extern std::vector<std::string> g_faceType;

const std::vector<std::string> &TanLevel = {
	"0",
	"1",
	"2",
	"3",
};

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
	Gui::DrawTop(true);
	printTextCentered("LeafEdit - Player Selection", 0, 0, true, true);
	DrawPlayerBoxes();
	printTextCentered("Current Player: " + std::to_string(selectedPlayer+1), 0, 172, true, true);
	Gui::DrawBottom(true);
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
	Gui::DrawTop(true);
	printTextCentered("LeafEdit - Player Sub Menu", 0, 0, true, true); 
	Gui::DrawBottom(true);
	for (int i = 0; i < 3; i++) {
		drawRectangle(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, DARK_GREEN, DARK_GREEN, false, true);
	}
	printTextCentered("Player", 0, 40, false, true);
	printTextCentered("Items", 0, 90, false, true);
	printTextCentered("Appearance", 0, 140, false, true);
}

void PlayerEditor::DrawPlayerScreen(void) const {
	Gui::DrawTop(true);
	printTextCentered("LeafEdit - Player Editor", 0, 0, true, true);
	drawRectangle(20, 35, 216, 30, DARK_GREEN, true, true);
	printTextCentered(player->name(), 0, 40, true, true);
	printTextCentered("Bells: " + std::to_string(player->wallet()), 0, 65, true, true);
	printTextCentered("Gender: " + std::to_string(player->gender()), 0, 90, true, true);
	printTextCentered("TAN: " + std::to_string(player->tan()), 0, 115, true, true);
	printTextCentered("Facetype: " + std::to_string(player->face()), 0, 140, true, true);
	printTextCentered("HairStyle: " + std::to_string(player->hairstyle()), 0, 165, true, true);
	Gui::DrawBottom(true);
	for (int i = 0; i < 6; i++) {
		drawRectangle(playerButtons[i].x, playerButtons[i].y, playerButtons[i].w, playerButtons[i].h, DARK_GREEN, DARK_GREEN, false, true);
	}
	printTextCentered("Bells", -64, 40, false, true);
	printTextCentered("Name", -64, 90, false, true);
	printTextCentered("Gender", -64, 140, false, true);
	printTextCentered("TAN", 64, 40, false, true);
	printTextCentered("FaceType", 64, 90, false, true);
	printTextCentered("HairStyle", 64, 140, false, true);
}

void PlayerEditor::PlayerLogic(u16 hDown, touchPosition touch) {
	Gui::updatePointer(playerButtons[selection].x+60, playerButtons[selection].y+12);

	if (hDown & KEY_B) {
		selection = 0;
		screen = 1;
		Gui::DrawScreen();
		selected = true;
	}

	// Selection.
	if (hDown & KEY_UP) {
		if(selection > 0)	selection--;
		selected = true;
	}
	
	if (hDown & KEY_DOWN) {
		if(selection < 5)	selection++;
		selected = true;
	}

	if (hDown & KEY_RIGHT) {
		if (selection < 3) {
			selection += 3;
			selected = true;
		}
	}

	if (hDown & KEY_LEFT) {
		if (selection < 6 && selection > 2) {
			selection -= 3;
			selected = true;
		}
	}
}


void PlayerEditor::SubMenuLogic(u16 hDown, touchPosition touch) {
	Gui::updatePointer(mainButtons[selection].x+60, mainButtons[selection].y+12);
	if (hDown & KEY_B) {
		selection = 0;
		screen = 0;
		Gui::hidePointer();
		Gui::DrawScreen();
	}

	if (hDown & KEY_DOWN) {
		if (selection < 2)	selection++;
		selected = true;
	}

	if (hDown & KEY_UP) {
		if (selection > 0)	selection--;
		selected = true;
	}

	if (hDown & KEY_A) {
			// Player.
		if (selection == 0) {
			selection = 0;
			screen = 2;
			Gui::DrawScreen();
			selected = true;
		}
	}
}

void PlayerEditor::PlayerSelectionLogic(u16 hDown, touchPosition touch) {
	for (int player = 0; player < 4; player++) {
		if (save->player(player)->exist())	maxPlayer = player;
	}

	if (hDown & KEY_RIGHT) {
		selectedPlayer++;
		Gui::DrawScreen();
		if(selectedPlayer > maxPlayer)	selectedPlayer = 0;
	} else if (hDown & KEY_LEFT) {
		selectedPlayer--;
		if(selectedPlayer < 0)	selectedPlayer = maxPlayer;
		Gui::DrawScreen();
	}

	if (hDown & KEY_A) {
		if (save->player(selectedPlayer)->exist()) {
			player = save->player(selectedPlayer);
			selection = 0;
			screen = 1;
			Gui::DrawScreen();
			Gui::showPointer();
			selected = true;
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
		player = nullptr;
		// Display Save icon.
		setSpriteVisibility(Gui::saveID, false, true);
		setSpritePosition(Gui::saveID, false, 225, 172);
		updateOam();
		Gui::DrawScreen();
		Gui::showPointer();
		selected = true;
		return;
	}
}