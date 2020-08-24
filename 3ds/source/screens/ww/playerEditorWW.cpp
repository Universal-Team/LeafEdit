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
#include "playerEditorWW.hpp"
#include "Sav.hpp"
#include "stringUtils.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern std::shared_ptr<Sav> save;

PlayerEditorWW::PlayerEditorWW(std::shared_ptr<Player> p): player(p) { }

void PlayerEditorWW::Draw(void) const {
	if (Mode == 0) this->DrawSubMenu();
	else if (this->Mode == 1) return; /* Appearance. */ 
	else if (this->Mode == 2) this->DisplayPattern();
}

void PlayerEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->Mode == 0) this->SubMenuLogic(hDown, hHeld, touch);
	else if (this->Mode == 1) return; /* Appearance. */
	else if (this->Mode == 2) this->PatternLogic(hDown, hHeld, touch);
}

/* Sub Menu. */
void PlayerEditorWW::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - " + Lang::get("PLAYER_SUBMENU"), 395, 0, font);
	Gui::DrawStringCentered(0, 40, 0.7f, BLACK, Lang::get("PLAYER_NAME") + ": " + StringUtils::UTF16toUTF8(player->name()), 0, 0, font);
	Gui::DrawStringCentered(0, 60, 0.7f, BLACK, Lang::get("PLAYER_WALLET") + ": " + std::to_string(player->wallet()), 0, 0, font);
	Gui::DrawStringCentered(0, 90, 0.7f, BLACK, Lang::get("PLAYER_BANK") + ": " + std::to_string(player->bank()), 0, 0, font);
	Gui::DrawStringCentered(0, 120, 0.7f, BLACK, Lang::get("PLAYER_FACETYPE") + ": " + std::to_string(player->face()), 0, 0, font);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x + 100, mainButtons[i].y + 30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void PlayerEditorWW::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	/* Navigation. */
	if (hDown & KEY_UP) {
		if(Selection > 0) Selection--;
	}

	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				/* this->Mode = 1; */
				break;
			case 1:
				/* No idea. */
				break;
			case 2:
				/* Load Pattern. */
				C3D_FrameEnd(0);
				for (int i = 0; i < 8; i++) {
					this->pattern[i] = this->player->pattern(i);
					this->images[i] = this->pattern[i]->image(0);
					this->patternImage[i] = CoreUtils::patternImage(this->images[i], SaveType::WW);
				}

				this->Selection = 0;
				this->Mode = 2;
				break;
			case 3:
				break;
		}
	}

	if (hDown & KEY_DOWN) {
		if(Selection < 5) Selection++;
	}

	if (hDown & KEY_RIGHT) {
		if (Selection < 3) {
			Selection += 3;
		}
	}
	
	if (hDown & KEY_LEFT) {
		if (Selection < 6 && Selection > 2) {
			Selection -= 3;
		}
	}
	
	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}
}

void PlayerEditorWW::DisplayPattern(void) const {
	int selectX = 0, selectY = 0;

	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - " + Lang::get("PATTERN_VIEWER"), 395, 0, font);

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

	for (int i = 0; i < 8; i++) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 4; x++, i++) {
				C2D_DrawImageAt(this->patternImage[i], 40 + (x * 60), 60 + (y * 80), 0.5f, nullptr, 1.5f, 1.5f);
			}
		}
	}

	if (this->Selection < 4)	selectY = 0;	else	selectY = 1;
	if (this->Selection > 3)	selectX = this->Selection - 4;	else selectX = this->Selection;

	GFX::DrawGUI(gui_pointer_idx, 45 + (selectX * 60), 67 + (selectY * 80));
}

void PlayerEditorWW::PatternLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_RIGHT) {
		if (this->Selection < 7) {
			this->Selection++;
		}
	}

	if (hDown & KEY_LEFT) {
		if (this->Selection > 0) {
			this->Selection--;
		}
	}

	if (hDown & KEY_B) {
		/* Free. */
		C3D_FrameEnd(0);
		for (int i = 0; i < 8; i++) {
			if (this->patternImage[i].subtex != nullptr) C2DUtils::C2D_ImageDelete(this->patternImage[i]);
		}

		this->Selection = 0;
		this->Mode = 0;
	}

	/* Open Pattern Editor. */
	if (hDown & KEY_A) {
		Gui::setScreen(std::make_unique<PatternEditor>(this->pattern[this->Selection]), doFade, true);
	}
}