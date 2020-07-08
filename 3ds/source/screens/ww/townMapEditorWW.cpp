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

#include "itemManager.hpp"
#include "itemUtils.hpp"
#include "keyboard.hpp"
#include "Sav.hpp"
#include "saveUtils.hpp"
#include "screenCommon.hpp"
#include "spriteManagement.hpp"
#include "townMapEditorWW.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);
extern std::vector<std::pair<u16, std::string>> itemDB;
extern std::shared_ptr<Sav> save;

void TownMapEditorWW::Draw(void) const {
	if (Mode == 0) {
		DrawMapScreen();
	} else if (Mode == 1) {
		DrawAcres();
	} else if (Mode == 2) {
		DrawTempItem();
	}
}

void TownMapEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0) {
		MapScreenLogic(hDown, hHeld, touch);
	} else if (Mode == 1) {
		AcresLogic(hDown, hHeld, touch);
	} else if (Mode == 2) {
		TempItemLogic(hDown, hHeld, touch);
	}
}

// Initialize Screen.
TownMapEditorWW::TownMapEditorWW() {
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
}

// Convert the Selection to the current Acre.
int TownMapEditorWW::SelectionToAcre(int i) const {
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

// Draw the Item Grid.
void TownMapEditorWW::DrawGrid(void) const {
	for (int i = 0 + (currentAcre * 256); i < 256 + (currentAcre * 256); i++) {
		for (u32 y = 0; y < 16; y++) {
			for (u32 x = 0; x < 16; x++, i++) {
				GFX::drawGrid(10 + (x*10), 40 + (y*10), 10, 10, ItemManager::getColor(MapItems[i]->itemtype()));
			}
		}
	}
}

// Convert to Position.
void TownMapEditorWW::convertToPosition() {
	int acre = 0;
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
}

// Convert to the Map Selection.
void TownMapEditorWW::convertToSelection() {
	MapSelection = (currentAcre * 256) + (currentPosY * 16) + currentPosX;
}

// Display a few information on the Top Screen.
void TownMapEditorWW::DrawInformation() const {
	int x;
	for (int i = 0; i < 16; i++) {
		SpriteManagement::DrawAcres(FullAcres[SelectionToAcre(i)]->id(), WWMapPos[i].x, WWMapPos[i].y);
	}

	if (currentAcre < 4)	x = currentAcre;
	else if (currentAcre > 3 && currentAcre < 8)	x = currentAcre - 4;
	else if (currentAcre > 7 && currentAcre < 12)	x = currentAcre - 8;
	else	x = currentAcre - 12;
	// Display Informations.
	Gui::DrawString(190, 40, 0.8f, BLACK, "Current Position:\n" +  std::to_string(PositionX) + " | " + std::to_string(PositionY), 0, 0, font);
	Gui::DrawString(190, 90, 0.8f, BLACK, "Current Item:\n" + MapItems[MapSelection]->name(), 0, 0, font);
	Gui::drawGrid(5 + (x*32), 40 + (currentAcre/4*32), 32, 32, BLACK);
}

// Draw the current Position of the selection.
void TownMapEditorWW::DrawCurrentPos(void) const {
	Gui::drawGrid(10 + (currentPosX*10), 40 + (currentPosY*10), 10, 10, WHITE);
}

// Draw Town Map Editor screen.
void TownMapEditorWW::DrawMapScreen(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - Town Map Editor", 400, 0, font);
	DrawInformation();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	// Bottom Screen part. Grid & Acre.
	GFX::DrawBottom(true); // We need the full screen.
	SpriteManagement::DrawAcres(FullAcres[SelectionToAcre(currentAcre)]->id(), 10, 40, 5, 5);
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

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

// Inject the current Temp Item to a slot.
void TownMapEditorWW::injectTo(int MapSlot) {
	if (ItemUtils::getName(itemID) != "???") {
		MapItems[MapSlot]->id(itemID);
		changes = true;
	}
}

// Replace all Items which have the specified ID with Empty.
void TownMapEditorWW::remove(u16 ID) {
	u32 removedAmount = 0;
	for (int i = 0; i < 4096; i++) {
		while (MapItems[i]->id() == ID && MapItems[i]->itemtype() != ItemType::Building) {
			MapItems[i]->id(0xFFF1);
			removedAmount++;
		}
	}

	if (removedAmount > 0)	changes = true;
	Msg::DisplayWaitMsg("Removed " + std::to_string(removedAmount) + " Items.");
}

// Remove all Weeds.
void TownMapEditorWW::removeWeeds() {
	u32 removedAmount = 0;
	for (int i = 0; i < 4096; i++) {
		while (MapItems[i]->id() == 0x001D || MapItems[i]->id() == 0x001E || MapItems[i]->id() == 0x001F || MapItems[i]->id() == 0x0020
		|| MapItems[i]->id() == 0x0021 || MapItems[i]->id() == 0x0022 || MapItems[i]->id() == 0x0023 || MapItems[i]->id() == 0x0024) {
			MapItems[i]->id(0xFFF1);
			removedAmount++;
		}
	}

	if (removedAmount > 0)	changes = true;
	Msg::DisplayWaitMsg("Removed " + std::to_string(removedAmount) + " Weeds.");
}

// Fill the Town with a specific Item.
void TownMapEditorWW::setAll(u16 ID) {
	if (ItemUtils::getName(ID) != "???") {
		for (int i = 0; i < 4096; i++) {
			if (MapItems[i]->itemtype() != ItemType::Building) {
				MapItems[i]->id(ID);
			}
		}

		changes = true;
		Msg::DisplayWaitMsg("Filled Town with " + ItemUtils::getName(ID) + ".");
	}
}

// Water all Flowers.
void TownMapEditorWW::waterFlowers() {
	int wateredAmount = 0;
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

	if (wateredAmount > 0)	changes = true;
	Msg::DisplayWaitMsg("Watered " + std::to_string(wateredAmount) + " Flowers.");
}

// Replace all Items which start with the old ID to the new ID. Example: Replace all Empty slots with 99.000 Bells.
void TownMapEditorWW::replace(u16 oldID, u16 newID) {
	int replacedAmount = 0;
	if (ItemUtils::getName(newID) != "???") {
		for (int i = 0; i < 4096; i++) {
			while (MapItems[i]->id() == oldID) {
				MapItems[i]->id(newID);
				replacedAmount++;
			}
		}
	}

	if (replacedAmount > 0)	changes = true;
}

// Update Position, Selection & Acre Image only by navigating.
void TownMapEditorWW::updateStuff() {
	convertToSelection();
	convertToPosition();
}

void TownMapEditorWW::MapScreenLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if ((hDown & KEY_B) || (hDown & KEY_TOUCH && iconTouch(touch, icons[0]))) {
		Gui::screenBack(doFade);
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
				selection = 0;
				this->itemIndex = ItemManager::getIndex(this->itemID); // Here we get the Index.
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
		}
	}

	if (selectMode == 0) {
		if (hDown & KEY_RIGHT) {
			if (currentPosX == 15 && currentAcre < 15) {
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
			if (currentPosY == 15 && currentAcre < 12) {
				// Go one Acre down & reset Y to 0.
				currentAcre += 4;
				currentPosY = 0;
				updateStuff();
			} else if (currentPosY < 15) {
				currentPosY++;
				updateStuff();
			}
		}

		if (hDown & KEY_UP) {
			if (currentPosY == 0 && currentAcre > 3) {
				// Go one Acre up.
				currentAcre -= 4;
				currentPosY = 15;
				updateStuff();
			} else if (currentPosY > 0) {
				currentPosY--;
				updateStuff();
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

void TownMapEditorWW::DrawTempItem(void) const {
	std::string itemList;
	GFX::DrawFileBrowseBG(true);
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9, WHITE, "Current Item: " + ItemUtils::getName(this->itemID), 390, 0, font);

	for (int i=(this->itemIndex<8) ? 0 : (int)this->itemIndex-8;i<(int)itemDB.size()&&i<(((int)this->itemIndex<8) ? 9 : (int)this->itemIndex+1);i++) {
		itemList += itemDB[i].second + "\n";
	}

	for (uint i=0;i<((itemDB.size()<9) ? 9-itemDB.size() : 0);i++) {
		itemList += "\n";
	}

	// Selector Logic.
	if (this->itemIndex < 9)	GFX::DrawSelector(true, 24 + ((int)this->itemIndex * 21));
	else				GFX::DrawSelector(true, 24 + (8 * 21));
	Gui::DrawString(5, 25, 0.85f, BLACK, itemList, 360, 0, font);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, std::to_string(this->itemIndex + 1) + " | " + std::to_string(itemDB.size()), 395, 0, font);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	for (int i = 0; i < 2; i++) {
		GFX::DrawButton(tempItemPos[i], 0.8f);
	}
	// We have no other choice.
	Gui::DrawStringCentered(tempItemPos[0].x - 160 + (tempItemPos[0].xLength/2), tempItemPos[0].y + (tempItemPos[0].yLength/2) - 10, 0.9f, BLACK, "Item ID: " + std::to_string(this->itemID), tempItemPos[0].xLength-17, tempItemPos[0].yLength-5, font);
	
	if (!this->isItemSelection) {
		GFX::DrawGUI(gui_pointer_idx, tempItemPos[selection].x+130, tempItemPos[selection].y+25);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void TownMapEditorWW::TempItemLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		this->isItemSelection = false;
		selection = 0;
		Mode = 0;
	}

	if (this->isItemSelection) {
		if (this->keyRepeatDelay)	this->keyRepeatDelay--;

		if (hHeld & KEY_DOWN && !this->keyRepeatDelay) {
			if (this->itemIndex < itemDB.size()-1)	this->itemIndex++;
			else	this->itemIndex = 0;
			this->keyRepeatDelay = 6;
		}

		if (hHeld & KEY_UP && !this->keyRepeatDelay) {
			if (this->itemIndex > 0)	this->itemIndex--;
			else if (this->itemIndex == 0)	this->itemIndex = itemDB.size()-1;
			this->keyRepeatDelay = 6;
		}


		if (hHeld & KEY_RIGHT && !this->keyRepeatDelay) {
			if ((this->itemIndex + 9) > itemDB.size()-1) {
				this->itemIndex = itemDB.size()-1;
			} else {
				this->itemIndex += 9;
			}
			this->keyRepeatDelay = 6;
		}

		if (hDown & KEY_A) {
			this->itemID = itemDB[this->itemIndex].first;
		}

		if (hDown & KEY_X) {
			this->isItemSelection = false;
		}
	} else {
		if (hDown & KEY_DOWN) {
			if (selection < 1)	selection++;
		}

		if (hDown & KEY_UP) {
			if (selection > 0)	selection--;
		}

		if (hDown & KEY_A) {
			switch (selection) {
				case 0:
					this->itemID = Input::handleu16(5, "Enter the Decimal ID of the Item.", 32766, this->itemID);
					this->itemIndex = ItemManager::getIndex(this->itemID); // Here we get the Index.
					break;
				case 1:
					this->isItemSelection = true;
					break;
			}
		}

		if (hDown & KEY_TOUCH) {
			if (touching(touch, tempItemPos[0])) {
				itemID = Input::handleu16(5, "Enter the Decimal ID of the Item.", 32766, itemID);
				this->itemIndex = ItemManager::getIndex(this->itemID); // Here we get the Index.
			} else if (touching(touch, tempItemPos[1])) {
				this->isItemSelection = true;
			}
		}
	}
}


/* Acre Editor Stuff. */

void TownMapEditorWW::DrawAcres(void) const {
	DrawTopSelection();
	DrawMap();
}

// Draw TownMap.
void TownMapEditorWW::DrawMap(void) const {
	GFX::DrawBottom(true);
	for (int i = 0; i < 36; i++) {
		SpriteManagement::DrawAcres(FullAcres[i]->id(), wwPos[i].x, wwPos[i].y, 1, 1);
	}
	
	GFX::DrawGUI(gui_pointer_idx, wwPos[selection].x+14, wwPos[selection].y+14);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

// Draw the Acre Selection from the Top Screen.
void TownMapEditorWW::DrawTopSelection(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - Acre Editor", 400, 0, font);
	Gui::DrawStringCentered(0, 180, 0.8f, BLACK, "Acre ID: " + std::to_string(selectedAcre), 400, 0, font);
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

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void TownMapEditorWW::AcresLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Go back to the Town Map Editor.
	if (hDown & KEY_B) {
		selection = 0;
		Mode = 0;
	}

	// Set Top Acre to Selection.
	if (hDown & KEY_A) {
		FullAcres[selection]->id(selectedAcre);
		changes = true;
	}

	// Display Selection on Top Acre.
	if (hDown & KEY_Y) {
		selectedAcre = FullAcres[selection]->id();
	}

	if (hDown & KEY_DOWN) {
		if (selection < 30)	selection += 6;
	}

	if (hDown & KEY_UP) {
		if (selection > 5)	selection -= 6;
	}

	if (hDown & KEY_RIGHT) {
		if (selection < 35)	selection++;
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
		for (int i = 0; i < 36; i++) {
			if (iconTouch(touch, wwPos[i])) {
				selection = i;
			}
		}
	}
}