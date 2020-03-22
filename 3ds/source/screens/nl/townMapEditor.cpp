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
#include "itemManagement.hpp"
#include "keyboard.hpp"
#include "offsets.hpp"
#include "save.hpp"
#include "townMapEditor.hpp"
#include "utils.hpp"

#include <3ds.h>

extern Save* SaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

TownMapEditor::TownMapEditor() {
	Msg::DisplayMsg(Lang::get("PREPARING_TOWN_EDITOR"));
	ItemManagement::loadItems();
	for(int i = 0; i < 5120; i++) {
		MapItems[i] = std::make_shared<ItemContainer>(Save::Instance()->town->MapItem[i]);
	}
	// Initialize Temp Items.
	TempItem = std::make_shared<Item>(0x7FFE, 0);
	TempContainer = std::make_shared<ItemContainer>(this->TempItem);
}

TownMapEditor::~TownMapEditor()
{
	ItemManagement::unloadItems();
}

void TownMapEditor::Draw(void) const {
	if (Mode == 0) {
		DrawMapScreen();
	} else if (Mode == 1) {
		DrawAcres(); // TODO.
	} else if (Mode == 2) {
		DrawTempItem();
	}
}

void TownMapEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0) {
		MapScreenLogic(hDown, hHeld, touch);
	} else if (Mode == 1) {
		AcresLogic(hDown, hHeld, touch); // TODO.
	} else if (Mode == 2) {
		TempItemLogic(hDown, hHeld, touch);
	}
}

int TownMapEditor::SelectionToAcre(int selection) const {
	switch (selection) {
		case 0:
			return 8;
			break;
		case 1:
			return 9;
			break;
		case 2:
			return 10;
			break;
		case 3:
			return 11;
			break;
		case 4:
			return 12;
		case 5:
			return 15;
			break;
		case 6:
			return 16;
			break;
		case 7:
			return 17;
		case 8:
			return 18;
			break;
		case 9:
			return 19;
			break;
		case 10:
			return 22;
			break;
		case 11:
			return 23;
			break;
		case 12:
			return 24;
			break;
		case 13:
			return 25;
			break;
		case 14:
			return 26;
			break;
		case 15:
			return 29;
			break;
		case 16:
			return 30;
			break;
		case 17:
			return 31;
			break;
		case 18:
			return 32;
			break;
		case 19:
			return 33;
			break;
	}
	return 8; // Should Never Happen.
}

void TownMapEditor::convertToSelection() {
	MapSelection = (currentAcre * 256) + (currentPosY * 16) + currentPosX;
}

void TownMapEditor::convertToPosition() {
	if (currentAcre < 5) {
		PositionX = (16 + (currentAcre * 16)) + currentPosX;
	} else if (currentAcre > 4 && currentAcre < 10) {
		PositionX = (16 + ((currentAcre - 5) * 16)) + currentPosX;
	} else if (currentAcre > 9 && currentAcre < 15) {
		PositionX = (16 + ((currentAcre - 10) * 16)) + currentPosX;
	} else if (currentAcre > 14 && currentAcre < 20) {
		PositionX = (16 + ((currentAcre - 15) * 16)) + currentPosX;
	}

	int acre = 0;
	if (currentAcre < 5) {
		acre = 0;
	} else if (currentAcre > 4 && currentAcre < 10) {
		acre = 1;
	} else if (currentAcre > 9 && currentAcre < 15) {
		acre = 2;
	} else if (currentAcre > 14 && currentAcre < 20) {
		acre = 3;
	}

	PositionY = (16 + acre * 16) + currentPosY;
}

// Display full Map on top screen for a better overview.
void TownMapEditor::DrawTownMap() const
{
	for (int i = 0; i < 20; i++) {
		AcreManagement::DrawAcre(SaveFile->town->FullAcres[SelectionToAcre(i)], townPos[i].x, townPos[i].y);
	}

	// Display Informations. The informations are placeholder for now, BTW.
	Gui::DrawString(210, 60, 0.7f, WHITE, Lang::get("CURRENT_POSITION") + std::to_string(PositionX) + " | " + std::to_string(PositionY), 150);
	Gui::DrawString(210, 90, 0.6f, WHITE, Lang::get("CURRENT_ITEM") + this->MapItems[MapSelection]->returnName(), 190);
	Gui::drawGrid(townPos[currentAcre].x, townPos[currentAcre].y, townPos[currentAcre].w, townPos[currentAcre].h, WHITE);
}

