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

#include "patternEditor.hpp"
#include "patternViewer.hpp"
#include "screenCommon.hpp"
#include "spriteManagement.hpp"
#include "stringUtils.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);
// Bring that to other screens too.
extern SaveType savesType;

void PatternViewer::DisplayPatternInfo(void) const {
	Gui::DrawStringCentered(0, 40, 0.7f, BLACK, "Pattern Name: " + StringUtils::UTF16toUTF8(this->pattern->name()), 395, 0, font);
	Gui::DrawStringCentered(0, 60, 0.7f, BLACK, "Creator Name: " + StringUtils::UTF16toUTF8(this->pattern->creatorname()), 395, 0, font);
	Gui::DrawStringCentered(0, 80, 0.7f, BLACK, "Creator ID: " + std::to_string(pattern->creatorid()), 395, 0, font);
	Gui::DrawStringCentered(0, 100, 0.7f, BLACK, "Origin Town Name: " + StringUtils::UTF16toUTF8(this->pattern->origtownname()), 395, 0, font);
	Gui::DrawStringCentered(0, 120, 0.7f, BLACK, "Origin Town ID: " + std::to_string(this->pattern->origtownid()), 395, 0, font);
	if (this->pattern->creatorGender()) {
		Gui::DrawStringCentered(0, 140, 0.7f, BLACK, "Gender: Female", 395, 0, font);
	} else {
		Gui::DrawStringCentered(0, 140, 0.7f, BLACK, "Gender: Male", 395, 0, font);
	}
}

void PatternViewer::DrawPattern(void) const {
	for (u32 i = 0; i < 1024; i++) {
		for (u32 y = 0; y < 32; y++) {
			for (u32 x = 0; x < 32; x++, i++) {
				Gui::Draw_Rect(100 + x, 50 + y, 1, 1, this->image->getPixelColor(x, y));
			}
		}
	}
}

void PatternViewer::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - " + Lang::get("PATTERN_VIEWER"), 395, 0, font);
	this->DisplayPatternInfo();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	this->DrawPattern();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void PatternViewer::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}

	if (hDown & KEY_RIGHT) {
		if (savesType == SaveType::WW) {
			if (this->selectedPattern < 7) {
				this->selectedPattern++;
				this->pattern = this->player->pattern(this->selectedPattern);
				this->image = this->pattern->image(0);
			}
		} else {
			if (this->selectedPattern < 9) {
				this->selectedPattern++;
				this->pattern = this->player->pattern(this->selectedPattern);
				this->image = this->pattern->image(0);
			}
		}
	}

	if (hDown & KEY_LEFT) {
		if (this->selectedPattern > 0) {
			this->selectedPattern--;
			this->pattern = this->player->pattern(this->selectedPattern);
			this->image = this->pattern->image(0);
		}
	}

	if (hDown & KEY_A) {
		Gui::setScreen(std::make_unique<PatternEditor>(this->pattern), doFade, true);
	}
}