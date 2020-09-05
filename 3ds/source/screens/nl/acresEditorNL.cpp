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

#include "acresEditorNL.hpp"
#include "screenCommon.hpp"
#include "spriteManagement.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);
/* Bring that to other screens too. */
extern SaveType savesType;

/* Initialize Screen. */
AcresEditorNL::AcresEditorNL(std::unique_ptr<Town> &refTown): town(refTown) {
	/* Get Full Acres. */
	for (int i = 0; i < 42; i++) {
		this->FullAcres[i] = this->town->acre(i);
	}

	if (savesType == SaveType::NL) {
		this->maxAcres = 203;
	} else if (savesType == SaveType::WA) {
		this->maxAcres = 205;
	}
}

/* Draw the Acre Selection from the Top Screen. */
void AcresEditorNL::DrawTopSelection(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("ACRE_EDITOR"), 390, 0, font);
	Gui::DrawStringCentered(0, 180, 0.8f, BLACK, Lang::get("ACRE_ID") + std::to_string(this->selectedAcre), 390, 0, font);
	
	if (this->selectedAcre == 0) {
		SpriteManagement::DrawAcres(this->selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(this->selectedAcre+1, 300, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre+2, 350, 100, 1, 1);

	} else if (this->selectedAcre == 1) {
		SpriteManagement::DrawAcres(this->selectedAcre-1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(this->selectedAcre+1, 300, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre+2, 350, 100, 1, 1);

	} else if (this->selectedAcre == this->maxAcres-1) {
		SpriteManagement::DrawAcres(this->selectedAcre-2, 10, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre-1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(this->selectedAcre+1, 300, 100, 1, 1);

	} else if (this->selectedAcre == this->maxAcres) {
		SpriteManagement::DrawAcres(this->selectedAcre-2, 10, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre-1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.

	} else {
		SpriteManagement::DrawAcres(this->selectedAcre-2, 10, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre-1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(this->selectedAcre+1, 300, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre+2, 350, 100, 1, 1);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

/* Draw the Full TownMap. */
void AcresEditorNL::DrawFullMap(void) const {
	GFX::DrawBottom(true);
	for (int i = 0; i < 42; i++) {
		SpriteManagement::DrawAcres(this->FullAcres[i]->id(), acreMapPos[i].x, acreMapPos[i].y);
	}

	/* Draw Grid. */
	for (int i = 0; i < 42; i++) {
		for (u32 y = 0; y < 6; y++) {
			for (u32 x = 0; x < 7; x++, i++) {
				Gui::drawGrid(20 + (x * 40), 0 + (y * 40), 40, 40, C2D_Color32(40, 40, 40, 160));
			}
		}
	}

	GFX::DrawGUI(gui_pointer_idx, acreMapPos[this->selection].x+20, acreMapPos[this->selection].y+20);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void AcresEditorNL::Draw(void) const {
	this->DrawTopSelection();
	this->DrawFullMap();
}

void AcresEditorNL::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	/* Go back to the Town Editor. */
	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}

	/* Set Top Acre to Selection. */
	if (hDown & KEY_A) {
		if (this->selectedAcre < 182 || this->selectedAcre > 205) {
			this->FullAcres[this->selection]->id(this->selectedAcre);
		}
	}

	/* Display Selection on Top Acre. */
	if (hDown & KEY_Y) {
		this->selectedAcre = this->FullAcres[this->selection]->id();
	}

	if (hRepeat & KEY_DOWN) {
		if (this->selection < 35) this->selection += 7;
	}

	if (hRepeat & KEY_UP) {
		if (this->selection > 6) this->selection -= 7;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->selection < 41) this->selection++;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->selection > 0) this->selection--;
	}

	/*
		Top Screen Acre Selection.
		Faster Scroll Mode.
	*/
	if (this->FastMode) {
		if (keysHeld() & KEY_L) {
			if (this->selectedAcre > 0) this->selectedAcre--;
		}
		
		if (keysHeld() & KEY_R) {
			if (this->selectedAcre < this->maxAcres) this->selectedAcre++;
		}

		/* Normal speed. (1 by 1.) */
	} else {
		if (hRepeat & KEY_L) {
			if (this->selectedAcre > 0) this->selectedAcre--;
		}

		if (hRepeat & KEY_R) {
			if (this->selectedAcre < this->maxAcres) this->selectedAcre++;
		}
	}

	if (hDown & KEY_START) {
		if (this->FastMode) this->FastMode = false;
		else this->FastMode = true;
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 42; i++) {
			if (iconTouch(touch, this->acreMapPos[i])) {
				this->selection = i;
			}
		}
	}
}

/* Convert the Selection to the current Acre. */
int AcresEditorNL::SelectionToAcre(int i) const {
	switch(i) {
		case 0:
			return 8;

		case 1:
			return 9;

		case 2:
			return 10;

		case 3:
			return 11;

		case 4:
			return 12;

		case 5:
			return 15;

		case 6:
			return 16;

		case 7:
			return 17;

		case 8:
			return 18;

		case 9:
			return 19;

		case 10:
			return 22;

		case 11:
			return 23;

		case 12:
			return 24;

		case 13:
			return 25;

		case 14:
			return 26;

		case 15:
			return 29;

		case 16:
			return 30;

		case 17:
			return 31;

		case 18:
			return 32;

		case 19:
			return 33;
	}

	return 8; // Should Never Happen.
}