void TownMapEditor::DrawMapScreen(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("ISLANDMAP_EDITOR"), 400);
	DrawTownMap();

	// Bottom Screen part. Grid & Acre.
	GFX::DrawBottom(false);
	AcreManagement::DrawAcre(SaveFile->town->FullAcres[SelectionToAcre(currentAcre)], 20, 20, 5, 5);
	DrawGrid();
	DrawCurrentPos();

	// Draw Buttons on the right side.
	Gui::Draw_Rect(230, 75, 75, 30, DARKER_COLOR);
	Gui::Draw_Rect(230, 140, 75, 30, DARKER_COLOR);
	// 160 + 70 + 32.5
	Gui::DrawStringCentered(70+33, 83, 0.65f, WHITE, Lang::get("ITEMS"), 65);
	Gui::DrawStringCentered(70+33, 148, 0.65f, WHITE, Lang::get("ACRES"), 65);
	if (selectionMode == 0) {
		GFX::DrawSpriteBlend(sprites_pointer_idx, mapButtons[selection].x+70, mapButtons[selection].y+20, C2D_Color32(0, 0, 0, 170));
	} else {
		GFX::DrawSprite(sprites_pointer_idx, mapButtons[selection].x+70, mapButtons[selection].y+20);
	}
}

void TownMapEditor::DrawCurrentPos(void) const {
	Gui::drawGrid(20 + (currentPosX*12.5), 20 + (currentPosY*12.5), 12.5, 12.5, WHITE);
}



/* NOTES:
	- The native acre resolution is 40x40, I scaled it x4, so it's around ~ 160x160. x5 -> 200 --> 12.5.
	- A Grid is 12.5.
	- The Grid might be not like the actual ACNL ones, but it's the best I can do atm.
	- Notes end.
*/

void TownMapEditor::DrawGrid(void) const {
	for (int i = 0; i < 256; i++) {
		for (u32 y = 0; y < 16; y++) {
			for (u32 x = 0; x < 16; x++, i++) {
				Gui::drawGrid(20 + (x*12.5), 20 + (y*12.5), 12.5, 12.5, C2D_Color32(80, 80, 80, 100));
			}
		}
	}
}

// Only *Inject*, if Item is valid and not "???".
void TownMapEditor::injectTo(int MapSlot) {
	if (this->TempContainer->returnName() != "???") {
		ItemManagement::SetID(SaveFile->town->MapItem[MapSlot], this->TempItem->ID);
		ItemManagement::SetFlag(SaveFile->town->MapItem[MapSlot], this->TempItem->Flags);
		ItemManagement::RefreshItem(this->MapItems[MapSlot]);
	}
}

// Update Position, Selection & Acre Image only by navigating.
void TownMapEditor::updateStuff() {
	convertToSelection();
	convertToPosition();
}

void TownMapEditor::MapScreenLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("NOTE_TOWN_MAP_EDITOR") + "\n\n" + Lang::get("X_MODESWITCH") + "\n" + Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mapButtons[0])) {
			Mode = 2;
		} else if (touching(touch, mapButtons[1])) {
			Mode = 1;
		}
	}

	if (hDown & KEY_A) {
		injectTo(MapSelection);
	}

	if (hDown & KEY_Y) { 
		ItemManagement::SetID(this->TempItem, SaveFile->town->MapItem[MapSelection]->ID);
		ItemManagement::SetFlag(this->TempItem, SaveFile->town->MapItem[MapSelection]->Flags);
		this->TempContainer = std::make_shared<ItemContainer>(this->TempItem);
		ItemManagement::RefreshItem(this->TempContainer);
	}
	
	if (selectionMode == 0) {
		if (hDown & KEY_X) {
			selectionMode = 1;
		}

		if (hDown & KEY_RIGHT) {
			if (currentPosX == 15 && currentAcre < 19) {
				// Go one Acre next and reset X to 0.
				currentAcre++;
				currentPosX = 0;
				updateStuff();
			} else if (currentPosX < 15) {
				currentPosX++;
				updateStuff();
			}
		}

		if (hDown & KEY_LEFT) {
			if (currentPosX == 0 && currentAcre > 0) {
				// Go one Acre before.
				currentAcre--;
				currentPosX = 15;
				updateStuff();
			} else if (currentPosX > 0) {
				currentPosX--;
				updateStuff();
			}
		}

		if (hDown & KEY_DOWN) {
			if (currentPosY == 15 && currentAcre < 15) {
				// Go one Acre down & reset Y to 0.
				currentAcre += 5;
				currentPosY = 0;
				updateStuff();
			} else if (currentPosY < 15) {
				currentPosY++;
				updateStuff();
			}
		}

		if (hDown & KEY_UP) {
			if (currentPosY == 0 && currentAcre > 4) {
				// Go one Acre up.
				currentAcre -= 5;
				currentPosY = 15;
				updateStuff();
			} else if (currentPosY > 0) {
				currentPosY--;
				updateStuff();
			}
		}
	} else {
		if (hDown & KEY_UP) {
			if (selection > 0)	selection = 0;
		}

		if (hDown & KEY_DOWN) {
			if (selection < 1)	selection = 1;
		}

		if (hDown & KEY_X) {
			selectionMode = 0;
		}
		
		if (hDown & KEY_A) {
			if (selection == 0) {
				Mode = 2;
			} else if (selection == 1) {
				selection = 0;
				Mode = 1;
			}
		}
	}
}

