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

#include "keyboard.hpp"
#include "mapEditorWW.hpp"
#include "wwAcreManagement.hpp"
#include "wwsave.hpp"

extern WWSave * WWSaveFile;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

MapEditorWW::MapEditorWW() {
	// Initialize Temp Item.
	TempItem = std::make_shared<WWItem>(0);
	// Set Empty ID.
	this->TempItem->SetItemID(65521);
	TempContainer = std::make_shared<WWItemContainer>(this->TempItem);
}

int MapEditorWW::SelectionToAcre(int i) const {
	switch(i) {
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

void MapEditorWW::Draw(void) const {
	if (Mode == 0) {
		DrawMap();
	} else {
		DrawTempItem();
	}
}


// Max Position: 63x63. Cause 16x4 -1 (cause of 0) -> 63.
void MapEditorWW::DrawMap(void) const
{
	GFX::DrawTop();
	for (int i = 0; i < 16; i++) {
		WWAcreManagement::DrawAcre(WWSaveFile->town->FullAcres[SelectionToAcre(i)], MapPos[i].x, MapPos[i].y);
	}
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("TOWNMAP_EDITOR"), 400);
	DrawInformation();
	GFX::DrawBottom();
	WWAcreManagement::DrawAcre(WWSaveFile->town->FullAcres[SelectionToAcre(currentAcre)], 10, 40, 5, 5);
	DrawGrid();
	DrawPosition();

	// Draw Buttons on the right side.
	Gui::Draw_Rect(230, 75, 75, 30, DARKER_COLOR);
	Gui::Draw_Rect(230, 140, 75, 30, DARKER_COLOR);
	// 160 + 70 + 32.5
	Gui::DrawStringCentered(70+33, 83, 0.65f, WHITE, Lang::get("ITEMS"), 65);
//	Gui::DrawStringCentered(70+33, 148, 0.65f, WHITE, Lang::get("ACRES"), 65);
}

void MapEditorWW::injectTo(int MapSlot) {
	if (this->TempItem->GetName() != "???") {
		WWSaveFile->town->MapItems[MapSlot]->SetItemID(this->TempItem->GetItemID());
	}
}

void MapEditorWW::DrawInformation() const {
	int x;
	if (currentAcre < 4)	x = currentAcre;
	else if (currentAcre > 3 && currentAcre < 8)	x = currentAcre - 4;
	else if (currentAcre > 7 && currentAcre < 12)	x = currentAcre - 8;
	else	x = currentAcre - 12;

	Gui::drawGrid(5 + (x*32), 40 + (currentAcre/4*32), 32, 32, WHITE);

	// Display Informations.
	Gui::DrawString(190, 40, 0.8f, WHITE, "Current Position:\n" +  std::to_string(PositionX) + " | " + std::to_string(PositionY));
	Gui::DrawString(190, 90, 0.8f, WHITE, "Current Item:\n" +  WWSaveFile->town->MapItems[MapSelection]->GetName());
}

void MapEditorWW::DrawPosition(void) const {
	Gui::drawGrid(10 + (currentPosX*10), 40 + (currentPosY*10), 10, 10, WHITE);
}

/* NOTES:
	32x32 is the native Acre resolution.
	32 * 5 -> 160;; 160 % 16 --> 10.
	Notes end.
*/
void MapEditorWW::DrawGrid(void) const {
	for (int i = 0; i < 256; i++) {
		for (u32 y = 0; y < 16; y++) {
			for (u32 x = 0; x < 16; x++, i++) {
				Gui::drawGrid(10 + (x*10), 40 + (y*10), 10, 10, C2D_Color32(140, 140, 140, 190));
			}
		}
	}
}

void MapEditorWW::convertToSelection() {
	MapSelection = (currentAcre * 256) + (currentPosY * 16) + currentPosX;
}

void MapEditorWW::convertToPosition() {
	if (currentAcre < 4) {
		PositionX = (currentAcre * 16) + currentPosX;
	} else if (currentAcre > 3 && currentAcre < 8) {
		PositionX = (((currentAcre - 4) * 16)) + currentPosX;
	} else if (currentAcre > 7 && currentAcre < 12) {
		PositionX = (((currentAcre - 8) * 16)) + currentPosX;
	} else if (currentAcre > 11 && currentAcre < 16) {
		PositionX = (((currentAcre - 12) * 16)) + currentPosX;
	}

	int acre = 0;
	if (currentAcre < 4) {
		acre = 0;
	} else if (currentAcre > 3 && currentAcre < 8) {
		acre = 1;
	} else if (currentAcre > 7 && currentAcre < 12) {
		acre = 2;
	} else if (currentAcre > 11 && currentAcre < 16) {
		acre = 3;
	}

	PositionY = (acre * 16) + currentPosY;
}

void MapEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0) {
		MainLogic(hDown, hHeld, touch);
	} else {
		TempItemLogic(hDown, hHeld, touch);
	}
}

