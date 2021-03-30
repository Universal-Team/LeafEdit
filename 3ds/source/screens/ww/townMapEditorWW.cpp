/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2021 Universal-Team
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
#include "townMapEditorWW.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

void TownMapEditorWW::Draw(void) const {
	switch(this->Mode) {
		case 0:
			this->DrawMapScreen();
			break;

		case 1:
			this->DrawTempItem();
			break;
	}
}

void TownMapEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	switch(this->Mode) {
		case 0:
			this->MapScreenLogic(hDown, hHeld, touch);
			break;

		case 1:
			this->TempItemLogic(hDown, hHeld, touch);
			break;
	}
}

/* Initialize Screen. */
TownMapEditorWW::TownMapEditorWW(std::unique_ptr<Town> &refTown): town(refTown) {
	/* Get Town Map Items. */
	for (int i = 0; i < 4096; i++) {
		this->MapItems[i] = this->town->item(i);
	}

	/* Get Full Acres. */
	for (int i = 0; i < 36; i++) {
		this->FullAcres[i] = this->town->acre(i);
	}

	this->PositionX = 0;
	this->PositionY = 0;
}

/* Convert the Selection to the current Acre. */
int TownMapEditorWW::SelectionToAcre(int i) const {
	switch(i) {
		case 0:
			return 7;

		case 1:
			return 8;

		case 2:
			return 9;

		case 3:
			return 10;

		case 4:
			return 13;

		case 5:
			return 14;

		case 6:
			return 15;

		case 7:
			return 16;

		case 8:
			return 19;

		case 9:
			return 20;

		case 10:
			return 21;

		case 11:
			return 22;

		case 12:
			return 25;

		case 13:
			return 26;

		case 14:
			return 27;

		case 15:
			return 28;

	}

	return 7; // Should Never Happen.
}

/* Draw the Item Grid. */
void TownMapEditorWW::DrawGrid(void) const {
	for (int i = 0 + (this->currentAcre * 256); i < 256 + (this->currentAcre * 256); i++) {
		for (u32 y = 0; y < 16; y++) {
			for (u32 x = 0; x < 16; x++, i++) {
				GFX::drawGrid(10 + (x * 10), 40 + (y * 10), 10, 10, ItemManager::getColor(this->MapItems[i]->itemtype()));

				if (this->town->itemBuried(i)) GFX::DrawGUI(gui_cross_idx, 11 + (x * 10), 41 + (y * 10));
			}
		}
	}
}