void TownMapEditor::DrawTempItem(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - Temp Item", 400);
	GFX::DrawBottom();
	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(tempItemPos[i].x, tempItemPos[i].y, tempItemPos[i].w, tempItemPos[i].h, DARKER_COLOR);
		if (TempSelection == i) {
			GFX::DrawSprite(sprites_pointer_idx, tempItemPos[i].x+130, tempItemPos[i].y+25);
		}
	}
	// Item ID.
	Gui::DrawStringCentered(0, 45, 0.7f, WHITE, "Item ID: " + std::to_string(this->TempContainer->returnItemID()), 320);
	// Item Flags.
	Gui::DrawStringCentered(0, 95, 0.7f, WHITE, "Item Flags: " + std::to_string(this->TempContainer->returnItemFlag()), 320);
	// Item Name.
	Gui::DrawStringCentered(0, 145, 0.7f, WHITE, "Item Name: " + this->TempContainer->returnName(), 320);
}

void TownMapEditor::TempItemLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Mode = 0;
	}

	if (hDown & KEY_DOWN) {
		if (TempSelection < 2)	TempSelection++;
	}

	if (hDown & KEY_UP) {
		if (TempSelection > 0)	TempSelection--;
	}

	if (hDown & KEY_A) {
		switch (TempSelection) {
			case 0:
				this->TempItem->ID = Input::handleu16(5, "Enter the Decimal ID of the Item.", 32766, this->TempItem->ID);
				this->TempContainer = std::make_shared<ItemContainer>(this->TempItem);
				ItemManagement::RefreshItem(this->TempContainer);
				break;
			case 1:
				this->TempItem->Flags = Input::handleu16(2, "Enter the Flags.", 99, this->TempItem->Flags);
				this->TempContainer = std::make_shared<ItemContainer>(this->TempItem);
				ItemManagement::RefreshItem(this->TempContainer);
				break;
			case 2:
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, tempItemPos[0])) {
			this->TempItem->ID = Input::handleu16(5, "Enter the Decimal ID of the Item.", 32766, this->TempItem->ID);
			this->TempContainer = std::make_shared<ItemContainer>(this->TempItem);
			ItemManagement::RefreshItem(this->TempContainer);
		} else if (touching(touch, tempItemPos[1])) {
			this->TempItem->Flags = Input::handleu16(2, "Enter the Flags.", 99, this->TempItem->Flags);
			this->TempContainer = std::make_shared<ItemContainer>(this->TempItem);
			ItemManagement::RefreshItem(this->TempContainer);
		} else if (touching(touch, tempItemPos[2])) {
			// TODO: Item List Selection.
		}
	}
}

/* Acre Editor Stuff. */

#define MAX_ACRE 218 // Define the Max Amount of Acres.

void TownMapEditor::DrawAcres(void) const {
	DrawTopSelection();
	if (GodMode) {
		DrawFullMap();
	} else {
		DrawMap();
	}
}

void TownMapEditor::DrawFullMap(void) const {
	GFX::DrawBottom(false);
	for (int i = 0; i < 42; i++) {
		AcreManagement::DrawAcre(SaveFile->town->FullAcres[i], acreMapPos[i].x, acreMapPos[i].y);
	}
	// Draw Grid.
	for (int i = 0; i < 42; i++) {
		for (u32 y = 0; y < 6; y++) {
			for (u32 x = 0; x < 7; x++, i++) {
				Gui::drawGrid(20 + (x*40), 0 + (y*40), 40, 40, C2D_Color32(40, 40, 40, 160));
			}
		}
	}
	GFX::DrawSprite(sprites_pointer_idx, acreMapPos[selection].x+20, acreMapPos[selection].y+20);
}

void TownMapEditor::DrawMap(void) const {
	GFX::DrawBottom();
	for (int i = 0; i < 20; i++) {
		AcreManagement::DrawAcre(SaveFile->town->FullAcres[SelectionToAcre(i)], acreTownPos[i].x, acreTownPos[i].y);
	}
	// Draw Grid.
	for (int i = 0; i < 20; i++) {
		for (u32 y = 0; y < 4; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				Gui::drawGrid(60 + (x*40), 40 + (y*40), 40, 40, C2D_Color32(40, 40, 40, 160));
			}
		}
	}
	GFX::DrawSprite(sprites_pointer_idx, acreTownPos[selection].x+20, acreTownPos[selection].y+20);
}

