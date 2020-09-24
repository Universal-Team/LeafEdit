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
#include "playerEditorHHD.hpp"
#include "stringUtils.hpp"

PlayerEditorHHD::PlayerEditorHHD(std::unique_ptr<PlayerHHD> refPlayer): player(std::move(refPlayer)) { }

void PlayerEditorHHD::Draw(void) const {
	switch(this->Mode) {
		case 0:
			this->DrawSubMenu();
			break;

		case 1:
			this->DrawPattern();
			break;
	}
}

void PlayerEditorHHD::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	switch(this->Mode) {
		case 0:
			this->SubMenuLogic(hDown, hHeld, touch);
			break;
		
		case 1:
			this->PatternLogic(hDown, hHeld, touch);
			break;
	}
}

void PlayerEditorHHD::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("PLAYER_SUBMENU"), 395, 0, font);
	Gui::DrawStringCentered(0, 40, 0.7f, BLACK, Lang::get("PLAYER_NAME") + ": " + StringUtils::UTF16toUTF8(this->player->name()), 0, 0, font);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	
	GFX::DrawBottom();

	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(this->mainButtons[i]);
		
		if (i == this->Selection) GFX::DrawGUI(gui_pointer_idx, this->mainButtons[i].x+100, this->mainButtons[i].y+30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void PlayerEditorHHD::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}

	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				this->LoadPattern();
				this->Mode = 1;
				this->Selection = 0;
				break;
		}
	}
}

void PlayerEditorHHD::LoadPattern() {
	for (int i = 0; i < 15; i++) {
		this->loaded[i] = false;
	}

	C3D_FrameEnd(0);

	for (int i = 0, i2 = 0 + (this->PatternPage * 15); i < 15; i++, i2++) {
		this->pattern[i] = this->player->pattern(i2);

		if (this->pattern[i]) {
			this->images[i] = this->pattern[i]->image(0);
			if (this->images[i]) {
				this->patternImage[i] = CoreUtils::patternImage(this->images[i], SaveType::HHD);
				this->loaded[i] = true;
			}
		}
	}
}

void PlayerEditorHHD::DrawPattern(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("PATTERN_VIEWER"), 395, 0, font);

	Gui::DrawStringCentered(0, 40, 0.7f, BLACK, Lang::get("PATTERN_NAME") + ": " + StringUtils::UTF16toUTF8(this->pattern[this->Selection]->name()), 395, 0, font);
	Gui::DrawStringCentered(0, 60, 0.7f, BLACK, Lang::get("PATTERN_CREATOR_NAME") + ": " +  StringUtils::UTF16toUTF8(this->pattern[this->Selection]->creatorname()), 395, 0, font);
	Gui::DrawStringCentered(0, 80, 0.7f, BLACK, Lang::get("PATTERN_CREATOR_ID") + ": " + std::to_string(pattern[this->Selection]->creatorid()), 395, 0, font);
	Gui::DrawStringCentered(0, 100, 0.7f, BLACK, Lang::get("PATTERN_ORIGIN_NAME") + ": " + StringUtils::UTF16toUTF8(this->pattern[this->Selection]->origtownname()), 395, 0, font);
	Gui::DrawStringCentered(0, 120, 0.7f, BLACK, Lang::get("PATTERN_ORIGIN_ID") + ": " + std::to_string(this->pattern[this->Selection]->origtownid()), 395, 0, font);

	if (this->pattern[this->Selection]->creatorGender()) {
		Gui::DrawStringCentered(0, 140, 0.7f, BLACK, Lang::get("GENDER") + ": " + Lang::get("FEMALE"), 395, 0, font);

	} else {
		Gui::DrawStringCentered(0, 140, 0.7f, BLACK, Lang::get("GENDER") + ": " + Lang::get("MALE"), 395, 0, font);
	}

	GFX::DrawBottom();

	for (int i = 0; i < 15; i++) {
		if (this->patternImage[i].tex && this->loaded[i]) {
			C2D_DrawImageAt(this->patternImage[i], PatternPos[i].x, PatternPos[i].y, 0.5f, nullptr, 1.5f, 1.5f);
		}
	}

	GFX::DrawGUI(gui_pointer_idx, this->PatternPos[this->Selection].x + 20, this->PatternPos[this->Selection].y + 20);
}

void PlayerEditorHHD::PatternLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hDown & KEY_B) {
		/* Unload. */
		C3D_FrameEnd(0);

		for (int i = 0; i < 15; i++) {
			if (this->loaded[i]) C2DUtils::C2D_ImageDelete(this->patternImage[i]);
			this->loaded[i] = false;
		}

		this->Selection = 0;
		this->Mode = 0;
	}

	/* Switch Pattern Page. */
	if (hRepeat & KEY_R) {
		if (this->PatternPage < 7) {
			this->PatternPage++;
			this->LoadPattern();
		}
	}

	if (hRepeat & KEY_L) {
		if (this->PatternPage > 0) {
			this->PatternPage--;
			this->LoadPattern();
		}
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->Selection < 12) this->Selection += 3;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->Selection > 2) this->Selection -= 3;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 14) this->Selection++;
	}

	if (hRepeat & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}

	if (hDown & KEY_SELECT) {
		this->LoadPattern(); // Refresh.
	}
	
	if (hDown & KEY_A) {
		Gui::setScreen(std::make_unique<PatternEditor>(this->pattern[this->Selection]), doFade, true);
	}
}