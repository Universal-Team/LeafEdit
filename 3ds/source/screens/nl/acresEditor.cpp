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
#include "gfx.hpp"
#include "offsets.hpp"
#include "save.hpp"

#include <3ds.h>

extern Save* SaveFile;

extern bool touching(touchPosition touch, Structs::ButtonPos button);
#define MAX_ACRE 218 // Define the Max Amount of Acres.

void AcresEditor::DrawTopSelection(void) const {
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

void AcresEditor::DrawIsland(void) const {
	GFX::DrawBottom();
	for (int i = 0; i < 16; i++) {
		AcreManagement::DrawAcre(SaveFile->island->IslandAcres[i], islandPos[i].x, islandPos[i].y);
	}
	// Draw Grid.
	for (int i = 0; i < 16; i++) {
		for (u32 y = 0; y < 4; y++) {
			for (u32 x = 0; x < 4; x++, i++) {
				Gui::drawGrid(80 + (x*40), 40 + (y*40), 40, 40, C2D_Color32(40, 40, 40, 160));
			}
		}
	}
	GFX::DrawSprite(sprites_pointer_idx, islandPos[Selection].x+20, islandPos[Selection].y+20);
}

void AcresEditor::DrawFullMap(void) const {
	GFX::DrawBottom(false);
	for (int i = 0; i < 42; i++) {
		AcreManagement::DrawAcre(SaveFile->town->FullAcres[i], mapPos[i].x, mapPos[i].y);
	}
	// Draw Grid.
	for (int i = 0; i < 42; i++) {
		for (u32 y = 0; y < 6; y++) {
			for (u32 x = 0; x < 7; x++, i++) {
				Gui::drawGrid(20 + (x*40), 0 + (y*40), 40, 40, C2D_Color32(40, 40, 40, 160));
			}
		}
	}
	GFX::DrawSprite(sprites_pointer_idx, mapPos[Selection].x+20, mapPos[Selection].y+20);
}

void AcresEditor::DrawMap(void) const {
	GFX::DrawBottom();
	for (int i = 0; i < 20; i++) {
		AcreManagement::DrawAcre(SaveFile->town->FullAcres[SelectionToAcre(i)], townPos[i].x, townPos[i].y);
	}
	// Draw Grid.
	for (int i = 0; i < 20; i++) {
		for (u32 y = 0; y < 4; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				Gui::drawGrid(60 + (x*40), 40 + (y*40), 40, 40, C2D_Color32(40, 40, 40, 160));
			}
		}
	}
	GFX::DrawSprite(sprites_pointer_idx, townPos[Selection].x+20, townPos[Selection].y+20);
}

int AcresEditor::SelectionToAcre(int selection) const {
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


void AcresEditor::Draw(void) const {
	DrawTopSelection();
	if (AcreMode == 0) {
		if (GodMode) {
			DrawFullMap();
		} else {
			DrawMap();
		}
	} else {
		DrawIsland();
	}
}

void AcresEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_X) {
		if (AcreMode == 0) {
			AcreMode = 1;
			Selection = 0;
		} else {
			AcreMode = 0;
			Selection = 0;
		}
	}

	// Set Top Acre to Selection.
	if (hDown & KEY_A) {
		if (AcreMode == 0) {
			if (selectedAcre < 182 || selectedAcre > 205) {
				if (GodMode) {
					SaveFile->town->FullAcres[Selection] = selectedAcre;
				} else {
					SaveFile->town->FullAcres[SelectionToAcre(Selection)] = selectedAcre;
				}
			}
		} else {
			if (selectedAcre < 206 && selectedAcre > 181) {
				SaveFile->island->IslandAcres[Selection] = selectedAcre;
			}
		}
	}

	// Display Selected Acre on Top Selection.
	if (hDown & KEY_Y) {
		if (AcreMode == 0) {
			if (GodMode) {
				selectedAcre = SaveFile->town->FullAcres[Selection];
			} else {
				selectedAcre = SaveFile->town->FullAcres[SelectionToAcre(Selection)];
			}
		} else {
			selectedAcre = SaveFile->island->IslandAcres[Selection];
		}
	}

	if (hDown & KEY_DOWN) {
		if (AcreMode == 0) {
			if (GodMode) {
				if (Selection < 35)	Selection += 7;
			} else {
				if (Selection < 15)	Selection += 5;
			}
		} else {
			if (Selection < 12)	Selection += 4;
		}
	}

	if (hDown & KEY_UP) {
		if (AcreMode == 0) {
			if (GodMode) {
				if (Selection > 6)	Selection -= 7;
			} else {
				if (Selection > 4)	Selection -= 5;
			}
		} else {
			if (Selection > 3)	Selection -= 4;
		}
	}

	if (hDown & KEY_RIGHT) {
		if (AcreMode == 0) {
			if (GodMode) {
				if (Selection < 41)	Selection++;
			} else {
				if (Selection < 19)	Selection++;
			}
		} else {
			if (Selection < 15)	Selection++;
		}
	}

	if (hDown & KEY_LEFT) {
		if (Selection > 0)	Selection--;
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
		if (AcreMode == 0) {
			if (GodMode) {
				for (int i = 0; i < 42; i++) {
					if (touching(touch, mapPos[i])) {
						Selection = i;
					}
				}
			} else {
				for (int i = 0; i < 20; i++) {
					if (touching(touch, townPos[i])) {
						Selection = i;
					}
				}
			}
		} else {
			for (int i = 0; i < 16; i++) {
				if (touching(touch, islandPos[i])) {
					Selection = i;
				}
			}
		}
	}
}