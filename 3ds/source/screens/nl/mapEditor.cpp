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
#include "mapEditor.hpp"
#include "offsets.hpp"
#include "save.hpp"
#include "utils.hpp"

#include <3ds.h>

extern Save* SaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

MapEditor::MapEditor() {
	Msg::DisplayMsg(Lang::get("PREPARING_TOWN_EDITOR"));
	ItemManagement::loadItems();
	for(int i = 0; i < 5120; i++) {
		MapItems[i] = std::make_shared<ItemContainer>(Save::Instance()->town->MapItem[i]);
	}
}

MapEditor::~MapEditor()
{
	ItemManagement::unloadItems();
}

void MapEditor::Draw(void) const {
	if (Mode == 0) {
		DrawMapScreen();
	} else if (Mode == 1) {
		DrawBuildingList(); // TODO.
	} else if (Mode == 2) {
		DrawBuildingEditor();
	} else if (Mode == 3) {
		DisplayMap();
	}
}

void MapEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0) {
		MapScreenLogic(hDown, hHeld, touch);
	} else if (Mode == 1) {
		BuildingListLogic(hDown, hHeld, touch); // TODO.
	} else if (Mode == 2) {
		BuildingEditorLogic(hDown, hHeld, touch);
	} else if (Mode == 3) {
		BuildingSetLogic(hDown, hHeld, touch);
	}
}

void MapEditor::convertToSelection() {
	MapSelection = (currentAcre * 256) + (currentPosY * 16) + currentPosX;
}

void MapEditor::convertToPosition() {
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
void MapEditor::DrawTownMap() const
{
	AcreManagement::InitAcres(20, 5, 5, 40, 4, MAP_ACRES + 0x10);
	// Display Informations. The informations are placeholder for now, BTW.
	Gui::DrawString(210, 60, 0.7f, WHITE, Lang::get("CURRENT_POSITION") + std::to_string(PositionX) + " | " + std::to_string(PositionY), 150);
	Gui::DrawString(210, 90, 0.6f, WHITE, Lang::get("CURRENT_ITEM") + this->MapItems[MapSelection]->returnName(), 190);

	// Selection Logic.
	int x;
	if (currentAcre < 5)	x = currentAcre;
	else if (currentAcre > 4 && currentAcre < 10)	x = currentAcre - 5;
	else if (currentAcre > 9 && currentAcre < 15)	x = currentAcre - 10;
	else	x = currentAcre - 15;

	Gui::drawGrid(5 + x * 40, 40 + currentAcre/5 * 40, 40, 40, WHITE);
}

void MapEditor::DrawMapScreen(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("TOWNMAP_EDITOR"), 400);
	DrawTownMap();

	// Bottom Screen part. Grid & Acre.
	GFX::DrawBottom(false);
	AcreManagement::DrawAcre(SaveFile->ReadU8(MAP_ACRES + acreImage), 20, 20, 5, 5);
	DrawGrid();
	DrawCurrentPos();

	// Draw Buttons on the right side.
	Gui::Draw_Rect(230, 75, 75, 30, DARKER_COLOR);
	Gui::Draw_Rect(230, 140, 75, 30, DARKER_COLOR);
	// 160 + 70 + 32.5
	Gui::DrawStringCentered(70+33, 83, 0.65f, WHITE, Lang::get("ITEMS"), 65);
	Gui::DrawStringCentered(70+33, 148, 0.65f, WHITE, Lang::get("BUILDINGS"), 65);
	if (selectionMode == 0) {
		GFX::DrawSpriteBlend(sprites_pointer_idx, mapButtons[selection].x+70, mapButtons[selection].y+20, C2D_Color32(0, 0, 0, 170));
	} else {
		GFX::DrawSprite(sprites_pointer_idx, mapButtons[selection].x+70, mapButtons[selection].y+20);
	}
}

void MapEditor::DrawCurrentPos(void) const {
	Gui::drawGrid(20 + (currentPosX*12.5), 20 + (currentPosY*12.5), 12.5, 12.5, WHITE);
}



/* NOTES:
	- The native acre resolution is 40x40, I scaled it x4, so it's around ~ 160x160. x5 -> 200 --> 12.5.
	- A Grid is 12.5.
	- The Grid might be not like the actual ACNL ones, but it's the best I can do atm.
	- Notes end.
*/

void MapEditor::DrawGrid(void) const {
	for (int i = 0; i < 256; i++) {
		for (u32 y = 0; y < 16; y++) {
			for (u32 x = 0; x < 16; x++, i++) {
				Gui::drawGrid(20 + (x*12.5), 20 + (y*12.5), 12.5, 12.5, C2D_Color32(80, 80, 80, 100));
			}
		}
	}
}


