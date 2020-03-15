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
#include "gui.hpp"
#include "itemManagement.hpp"
#include "townMapEditor.hpp"
#include "wwsave.hpp"
#include "wwTown.hpp"

extern WWSave* SaveFile;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

TownMapEditor::TownMapEditor() {
	// Init Town Map Items.
	for(int i = 0; i < 4096; i++) {
		MapItems[i] = std::make_shared<WWItemContainer>(WWSave::Instance()->town->MapItems[i]);
	}
	// Initialize Temp Item.
	TempItem = std::make_shared<WWItem>(0xFFF1);
	TempContainer = std::make_shared<WWItemContainer>(this->TempItem);
}


/* NOTES:
	32x32 is the native Acre resolution.
	32 * 5 -> 160;; 160 % 16 --> 10.
	Notes end.
*/
void TownMapEditor::DrawGrid(void) const {
	for (int i = 0; i < 256; i++) {
		for (u32 y = 0; y < 16; y++) {
			for (u32 x = 0; x < 16; x++, i++) {
				drawOutline(20 + (x*10), 15 + (y*10), 10, 10, DARK_GREEN, false, true);
			}
		}
	}
}

int TownMapEditor::SelectionToAcre() const {
	switch(currentAcre) {
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

// Max Position: 64x64. Cause 16x4 -> 64.
void TownMapEditor::Draw(void) const {
	Gui::DrawTop(false);
	AcreManagement::DrawTownMapEditor();
	// Display Informations.
	printText("Current Position:\n" +  std::to_string(PositionX) + " | " + std::to_string(PositionY), 150, 40, true, true);
	printText("Current Item:\n" + this->MapItems[MapSelection]->returnName(), 150, 70, true, true);

	Gui::DrawBottom(false);
	AcreManagement::DrawAcre(SaveFile->town->FullAcres[SelectionToAcre()], 20, 15, 5, 5, false);
	DrawGrid();
}

void TownMapEditor::DrawCurrentPos(void) const {
	drawOutline(20 + (currentPosX*10), 15 + (currentPosY*10), 10, 10, GRAY, false, true);
}

// TODO.
void TownMapEditor::Logic(u16 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		Gui::DrawScreen();
		selected = true;
		return;
	}
}