void TownMapEditor::DrawTopSelection(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("ACRE_EDITOR"), 400);
	Gui::DrawStringCentered(0, 180, 0.8f, WHITE, "Acre ID: " + std::to_string(selectedAcre), 400);
	if (selectedAcre == 0) {
		AcreManagement::DrawAcre(selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		AcreManagement::DrawAcre(selectedAcre+1, 300, 100, 1, 1);
		AcreManagement::DrawAcre(selectedAcre+2, 350, 100, 1, 1);
	} else if (selectedAcre == 1) {
		AcreManagement::DrawAcre(selectedAcre-1, 60, 100, 1, 1);
		AcreManagement::DrawAcre(selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		AcreManagement::DrawAcre(selectedAcre+1, 300, 100, 1, 1);
		AcreManagement::DrawAcre(selectedAcre+2, 350, 100, 1, 1);
	} else if (selectedAcre == MAX_ACRE-1) {
		AcreManagement::DrawAcre(selectedAcre-2, 10, 100, 1, 1);
		AcreManagement::DrawAcre(selectedAcre-1, 60, 100, 1, 1);
		AcreManagement::DrawAcre(selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		AcreManagement::DrawAcre(selectedAcre+1, 300, 100, 1, 1);
	} else if (selectedAcre == MAX_ACRE) {
		AcreManagement::DrawAcre(selectedAcre-2, 10, 100, 1, 1);
		AcreManagement::DrawAcre(selectedAcre-1, 60, 100, 1, 1);
		AcreManagement::DrawAcre(selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
	} else {
		AcreManagement::DrawAcre(selectedAcre-2, 10, 100, 1, 1);
		AcreManagement::DrawAcre(selectedAcre-1, 60, 100, 1, 1);
		AcreManagement::DrawAcre(selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		AcreManagement::DrawAcre(selectedAcre+1, 300, 100, 1, 1);
		AcreManagement::DrawAcre(selectedAcre+2, 350, 100, 1, 1);
	}
}


void TownMapEditor::AcresLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		selection = 0;
		Mode = 0;
	}

	// Set Top Acre to Selection.
	if (hDown & KEY_A) {
		if (selectedAcre < 182 || selectedAcre > 205) {
			if (GodMode) {
				SaveFile->town->FullAcres[selection] = selectedAcre;
			} else {
				SaveFile->town->FullAcres[SelectionToAcre(selection)] = selectedAcre;
			}
		}
	}

	// Display Selected Acre on Top Selection.
	if (hDown & KEY_Y) {
		if (GodMode) {
			selectedAcre = SaveFile->town->FullAcres[selection];
		} else {
			selectedAcre = SaveFile->town->FullAcres[SelectionToAcre(selection)];
		}
	}

	if (hDown & KEY_DOWN) {
		if (GodMode) {
			if (selection < 35)	selection += 7;
		} else {
			if (selection < 15)	selection += 5;
		}
	}

	if (hDown & KEY_UP) {
		if (GodMode) {
			if (selection > 6)	selection -= 7;
		} else {
			if (selection > 4)	selection -= 5;
		}
	}

	if (hDown & KEY_RIGHT) {
		if (GodMode) {
			if (selection < 41)	selection++;
		} else {
			if (selection < 19)	selection++;
		}
	}

	if (hDown & KEY_LEFT) {
		if (selection > 0)	selection--;
	}

	// Top Screen Acre Selection.
	if (FastMode) {
		if (keysHeld() & KEY_L) {
			if (selectedAcre > 0) {
				selectedAcre--;
			}
		}
		if (keysHeld() & KEY_R) {
			if (selectedAcre < MAX_ACRE) {
				selectedAcre++;
			}
		}
	} else {
		if (hDown & KEY_L) {
			if (selectedAcre > 0) {
				selectedAcre--;
			}
		}
		if (hDown & KEY_R) {
			if (selectedAcre < MAX_ACRE) {
				selectedAcre++;
			}
		}
	}

	if (hDown & KEY_START) {
		if (FastMode)	FastMode = false;
		else	FastMode = true;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("NOTE_ACRE_EDITOR") + "\n\n" + Lang::get("A_SELECTION") +  "\n" + Lang::get("B_BACK"));
	}

	if (hDown & KEY_TOUCH) {
		if (GodMode) {
			for (int i = 0; i < 42; i++) {
				if (touching(touch, acreMapPos[i])) {
					selection = i;
				}
			}
		} else {
			for (int i = 0; i < 20; i++) {
				if (touching(touch, acreTownPos[i])) {
					selection = i;
				}
			}
		}
	}
}