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

#include "badgeEditor.hpp"
#include "coreUtils.hpp"
#include "itemUtils.hpp"
#include "spriteManagement.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);
extern std::vector<std::string> g_badges;
extern std::unique_ptr<Config> config;
extern touchPosition touch;

void BadgeEditor::DrawBadges(void) const {
	/* Draw all badges. */
	for (int i = 0; i < 24; i++) {
		SpriteManagement::DrawBadge(i, player->badge(i), this->badgeTouch[i].x, this->badgeTouch[i].y);
	}
}

void BadgeEditor::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("BADGE_EDITOR"), 395, 0, font);
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, Lang::get("CURRENT_BADGE") + g_badges[this->selectedBadge], 395, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();
	this->DrawBadges();
	GFX::DrawGUI(gui_pointer_idx, badgeTouch[this->selectedBadge].x+20, badgeTouch[this->selectedBadge].y+20);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void BadgeEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->selectedBadge < 23) this->selectedBadge++;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->selectedBadge > 0) this->selectedBadge--;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->selectedBadge < 18) this->selectedBadge += 6;
	}

	if (hRepeat & KEY_UP) {
		if (this->selectedBadge > 5) this->selectedBadge -= 6;
	}

	if (hDown & KEY_A) {
		if (this->player->badge(this->selectedBadge) < 3) this->player->badge(this->selectedBadge, (this->player->badge(this->selectedBadge) + 1));
		else this->player->badge(this->selectedBadge, 0);
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 24; i++) {
			if (iconTouch(touch, badgeTouch[i])) {
				if (this->player->badge(i) < 3) this->player->badge(i, (this->player->badge(i) + 1));
				else this->player->badge(i, 0);
			}
		}
	}

	if (hDown & KEY_Y) {
		u8 value = this->setAll();
		for (int i = 0; i < 24; i++) {
			this->player->badge(i, value);
		}
	}
}


u8 BadgeEditor::setAll() {
	s32 selection = 0;
	while(1) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, BLACK);
		C2D_TargetClear(Bottom, BLACK);

		GFX::DrawTop();
		Gui::DrawStringCentered(0, -2, 0.9f, WHITE, Lang::get("SET_BADGES_TO"), 395, 0, font);
		GFX::DrawBottom();

		for (int i = 0; i < 4; i++) {
			GFX::DrawButton(this->setPos[i]);
		}

		GFX::DrawGUI(gui_pointer_idx, setPos[selection].x + 100, setPos[selection].y + 15);
		C3D_FrameEnd(0);

		hidScanInput();
		u32 hRepeat = hidKeysDownRepeat();
		hidTouchRead(&touch);

		if (hidKeysDown() & KEY_TOUCH) {
			for (int i = 0; i < 4; i++) {
				if (touching(touch, setPos[i])) {
					return i;
				}
			}
		}

		if (hRepeat & KEY_DOWN) {
			if (selection < 3) selection++;
		}

		if (hRepeat & KEY_UP) {
			if (selection > 0) selection--;
		}

		if (hidKeysDown() & KEY_A) {
			return selection;
		}
	}
}