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

#include "common.hpp"
#include "overlay.hpp"
#include "spriteManagement.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern std::vector<std::tuple<u16, std::string, std::string>> villagerDB;

static void Draw(const int selection, const SaveType st, const int maxSelection) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawTop();
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, Lang::get("SELECT_VILLAGER"), 395, 0, font);
	SpriteManagement::DrawVillager(selection, 165, 80);

	Gui::DrawStringCentered(0, 140, 0.9f, WHITE, Lang::get("VILLAGER_NAME") + std::get<1>(villagerDB[selection]), 395, 0, font);

	Gui::DrawStringCentered(0, 160, 0.9f, WHITE, Lang::get("VILLAGER_ID") + std::to_string(selection), 395, 0, font);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, std::to_string(selection + 1) + " | " + std::to_string(maxSelection + 1), 400, 0, font);
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	C3D_FrameEnd(0);
}


u16 Overlays::SelectVillager(u16 oldID, const SaveType st) {
	int selection = (int)oldID;
	int maxSelection = 0;

	/* Get max amount, here. */
	switch(st) {
		case SaveType::WA:
			maxSelection = 398;
			break;
		case SaveType::NL:
			maxSelection = 332;
			break;
		case SaveType::WW:
			maxSelection = 149;
			break;
		case SaveType::UNUSED:
			return oldID;
	}

	while(1) {
		Draw(selection, st, maxSelection);
		
		/* Logic, here. */
		u32 hRepeat = hidKeysDownRepeat();
		hidScanInput();

		if (hRepeat & KEY_RIGHT) {
			if (selection < maxSelection) selection++;
		}

		if (hRepeat & KEY_LEFT) {
			if (selection > 0) selection--;
		}

		if (hRepeat & KEY_R) {
			if (selection < maxSelection) selection++;
		}

		if (hRepeat & KEY_L) {
			if (selection > 0) selection--;
		}

		if (hidKeysDown() & KEY_A) {
			return selection;
		}

		if (hidKeysDown() & KEY_B) {
			return oldID;
		}
	}
}