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

#include "player.h"
#include "save.h"

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

// Draw the Hair sprite. Exceptions: Hair ID 16 & 33 doesn't really exist, that's called a 'Bed Hair' one and they are drawn as transparent.
void PlayerManagement::DrawHair(u8 HairID, int x, int y, float ScaleX, float ScaleY) {
	DrawSprite(Hairs, HairID, x, y, ScaleX, ScaleY);
}

// Hair Selection.
void PlayerManagement::DrawHairSelection(int selection, bool isFemale) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	C2D_TargetClear(bottom, BLACK);
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, WHITE, Lang::get("SELECT_HAIR"), 400);
	for (int i = 0; i < 16; i++) {
		for (u32 y = 0; y < 4; y++) {
			for (u32 x = 0; x < 4; x++, i++) {
				Gui::Draw_Rect(5 + x * 100, 32.5 + y * 45, 90, 40, UNSELECTED_COLOR);
				if (isFemale == true) {
					DrawSprite(Hairs, 17+i, 33 + x * 100, 33 + y * 45, 1, 1);
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
	Gui::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, DIM);
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

// Hair RGB Colors.
/*
	Dark Brown -> 89, 58, 56.
	Light Brown -> 147, 89, 41.
	Orange -> 239, 87, 46.
	Light Blue -> 65, 166, 220.
	Gold -> 255, 231, 121.
	Light Green -> 139, 207, 98.
	Pink -> 238, 121, 139.
	White -> 255, 255, 255.
	Black -> 0, 0, 0.
	Auburn -> 65, 6, 1.
	Red -> 187, 12, 7.
	Dark Blue -> 0, 20, 73.
	Blonde -> 222, 167, 15.
	Dark Green -> 1, 90, 34.
	Light Purple -> 173, 117, 188.
	Ash Brown -> 122, 121, 90.
*/

// Hair Color Selection.
void PlayerManagement::DrawHairColorSelection(int selection) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	C2D_TargetClear(bottom, BLACK);
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, WHITE, Lang::get("SELECT_HAIR_COLOR"), 400);

	Gui::Draw_Rect(5 + 0 * 100, 32.5 + 0 * 45, 90, 40, C2D_Color32(89, 58, 56, 255)); // Dark Brown.
	Gui::Draw_Rect(5 + 1 * 100, 32.5 + 0 * 45, 90, 40, C2D_Color32(147, 89, 41, 255)); // Light Brown.
	Gui::Draw_Rect(5 + 2 * 100, 32.5 + 0 * 45, 90, 40, C2D_Color32(239, 87, 46, 255)); // Orange.
	Gui::Draw_Rect(5 + 3 * 100, 32.5 + 0 * 45, 90, 40, C2D_Color32(65, 166, 220, 255)); // Light Blue.

	Gui::Draw_Rect(5 + 0 * 100, 32.5 + 1 * 45, 90, 40, C2D_Color32(255, 231, 121, 255)); // Gold.
	Gui::Draw_Rect(5 + 1 * 100, 32.5 + 1 * 45, 90, 40, C2D_Color32(139, 207, 98, 255)); // Light Green.
	Gui::Draw_Rect(5 + 2 * 100, 32.5 + 1 * 45, 90, 40, C2D_Color32(238, 121, 139, 255)); // Pink.
	Gui::Draw_Rect(5 + 3 * 100, 32.5 + 1 * 45, 90, 40, C2D_Color32(255, 255, 255, 255)); // White.

	Gui::Draw_Rect(5 + 0 * 100, 32.5 + 2 * 45, 90, 40, C2D_Color32(0, 0, 0, 255)); // Black.
	Gui::Draw_Rect(5 + 1 * 100, 32.5 + 2 * 45, 90, 40, C2D_Color32(65, 6, 1, 255)); // Auburn.
	Gui::Draw_Rect(5 + 2 * 100, 32.5 + 2 * 45, 90, 40, C2D_Color32(187, 12, 7, 255)); // Red.
	Gui::Draw_Rect(5 + 3 * 100, 32.5 + 2 * 45, 90, 40, C2D_Color32(0, 20, 73, 255)); // Dark Blue.

	Gui::Draw_Rect(5 + 0 * 100, 32.5 + 3 * 45, 90, 40, C2D_Color32(222, 167, 15, 255)); // Blonde.
	Gui::Draw_Rect(5 + 1 * 100, 32.5 + 3 * 45, 90, 40, C2D_Color32(1, 90, 34, 255)); // Dark Green.
	Gui::Draw_Rect(5 + 2 * 100, 32.5 + 3 * 45, 90, 40, C2D_Color32(173, 117, 188, 255)); // Light Purple.
	Gui::Draw_Rect(5 + 3 * 100, 32.5 + 3 * 45, 90, 40, C2D_Color32(122, 121, 90, 255)); // Ash Brown.

	int selectY = 0, selectX = 0;
	if (selection > 3 && selection < 8)	selectX = selection - 4;	else if (selection > 7 && selection < 12)	selectX = selection - 8;
	else if (selection > 11)	selectX = selection - 12;	else if (selection < 4)	selectX = selection;

	if (selection < 4)	selectY = 0;	else if (selection > 3 && selection < 8)	selectY = 1;
	else if (selection > 7 && selection < 12)	selectY = 2;	else	selectY = 3;

	Gui::drawAnimatedSelector(5 + selectX * 100, 32.5 + selectY * 45, 90, 40, .030f, C2D_Color32(0, 0, 0, 0));
	Gui::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, DIM);
	C3D_FrameEnd(0);
}

