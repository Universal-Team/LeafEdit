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

#include "logging.hpp"

#include "common/config.hpp"
#include "common/utils.hpp"

#include "core/management/playerManagement.hpp"

#include "gui/keyboard.hpp"
#include "gui/msg.hpp"

#include "gui/screens/screenCommon.hpp"

#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"

#include <3ds.h>

extern Save* SaveFile;

extern C2D_SpriteSheet Hairs;
extern C2D_SpriteSheet Faces;

// Draw the Face sprite. 0-11 -> Male, 12-23 -> Female.
void PlayerManagement::DrawFace(u16 Gender, u8 FaceID, int x, int y, float ScaleX, float ScaleY) {
	if (Gender == 1) {
		DrawSprite(Faces, FaceID+11, x, y, ScaleX, ScaleY);
	} else {
		DrawSprite(Faces, FaceID, x, y, ScaleX, ScaleY);
	}
}

// Draw the Hair sprite. Exceptions: Hair ID 16 & 33 doesn't really exist, that's called a 'Bad Hair' one.
void PlayerManagement::DrawHair(u8 HairID, int x, int y, float ScaleX, float ScaleY) {
	if (HairID < 16)	DrawSprite(Hairs, HairID, x, y, ScaleX, ScaleY);
	else if (HairID > 16)	DrawSprite(Hairs, HairID-1, x, y, ScaleX, ScaleY);
}

void PlayerManagement::DrawHairSelection(int selection, bool isFemale) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, WHITE, Lang::get("SELECT_HAIR"), 400);
	for (int i = 0; i < 16; i++) {
		for (u32 y = 0; y < 4; y++) {
			for (u32 x = 0; x < 4; x++, i++) {
				Gui::Draw_Rect(5 + x * 100, 32.5 + y * 45, 90, 40, unselectedColor);
				if (isFemale == true) {
					DrawSprite(Hairs, 16+i, 33 + x * 100, 33 + y * 45, 1, 1);
				} else {
					DrawSprite(Hairs, i, 33 + x * 100, 33 + y * 45, 1, 1);
				}
			}
		}
	}
	int selectY = 0, selectX = 0;
	if (selection > 3 && selection < 8)	selectX = selection - 4;	else if (selection > 7 && selection < 12)	selectX = selection - 8;
	else if (selection > 11)	selectX = selection - 12;	else if (selection < 4)	selectX = selection;

	if (selection < 4)	selectY = 0;	else if (selection > 3 && selection < 8)	selectY = 1;
	else if (selection > 7 && selection < 12)	selectY = 2;	else	selectY = 3;

	Gui::drawAnimatedSelector(5 + selectX * 100, 32.5 + selectY * 45, 90, 40, .030f, C2D_Color32(0, 0, 0, 0));
	C3D_FrameEnd(0);
}


u8 PlayerManagement::SelectHair(u8 currentHair, u16 Gender) {
	s32 selection = 0;
	while(1)
	{
		u8 hairImage;
		for (hairImage = 0; hairImage < 16; hairImage++) {
			if (selection == hairImage) {
				if (Gender == 0) {
					DrawHairSelection(selection);
				} else {
					DrawHairSelection(selection, true);
				}
			}
		}
		hidScanInput();

		if (hidKeysDown() & KEY_DOWN) {
			if (selection < 12)	selection += 4;
		}

		if (hidKeysDown() & KEY_UP) {
			if (selection > 3)	selection -= 4;
		}
		if (hidKeysDown() & KEY_RIGHT) {
			if (selection < 15)	selection++;
		}
		if (hidKeysDown() & KEY_LEFT) {
			if (selection > 0)	selection--;
		}
		if (hidKeysDown() & KEY_A) {
			if (Gender == 0) {
				return selection;
			} else if (Gender == 1) {
				return selection+17; // 17 -> Female 1.
			}
		}
		if (hidKeysDown() & KEY_B) {
			return currentHair;
		}
	}
}