/* Convert to Position. */
void TownMapEditorWW::convertToPosition() {
	int acre = 0;
	if (this->currentAcre < 4) {
		this->PositionX = (this->currentAcre * 16) + this->currentPosX;

	} else if (this->currentAcre > 3 && this->currentAcre < 8) {
		this->PositionX = (((this->currentAcre - 4) * 16)) + this->currentPosX;

	} else if (this->currentAcre > 7 && this->currentAcre < 12) {
		this->PositionX = (((this->currentAcre - 8) * 16)) + this->currentPosX;

	} else if (this->currentAcre > 11 && this->currentAcre < 16) {
		this->PositionX = (((this->currentAcre - 12) * 16)) + this->currentPosX;
	}

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

/* Convert to the Map Selection. */
void TownMapEditorWW::convertToSelection() {
	this->MapSelection = (this->currentAcre * 256) + (this->currentPosY * 16) + this->currentPosX;
}

/* Display a few information on the Top Screen. */
void TownMapEditorWW::DrawInformation() const {
	int x;

	for (int i = 0; i < 16; i++) {
		SpriteManagement::DrawAcres(this->FullAcres[SelectionToAcre(i)]->id(), this->WWMapPos[i].x, this->WWMapPos[i].y);
	}

	if (this->currentAcre < 4) x = this->currentAcre;
	else if (this->currentAcre > 3 && this->currentAcre < 8) x = this->currentAcre - 4;
	else if (this->currentAcre > 7 && this->currentAcre < 12) x = this->currentAcre - 8;
	else x = this->currentAcre - 12;

	/* Display Informations. */
	Gui::DrawString(190, 40, 0.8f, BLACK, Lang::get("CURRENT_POSITION") + "\n" + std::to_string(this->PositionX) + " | " + std::to_string(this->PositionY), 0, 0, font);
	Gui::DrawString(190, 90, 0.8f, BLACK, Lang::get("CURRENT_ITEM") + "\n" + this->MapItems[this->MapSelection]->name(), 0, 0, font);
	Gui::drawGrid(5 + (x * 32), 40 + (this->currentAcre / 4 * 32), 32, 32, BLACK);
}

/* Draw the current Position of the selection. */
void TownMapEditorWW::DrawCurrentPos(void) const {
	Gui::drawGrid(10 + (this->currentPosX * 10), 40 + (this->currentPosY * 10), 10, 10, WHITE);
}

/* Draw Town Map Editor screen. */
void TownMapEditorWW::DrawMapScreen(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("TOWN_MAP_EDITOR"), 400, 0, font);
	DrawInformation();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	/* Bottom Screen part. Grid & Acre. */
	GFX::DrawBottom(true); // We need the full screen.
	SpriteManagement::DrawAcres(this->FullAcres[SelectionToAcre(this->currentAcre)]->id(), 10, 40, 5, 5);

	/* Draw current Position + Grid. */
	this->DrawGrid();
	this->DrawCurrentPos();

	/* Draw the Operation Buttons. */
	for (int i = 0; i < 3; i++) {
		GFX::DrawButton(mainButtons[i], 0.6f);
	}

	GFX::DrawGUI(gui_back_idx, icons[0].x, icons[0].y);

	switch(this->selectMode) {
		case 0:
			GFX::DrawGUIBlend(gui_pointer_idx, mainButtons[this->selection].x + 40, mainButtons[this->selection].y + 13);
			break;

		case 1:
			GFX::DrawGUI(gui_pointer_idx, mainButtons[this->selection].x + 40, mainButtons[this->selection].y + 13);
			break;
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

/* Inject the current Temp Item to a slot. */
void TownMapEditorWW::injectTo(int MapSlot) {
	if (ItemUtils::getName(this->itemID) != "???") {
		this->MapItems[MapSlot]->id(this->itemID);
	}
}

/* Replace all Items which have the specified ID with Empty. */
void TownMapEditorWW::remove(u16 ID) {
	u32 removedAmount = 0;
	for (int i = 0; i < 4096; i++) {
		while (this->MapItems[i]->id() == ID && this->MapItems[i]->itemtype() != ItemType::Building) {
			this->MapItems[i]->id(0xFFF1);
			removedAmount++;
		}
	}

	char message [100];
	snprintf(message, sizeof(message), Lang::get("REMOVED_ITEMS").c_str(), removedAmount);
	Msg::DisplayWaitMsg(message);
}

/* Remove all Weeds. */
void TownMapEditorWW::removeWeeds() {
	u32 removedAmount = 0;
	for (int i = 0; i < 4096; i++) {
		while (this->MapItems[i]->id() == 0x001D || this->MapItems[i]->id() == 0x001E || this->MapItems[i]->id() == 0x001F || this->MapItems[i]->id() == 0x0020
		|| this->MapItems[i]->id() == 0x0021 || this->MapItems[i]->id() == 0x0022 || this->MapItems[i]->id() == 0x0023 || this->MapItems[i]->id() == 0x0024) {
			this->MapItems[i]->id(0xFFF1);
			removedAmount++;
		}
	}

	char message [100];
	snprintf(message, sizeof(message), Lang::get("REMOVED_WEEDS").c_str(), removedAmount);
	Msg::DisplayWaitMsg(message);
}

/* Fill the Town with a specific Item. */
void TownMapEditorWW::setAll(u16 ID) {
	if (ItemUtils::getName(ID) != "???") {
		for (int i = 0; i < 4096; i++) {
			if (this->MapItems[i]->itemtype() != ItemType::Building) {
				this->MapItems[i]->id(ID);
			}
		}

		char message [100];
		snprintf(message, sizeof(message), Lang::get("FILLED_TOWN").c_str(), ItemUtils::getName(ID));
		Msg::DisplayWaitMsg(message);
	}
}

/* Water all Flowers. */
void TownMapEditorWW::waterFlowers() {
	int wateredAmount = 0;
	for (int i = 0; i < 4096; i++) {
		while (this->MapItems[i]->id() >= 0x0 && this->MapItems[i]->id() <= 0x1C) {
			this->MapItems[i]->id(this->MapItems[i]->id() + 0x8A);
			wateredAmount++;
		}

		while (this->MapItems[i]->id() >= 0x6E && this->MapItems[i]->id() <= 0x89) {
			this->MapItems[i]->id(this->MapItems[i]->id() + 0x1C);
			wateredAmount++;
		}
	}

	char message [100];
	snprintf(message, sizeof(message), Lang::get("WATERED_FLOWERS").c_str(), wateredAmount);
	Msg::DisplayWaitMsg(message);
}

/* Replace all Items which start with the old ID to the new ID. Example: Replace all Empty slots with 99.000 Bells. */
void TownMapEditorWW::replace(u16 oldID, u16 newID) {
	int replacedAmount = 0;
	if (ItemUtils::getName(newID) != "???") {
		for (int i = 0; i < 4096; i++) {
			while (this->MapItems[i]->id() == oldID) {
				this->MapItems[i]->id(newID);
				replacedAmount++;
			}
		}
	}

	char message [100];
	snprintf(message, sizeof(message), Lang::get("REPLACED_ITEMS").c_str(), replacedAmount);
	Msg::DisplayWaitMsg(message);
}

/* Update Position, Selection & Acre Image only by navigating. */
void TownMapEditorWW::updateStuff() {
	this->convertToSelection();
	this->convertToPosition();
}

void TownMapEditorWW::MapScreenLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if ((hDown & KEY_B) || (hDown & KEY_TOUCH && iconTouch(touch, icons[0]))) {
		Gui::screenBack(doFade);
	}

	/* Switch the select Mode from Town Map Selection to operation selection. */
	if (hDown & KEY_X) {
		switch(this->selectMode) {
			case 0:
				this->selectMode = 1;
				break;

			case 1:
				this->selectMode = 0;
				break;
		}
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
					/* this->Mode = 2; // Item Misc. */
					break;
			}
		}
	}

	/* Set current Item to selection. */
	if (hDown & KEY_Y) {
		if (this->selectMode == 0) {
			this->itemID = this->MapItems[this->MapSelection]->id();
		}
	}

	if (this->selectMode == 0) {
		if (hRepeat & KEY_RIGHT) {
			if (this->currentPosX == 15 && this->currentAcre < 15) {
				/* Go one Acre next and reset X to 0. */
				this->currentAcre++;
				this->currentPosX = 0;
				updateStuff();

			} else if (this->currentPosX < 15) {
				this->currentPosX++;
				updateStuff();
			}
		}

		if (hRepeat & KEY_LEFT) {
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

		if (hRepeat & KEY_DOWN) {
			if (this->currentPosY == 15 && this->currentAcre < 12) {
				/* Go one Acre down & reset Y to 0. */
				this->currentAcre += 4;
				this->currentPosY = 0;
				updateStuff();

			} else if (this->currentPosY < 15) {
				this->currentPosY++;
				updateStuff();
			}
		}

		if (hRepeat & KEY_UP) {
			if (this->currentPosY == 0 && this->currentAcre > 3) {
				/* Go one Acre up. */
				this->currentAcre -= 4;
				this->currentPosY = 15;
				updateStuff();

			} else if (this->currentPosY > 0) {
				this->currentPosY--;
				updateStuff();
			}
		}
	} else {

		if (hRepeat & KEY_DOWN) {
			if (this->selection < 3) this->selection++;
		}

		if (hRepeat & KEY_UP) {
			if (this->selection > 0) this->selection--;
		}
	}

	if (touching(touch, this->mainButtons[0])) {
		this->selection = 0;
		this->Mode = 1;
	}
}

