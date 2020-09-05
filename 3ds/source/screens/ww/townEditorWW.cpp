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
#include "patternEditor.hpp"
#include "townEditorWW.hpp"
#include "townMapEditorWW.hpp"
#include "Sav.hpp"
#include "stringUtils.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern std::shared_ptr<Sav> save;

TownEditorWW::TownEditorWW(std::unique_ptr<Town> refTown) {
	this->town = std::move(refTown);
	if (this->town->townflag()) this->pattern = this->town->townflag();
}

void TownEditorWW::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("TOWN_EDITOR"), 395, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == this->Selection) GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x + 100, mainButtons[i].y + 30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void TownEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 5) this->Selection++;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->Selection < 3) this->Selection += 3;
	}
	
	if (hRepeat & KEY_LEFT) {
		if (this->Selection < 6 && this->Selection > 2) this->Selection -= 3;
	}

	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				Gui::setScreen(std::make_unique<TownMapEditorWW>(this->town), doFade, true);
				break;

			case 1:
				Gui::setScreen(std::make_unique<AcresEditorWW>(this->town), doFade, true);
				break;

			case 2:
				if (this->pattern) Gui::setScreen(std::make_unique<PatternEditor>(this->pattern), doFade, true);
				break;
		}
	}
	
	if (hDown & KEY_TOUCH) {
		if (touching(touch, this->mainButtons[0])) {
			Gui::setScreen(std::make_unique<TownMapEditorWW>(this->town), doFade, true);


		} else if (touching(touch, this->mainButtons[1])) {
			Gui::setScreen(std::make_unique<AcresEditorWW>(this->town), doFade, true);


		} else if (touching(touch, this->mainButtons[2])) {
			if (this->pattern) Gui::setScreen(std::make_unique<PatternEditor>(this->pattern), doFade, true);
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}
}