u8 PlayerManagement::SelectHairColor(u8 currentColor) {
	s32 selection = 0;
	while(1)
	{
		u8 hairColor;
		for (hairColor = 0; hairColor < 16; hairColor++) {
			if (selection == hairColor) {
				DrawHairColorSelection(selection);
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
			return selection;
		}
		if (hidKeysDown() & KEY_B) {
			return currentColor;
		}
	}
}

// Face Selection.
void PlayerManagement::DrawFaceSelection(int selection, bool isFemale) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	C2D_TargetClear(bottom, BLACK);
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, WHITE, Lang::get("SELECT_FACE"), 400);
	for (int i = 0; i < 12; i++) {
		for (u32 y = 0; y < 4; y++) {
			for (u32 x = 0; x < 3; x++, i++) {
				Gui::Draw_Rect(50 + x * 100, 32.5 + y * 45, 90, 40, UNSELECTED_COLOR);
				if (isFemale == true) {
					DrawFace(1, i, 78 + x * 100, 45 + y * 45);
				} else {
					DrawFace(0, i, 78 + x * 100, 45 + y * 45);
				}
			}
		}
	}
	int selectY = 0, selectX = 0;
	if (selection > 2 && selection < 6)	selectX = selection - 3;	else if (selection > 5 && selection < 9)	selectX = selection - 6;
	else if (selection > 9)	selectX = selection - 9;	else if (selection < 3)	selectX = selection;

	if (selection < 3)	selectY = 0;	else if (selection > 2 && selection < 6)	selectY = 1;
	else if (selection > 5 && selection < 9)	selectY = 2;	else	selectY = 3;

	Gui::drawAnimatedSelector(50 + selectX * 100, 32.5 + selectY * 45, 90, 40, .030f, C2D_Color32(0, 0, 0, 0));
	Gui::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, DIM);
	C3D_FrameEnd(0);
}

