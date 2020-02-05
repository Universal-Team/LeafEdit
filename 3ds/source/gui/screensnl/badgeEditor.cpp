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

#include "core/management/playerManagement.hpp"

#include "gui/gui.hpp"

#include "gui/screensnl/badgeEditor.hpp"
#include "gui/screens/screenCommon.hpp"

#include "player.hpp"
#include "save.hpp"

#include <3ds.h>

extern int selectedPassedPlayer;
extern std::vector<std::string> g_badges;
extern Save * SaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern touchPosition touch;

void BadgeEditor::Draw(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("BADGE_EDITOR"), 400);
	Gui::DrawString(150, 70, 0.7f, WHITE, Lang::get("PHINEAS_TEXT"), 240);
	Gui::sprite(5, npc_phineas_idx, 30, 45);
	Gui::DrawStringCentered(0, 215, 0.8f, WHITE, Lang::get("CURRENT_BADGE") + g_badges[selectedBadge], 390);
	Gui::DrawBottom();
	DrawBadges();
	Gui::sprite(0, sprites_pointer_idx, badgeTouch[selectedBadge].x+20, badgeTouch[selectedBadge].y+20);
}

void BadgeEditor::DrawBadges(void) const {
	PlayerManagement::DrawBadge(0, SaveFile->players[selectedPassedPlayer]->Badges[0], 3, 33);
	PlayerManagement::DrawBadge(1, SaveFile->players[selectedPassedPlayer]->Badges[1], 58, 33);
	PlayerManagement::DrawBadge(2, SaveFile->players[selectedPassedPlayer]->Badges[2], 113, 33);
	PlayerManagement::DrawBadge(3, SaveFile->players[selectedPassedPlayer]->Badges[3], 168, 33);
	PlayerManagement::DrawBadge(4, SaveFile->players[selectedPassedPlayer]->Badges[4], 223, 33);
	PlayerManagement::DrawBadge(5, SaveFile->players[selectedPassedPlayer]->Badges[5], 278, 33);

	PlayerManagement::DrawBadge(6, SaveFile->players[selectedPassedPlayer]->Badges[6], 3, 78);
	PlayerManagement::DrawBadge(7, SaveFile->players[selectedPassedPlayer]->Badges[7], 58, 78);
	PlayerManagement::DrawBadge(8, SaveFile->players[selectedPassedPlayer]->Badges[8], 113, 78);
	PlayerManagement::DrawBadge(9, SaveFile->players[selectedPassedPlayer]->Badges[9], 168, 78);
	PlayerManagement::DrawBadge(10, SaveFile->players[selectedPassedPlayer]->Badges[10], 223, 78);
	PlayerManagement::DrawBadge(11, SaveFile->players[selectedPassedPlayer]->Badges[11], 278, 78);

	PlayerManagement::DrawBadge(12, SaveFile->players[selectedPassedPlayer]->Badges[12], 3, 123);
	PlayerManagement::DrawBadge(13, SaveFile->players[selectedPassedPlayer]->Badges[13], 58, 123);
	PlayerManagement::DrawBadge(14, SaveFile->players[selectedPassedPlayer]->Badges[14], 113, 123);
	PlayerManagement::DrawBadge(15, SaveFile->players[selectedPassedPlayer]->Badges[15], 168, 123);
	PlayerManagement::DrawBadge(16, SaveFile->players[selectedPassedPlayer]->Badges[16], 223, 123);
	PlayerManagement::DrawBadge(17, SaveFile->players[selectedPassedPlayer]->Badges[17], 278, 123);


	PlayerManagement::DrawBadge(18, SaveFile->players[selectedPassedPlayer]->Badges[18], 3, 168);
	PlayerManagement::DrawBadge(19, SaveFile->players[selectedPassedPlayer]->Badges[19], 58, 168);
	PlayerManagement::DrawBadge(20, SaveFile->players[selectedPassedPlayer]->Badges[20], 113, 168);
	PlayerManagement::DrawBadge(21, SaveFile->players[selectedPassedPlayer]->Badges[21], 168, 168);
	PlayerManagement::DrawBadge(22, SaveFile->players[selectedPassedPlayer]->Badges[22], 223, 168);
	PlayerManagement::DrawBadge(23, SaveFile->players[selectedPassedPlayer]->Badges[23], 278, 168);
}

void BadgeEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_RIGHT) {
		if (selectedBadge < 23)	selectedBadge++;
	}
	if (hDown & KEY_LEFT) {
		if (selectedBadge > 0)	selectedBadge--;
	}
	if (hDown & KEY_DOWN) {
		if (selectedBadge < 18)	selectedBadge += 6;
	}
	if (hDown & KEY_UP) {
		if (selectedBadge > 5)	selectedBadge -= 6;
	}

	if (hDown & KEY_A) {
		if (SaveFile->players[selectedPassedPlayer]->Badges[selectedBadge] < 3)	SaveFile->players[selectedPassedPlayer]->Badges[selectedBadge]++;
		else	SaveFile->players[selectedPassedPlayer]->Badges[selectedBadge] = 0;
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 24; i++) {
			if (touching(touch, badgeTouch[i])) {
				if (SaveFile->players[selectedPassedPlayer]->Badges[i] < 3)	SaveFile->players[selectedPassedPlayer]->Badges[i]++;
				else	SaveFile->players[selectedPassedPlayer]->Badges[i] = 0;
			}
		}
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK") + "\n" + Lang::get("Y_MULTISET") + "\n" + Lang::get("TOUCH_BADGE"));
	}

	if (hDown & KEY_Y) {
		u8 value = setAll();
		for (int i = 0; i < 24; i++) {
			SaveFile->players[selectedPassedPlayer]->Badges[i] = value;
		}
	}
}

u8 BadgeEditor::setAll() {
	s32 selection = 0;
	while(1)
	{
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, BLACK);
		C2D_TargetClear(bottom, BLACK);
		Gui::DrawTop();
		Gui::DrawStringCentered(0, 0, 0.9f, WHITE, Lang::get("SET_BADGES_TO"), 400);
		Gui::DrawBottom();
		Gui::Draw_Rect(100, 45, 120, 30, DARKER_COLOR);
		Gui::Draw_Rect(100, 85, 120, 30, DARKER_COLOR);
		Gui::Draw_Rect(100, 125, 120, 30, DARKER_COLOR);
		Gui::Draw_Rect(100, 165, 120, 30, DARKER_COLOR);
		Gui::DrawStringCentered(0, 50, 0.9f, WHITE, Lang::get("NONE"), 110);
		Gui::DrawStringCentered(0, 90, 0.9f, WHITE, Lang::get("BRONZE"), 110);
		Gui::DrawStringCentered(0, 130, 0.9f, WHITE, Lang::get("SILVER"), 110);
		Gui::DrawStringCentered(0, 170, 0.9f, WHITE, Lang::get("GOLD"), 110);
		Gui::sprite(0, sprites_pointer_idx, setPos[selection].x+100, setPos[selection].y+15);
		C3D_FrameEnd(0);

		hidScanInput();
		hidTouchRead(&touch);

		if (hidKeysDown() & KEY_TOUCH) {
			for (int i = 0; i < 4; i++) {
				if (touching(touch, setPos[i])) {
					return i;
				}
			}
		}

		if (hidKeysDown() & KEY_DOWN) {
			if (selection < 3)	selection++;
		}
		if (hidKeysDown() & KEY_UP) {
			if (selection > 0)	selection--;
		}
		if (hidKeysDown() & KEY_A) {
			return selection;
		}
	}
}