void MapEditor::updateAcre(void) {
	// First row.
	if (currentAcre == 0) {
		acreImage = 0x10;
	} else if (currentAcre == 1) {
		acreImage = 0x12;
	} else if (currentAcre == 2) {
		acreImage = 0x14;
	} else if (currentAcre == 3) {
		acreImage = 0x16;
	} else if (currentAcre == 4) {
		acreImage = 0x18;
	}
	// Second row.
	else if (currentAcre == 5) {
		acreImage = 0x1E;
	} else if (currentAcre == 6) {
		acreImage = 0x20;
	} else if (currentAcre == 7) {
		acreImage = 0x22;
	} else if (currentAcre == 8) {
		acreImage = 0x24;
	} else if (currentAcre == 9) {
		acreImage = 0x26;
	}
	// Third row.
	else if (currentAcre == 10) {
		acreImage = 0x2C;
	} else if (currentAcre == 11) {
		acreImage = 0x2E;
	} else if (currentAcre == 12) {
		acreImage = 0x30;
	} else if (currentAcre == 13) {
		acreImage = 0x32;
	} else if (currentAcre == 14) {
		acreImage = 0x34;
	}
	// Fourth row.
	else if (currentAcre == 15) {
		acreImage = 0x3A;
	} else if (currentAcre == 16) {
		acreImage = 0x3C;
	} else if (currentAcre == 17) {
		acreImage = 0x3E;
	} else if (currentAcre == 18) {
		acreImage = 0x40;
	} else if (currentAcre == 19) {
		acreImage = 0x42;
	}
}

// Update Position, Selection & Acre Image only by navigating.
void MapEditor::updateStuff() {
	convertToSelection();
	convertToPosition();
	updateAcre();
}

void MapEditor::MapScreenLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("NOTE_TOWN_MAP_EDITOR") + "\n\n" + Lang::get("X_MODESWITCH") + "\n" + Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mapButtons[1])) {
			Mode = 1;
		}
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
			if (selection == 1) {
				Mode = 1;
			}
		}
	}
}

// The Building list.
void MapEditor::DrawBuildingList(void) const {
	std::string buildings;
	GFX::DrawFileBrowseBG();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("BUILDINGS"), 400);
	for (int i = (BuildingSelection<5) ? 0 : BuildingSelection-5;i< 58 && i <((BuildingSelection<5) ? 6 : BuildingSelection+1);i++) {
		if (i == BuildingSelection) {
			buildings += "> " + std::to_string(i+1) + ".) " + SaveFile->buildings->GetName(i) + " - " + std::to_string(SaveFile->buildings->returnXPos(i)) + " | " + std::to_string(SaveFile->buildings->returnYPos(i)) + "\n\n";
		} else {
			buildings += std::to_string(i+1) + ".) " + SaveFile->buildings->GetName(i) + " - " + std::to_string(SaveFile->buildings->returnXPos(i)) + " | " + std::to_string(SaveFile->buildings->returnYPos(i)) + "\n\n";
		}
	}
	for (int i=0;i<((58<6) ? 6-58 : 0);i++) {
		buildings += "\n\n";
	}
	Gui::DrawString(26, 32, 0.65f, WHITE, buildings, 360);
	Gui::DrawStringCentered(0, 215, 0.9f, WHITE, Lang::get("BUILDING_COUNT") + std::to_string(SaveFile->buildings->getBuildCount()), 400);
	GFX::DrawFileBrowseBG(false);
}

void MapEditor::BuildingListLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Mode = 0;
		BuildingSelection = 0; // Reset that one too.
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("B_BACK") + "\n" + Lang::get("A_SELECTION"));
	}

	if (hDown & KEY_A) {
		if (SaveFile->buildings->returnID(BuildingSelection) != 0xFC) {
			Mode = 2;
			selection = 0;
		}
	}

	if (hDown & KEY_DOWN) {
		if (BuildingSelection < 57)	BuildingSelection++;
	}
	if (hDown & KEY_UP) {
		if (BuildingSelection > 0)	BuildingSelection--;
	}
}