u8 PlayerManagement::SelectFace(u8 currentFace, u16 Gender) {
	s32 selection = 0;
	while(1)
	{
		u8 faceImage;
		for (faceImage = 0; faceImage < 12; faceImage++) {
			if (selection == faceImage) {
				if (Gender == 0) {
					DrawFaceSelection(selection);
				} else {
					DrawFaceSelection(selection, true);
				}
			}
		}
		hidScanInput();

		if (hidKeysDown() & KEY_DOWN) {
			if (selection < 9)	selection += 3;
		}

		if (hidKeysDown() & KEY_UP) {
			if (selection > 2)	selection -= 3;
		}
		if (hidKeysDown() & KEY_RIGHT) {
			if (selection < 11)	selection++;
		}
		if (hidKeysDown() & KEY_LEFT) {
			if (selection > 0)	selection--;
		}
		if (hidKeysDown() & KEY_A) {
			return selection;
		}
		if (hidKeysDown() & KEY_B) {
			return currentFace;
		}
	}
}

// TAN Selection.
void PlayerManagement::DrawTanSelection(int selection) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	C2D_TargetClear(bottom, BLACK);
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, WHITE, Lang::get("SELECT_TAN"), 400);
	for (int i = 0; i < 16; i++) {
		for (u32 y = 0; y < 4; y++) {
			for (u32 x = 0; x < 4; x++, i++) {
				Gui::Draw_Rect(5 + x * 100, 32.5 + y * 45, 90, 40, UNSELECTED_COLOR);
				Gui::DrawString(40 + x * 100, 40 + y * 45, 0.7f, WHITE, std::to_string(i));
			}
		}
	}
	int selectY = 0, selectX = 0;
	if (selection > 3 && selection < 8)	selectX = selection - 4;	else if (selection > 7 && selection < 12)	selectX = selection - 8;
	else if (selection > 11)	selectX = selection - 12;	else if (selection < 4)	selectX = selection;

	if (selection < 4)	selectY = 0;	else if (selection > 3 && selection < 8)	selectY = 1;
	else if (selection > 7 && selection < 12)	selectY = 2;	else	selectY = 3;

	Gui::drawAnimatedSelector(5 + selectX * 100, 32.5 + selectY * 45, 90, 40, .030f, C2D_Color32(0, 0, 0, 0));
	Gui::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, DIM);
	C3D_FrameEnd(0);
}

u32 PlayerManagement::SelectTan(u32 currentTan) {
	s32 selection = 0;
	while(1)
	{
		u32 TAN;
		for (TAN = 0; TAN < 16; TAN++) {
			if (selection == (int)TAN) {
				DrawTanSelection(selection);
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
			return selection;
		}
		if (hidKeysDown() & KEY_B) {
			return currentTan;
		}
	}
}

// Eye RGB Colors.
/*
	0 -> 50, 54, 39.
	1 -> 205, 114, 70.
	2 -> 91, 151, 115.
	3 -> 109, 134, 128.
	4 -> 96, 128, 196.
	5 -> 63, 136, 189.
*/

// Eye Color Selection.
void PlayerManagement::DrawEyeColorSelection(int selection) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	C2D_TargetClear(bottom, BLACK);
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, WHITE, Lang::get("SELECT_EYE_COLOR"), 400);

	Gui::Draw_Rect(50 + 0 * 100, 80 + 0 * 45, 90, 40, C2D_Color32(50, 54, 39, 255)); // 0.
	Gui::Draw_Rect(50 + 1 * 100, 80 + 0 * 45, 90, 40, C2D_Color32(205, 114, 70, 255)); // 1.
	Gui::Draw_Rect(50 + 2 * 100, 80 + 0 * 45, 90, 40, C2D_Color32(91, 151, 115, 255)); // 2.
	Gui::Draw_Rect(50 + 0 * 100, 80 + 1 * 45, 90, 40, C2D_Color32(109, 134, 128, 255)); // 3.
	Gui::Draw_Rect(50 + 1 * 100, 80 + 1 * 45, 90, 40, C2D_Color32(96, 128, 196, 255)); // 4.
	Gui::Draw_Rect(50 + 2 * 100, 80 + 1 * 45, 90, 40, C2D_Color32(63, 136, 189, 255)); // 5.


	int selectY = 0, selectX = 0;
	if (selection > 2)	selectX = selection - 3;	else	selectX = selection;

	if (selection < 3)	selectY = 0;	else	selectY = 1;
	

	Gui::drawAnimatedSelector(50 + selectX * 100, 80 + selectY * 45, 90, 40, .030f, C2D_Color32(0, 0, 0, 0));
	Gui::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, DIM);
	C3D_FrameEnd(0);
}

