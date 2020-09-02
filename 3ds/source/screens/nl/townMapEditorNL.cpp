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
#include "overlay.hpp"
#include "saveUtils.hpp"
#include "screenCommon.hpp"
#include "spriteManagement.hpp"
#include "townMapEditorNL.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);
/* Bring that to other screens too. */
extern SaveType savesType;

void TownMapEditorNL::Draw(void) const {
	if (Mode == 0) {
		DrawMapScreen();
	} else if (Mode == 1) {
		DrawTempItem();
	}
}

void TownMapEditorNL::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0) {
		MapScreenLogic(hDown, hHeld, touch);
	} else if (Mode == 1) {
		TempItemLogic(hDown, hHeld, touch);
	}
}

/* Initialize Screen. */
TownMapEditorNL::TownMapEditorNL(std::unique_ptr<Town> &refTown) : town(refTown) {
	/* Get Town Map Items. */
	for (int i = 0; i < 5120; i++) {
		this->MapItems[i] = save->town()->item(i);
		this->ItemBuried[i] = save->town()->itemBuried(i);
	}

	/* Get Full Acres. */
	for (int i = 0; i < 42; i++) {
		this->FullAcres[i] = save->town()->acre(i);
	}
}

/* Convert the Selection to the current Acre. */
int TownMapEditorNL::SelectionToAcre(int i) const {
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

	return 8; // Should Never Happen.
}

/* Draw the Item Grid. */
void TownMapEditorNL::DrawGrid(void) const {
	for (int i = 0 + (currentAcre * 256); i < 256 + (currentAcre * 256); i++) {
		for (u32 y = 0; y < 16; y++) {
			for (u32 x = 0; x < 16; x++, i++) {
				GFX::drawGrid(15 + (x * 12.5), 20 + (y * 12.5), 12.5, 12.5, ItemManager::getColor(this->MapItems[i]->itemtype()));
				if (this->ItemBuried[i]) GFX::DrawGUI(gui_cross_idx, 16.5 + (x * 12.5), 21.5 + (y * 12.5));
			}
		}
	}
}

/* Convert to Position. */
void TownMapEditorNL::convertToPosition() {
	int acre = 0;

	if (this->currentAcre < 5) {
		this->PositionX = (16 + (this->currentAcre * 16)) + this->currentPosX;
	} else if (this->currentAcre > 4 && this->currentAcre < 10) {
		this->PositionX = (16 + ((this->currentAcre - 5) * 16)) + this->currentPosX;
	} else if (currentAcre > 9 && this->currentAcre < 15) {
		this->PositionX = (16 + ((this->currentAcre - 10) * 16)) + this->currentPosX;
	} else if (currentAcre > 14 && this->currentAcre < 20) {
		this->PositionX = (16 + ((this->currentAcre - 15) * 16)) + this->currentPosX;
	}

	if (this->currentAcre < 5) {
		acre = 0;
	} else if (this->currentAcre > 4 && this->currentAcre < 10) {
		acre = 1;
	} else if (this->currentAcre > 9 && this->currentAcre < 15) {
		acre = 2;
	} else if (this->currentAcre > 14 && this->currentAcre < 20) {
		acre = 3;
	}

	this->PositionY = (16 + acre * 16) + this->currentPosY;
}

/* Convert to the Map Selection. */
void TownMapEditorNL::convertToSelection() {
	this->MapSelection = (this->currentAcre * 256) + (this->currentPosY * 16) + this->currentPosX;
}

/* Display a few information on the Top Screen. */
void TownMapEditorNL::DrawInformation() const {
	for (int i = 0; i < 20; i++) {
		SpriteManagement::DrawAcres(this->FullAcres[SelectionToAcre(i)]->id(), townPos[i].x, townPos[i].y);
	}

	Gui::DrawString(210, 60, 0.7f, BLACK, Lang::get("CURRENT_POSITION") + std::to_string(PositionX) + " | " + std::to_string(PositionY), 150, 0, font);
	Gui::DrawString(210, 90, 0.6f, BLACK, Lang::get("CURRENT_ITEM") + this->MapItems[this->MapSelection]->name(), 190, 0, font);
	Gui::drawGrid(townPos[this->currentAcre].x, townPos[this->currentAcre].y, townPos[this->currentAcre].w, townPos[this->currentAcre].h, BLACK);
}

/* Draw the current Position of the selection. */
void TownMapEditorNL::DrawCurrentPos(void) const {
	Gui::drawGrid(15 + (this->currentPosX * 12.5), 20 + (this->currentPosY * 12.5), 12.5, 12.5, WHITE);
}