void MapEditor::DrawBuildingEditor(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("BUILDINGS"), 400);

	Gui::DrawStringCentered(0, 40, 0.8f, WHITE, Lang::get("CURRENT_BUILDING") + SaveFile->buildings->GetName(BuildingSelection), 400);
	Gui::DrawStringCentered(0, 80, 0.8f, WHITE, Lang::get("BUILDING_ID") + std::to_string(SaveFile->buildings->returnID(BuildingSelection)), 400);
	Gui::DrawStringCentered(0, 120, 0.8f, WHITE, Lang::get("BUILDING_XPOS") + std::to_string(SaveFile->buildings->returnXPos(BuildingSelection)), 400);
	Gui::DrawStringCentered(0, 160, 0.8f, WHITE, Lang::get("BUILDING_YPOS") + std::to_string(SaveFile->buildings->returnYPos(BuildingSelection)), 400);
	GFX::DrawBottom();
	for (int i = 0; i < 2; i++) {
		Gui::Draw_Rect(buildingButtons[i].x, buildingButtons[i].y, buildingButtons[i].w, buildingButtons[i].h, UNSELECTED_COLOR);
		if (selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, buildingButtons[i].x+130, buildingButtons[i].y+25);
		}
	}
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("SET_ID")))/2-80+17.5, 0.8, WHITE, Lang::get("SET_ID"), 130, 25);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("SET_POSITION")))/2-20+17.5, 0.8, WHITE, Lang::get("SET_POSITION"), 130, 25);
}

void MapEditor::BuildingEditorLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Mode = 1;
		selection = 0;
	}

	if (hDown & KEY_DOWN) {
		if (selection < 2)	selection++;
	}
	if (hDown & KEY_UP) {
		if (selection > 0)	selection--;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("B_BACK") + "\n" + Lang::get("A_SELECTION"));
	}

	if (hDown & KEY_A) {
		if (selection == 0) {
			u16 newID = Input::handleu16(10, Lang::get("ENTER_NEW_ID"), 0xfc, SaveFile->buildings->returnID(BuildingSelection));
			SaveFile->buildings->setBuilding(BuildingSelection, newID);
		} else if (selection == 1) {
			ResetPositions();
			Mode = 3;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, buildingButtons[0])) {
			u16 newID = Input::handleu16(10, Lang::get("ENTER_NEW_ID"), 0xfc, SaveFile->buildings->returnID(BuildingSelection));
			SaveFile->buildings->setBuilding(BuildingSelection, newID);
		} else if (touching(touch, buildingButtons[1])) {
			ResetPositions();
			Mode = 3;
		}
	}
}

// Display the Map, so you can set the Building position.
void MapEditor::DisplayMap(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, Lang::get("SELECT_POSITION"), 400);
	AcreManagement::InitAcres(20, 5, 5, 40, 4, MAP_ACRES + 0x10);
	Gui::DrawString(210, 60, 0.7f, WHITE, Lang::get("NEW_POSITION") + std::to_string(PositionX) + " | " + std::to_string(PositionY), 150);
	Gui::DrawString(210, 100, 0.7f, WHITE, Lang::get("OLD_POSITION") + std::to_string(SaveFile->buildings->returnXPos(BuildingSelection)) + " | " + std::to_string(SaveFile->buildings->returnYPos(BuildingSelection)), 150);
	// Selection Logic.
	int x;
	if (currentAcre < 5)	x = currentAcre;
	else if (currentAcre > 4 && currentAcre < 10)	x = currentAcre - 5;
	else if (currentAcre > 9 && currentAcre < 15)	x = currentAcre - 10;
	else	x = currentAcre - 15;
	Gui::drawGrid(5 + x * 40, 40 + currentAcre/5 * 40, 40, 40, WHITE);
	Gui::DrawStringCentered(0, 216, 0.9f, WHITE, Lang::get("BUILDING_NOTE"), 400);
	// Bottom Screen part. Grid & Acre.
	GFX::DrawBottom(false);
	AcreManagement::DrawAcre(SaveFile->ReadU8(MAP_ACRES + acreImage), 20, 20, 5, 5);
	DrawGrid();
	DrawCurrentPos();
}

// Reset everything to the begin.
void MapEditor::ResetPositions() {
	currentAcre = 0;
	currentPosX = 0;
	currentPosY = 0;
	// Update Acre Image and such after setting it.
	updateStuff();
}

void MapEditor::BuildingSetLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_A) {
		if (Msg::promptMsg2(Lang::get("PLACE_BUILDING_HERE") + "\n" + std::to_string(PositionX) + " | " + std::to_string(PositionY))) {
			SaveFile->buildings->setXPos(BuildingSelection, (u8)PositionX);
			SaveFile->buildings->setYPos(BuildingSelection, (u8)PositionY);
			ResetPositions();
			Mode = 2;
		}
	}

	if (hDown & KEY_B) {
		ResetPositions();
		Mode = 2;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("B_BACK") + "\n" + Lang::get("A_SELECTION"));
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
}