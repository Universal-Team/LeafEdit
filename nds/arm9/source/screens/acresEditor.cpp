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

#include "acresEditor.hpp"
#include "graphicManagement.hpp"
#include "Sav.hpp"

extern std::shared_ptr<Sav> save;
#define MAX_ACRE 130 // Define the Max Amount of Acres.

extern bool touching(touchPosition touch, Structs::ButtonPos button);

AcresEditor::AcresEditor() {
	// Get Acres.
	for (int i = 0; i < 36; i++) {
		this->acres[i] = save->town()->acre(i);
	}
}

// Draw Functions.
void AcresEditor::DrawTop(void) const {
	printTextCentered("LeafEdit - Acres Editor", 0, 0, true, true);
	GraphicManagement::DrawAcre(selectedAcre, 90, 70, 2, 2); // Current Selected ACRE.
	printTextCentered("Acre ID: " + std::to_string(selectedAcre), 0, 150, true, true);
}

void AcresEditor::Draw(void) const {
	Gui::DrawTop(true);
	DrawTop();
	Gui::DrawBottom(false);
	DrawAcres();
}

void AcresEditor::DrawAcres() const {
	for (int i = 0; i < 36; i++) {
		GraphicManagement::DrawAcre(this->acres[i]->id(), MapPos[i].x, MapPos[i].y, 1, 1, false);
	}
}

// Update Functions.
void AcresEditor::updateTop() {
	Gui::clearScreen(true, true);
	DrawTop(); // Refresh Top Screen.
}

void AcresEditor::updateAcres() {
	Gui::clearScreen(false, true);
	DrawAcres();
}

// Complete Logic.
void AcresEditor::Logic(u16 hDown, touchPosition touch) {
	u16 held = keysDownRepeat();
	Gui::updatePointer(MapPos[Selection].x+14, MapPos[Selection].y+14);

	if (hDown & KEY_B) {
		Gui::screenBack();
		Gui::DrawScreen();
		selected = true;
		return;
	}

	// Set current Acre.
	if (hDown & KEY_A) {
		this->acres[Selection]->id(selectedAcre);
		updateAcres();
		changes = true;
	}

	// Display current Acre on Top.
	if (hDown & KEY_Y) {
		selectedAcre = this->acres[Selection]->id();
		updateTop();
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 36; i++) {
			if (touching(touch, MapPos[i])) {
				Selection = i;
				selected = true;
			}
		}
	}
	if (held & KEY_RIGHT) {
		if (Selection < 35)	Selection++;
		selected = true;
	}

	if (held & KEY_LEFT) {
		if (Selection > 0)	Selection--;
		selected = true;
	}

	if (held & KEY_UP) {
		if (Selection > 5)	Selection -= 6;
		selected = true;
	}

	if (held & KEY_DOWN) {
		if (Selection < 30)	Selection += 6;
		selected = true;
	}

	// Top Screen Acre Selection.
	if (held & KEY_L) {
		if (selectedAcre > 0) {
			selectedAcre--;
			updateTop();
		}
	}
	
	if (held & KEY_R) {
		if (selectedAcre < MAX_ACRE) {
			selectedAcre++;
			updateTop();
		}
	}
}