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

#include "graphicManagement.hpp"
#include "itemManager.hpp"
#include "itemUtils.hpp"
#include "gui.hpp"
#include "input.hpp"
#include "overlay.hpp"
#include "townMapEditor.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

TownMapEditor::TownMapEditor() {
	// Get Town Items.
	for (int i = 0; i < 4096; i++) {
		this->MapItems[i] = save->town()->item(i);
	}

	for (int i = 0; i < 36; i++) {
		this->townAcres[i] = save->town()->acre(i);
	}

	// Initialize Temp Item.
	this->TempItem = 65521;
}

void TownMapEditor::DrawTempItem() const {
	Gui::DrawTop(true);
	printTextCentered("LeafEdit - " + Lang::get("TEMP_ITEM"), 0, 0, true, true);
	printTextCentered(Lang::get("ITEM_NAME") + ItemUtils::getName(this->TempItem), 0, 50, true, true);
	printTextCentered(Lang::get("ITEM_ID") + std::to_string(this->TempItem), 0, 80, true, true);
	Gui::DrawBottom(true);

	for (int i = 0; i < 3; i++) {
		drawRectangle(TempPos[i].x, TempPos[i].y, TempPos[i].w, TempPos[i].h, GRAY, false, true);
	}

	printTextCentered(Lang::get("ITEM_MANUALLY"), 0, 40, false, true);
	printTextCentered(Lang::get("ITEM_SELECTION"), 0, 90, false, true);
}


/* NOTES:
	32x32 is the native Acre resolution.
	32 * 5 -> 160;; 160 % 16 --> 10.
	Notes end.
*/
void TownMapEditor::DrawGrid(void) const {
	for (int i = 0 + (currentAcre * 256); i < 256 + (currentAcre * 256); i++) {
		for (u32 y = 0; y < 16; y++) {
			for (u32 x = 0; x < 16; x++, i++) {
				drawRectangle(10 + (x*10), 15 + (y*10), 10, 10, ItemManager::getColor(this->MapItems[i]->itemtype()), false, true);
				drawOutline(10 + (x*10), 15 + (y*10), 10, 10, BLACK, false, true);
			}
		}
	}
}

void TownMapEditor::DrawTownMapEditor() const {
	// First Line.
	GraphicManagement::DrawAcre(this->townAcres[7]->id(), 10, 30, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[8]->id(), 42, 30, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[9]->id(), 74, 30, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[10]->id(), 106, 30, 1, 1, true, false);
	// Second Line.
	GraphicManagement::DrawAcre(this->townAcres[13]->id(), 10, 62, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[14]->id(), 42, 62, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[15]->id(), 74, 62, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[16]->id(), 106, 62, 1, 1, true, false);
	// Third Line.
	GraphicManagement::DrawAcre(this->townAcres[19]->id(), 10, 94, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[20]->id(), 42, 94, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[21]->id(), 74, 94, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[22]->id(), 106, 94, 1, 1, true, false);
	// Fourth Line.
	GraphicManagement::DrawAcre(this->townAcres[25]->id(), 10, 126, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[26]->id(), 42, 126, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[27]->id(), 74, 126, 1, 1, true, false);
	GraphicManagement::DrawAcre(this->townAcres[28]->id(), 106, 126, 1, 1, true, false);
}