void TownMapEditorWW::DrawTempItem(void) const {
	GFX::DrawTop();
	GFX::DrawFileBrowseBG(true);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();

	for (int i = 0; i < 2; i++) {
		GFX::DrawButton(this->tempItemPos[i], 0.7f);
	}

	/* We have no other choice. */
	Gui::DrawStringCentered(this->tempItemPos[0].x + 14 - 160 + (this->tempItemPos[0].xLength / 2), this->tempItemPos[0].y + ((this->tempItemPos[0].yLength + 6) - Gui::GetStringHeight(0.7, "Item ID: " + std::to_string(this->itemID), font)) / 2 + 14, 0.7f, BLACK, Lang::get("ITEM_ID") + ": " + std::to_string(this->itemID), this->tempItemPos[0].xLength, this->tempItemPos[0].yLength + 7, font);

	GFX::DrawGUI(gui_pointer_idx, this->tempItemPos[this->selection].x + 130, this->tempItemPos[this->selection].y + 25);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void TownMapEditorWW::TempItemLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hDown & KEY_B) {
		this->selection = 0;
		this->Mode = 0;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->selection < 1) this->selection++;
	}

	if (hRepeat & KEY_UP) {
		if (this->selection > 0) this->selection--;
	}

	if (hDown & KEY_A) {
		switch (this->selection) {
			case 0:
				this->itemID = (u16)Input::setInt(32766, Lang::get("ENTER_DECIMAL_ID"), 5, this->itemID);
				break;

			case 1:
				this->itemID = Overlays::SelectItem(this->itemID, SaveType::WW);
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, tempItemPos[0])) {
			this->itemID = (u16)Input::setInt(32766, Lang::get("ENTER_DECIMAL_ID"), 5, this->itemID);


		} else if (touching(touch, tempItemPos[1])) {
			this->itemID = Overlays::SelectItem(this->itemID, SaveType::WW);
		}
	}
}