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


#include "acresEditorWW.hpp"
#include "gfx.hpp"
#include "wwAcreManagement.hpp"
#include "wwsave.hpp"

#include <3ds.h>

extern WWSave* WWSaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

#define WWMAX_ACRE 130 // Define the Max Amount of Acres.

void AcresEditorWW::DrawTopSelection(void) const {
	u8 acreImage;
	for (acreImage = 0; acreImage < WWMAX_ACRE+1; acreImage++) {
		if (selectedAcre == acreImage) {
			GFX::DrawTop();
			if (acreImage == 0) {
				WWAcreManagement::DrawAcre(acreImage, 150, 100, 2, 2); // Current Selected ACRE.
				WWAcreManagement::DrawAcre(acreImage+1, 300, 100, 1, 1);
				WWAcreManagement::DrawAcre(acreImage+2, 350, 100, 1, 1);
			} else if (acreImage == 1) {
				WWAcreManagement::DrawAcre(acreImage-1, 60, 100, 1, 1);
				WWAcreManagement::DrawAcre(acreImage, 150, 100, 2, 2); // Current Selected ACRE.
				WWAcreManagement::DrawAcre(acreImage+1, 300, 100, 1, 1);
				WWAcreManagement::DrawAcre(acreImage+2, 350, 100, 1, 1);
			} else if (acreImage == WWMAX_ACRE-1) {
				WWAcreManagement::DrawAcre(acreImage-2, 10, 100, 1, 1);
				WWAcreManagement::DrawAcre(acreImage-1, 60, 100, 1, 1);
				WWAcreManagement::DrawAcre(acreImage, 150, 100, 2, 2); // Current Selected ACRE.
				WWAcreManagement::DrawAcre(acreImage+1, 300, 100, 1, 1);
			} else if (acreImage == WWMAX_ACRE) {
				WWAcreManagement::DrawAcre(acreImage-2, 10, 100, 1, 1);
				WWAcreManagement::DrawAcre(acreImage-1, 60, 100, 1, 1);
				WWAcreManagement::DrawAcre(acreImage, 150, 100, 2, 2); // Current Selected ACRE.
			} else {
				WWAcreManagement::DrawAcre(acreImage-2, 10, 100, 1, 1);
				WWAcreManagement::DrawAcre(acreImage-1, 60, 100, 1, 1);
				WWAcreManagement::DrawAcre(acreImage, 150, 100, 2, 2); // Current Selected ACRE.
				WWAcreManagement::DrawAcre(acreImage+1, 300, 100, 1, 1);
				WWAcreManagement::DrawAcre(acreImage+2, 350, 100, 1, 1);
			}
		}
	}
}

void AcresEditorWW::Draw(void) const {
	DrawTopSelection();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("ACRE_EDITOR"), 400);
	Gui::DrawStringCentered(0, 170, 0.9f, WHITE, "Acre ID: " + std::to_string(selectedAcre), 400);
	GFX::DrawBottom(false);
	DrawAcres();
	GFX::DrawSprite(sprites_pointer_idx, MapPos[Selection].x+14, MapPos[Selection].y+14);
}

void AcresEditorWW::DrawAcres() const {
	for (int i = 0; i < 36; i++) {
		WWAcreManagement::DrawAcre(WWSaveFile->town->FullAcres[i], MapPos[i].x, MapPos[i].y, 1, 1);
	}
}

void AcresEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") +  "\n" + Lang::get("B_BACK"));
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 36; i++) {
			if (touching(touch, MapPos[i])) {
				Selection = i;
			}
		}
	}

	// Set current Acre.
	if (hDown & KEY_A) {
		WWSaveFile->town->FullAcres[Selection] = selectedAcre;
	}

	// Display current Acre on Top.
	if (hDown & KEY_Y) {
		selectedAcre = WWSaveFile->town->FullAcres[Selection];
	}

	if (hDown & KEY_RIGHT) {
		if (Selection < 35)	Selection++;
	}

	if (hDown & KEY_LEFT) {
		if (Selection > 0)	Selection--;
	}

	if (hDown & KEY_UP) {
		if (Selection > 5)	Selection -= 6;
	}

	if (hDown & KEY_DOWN) {
		if (Selection < 30)	Selection += 6;
	}


	// Top Screen Acre Selection.
	if (FastMode) {
		if (keysHeld() & KEY_L) {
			if (selectedAcre > 0) {
				selectedAcre--;
			}
		}
		if (keysHeld() & KEY_R) {
			if (selectedAcre < WWMAX_ACRE) {
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
			if (selectedAcre < WWMAX_ACRE) {
				selectedAcre++;
			}
		}
	}

	if (hDown & KEY_X) {
		if (FastMode)	FastMode = false;
		else	FastMode = true;
	}
}