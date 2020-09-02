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

#include "acresEditorWW.hpp"
#include "saveUtils.hpp"
#include "screenCommon.hpp"
#include "spriteManagement.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

/* Initialize Screen. */
AcresEditorWW::AcresEditorWW(std::unique_ptr<Town> &refTown) : town(refTown) {

	/* Get Full Acres. */
	for (int i = 0; i < 36; i++) {
		this->FullAcres[i] = this->town->acre(i);
	}

	this->maxAcres = 130;
}

/* Draw the Acre Selection from the Top Screen. */
void AcresEditorWW::DrawTopSelection(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - " + Lang::get("ACRE_EDITOR"), 400, 0, font);
	Gui::DrawStringCentered(0, 180, 0.8f, BLACK, Lang::get("ACRE_ID") + std::to_string(this->selectedAcre), 400, 0, font);

	if (this->selectedAcre == 0) {
		SpriteManagement::DrawAcres(this->selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(this->selectedAcre + 1, 300, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre + 2, 350, 100, 1, 1);

	} else if (this->selectedAcre == 1) {
		SpriteManagement::DrawAcres(this->selectedAcre - 1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(this->selectedAcre + 1, 300, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre + 2, 350, 100, 1, 1);

	} else if (this->selectedAcre == this->maxAcres - 1) {
		SpriteManagement::DrawAcres(this->selectedAcre - 2, 10, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre - 1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(this->selectedAcre + 1, 300, 100, 1, 1);
	} else if (this->selectedAcre == this->maxAcres) {
		SpriteManagement::DrawAcres(this->selectedAcre - 2, 10, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre - 1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.

	} else {
		SpriteManagement::DrawAcres(this->selectedAcre - 2, 10, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre - 1, 60, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre, 150, 100, 2, 2); // Current Selected ACRE.
		SpriteManagement::DrawAcres(this->selectedAcre + 1, 300, 100, 1, 1);
		SpriteManagement::DrawAcres(this->selectedAcre + 2, 350, 100, 1, 1);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

/* Draw TownMap. */
void AcresEditorWW::DrawMap(void) const {
	GFX::DrawBottom(true);
	
	for (int i = 0; i < 36; i++) {
		SpriteManagement::DrawAcres(this->FullAcres[i]->id(), acrePos[i].x, acrePos[i].y, 1, 1);
	}
	
	GFX::DrawGUI(gui_pointer_idx, acrePos[this->selection].x + 14, acrePos[this->selection].y + 14);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void AcresEditorWW::Draw(void) const {
	DrawTopSelection();
	DrawMap();
}

void AcresEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	/* Go back to the Town Map Editor. */
	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}

	/* Set Top Acre to Selection. */
	if (hDown & KEY_A) {
		this->FullAcres[this->selection]->id(this->selectedAcre);
		changes = true;
	}

	/* Display Selection on Top Acre. */
	if (hDown & KEY_Y) {
		this->selectedAcre = this->FullAcres[this->selection]->id();
	}

	if (hRepeat & KEY_DOWN) {
		if (this->selection < 30) this->selection += 6;
	}

	if (hRepeat & KEY_UP) {
		if (this->selection > 5) this->selection -= 6;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->selection < 35) this->selection++;
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
			if (this->selectedAcre <this-> maxAcres) this->selectedAcre++;
		}

		/* Normal speed. (1 by 1) */
	} else {
		if (hRepeat & KEY_L) {
			if (this->selectedAcre > 0) this->selectedAcre--;
		}

		if (hRepeat & KEY_R) {
			if (this->selectedAcre < this->maxAcres) this->selectedAcre++;
		}
	}

	if (hDown & KEY_START) {
		if (this->FastMode) FastMode = false;
		else this->FastMode = true;
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 36; i++) {
			if (iconTouch(touch, acrePos[i])) {
				this->selection = i;
			}
		}
	}
}