/* Draw Town Map Editor screen. */
void TownMapEditorNL::DrawMapScreen(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - " + Lang::get("TOWN_MAP_EDITOR"), 400, 0, font);
	DrawInformation();

	/* Bottom Screen part. Grid & Acre. */
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom(true); // We need the full screen.
	SpriteManagement::DrawAcres(FullAcres[SelectionToAcre(this->currentAcre)]->id(), 15, 20, 5, 5);
	/* Draw current Position + Grid. */
	DrawGrid();
	DrawCurrentPos();

	/* Draw the Operation Buttons. */
	for (int i = 0; i < 4; i++) {
		GFX::DrawButton(mainButtons[i], 0.8f);
	}

	GFX::DrawGUI(gui_back_idx, icons[0].x, icons[0].y);

	if (this->selectMode == 0) {
		GFX::DrawGUIBlend(gui_pointer_idx, mainButtons[this->selection].x + 40, mainButtons[this->selection].y + 13);
	} else {
		GFX::DrawGUI(gui_pointer_idx, mainButtons[this->selection].x + 40, mainButtons[this->selection].y + 13);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

/* Inject the current Temp Item to a slot. */
void TownMapEditorNL::injectTo(int MapSlot) {
	if (ItemUtils::getName(this->itemID) != "???") {
		MapItems[MapSlot]->id(this->itemID);
		MapItems[MapSlot]->flags(this->itemFlag);
		changes = true;
	}
}

/* Replace all Items which have the specified ID with Empty. */
void TownMapEditorNL::remove(u16 ID) {
	u32 removedAmount = 0;
	for (int i = 0; i < 5120; i++) {
		while (this->MapItems[i]->id() == ID) {
			this->MapItems[i]->id(0x7FFE);
			removedAmount++;
		}
	}

	if (removedAmount > 0) changes = true;
	char message [100];
	snprintf(message, sizeof(message), Lang::get("REMOVED_ITEMS").c_str(), removedAmount);
	Msg::DisplayWaitMsg(message);
}

/* Remove all Weeds. */
void TownMapEditorNL::removeWeeds() {
	u32 removedAmount = 0;
	for (int i = 0; i < 5120; i++) {
		while (this->MapItems[i]->id() == 0x007C || this->MapItems[i]->id() == 0x007D || this->MapItems[i]->id() == 0x007E || this->MapItems[i]->id() == 0x007F) {
			this->MapItems[i]->id(0x7FFE);
			removedAmount++;
		}
	}

	if (removedAmount > 0) changes = true;
	char message [100];
	snprintf(message, sizeof(message), Lang::get("REMOVED_WEEDS").c_str(), removedAmount);
	Msg::DisplayWaitMsg(message);
}

/* Fill the Town with a specific Item. */
void TownMapEditorNL::setAll(u16 ID) {
	if (ItemUtils::getName(ID) != "???") {
		for (int i = 0; i < 5120; i++) {
			this->MapItems[i]->id(ID);
		}

		changes = true;
		char message [100];
		snprintf(message, sizeof(message), Lang::get("FILLED_TOWN").c_str(), ItemUtils::getName(ID));
		Msg::DisplayWaitMsg(message);
	}
}

/* Water all Flowers. */
void TownMapEditorNL::waterFlowers() {
	int wateredAmount = 0;
	for (int i = 0; i < 5120; i++) {
		while (this->MapItems[i]->id() >= 0xCE && this->MapItems[i]->id() <= 0xF7) {
			this->MapItems[i]->id(this->MapItems[i]->id() - 0x2F);
			wateredAmount++;
		}
	}

	if (wateredAmount > 0) changes = true;
	char message [100];
	snprintf(message, sizeof(message), Lang::get("WATERED_FLOWERS").c_str(), wateredAmount);
	Msg::DisplayWaitMsg(message);
}

/* Replace all Items which start with the old ID to the new ID. Example: Replace all Empty slots with 99.000 Bells. */
void TownMapEditorNL::replace(u16 oldID, u16 newID) {
	int replacedAmount = 0;
	if (ItemUtils::getName(newID) != "???") {
		for (int i = 0; i < 5120; i++) {
			while (this->MapItems[i]->id() == oldID) {
				this->MapItems[i]->id(newID);
				replacedAmount++;
			}
		}
	}

	if (replacedAmount > 0) changes = true;
	char message [100];
	snprintf(message, sizeof(message), Lang::get("REPLACED_ITEMS").c_str(), replacedAmount);
	Msg::DisplayWaitMsg(message);
}

/* Update Position, Selection & Acre Image only by navigating. */
void TownMapEditorNL::updateStuff() {
	convertToSelection();
	convertToPosition();
}

void TownMapEditorNL::MapScreenLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if ((hDown & KEY_B) || (hDown & KEY_TOUCH && iconTouch(touch, icons[0]))) {
		Gui::screenBack(doFade);
	}

	/* Switch the select Mode from Town Map Selection to operation selection. */
	if (hDown & KEY_X) {
		if (this->selectMode == 0) this->selectMode = 1;
		else this->selectMode = 0;
	}

	/* Inject Item and other operations. */
	if (hDown & KEY_A) {
		if (this->selectMode == 0) {
			injectTo(this->MapSelection);
		} else {
			switch(this->selection) {
				case 0:
					this->selection = 0;
					this->Mode = 1; // Temp Selection.
					break;
				case 1:
					removeWeeds();
					break;
				case 2:
					waterFlowers();
					break;
				case 3:
					// this->Mode = 3; // Item Misc.
					break;
			}
		}
	}

	/* Set current Item to selection. */
	if (hDown & KEY_Y) {
		if (this->selectMode == 0) {
			this->itemID = this->MapItems[this->MapSelection]->id();
			this->itemFlag = this->MapItems[this->MapSelection]->flags();
		}
	}

	if (this->selectMode == 0) {
		if (hDown & KEY_RIGHT) {
			if (this->currentPosX == 15 && this->currentAcre < 19) {
				/* Go one Acre next and reset X to 0. */
				this->currentAcre++;
				this->currentPosX = 0;
				updateStuff();
			} else if (this->currentPosX < 15) {
				this->currentPosX++;
				updateStuff();
			}
		}

		if (hDown & KEY_LEFT) {
			if (this->currentPosX == 0 && this->currentAcre > 0) {
				/* Go one Acre before. */
				this->currentAcre--;
				this->currentPosX = 15;
				updateStuff();
			} else if (this->currentPosX > 0) {
				this->currentPosX--;
				updateStuff();
			}
		}

		if (hDown & KEY_DOWN) {
			if (this->currentPosY == 15 && this->currentAcre < 15) {
				/* Go one Acre down & reset Y to 0. */
				this->currentAcre += 5;
				this->currentPosY = 0;
				updateStuff();
			} else if (this->currentPosY < 15) {
				this->currentPosY++;
				updateStuff();
			}
		}

		if (hDown & KEY_UP) {
			if (this->currentPosY == 0 && this->currentAcre > 4) {
				/* Go one Acre up. */
				this->currentAcre -= 5;
				this->currentPosY = 15;
				updateStuff();
			} else if (this->currentPosY > 0) {
				this->currentPosY--;
				updateStuff();
			}
		}

	} else {
		if (hDown & KEY_DOWN) {
			if (this->selection < 3) this->selection++;
		} else if (hDown & KEY_UP) {
			if (this->selection > 0) this->selection--;
		}
	}

	if (touching(touch, mainButtons[0])) {
		this->selection = 0;
		this->Mode = 1;
	}
}

void TownMapEditorNL::DrawTempItem(void) const {
	GFX::DrawTop();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	for (int i = 0; i < 3; i++) {
		GFX::DrawButton(tempItemPos[i], 0.8f);
	}

	/* We have no other choice. */
	Gui::DrawStringCentered(tempItemPos[0].x - 160 + (tempItemPos[0].xLength/2), tempItemPos[0].y + (tempItemPos[0].yLength/2) - 10, 0.9f, BLACK, "Item ID: " + std::to_string(this->itemID), tempItemPos[0].xLength-17, tempItemPos[0].yLength-5, font);
	Gui::DrawStringCentered(tempItemPos[1].x - 160 + (tempItemPos[1].xLength/2), tempItemPos[1].y + (tempItemPos[1].yLength/2) - 10, 0.9f, BLACK, "Item Flags: " + std::to_string(this->itemFlag), tempItemPos[1].xLength-17, tempItemPos[1].yLength-5, font);
	GFX::DrawGUI(gui_pointer_idx, tempItemPos[this->selection].x+130, tempItemPos[this->selection].y+25);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void TownMapEditorNL::TempItemLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		this->selection = 0;
		this->Mode = 0;
	}

	if (hDown & KEY_DOWN) {
		if (this->selection < 2) this->selection++;
	}

	if (hDown & KEY_UP) {
		if (this->selection > 0) this->selection--;
	}

	if (hDown & KEY_A) {
		switch (this->selection) {
			case 0:
				this->itemID = Input::handleu16(5, Lang::get("ENTER_DECIMAL_ID"), 32766, this->itemID);
				break;
			case 1:
				this->itemFlag = Input::handleu16(5, Lang::get("ENTER_DECIMAL_ID"), 99, this->itemFlag);
				break;
			case 2:
				this->itemID = Overlays::SelectItem(this->itemID, savesType);
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, tempItemPos[0])) {
			this->itemID = Input::handleu16(5, Lang::get("ENTER_DECIMAL_ID"), 32766, this->itemID);
		} else if (touching(touch, tempItemPos[1])) {
			this->itemFlag = Input::handleu16(5, Lang::get("ENTER_DECIMAL_ID"), 99, this->itemFlag);
		} else if (touching(touch, tempItemPos[2])) {
			this->itemID = Overlays::SelectItem(this->itemID, savesType);
		}
	}
}