u8 PlayerManagement::SelectEyeColor(u8 currentColor) {
	s32 selection = 0;
	while(1)
	{
		u8 eyecolor;
		for (eyecolor = 0; eyecolor < 6; eyecolor++) {
			DrawEyeColorSelection(selection);
		}
		hidScanInput();

		if (hidKeysDown() & KEY_DOWN) {
			if (selection < 3)	selection += 3;
		}

		if (hidKeysDown() & KEY_UP) {
			if (selection > 2)	selection -= 3;
		}
		if (hidKeysDown() & KEY_RIGHT) {
			if (selection < 5)	selection++;
		}
		if (hidKeysDown() & KEY_LEFT) {
			if (selection > 0)	selection--;
		}
		if (hidKeysDown() & KEY_A) {
			return selection;
		}
		if (hidKeysDown() & KEY_B) {
			return currentColor;
		}
	}
}

// Get Hair & Eye color.
u32 PlayerManagement::getHairColor(u8 hairColor) {
	if (hairColor == 0) {
		return C2D_Color32(89, 58, 56, 255);
	}
	 else if (hairColor == 1) {
		return C2D_Color32(147, 89, 41, 255);
	}
	 else if (hairColor == 2) {
		return C2D_Color32(239, 87, 46, 255);
	}
	 else if (hairColor == 3) {
		return C2D_Color32(65, 166, 220, 255);
	}
	 else if (hairColor == 4) {
		return C2D_Color32(255, 231, 121, 255);
	}
	 else if (hairColor == 5) {
		return C2D_Color32(139, 207, 98, 255);
	}
	 else if (hairColor == 6) {
		return C2D_Color32(238, 121, 139, 255);
	}
	 else if (hairColor == 7) {
		return C2D_Color32(255, 255, 255, 255);
	}
	 else if (hairColor == 8) {
		return C2D_Color32(0, 0, 0, 255);
	}
	 else if (hairColor == 9) {
		return C2D_Color32(65, 6, 1, 255);
	}
	 else if (hairColor == 10) {
		return C2D_Color32(187, 12, 7, 255);
	}
	 else if (hairColor == 11) {
		return C2D_Color32(0, 20, 73, 255);
	}
	 else if (hairColor == 12) {
		return C2D_Color32(222, 167, 15, 255);
	}
	 else if (hairColor == 13) {
		return C2D_Color32(1, 90, 34, 255);
	}
	 else if (hairColor == 14) {
		return C2D_Color32(173, 117, 188, 255);
	}
	else {
		return C2D_Color32(122, 121, 90, 255);
	}
}

u32 PlayerManagement::getEyeColor(u8 eyeColor) {
	if (eyeColor == 0) {
		return C2D_Color32(50, 54, 39, 255);
	}
	 else if (eyeColor == 1) {
		return C2D_Color32(205, 114, 70, 255);
	}
	 else if (eyeColor == 2) {
		return C2D_Color32(91, 151, 115, 255);
	}
	 else if (eyeColor == 3) {
		return C2D_Color32(109, 134, 128, 255);
	}
	 else if (eyeColor == 4) {
		return C2D_Color32(96, 128, 196, 255);
	}
	 else if (eyeColor == 5) {
		return C2D_Color32(63, 136, 189, 255);
	}
	 else {
		 return C2D_Color32(63, 136, 189, 255); // Actually no real color. [6/7]
	}
}