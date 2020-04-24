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

#include "itemManager.hpp"
#include "itemUtils.hpp"
#include "keyboard.hpp"
#include "Sav.hpp"
#include "saveUtils.hpp"
#include "screenCommon.hpp"
#include "spriteManagement.hpp"
#include "townMapEditor.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

extern std::shared_ptr<Sav> save;
// Bring that to other screens too.
extern SaveType savesType;

void TownMapEditor::Draw(void) const
{
	if (Mode == 0) {
		DrawMapScreen();
	} else if (Mode == 1) {
		DrawAcres();
	} else if (Mode == 2) {
		DrawTempItem();
	}
}

void TownMapEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0) {
		MapScreenLogic(hDown, hHeld, touch);
	} else if (Mode == 1) {
		AcresLogic(hDown, hHeld, touch);
	} else if (Mode == 2) {
		TempItemLogic(hDown, hHeld, touch);
	}
}

// Initialize Screen.
TownMapEditor::TownMapEditor() {
	ItemUtils::LoadDatabase(1, savesType); // TODO: Handle this at Editor loading instead?
	switch(savesType) {
		case SaveType::WW:
			// Get Town Map Items.
			for (int i = 0; i < 4096; i++) {
				this->MapItems[i] = save->town()->item(i);
			}
			// Get Full Acres.
			for (int i = 0; i < 36; i++) {
				this->FullAcres[i] = save->town()->acre(i);
			}
			maxAcres = 130;
			PositionX = 0;
			PositionY = 0;
			break;
		case SaveType::NL:
			// Get Town Map Items.
			for (int i = 0; i < 5120; i++) {
				this->MapItems[i] = save->town()->item(i);
			}
			// Get Full Acres.
			for (int i = 0; i < 42; i++) {
				this->FullAcres[i] = save->town()->acre(i);
			}
			maxAcres = 203;
			break;
		case SaveType::WA:
			// Get Town Map Items.
			for (int i = 0; i < 5120; i++) {
				this->MapItems[i] = save->town()->item(i);
			}
			// Get Full Acres.
			for (int i = 0; i < 42; i++) {
				this->FullAcres[i] = save->town()->acre(i);
			}
			maxAcres = 205;
			break;
		case SaveType::UNUSED:
			break;
	}
}