int TownMapEditor::SelectionToAcre() const {
	switch(this->currentAcre) {
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

void TownMapEditor::DrawPosition(void) const {
	drawOutline(10 + (this->currentPosX * 10), 15 + (this->currentPosY * 10), 10, 10, LIGHT_GRAY, false, true);
}

// Max Position: 63x63. Cause 16x4 -1 (cause of 0) -> 63.
void TownMapEditor::DrawMain(void) const {
	Gui::DrawTop(false);
	DrawTownMapEditor();
	for (int i = 0; i < 16; i++) {
		for (u32 y = 0; y < 4; y++) {
			for (u32 x = 0; x < 4; x++, i++) {
				drawOutline(10 + (x*32), 30 + (y*32), 32, 32, BLACK, true, false);
			}
		}
	}

	DrawInformation();
	Gui::DrawBottom(false);
	GraphicManagement::DrawAcre(this->townAcres[SelectionToAcre()]->id(), 10, 15, 5, 5, false, false);
	DrawGrid();
	DrawPosition();
	drawRectangle(175, 100, 75, 25, GRAY, false, false);
	printText(Lang::get("TEMP_ITEM"), 178, 105, false, false);
}

void TownMapEditor::Draw(void) const {
	if (this->Mode == 0) {
		DrawMain();
	} else {
		DrawTempItem();
	}
}

void TownMapEditor::MainLogic(u16 hDown, touchPosition touch) {
	u16 held = keysDownRepeat();

	if (hDown & KEY_B) {
		Gui::screenBack();
		Gui::DrawScreen();
		Gui::showPointer();
		selected = true;
		return;
	}

	if ((hDown & KEY_X) || (hDown & KEY_TOUCH && touching(touch, TempPos[3]))) {
		Mode = 1;
		Gui::DrawScreen();
		Gui::showPointer();
		selected = true;
	}

	if (hDown & KEY_A) {
		injectTo(this->MapSelection);
	}

	if (hDown & KEY_Y) {
		this->TempItem = this->MapItems[MapSelection]->id();
	}

	if (held & KEY_RIGHT) {
		if (this->currentPosX == 15 && this->currentAcre < 15) {
			// Go one Acre next and reset X to 0.
			this->currentAcre++;
			this->currentPosX = 0;
			convertToSelection();
			updateAcreImage();
			convertToPosition();
			updateBottomGrid();
			updateTopGrid();
		} else if (this->currentPosX < 15) {
			this->currentPosX++;
			convertToSelection();
			convertToPosition();
			updateBottomGrid();
			updateTopGrid();
		}
	}

	if (held & KEY_LEFT) {
		if (this->currentPosX == 0 && this->currentAcre > 0) {
			// Go one Acre before.
			this->currentAcre--;
			this->currentPosX = 15;
			convertToSelection();
			updateAcreImage();
			convertToPosition();
			updateBottomGrid();
			updateTopGrid();
		} else if (this->currentPosX > 0) {
			this->currentPosX--;
			convertToSelection();
			convertToPosition();
			updateBottomGrid();
			updateTopGrid();
		}
	}

	if (held & KEY_DOWN) {
		if (this->currentPosY == 15 && this->currentAcre < 12) {
			// Go one Acre down & reset Y to 0.
			this->currentAcre += 4;
			this->currentPosY = 0;
			convertToSelection();
			updateAcreImage();
			convertToPosition();
			updateBottomGrid();
			updateTopGrid();
		} else if (this->currentPosY < 15) {
			this->currentPosY++;
			convertToSelection();
			convertToPosition();
			updateBottomGrid();
			updateTopGrid();
		}
	}

	if (held & KEY_UP) {
		if (this->currentPosY == 0 && this->currentAcre > 3) {
			// Go one Acre up.
			this->currentAcre -= 4;
			this->currentPosY = 15;
			convertToSelection();
			updateAcreImage();
			convertToPosition();
			updateBottomGrid();
			updateTopGrid();
		} else if (this->currentPosY > 0) {
			this->currentPosY--;
			convertToSelection();
			convertToPosition();
			updateBottomGrid();
			updateTopGrid();
		}
	}
}

void TownMapEditor::TempLogic(u16 hDown, touchPosition touch) {
	Gui::updatePointer(TempPos[this->selection].x+60, TempPos[this->selection].y+12);

	if (hDown & KEY_B) {
		this->Mode = 0;
		Gui::DrawScreen();
		Gui::hidePointer();
	}

	if (hDown & KEY_A) {
		if (selection == 0) {
			int ID = Input::getInt(Lang::get("ENTER_DECIMAL_ID"), 99999);
			if (ID != -1) {
				this->TempItem = ID;
				Gui::DrawScreen();
			}
		} else if (this->selection == 1) {
			Gui::clearScreen(true, true);
			this->TempItem = Overlays::SelectItem(this->TempItem);
			Gui::DrawScreen();
		}
	}

	if (hDown & KEY_DOWN) {
		if (this->selection < 2) {
			this->selection++;
			selected = true;
		}
	}

	if (hDown & KEY_UP) {
		if (this->selection > 0) {
			this->selection--;
			selected = true;
		}
	}
}

void TownMapEditor::injectTo(int MapSlot) {
	if (ItemUtils::getName(this->TempItem) != "???") {
		this->MapItems[MapSlot]->id(this->TempItem);
		updateBottomGrid();
		Gui::DrawScreen();
		changes = true;
	}
}

void TownMapEditor::DrawInformation() const {
	int x;
	if (this->currentAcre < 4)	x = this->currentAcre;
	else if (this->currentAcre > 3 && this->currentAcre < 8) x = this->currentAcre - 4;
	else if (this->currentAcre > 7 && this->currentAcre < 12) x = this->currentAcre - 8;
	else x = this->currentAcre - 12;
	drawOutline(10 + (x*32), 30 + (this->currentAcre/4*32), 32, 32, WHITE, true, true);

	// Display Informations.
	printText(Lang::get("CURRENT_POSITION") + "\n" + std::to_string(this->PositionX) + " | " + std::to_string(this->PositionY), 150, 40, true, true);
	printText(Lang::get("CURRENT_ITEM") + "\n" + ItemUtils::getName(this->MapItems[this->MapSelection]->id()), 150, 70, true, true);
}

void TownMapEditor::updateTopGrid() {
	Gui::clearScreen(true, true); // Clear Top.
	DrawInformation();
}

void TownMapEditor::convertToPosition() {
	if (this->currentAcre < 4) {
		this->PositionX = (this->currentAcre * 16) + this->currentPosX;
	} else if (this->currentAcre > 3 && this->currentAcre < 8) {
		this->PositionX = (((this->currentAcre - 4) * 16)) + this->currentPosX;
	} else if (this->currentAcre > 7 && this->currentAcre < 12) {
		this->PositionX = (((this->currentAcre - 8) * 16)) + this->currentPosX;
	} else if (this->currentAcre > 11 && this->currentAcre < 16) {
		this->PositionX = (((this->currentAcre - 12) * 16)) + this->currentPosX;
	}

	int acre = 0;
	if (this->currentAcre < 4) {
		acre = 0;
	} else if (this->currentAcre > 3 && this->currentAcre < 8) {
		acre = 1;
	} else if (this->currentAcre > 7 && this->currentAcre < 12) {
		acre = 2;
	} else if (this->currentAcre > 11 && this->currentAcre < 16) {
		acre = 3;
	}

	this->PositionY = (acre * 16) + this->currentPosY;
}

void TownMapEditor::updateBottomGrid() {
	Gui::clearScreen(false, true); // Clear Bottom.
	// Update Item Grid.
	for (int i = 0 + (this->currentAcre * 256); i < 256 + (this->currentAcre * 256); i++) {
		for (u32 y = 0; y < 16; y++) {
			for (u32 x = 0; x < 16; x++, i++) {
				drawRectangle(10 + (x*10), 15 + (y*10), 10, 10, ItemManager::getColor(this->MapItems[i]->itemtype()), false, true);
				drawOutline(10 + (x*10), 15 + (y*10), 10, 10, BLACK, false, true);
			}
		}
	}

	DrawPosition();
}

void TownMapEditor::updateAcreImage() {
	Gui::clearScreen(false, false); // Clear Layer.
	Gui::DrawBottom(false); // Draw Base UI.
	GraphicManagement::DrawAcre(this->townAcres[SelectionToAcre()]->id(), 10, 15, 5, 5, false, false);
	drawRectangle(175, 100, 75, 25, GRAY, false, false);
	printText(Lang::get("TEMP_ITEM"), 178, 105, false, false);
}

void TownMapEditor::convertToSelection() {
	this->MapSelection = (this->currentAcre * 256) + (this->currentPosY * 16) + this->currentPosX;
}

// TODO.
void TownMapEditor::Logic(u16 hDown, touchPosition touch) {
	if (this->Mode == 0) {
		MainLogic(hDown, touch);
	} else {
		TempLogic(hDown, touch);
	}
}