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

#include "acreManagement.hpp"
#include "acresEditor.hpp"
#include "wwsave.hpp"
#include "wwTown.hpp"

extern WWSave* SaveFile;
#define MAX_ACRE 130 // Define the Max Amount of Acres.

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void AcresEditor::DrawTop(void) const {
	printTextCentered("LeafEdit - Acres Editor", 0, 0, true, true);
	AcreManagement::DrawAcre(selectedAcre, 90, 70, 2, 2); // Current Selected ACRE.
}

void AcresEditor::Draw(void) const {
	Gui::DrawTop();
	DrawTop();
	Gui::DrawBottom();
	AcreManagement::DrawTownMap();
}

int AcresEditor::SelectionToPos() {
	switch(Selection) {
		case 0:
			return 7;
			break;
		case 1:
			return 8;
			break;
		case 2:
			return 9;
			break;
		case 3:
			return 10;
			break;
		case 4:
			return 13;
			break;
		case 5:
			return 14;
			break;
		case 6:
			return 15;
			break;
		case 7:
			return 16;
			break;
		case 8:
			return 19;
			break;
		case 9:
			return 20;
			break;
		case 10:
			return 21;
			break;
		case 11:
			return 22;
			break;
		case 12:
			return 25;
			break;
		case 13:
			return 26;
			break;
		case 14:
			return 27;
			break;
		case 15:
			return 28;
			break;
	}
	return 7; // Should Never Happen.
}

void AcresEditor::updateTop() {
	Gui::clearScreen(true, true);
	DrawTop(); // Refresh Top Screen.
}

void AcresEditor::updateMap() {
	Gui::clearScreen(false, true);
	AcreManagement::DrawTownMap();
}

void AcresEditor::Logic(u16 hDown, touchPosition touch) {
	Gui::updatePointer(MapPos[Selection].x+20, MapPos[Selection].y+20);

	if (hDown & KEY_B) {
		Gui::screenBack();
		Gui::DrawScreen();
		selected = true;
		return;
	}

	if (AcreMode == 0) {
		if (hDown & KEY_A) {
			SaveFile->town->FullAcres[SelectionToPos()] = selectedAcre;
			updateMap();
		}

		if (hDown & KEY_RIGHT) {
			if (Selection < 15)	Selection++;
			selected = true;
		}

		if (hDown & KEY_LEFT) {
			if (Selection > 0)	Selection--;
			selected = true;
		}

		if (hDown & KEY_UP) {
			if (Selection > 4)	Selection -= 4;
			selected = true;
		}

		if (hDown & KEY_DOWN) {
			if (Selection < 12)	Selection += 4;
			selected = true;
		}
	} else if (AcreMode == 1) {
		if (hDown & KEY_RIGHT) {
			if (selectedAcre < MAX_ACRE)	selectedAcre++;
			updateTop();
		}
		if (hDown & KEY_LEFT) {
			if (selectedAcre > 0)	selectedAcre--;
			updateTop();
		}
	}

	if (hDown & KEY_X) {
		if (AcreMode == 0)	AcreMode = 1;
		else	AcreMode = 0;
	}
}