// Convert the Selection to the current Acre.
int TownMapEditor::SelectionToAcre(int i) const {
	if (savesType == SaveType::WW) {
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
	} else {
		switch(i) {
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
	}
	return 8; // Should Never Happen.
}

// Draw the Item Grid.
void TownMapEditor::DrawGrid(void) const {
	for (int i = 0 + (currentAcre * 256); i < 256 + (currentAcre * 256); i++) {
		for (u32 y = 0; y < 16; y++) {
			for (u32 x = 0; x < 16; x++, i++) {
				if(savesType == SaveType::WW) {
					GFX::drawGrid(10 + (x*10), 40 + (y*10), 10, 10, ItemManager::getColor(MapItems[i]->itemtype()));
				} else {
					GFX::drawGrid(15 + (x*12.5), 20 + (y*12.5), 12.5, 12.5, ItemManager::getColor(MapItems[i]->itemtype()));
				}
			}
		}
	}
}

// Convert to Position.
void TownMapEditor::convertToPosition() {
	int acre = 0;
	if (savesType == SaveType::WW) {
		if (currentAcre < 4) {
			PositionX = (currentAcre * 16) + currentPosX;
		} else if (currentAcre > 3 && currentAcre < 8) {
			PositionX = (((currentAcre - 4) * 16)) + currentPosX;
		} else if (currentAcre > 7 && currentAcre < 12) {
			PositionX = (((currentAcre - 8) * 16)) + currentPosX;
		} else if (currentAcre > 11 && currentAcre < 16) {
			PositionX = (((currentAcre - 12) * 16)) + currentPosX;
		}

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
	} else {
		if (currentAcre < 5) {
			PositionX = (16 + (currentAcre * 16)) + currentPosX;
		} else if (currentAcre > 4 && currentAcre < 10) {
			PositionX = (16 + ((currentAcre - 5) * 16)) + currentPosX;
		} else if (currentAcre > 9 && currentAcre < 15) {
			PositionX = (16 + ((currentAcre - 10) * 16)) + currentPosX;
		} else if (currentAcre > 14 && currentAcre < 20) {
			PositionX = (16 + ((currentAcre - 15) * 16)) + currentPosX;
		}

	
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
}

// Convert to the Map Selection.
void TownMapEditor::convertToSelection() {
	MapSelection = (currentAcre * 256) + (currentPosY * 16) + currentPosX;
}

// Display a few information on the Top Screen.
void TownMapEditor::DrawInformation() const {
	int x;
	if(savesType == SaveType::WW) {
		for (int i = 0; i < 16; i++) {
			SpriteManagement::DrawAcres(FullAcres[SelectionToAcre(i)]->id(), WWMapPos[i].x, WWMapPos[i].y);
		}
		if (currentAcre < 4)	x = currentAcre;
		else if (currentAcre > 3 && currentAcre < 8)	x = currentAcre - 4;
		else if (currentAcre > 7 && currentAcre < 12)	x = currentAcre - 8;
		else	x = currentAcre - 12;
		// Display Informations.
		Gui::DrawString(190, 40, 0.8f, BLACK, "Current Position:\n" +  std::to_string(PositionX) + " | " + std::to_string(PositionY));
		Gui::DrawString(190, 90, 0.8f, BLACK, "Current Item:\n" + MapItems[MapSelection]->name());
		Gui::drawGrid(5 + (x*32), 40 + (currentAcre/4*32), 32, 32, BLACK);
	} else {
		for (int i = 0; i < 20; i++) {
			SpriteManagement::DrawAcres(FullAcres[SelectionToAcre(i)]->id(), townPos[i].x, townPos[i].y);
		}
		Gui::DrawString(210, 60, 0.7f, BLACK, "Current Position: " + std::to_string(PositionX) + " | " + std::to_string(PositionY), 150);
		Gui::DrawString(210, 90, 0.6f, BLACK, "Current Item: " + MapItems[MapSelection]->name(), 190);
		Gui::drawGrid(townPos[currentAcre].x, townPos[currentAcre].y, townPos[currentAcre].w, townPos[currentAcre].h, BLACK);
	}
}

// Draw the current Position of the selection.
void TownMapEditor::DrawCurrentPos(void) const {
	if (savesType == SaveType::WW) {
		Gui::drawGrid(10 + (currentPosX*10), 40 + (currentPosY*10), 10, 10, WHITE);
	} else {
		Gui::drawGrid(15 + (currentPosX*12.5), 20 + (currentPosY*12.5), 12.5, 12.5, WHITE);
	}
}

// Draw Town Map Editor screen.
void TownMapEditor::DrawMapScreen(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - Town Map Editor", 400);
	DrawInformation();

	// Bottom Screen part. Grid & Acre.
	GFX::DrawBottom(true); // We need the full screen.
	if (savesType == SaveType::WW) {
		SpriteManagement::DrawAcres(FullAcres[SelectionToAcre(currentAcre)]->id(), 10, 40, 5, 5);
	} else {
		SpriteManagement::DrawAcres(FullAcres[SelectionToAcre(currentAcre)]->id(), 15, 20, 5, 5);
	}
	// Draw current Position + Grid.
	DrawGrid();
	DrawCurrentPos();

	// Draw the Operation Buttons.
	for (int i = 0; i < 5; i++) {
		GFX::DrawButton(mainButtons[i], 0.8f);
	}

	GFX::DrawGUI(gui_back_idx, icons[0].x, icons[0].y);
	if (selectMode == 0) {
		GFX::DrawGUIBlend(gui_pointer_idx, mainButtons[selection].x+40, mainButtons[selection].y+13);
	} else {
		GFX::DrawGUI(gui_pointer_idx, mainButtons[selection].x+40, mainButtons[selection].y+13);
	}
}

// Inject the current Temp Item to a slot.
void TownMapEditor::injectTo(int MapSlot) {
	if (ItemUtils::getName(itemID) != "???") {
		MapItems[MapSlot]->id(itemID);
		MapItems[MapSlot]->flags(itemFlag);
		changes = true;
	}
}

// Replace all Items which have the specified ID with Empty.
void TownMapEditor::remove(u16 ID) {
	u32 removedAmount = 0;
	if (savesType == SaveType::WW) {
		for (int i = 0; i < 4096; i++) {
			while (MapItems[i]->id() == ID && MapItems[i]->itemtype() != ItemType::Building) {
				MapItems[i]->id(0xFFF1);
				removedAmount++;
			}
		}
	} else {
		for (int i = 0; i < 5120; i++) {
			while (MapItems[i]->id() == ID) {
				MapItems[i]->id(0x7FFE);
				removedAmount++;
			}
		}
	}
	if (removedAmount > 0)	changes = true;
	Msg::DisplayWaitMsg("Removed " + std::to_string(removedAmount) + " Items.");
}

// Remove all Weeds.
void TownMapEditor::removeWeeds() {
	u32 removedAmount = 0;
	if (savesType == SaveType::WW) {
		for (int i = 0; i < 4096; i++) {
			while (MapItems[i]->id() == 0x001D || MapItems[i]->id() == 0x001E || MapItems[i]->id() == 0x001F || MapItems[i]->id() == 0x0020
			|| MapItems[i]->id() == 0x0021 || MapItems[i]->id() == 0x0022 || MapItems[i]->id() == 0x0023 || MapItems[i]->id() == 0x0024) {
				MapItems[i]->id(0xFFF1);
				removedAmount++;
			}
		}
	} else {
		for (int i = 0; i < 5120; i++) {
			while (MapItems[i]->id() == 0x007C || MapItems[i]->id() == 0x007D || MapItems[i]->id() == 0x007E || MapItems[i]->id() == 0x007F) {
				MapItems[i]->id(0x7FFE);
				removedAmount++;
			}
		}
	}
	if (removedAmount > 0)	changes = true;
	Msg::DisplayWaitMsg("Removed " + std::to_string(removedAmount) + " Weeds.");
}

// Fill the Town with a specific Item.
void TownMapEditor::setAll(u16 ID) {
	if (ItemUtils::getName(ID) != "???") {
		if (savesType == SaveType::WW) {
			for (int i = 0; i < 4096; i++) {
				if (MapItems[i]->itemtype() != ItemType::Building) {
					MapItems[i]->id(ID);
					
				}
			}
		} else {
			for (int i = 0; i < 5120; i++) {
				MapItems[i]->id(ID);
			}
		}
		changes = true;
		Msg::DisplayWaitMsg("Filled Town with " + ItemUtils::getName(ID) + ".");
	}
}

// Water all Flowers.
void TownMapEditor::waterFlowers() {
	int wateredAmount = 0;
	if (savesType == SaveType::WW) {
		for (int i = 0; i < 4096; i++) {
			while (MapItems[i]->id() >= 0x0 && MapItems[i]->id() <= 0x1C) {
				MapItems[i]->id(MapItems[i]->id() + 0x8A);
				wateredAmount++;
			}
			while (MapItems[i]->id() >= 0x6E && MapItems[i]->id() <= 0x89) {
				MapItems[i]->id(MapItems[i]->id() + 0x1C);
				wateredAmount++;
			}
		}
	} else {
		for (int i = 0; i < 5120; i++) {
			while (MapItems[i]->id() >= 0xCE && MapItems[i]->id() <= 0xF7) {
				MapItems[i]->id(MapItems[i]->id() - 0x2F);
				wateredAmount++;
			}
		}
	}
	if (wateredAmount > 0)	changes = true;
	Msg::DisplayWaitMsg("Watered " + std::to_string(wateredAmount) + " Flowers.");
}

// Replace all Items which start with the old ID to the new ID. Example: Replace all Empty slots with 99.000 Bells.
void TownMapEditor::replace(u16 oldID, u16 newID) {
	int replacedAmount = 0;
	if (ItemUtils::getName(newID) != "???") {
		if (savesType == SaveType::WW) {
			for (int i = 0; i < 4096; i++) {
				while (MapItems[i]->id() == oldID) {
					MapItems[i]->id(newID);
					replacedAmount++;
				}
			}
		} else {
			for (int i = 0; i < 5120; i++) {
				while (MapItems[i]->id() == oldID) {
					MapItems[i]->id(newID);
					replacedAmount++;
				}
			}
		}
	}
	if (replacedAmount > 0)	changes = true;
}

// Update Position, Selection & Acre Image only by navigating.
void TownMapEditor::updateStuff() {
	convertToSelection();
	convertToPosition();
}

void TownMapEditor::MapScreenLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if ((hDown & KEY_B) || (hDown & KEY_TOUCH && iconTouch(touch, icons[0]))) {
		Gui::screenBack();
		return;
	}

	// Switch the select Mode from Town Map Selection to operation selection.
	if (hDown & KEY_X) {
		if (selectMode == 0)	selectMode = 1;
		else					selectMode = 0;
	}

	// Inject Item and other operations.
	if (hDown & KEY_A) {
		if (selectMode == 0) {
			injectTo(MapSelection);
		} else {
			if (selection == 0) {
				Mode = 1; // Acres.
			} else if (selection == 1) {
				Mode = 2; // Temp Selection.
			} else if (selection == 2) {
				removeWeeds();
			} else if (selection == 3) {
				waterFlowers();
			} else if (selection == 4) {
//				Mode = 3; // Item Misc.
			}
		}
	}

	// Set current Item to selection.
	if (hDown & KEY_Y) {
		if (selectMode == 0) {
			itemID = MapItems[MapSelection]->id();
			itemFlag = MapItems[MapSelection]->flags();
		}
	}
	if (selectMode == 0) {
		if (hDown & KEY_RIGHT) {
			if (savesType == SaveType::WW) {
				if (currentPosX == 15 && currentAcre < 15) {
				// Go one Acre next and reset X to 0.
					currentAcre++;
					currentPosX = 0;
					updateStuff();
				} else if (currentPosX < 15) {
					currentPosX++;
					updateStuff();
				}
			} else {
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
		}

		if (hDown & KEY_LEFT) {
			if (savesType == SaveType::WW) {
				if (currentPosX == 0 && currentAcre > 0) {
					// Go one Acre before.
					currentAcre--;
					currentPosX = 15;
					updateStuff();
				} else if (currentPosX > 0) {
					currentPosX--;
					updateStuff();
				}
			} else {
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
		}

		if (hDown & KEY_DOWN) {
			if (savesType == SaveType::WW) {
				if (currentPosY == 15 && currentAcre < 12) {
					// Go one Acre down & reset Y to 0.
					currentAcre += 4;
					currentPosY = 0;
					updateStuff();
				} else if (currentPosY < 15) {
					currentPosY++;
					updateStuff();
				}
			} else {
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
		}

		if (hDown & KEY_UP) {
			if (savesType == SaveType::WW) {
				if (currentPosY == 0 && currentAcre > 3) {
					// Go one Acre up.
					currentAcre -= 4;
					currentPosY = 15;
					updateStuff();
				} else if (currentPosY > 0) {
					currentPosY--;
					updateStuff();
				}
			} else {
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
	} else {
		if (hDown & KEY_DOWN) {
			if (selection < 4)	selection++;
		} else if (hDown & KEY_UP) {
			if (selection > 0)	selection--;
		}
	}

	if (touching(touch, mainButtons[0])) {
		selection = 0;
		Mode = 1;
	}
}

void TownMapEditor::DrawTempItem(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - Temp Item", 400);
	GFX::DrawBottom();
	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(tempItemPos[i].x, tempItemPos[i].y, tempItemPos[i].w, tempItemPos[i].h, DARKER_COLOR);
		if (selection == i) {
			GFX::DrawGUI(gui_pointer_idx, tempItemPos[i].x+130, tempItemPos[i].y+25);
		}
	}
	// Item ID.
	Gui::DrawStringCentered(0, 45, 0.7f, BLACK, "Item ID: " + std::to_string(itemID), 320);
	// Item Flags.
	Gui::DrawStringCentered(0, 95, 0.7f, BLACK, "Item Flags: " + std::to_string(itemFlag), 320);
	// Item Name.
	Gui::DrawStringCentered(0, 145, 0.7f, BLACK, "Item Name: " + ItemUtils::getName(itemID), 320);
}

void TownMapEditor::TempItemLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		selection = 0;
		Mode = 0;
	}

	if (hDown & KEY_DOWN) {
		if (selection < 2)	selection++;
	}

	if (hDown & KEY_UP) {
		if (selection > 0)	selection--;
	}

	if (hDown & KEY_A) {
		switch (selection) {
			case 0:
				itemID = Input::handleu16(5, "Enter the Decimal ID of the Item.", 32766, itemID);
				break;
			case 1:
				itemFlag = Input::handleu16(2, "Enter the Flags.", 99, itemFlag);
				break;
			case 2:
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (iconTouch(touch, tempItemPos[0])) {
			itemID = Input::handleu16(5, "Enter the Decimal ID of the Item.", 32766, itemID);
		} else if (iconTouch(touch, tempItemPos[1])) {
			itemFlag = Input::handleu16(2, "Enter the Flags.", 99, itemFlag);
		} else if (iconTouch(touch, tempItemPos[2])) {
			// TODO: Item List Selection.
		}
	}
}


/* Acre Editor Stuff. */

void TownMapEditor::DrawAcres(void) const {
	DrawTopSelection();
	if (GodMode) {
		DrawFullMap();
	} else {
		DrawMap();
	}
}

// Draw the Full TownMap. [Used by GodMode enabled.]
void TownMapEditor::DrawFullMap(void) const {
	if (savesType == SaveType::WW) {
		DrawMap();
	} else {
		GFX::DrawBottom(true);
		for (int i = 0; i < 42; i++) {
			SpriteManagement::DrawAcres(FullAcres[i]->id(), acreMapPos[i].x, acreMapPos[i].y);
		}
		// Draw Grid.
		for (int i = 0; i < 42; i++) {
			for (u32 y = 0; y < 6; y++) {
				for (u32 x = 0; x < 7; x++, i++) {
					Gui::drawGrid(20 + (x*40), 0 + (y*40), 40, 40, C2D_Color32(40, 40, 40, 160));
				}
			}
		}
		GFX::DrawGUI(gui_pointer_idx, acreMapPos[selection].x+20, acreMapPos[selection].y+20);
	}
}

// Draw smaller TownMap. (Only affects AC:NL.)
void TownMapEditor::DrawMap(void) const {
	GFX::DrawBottom(true);
	if (savesType == SaveType::WW) {
		for (int i = 0; i < 36; i++) {
			SpriteManagement::DrawAcres(FullAcres[i]->id(), wwPos[i].x, wwPos[i].y, 1, 1);
		}
		GFX::DrawGUI(gui_pointer_idx, wwPos[selection].x+14, wwPos[selection].y+14);
	} else {
		for (int i = 0; i < 20; i++) {
			SpriteManagement::DrawAcres(FullAcres[SelectionToAcre(i)]->id(), acreTownPos[i].x, acreTownPos[i].y);
		}
		// Draw Grid.
		for (int i = 0; i < 20; i++) {
			for (u32 y = 0; y < 4; y++) {
				for (u32 x = 0; x < 5; x++, i++) {
					Gui::drawGrid(60 + (x*40), 40 + (y*40), 40, 40, C2D_Color32(40, 40, 40, 160));
				}
			}
		}
		GFX::DrawGUI(gui_pointer_idx, acreTownPos[selection].x+20, acreTownPos[selection].y+20);
	}
}

// Draw the Acre Selection from the Top Screen.
void TownMapEditor::DrawTopSelection(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - Acre Editor", 400);
	Gui::DrawStringCentered(0, 180, 0.8f, BLACK, "Acre ID: " + std::to_string(selectedAcre), 400);
	if (selectedAcre == 0) {
		SpriteManagement::DrawAcres(selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(selectedAcre+1, 300, 100, 1, 1);
		SpriteManagement::DrawAcres(selectedAcre+2, 350, 100, 1, 1);
	} else if (selectedAcre == 1) {
		SpriteManagement::DrawAcres(selectedAcre-1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(selectedAcre+1, 300, 100, 1, 1);
		SpriteManagement::DrawAcres(selectedAcre+2, 350, 100, 1, 1);
	} else if (selectedAcre == maxAcres-1) {
		SpriteManagement::DrawAcres(selectedAcre-2, 10, 100, 1, 1);
		SpriteManagement::DrawAcres(selectedAcre-1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(selectedAcre+1, 300, 100, 1, 1);
	} else if (selectedAcre == maxAcres) {
		SpriteManagement::DrawAcres(selectedAcre-2, 10, 100, 1, 1);
		SpriteManagement::DrawAcres(selectedAcre-1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
	} else {
		SpriteManagement::DrawAcres(selectedAcre-2, 10, 100, 1, 1);
		SpriteManagement::DrawAcres(selectedAcre-1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(selectedAcre+1, 300, 100, 1, 1);
		SpriteManagement::DrawAcres(selectedAcre+2, 350, 100, 1, 1);
	}
}


void TownMapEditor::AcresLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Go back to the Town Map Editor.
	if (hDown & KEY_B) {
		selection = 0;
		Mode = 0;
	}

	// Set Top Acre to Selection.
	if (hDown & KEY_A) {
		if (savesType == SaveType::WW) {
			FullAcres[selection]->id(selectedAcre);
			changes = true;
		} else {
			if (selectedAcre < 182 || selectedAcre > 205) {
				if (GodMode) {
					FullAcres[selection]->id(selectedAcre);
					changes = true;
				} else {
					FullAcres[SelectionToAcre(selection)]->id(selectedAcre);
					changes = true;
				}
			}
		}
	}

	// Display Selection on Top Acre.
	if (hDown & KEY_Y) {
		if (savesType == SaveType::WW) {
			selectedAcre = FullAcres[selection]->id();
		} else {
			if (GodMode) {
				selectedAcre = FullAcres[selection]->id();
			} else {
				selectedAcre = FullAcres[SelectionToAcre(selection)]->id();
			}
		}
	}

	if (hDown & KEY_DOWN) {
		if (savesType == SaveType::WW) {
			if (selection < 30)	selection += 6;
		} else {
			if (GodMode) {
				if (selection < 35)	selection += 7;
			} else {
				if (selection < 15)	selection += 5;
			}
		}
	}

	if (hDown & KEY_UP) {
		if (savesType == SaveType::WW) {
			if (selection > 5)	selection -= 6;
		} else {
			if (GodMode) {
				if (selection > 6)	selection -= 7;
			} else {
				if (selection > 4)	selection -= 5;
			}
		}
	}

	if (hDown & KEY_RIGHT) {
		if (savesType == SaveType::WW) {
			if (selection < 35)	selection++;
		} else {
			if (GodMode) {
				if (selection < 41)	selection++;
			} else {
				if (selection < 19)	selection++;
			}
		}
	}

	if (hDown & KEY_LEFT) {
		if (selection > 0)	selection--;
	}

	// Top Screen Acre Selection.
	// Faster Scroll Mode.
	if (FastMode) {
		if (keysHeld() & KEY_L) {
			if (selectedAcre > 0) {
				selectedAcre--;
			}
		}
		if (keysHeld() & KEY_R) {
			if (selectedAcre < maxAcres) {
				selectedAcre++;
			}
		}
		// Normal speed. (1 by 1.)
	} else {
		if (hDown & KEY_L) {
			if (selectedAcre > 0) {
				selectedAcre--;
			}
		}
		if (hDown & KEY_R) {
			if (selectedAcre < maxAcres) {
				selectedAcre++;
			}
		}
	}

	if (hDown & KEY_START) {
		if (FastMode)	FastMode = false;
		else	FastMode = true;
	}

	if (hDown & KEY_TOUCH) {
		if (savesType == SaveType::WW) {
			for (int i = 0; i < 36; i++) {
				if (iconTouch(touch, wwPos[i])) {
					selection = i;
				}
			}
		} else {
			if (GodMode) {
				for (int i = 0; i < 42; i++) {
					if (iconTouch(touch, acreMapPos[i])) {
						selection = i;
					}
				}
			} else {
				for (int i = 0; i < 20; i++) {
					if (iconTouch(touch, acreTownPos[i])) {
						selection = i;
					}
				}
			}
		}
	}
}