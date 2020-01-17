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

#include "core/management/acreManagement.hpp"

#include "core/save/offsets.h"
#include "core/save/save.h"

#include "gui/gui.hpp"

#include "gui/screens/acresEditor.hpp"

#include <3ds.h>

extern Save* SaveFile;

#define MAX_ACRE 218 // Define the Max Amount of Acres.

// Offsets for the current Town MAP.
// 0x08, 0x09, 0x0A, 0x0B, 0x0C // Row 1.
// 0x0F, 0x10, 0x11, 0x12, 0x13 // Row 2.
// 0x16, 0x17, 0x18, 0x19, 0x1A // Row 3.
// 0x1D, 0x1E, 0x1F, 0x20, 0x21 // Row 4.

// Not added yet to the Town MAP Editing. This is the complete Town Map.
// 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 // Row 1.
// 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D // Row 2.
// 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14 // Row 3.
// 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B // Row 4.
// 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22	// Row 5.
// 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29 // Row 6.


void AcresEditor::Draw(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("ACRE_EDITOR"), 400);
	Gui::DrawBottom();
	townMap();
	DrawSelection();
}

void AcresEditor::DrawSelection(void) const {
	int select = 0;
	for (u32 y = 0; y < 4; y++) {
		for (u32 x = 0; x < 5; x++, select++) {
			if (Selection == select) {
				Gui::drawAnimatedSelector(60 + x * 40, 40 + y * 40, 40, 40, .030, C2D_Color32(0, 0, 0, 0));
			}
		}
	}
}

void AcresEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		if (Selection == 0) {
			writeAcre(0x08, selectAcre());
		} else if (Selection == 1) {
			writeAcre(0x09, selectAcre());
		} else if (Selection == 2) {
			writeAcre(0x0A, selectAcre());
		} else if (Selection == 3) {
			writeAcre(0x0B, selectAcre());
		} else if (Selection == 4) {
			writeAcre(0x0C, selectAcre());
		} else if (Selection == 5) {
			writeAcre(0x0F, selectAcre());
		} else if (Selection == 6) {
			writeAcre(0x10, selectAcre());
		} else if (Selection == 7) {
			writeAcre(0x11, selectAcre());
		} else if (Selection == 8) {
			writeAcre(0x12, selectAcre());
		} else if (Selection == 9) {
			writeAcre(0x13, selectAcre());
		} else if (Selection == 10) {
			writeAcre(0x16, selectAcre());
		} else if (Selection == 11) {
			writeAcre(0x17, selectAcre());
		} else if (Selection == 12) {
			writeAcre(0x18, selectAcre());
		} else if (Selection == 13) {
			writeAcre(0x19, selectAcre());
		} else if (Selection == 14) {
			writeAcre(0x1A, selectAcre());
		} else if (Selection == 15) {
			writeAcre(0x1D, selectAcre());
		} else if (Selection == 16) {
			writeAcre(0x1E, selectAcre());
		} else if (Selection == 17) {
			writeAcre(0x1F, selectAcre());
		} else if (Selection == 18) {
			writeAcre(0x20, selectAcre());
		} else if (Selection == 19) {
			writeAcre(0x21, selectAcre());
		}
		
	}


	if (hDown & KEY_R) {
		if (Selection < 19)	Selection++;
	}

	if (hDown & KEY_L) {
		if (Selection > 0)	Selection--;
	}
}


// To-Do for the Acres Editor.
u8 AcresEditor::selectAcre() {
	while(1)
	{
		u8 acreImage;
		for (acreImage = 0; acreImage < MAX_ACRE+1; acreImage++) {
			if (selectedAcre == acreImage) {
				Gui::clearTextBufs();
				C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
				C2D_TargetClear(top, BLACK);
				Gui::DrawTop();
				if (acreImage == 0) {
					AcreManagement::DrawAcre(acreImage, 150, 100, 2, 2); // Current Selected ACRE.
					AcreManagement::DrawAcre(acreImage+1, 300, 100, 1, 1);
					AcreManagement::DrawAcre(acreImage+2, 350, 100, 1, 1);
				} else if (acreImage == 1) {
					AcreManagement::DrawAcre(acreImage-1, 60, 100, 1, 1);
					AcreManagement::DrawAcre(acreImage, 150, 100, 2, 2); // Current Selected ACRE.
					AcreManagement::DrawAcre(acreImage+1, 300, 100, 1, 1);
					AcreManagement::DrawAcre(acreImage+2, 350, 100, 1, 1);
				} else if (acreImage == MAX_ACRE-1) {
					AcreManagement::DrawAcre(acreImage-2, 10, 100, 1, 1);
					AcreManagement::DrawAcre(acreImage-1, 60, 100, 1, 1);
					AcreManagement::DrawAcre(acreImage, 150, 100, 2, 2); // Current Selected ACRE.
					AcreManagement::DrawAcre(acreImage+1, 300, 100, 1, 1);
				} else if (acreImage == MAX_ACRE) {
					AcreManagement::DrawAcre(acreImage-2, 10, 100, 1, 1);
					AcreManagement::DrawAcre(acreImage-1, 60, 100, 1, 1);
					AcreManagement::DrawAcre(acreImage, 150, 100, 2, 2); // Current Selected ACRE.
				} else {
					AcreManagement::DrawAcre(acreImage-2, 10, 100, 1, 1);
					AcreManagement::DrawAcre(acreImage-1, 60, 100, 1, 1);
					AcreManagement::DrawAcre(acreImage, 150, 100, 2, 2); // Current Selected ACRE.
					AcreManagement::DrawAcre(acreImage+1, 300, 100, 1, 1);
					AcreManagement::DrawAcre(acreImage+2, 350, 100, 1, 1);
				}
				C3D_FrameEnd(0);
			}
		}
		gspWaitForVBlank();
		hidScanInput();
		if (hidKeysDown() & KEY_RIGHT) {
			if (selectedAcre < MAX_ACRE)	selectedAcre++;
			gspWaitForVBlank();
		}

		if (hidKeysHeld() & KEY_R) {
			if (selectedAcre < MAX_ACRE)	selectedAcre++;
			gspWaitForVBlank();
		}

		if (hidKeysDown() & KEY_LEFT) {
			if (selectedAcre > 0)	selectedAcre--;
			gspWaitForVBlank();
		}

		if (hidKeysHeld() & KEY_L) {
			if (selectedAcre > 0)	selectedAcre--;
			gspWaitForVBlank();
		}

		
		if (hidKeysDown() & KEY_A) {
			acreImage = selectedAcre;
			return acreImage;
		}
	}
}

// Write to the SaveFile the new Acre.
void AcresEditor::writeAcre(s32 acre, u8 newAcre) {
	u32 writeOffset = MAP_ACRES + acre * 2;
	SaveFile->Write(writeOffset, newAcre);
}

// Uhh.. Maps and such.

void AcresEditor::fullTown() const
{
	AcreManagement::InitAcres(42, 7, 20, 0, 0, MAP_ACRES); // Ends at x = 230px (resized to 30x30 images)
}

void AcresEditor::fullIsland() const
{
	AcreManagement::InitAcres(16, 4, 80, 40, 0, ISLAND_ACRES);
}

void AcresEditor::townMap() const
{
	AcreManagement::InitAcres(20, 5, 60, 40, 4, MAP_ACRES + 0x10);
}

void AcresEditor::islandMap() const
{
	AcreManagement::InitAcres(4, 2, 120, 80, 4, ISLAND_ACRES + 0xA);
}