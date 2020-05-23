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

#include "common.hpp"
#include "Player.hpp"
#include "playerManagement.hpp"
#include "Sav.hpp"
#include "spriteManagement.hpp"

#include <3ds.h>

extern std::shared_ptr<Sav> save;

// Hair RGB Colors [NL / WA].
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

// Hair RGB Colors [WW].
/*
	Dark Brown -> 128, 70, 27
	Light Brown -> 210, 105, 30
	Orange -> 255, 69, 0
	Blue -> 0, 191, 255
	Yellow -> 255, 216, 0
	Green -> 173, 255, 47
	Pink -> 255, 20, 147
	White -> 220, 220, 220
*/

// Get Hair & Eye color.
u32 PlayerManagement::getHairColor(u8 hairColor, SaveType save) {
	switch(save) {
		case SaveType::WW:
			switch(hairColor) {
				case 0:
					return C2D_Color32(128, 70, 27, 255);
				case 1:
					return C2D_Color32(210, 105, 30, 255);
				case 2:
					return C2D_Color32(255, 69, 0, 255);
				case 3:
					return C2D_Color32(0, 191, 255, 255);
				case 4:
					return C2D_Color32(255, 216, 0, 255);
				case 5:
					return C2D_Color32(173, 255, 47, 255);
				case 6:
					return C2D_Color32(255, 20, 147, 255);
				case 7:
					return C2D_Color32(220, 220, 220, 255);
				default:
					return C2D_Color32(0, 0, 0, 0);
			}
		case SaveType::NL:
		case SaveType::WA:
			switch(hairColor) {
				case 0:
					return C2D_Color32(89, 58, 56, 255);
				case 1:
					return C2D_Color32(147, 89, 41, 255);
				case 2:
					return C2D_Color32(239, 87, 46, 255);
				case 3:
					return C2D_Color32(65, 166, 220, 255);
				case 4:
					return C2D_Color32(255, 231, 121, 255);
				case 5:
					return C2D_Color32(139, 207, 98, 255);
				case 6:
					return C2D_Color32(238, 121, 139, 255);
				case 7:
					return C2D_Color32(255, 255, 255, 255);
				case 8:
					return C2D_Color32(0, 0, 0, 255);
				case 9:
					return C2D_Color32(65, 6, 1, 255);
				case 10:
					return C2D_Color32(187, 12, 7, 255);
				case 11:
					return C2D_Color32(0, 20, 73, 255);
				case 12:
					return C2D_Color32(222, 167, 15, 255);
				case 13:
					return C2D_Color32(1, 90, 34, 255);
				case 14:
					return C2D_Color32(173, 117, 188, 255);
				default:
					return C2D_Color32(0, 0, 0, 0);
			}
		case SaveType::UNUSED:
			return C2D_Color32(0, 0, 0, 0);
	}
	return C2D_Color32(0, 0, 0, 0);
}

// Eye RGB Colors [NL / WA].
/*
	0 -> 50, 54, 39.
	1 -> 205, 114, 70.
	2 -> 91, 151, 115.
	3 -> 109, 134, 128.
	4 -> 96, 128, 196.
	5 -> 63, 136, 189.
*/

// Only available in New Leaf / Welcome Amiibo.
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

// Hair Selection. Only NL / WA.
void PlayerManagement::DrawHairSelection(int selection, bool isFemale) {
	Gui::clearTextBufs();
	int page;
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, WHITE, "Select the Hair you like.", 400);
	for (int i = 0; i < 16; i++) {
		for (u32 y = 0; y < 4; y++) {
			for (u32 x = 0; x < 4; x++, i++) {
				Gui::Draw_Rect(5 + x * 100, 32.5 + y * 45, 90, 40, C2D_Color32(0, 0, 130, 255));
				if (isFemale == true) {
					SpriteManagement::DrawHair(17+i, 33 + x * 100, 33 + y * 45, 1, 1);
				} else {
					SpriteManagement::DrawHair(i, 33 + x * 100, 33 + y * 45, 1, 1);
				}
			}
		}
	}
	if (isFemale == true) {
		page = 1;
	} else {
		page = 0;
	}

	Gui::DrawStringCentered(0, 212, 0.9f, WHITE, "Current Page: " + std::to_string(page+1) + " / 2", 400);
	int selectY = 0, selectX = 0;
	if (selection > 3 && selection < 8)	selectX = selection - 4;	else if (selection > 7 && selection < 12)	selectX = selection - 8;
	else if (selection > 11)	selectX = selection - 12;	else if (selection < 4)	selectX = selection;

	if (selection < 4)	selectY = 0;	else if (selection > 3 && selection < 8)	selectY = 1;
	else if (selection > 7 && selection < 12)	selectY = 2;	else	selectY = 3;

	Gui::drawAnimatedSelector(5 + selectX * 100, 32.5 + selectY * 45, 90, 40, .030f, C2D_Color32(0, 0, 200, 255), C2D_Color32(0, 0, 0, 0));
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	C3D_FrameEnd(0);
}

u8 PlayerManagement::SelectHair(u8 currentHair) {
	s32 selection = 0;
	int hairPage = 0;
	while(1)
	{
		u8 hairImage;
		for (hairImage = 0; hairImage < 16; hairImage++) {
			if (selection == hairImage) {
				if (hairPage == 0) {
					DrawHairSelection(selection);
				} else {
					DrawHairSelection(selection, true);
				}
			}
		}
		hidScanInput();
		// Switch hair pages.
		if (hidKeysDown() & KEY_R) {
			if (hairPage < 1)	hairPage++;
		}

		if (hidKeysDown() & KEY_L) {
			if (hairPage > 0)	hairPage--;
		}

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
			if (hairPage == 0) {
				return selection;
			} else if (hairPage == 1) {
				return selection+17; // 17 -> Female 1.
			}
		}
		if (hidKeysDown() & KEY_B) {
			return currentHair;
		}
	}
}