void MapEditorWW::MainLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		injectTo(MapSelection);
	}

	if (hDown & KEY_Y) {
		this->TempItem->SetItemID(WWSaveFile->town->MapItems[MapSelection]->GetItemID());
	}

	if (hDown & KEY_X) {
		Mode = 1;
	}

	if (hDown & KEY_RIGHT) {
		if (currentPosX == 15 && currentAcre < 15) {
			// Go one Acre next and reset X to 0.
			currentAcre++;
			currentPosX = 0;
			convertToPosition();
			convertToSelection();
		} else if (currentPosX < 15) {
			currentPosX++;
			convertToPosition();
			convertToSelection();
		}
	}

	if (hDown & KEY_LEFT) {
		if (currentPosX == 0 && currentAcre > 0) {
			// Go one Acre before.
			currentAcre--;
			currentPosX = 15;
			convertToPosition();
			convertToSelection();
		} else if (currentPosX > 0) {
			currentPosX--;
			convertToPosition();
			convertToSelection();
		}
	}

	if (hDown & KEY_DOWN) {
		if (currentPosY == 15 && currentAcre < 12) {
			// Go one Acre down & reset Y to 0.
			currentAcre += 4;
			currentPosY = 0;
			convertToPosition();
			convertToSelection();
		} else if (currentPosY < 15) {
			currentPosY++;
			convertToPosition();
			convertToSelection();
		}
	}

	if (hDown & KEY_UP) {
		if (currentPosY == 0 && currentAcre > 3) {
			// Go one Acre up.
			currentAcre -= 4;
			currentPosY = 15;
			convertToPosition();
			convertToSelection();
		} else if (currentPosY > 0) {
			currentPosY--;
			convertToPosition();
			convertToSelection();
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mapButtons[0])) {
			Mode = 1;
		}
	}
}

void MapEditorWW::DrawTempItem(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - Temp Item", 400);
	GFX::DrawBottom();
	for (int i = 0; i < 2; i++) {
		Gui::Draw_Rect(tempItemPos[i].x, tempItemPos[i].y, tempItemPos[i].w, tempItemPos[i].h, DARKER_COLOR);
		if (TempSelection == i) {
			GFX::DrawSprite(sprites_pointer_idx, tempItemPos[i].x+130, tempItemPos[i].y+25);
		}
	}
	// Item ID.
	Gui::DrawStringCentered(0, 45, 0.7f, WHITE, "Item ID: " + std::to_string(this->TempItem->GetItemID()), 320);
	// Item Name.
	Gui::DrawStringCentered(0, 95, 0.7f, WHITE, "Item Name: " + this->TempItem->GetName(), 320);
}


void MapEditorWW::TempItemLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Mode = 0;
	}

	if (hDown & KEY_DOWN) {
		if (TempSelection < 1)	TempSelection++;
	}

	if (hDown & KEY_UP) {
		if (TempSelection > 0)	TempSelection--;
	}

	if (hDown & KEY_A) {
		u16 newItem;
		switch (TempSelection) {
			case 0:
				newItem = Input::handleu16(5, "Enter the Decimal ID of the Item.", 32766, this->TempItem->GetItemID());
				this->TempItem->SetItemID(newItem);
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, tempItemPos[0])) {
			u16 newItem = Input::handleu16(5, "Enter the Decimal ID of the Item.", 32766, this->TempItem->GetItemID());
			this->TempItem->SetItemID(newItem);
		}
	}
}