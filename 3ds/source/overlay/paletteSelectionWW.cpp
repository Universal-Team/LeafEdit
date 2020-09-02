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

#include "coreUtils.hpp"
#include "overlay.hpp"

/* It needs to be ARGB to draw them as Rectangles. */
static const u32 WWPaletteColors[] = {
	0xFF0000FF, 0xFF3173FF, 0xFF00ADFF, 0xFF00FFFF, 0xFF00FFAD, 0xFF00FF52, 0xFF00FF00, 0xFF52AD00, 0xFFAD5200, 0xFFFF0000, 0xFFFF0052, 0xFFFF00AD, 0xFFFF00FF, 0xFF000000, 0xFFFFFFFF,
	0xFF7B7BFF, 0xFF7BB5FF, 0xFF7BE7FF, 0xFF7BFFFF, 0xFF7BFFDE, 0xFF7BFFAD, 0xFF7BFF7B, 0xFF84AD52, 0xFFAD8452, 0xFFFF7B7B, 0xFFFF7BB5, 0xFFFF7BE7, 0xFFFF7BFF, 0xFF000000, 0xFFFFFFFF,
	0xFF0000A5, 0xFF0031A5, 0xFF0073A5, 0xFF00A5A5, 0xFF00A573, 0xFF00A531, 0xFF00A500, 0xFF215200, 0xFF522100, 0xFFA50000, 0xFFA50031, 0xFFA50073, 0xFFA500A5, 0xFF000000, 0xFFFFFFFF,
	0xFF009C00, 0xFF6BCE5A, 0xFFDEFFB5, 0xFF6B9C00, 0xFFA5CE52, 0xFFD6FFAD, 0xFFAD5200, 0xFFD68429, 0xFFFFAD5A, 0xFFFF0000, 0xFFFF6B4A, 0xFFDE4A31, 0xFFB52118, 0xFF8C0000, 0xFFFFFFFF,
	0xFF0073AD, 0xFF42ADD6, 0xFF8CDEFF, 0xFF3908FF, 0xFF6B4AFF, 0xFF9C94FF, 0xFFFF00AD, 0xFFFF63D6, 0xFFFFCEFF, 0xFF9CBDFF, 0xFF7394DE, 0xFF4A63BD, 0xFF21399C, 0xFF00107B, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0052FF, 0xFF5AB5FF, 0xFFADEFFF, 0xFF00107B, 0xFF314AA5, 0xFF6B84D6, 0xFF9CBDFF, 0xFFFFAD5A, 0xFFFFC684, 0xFFFFE7AD, 0xFFFFFFD6, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF00FF00, 0xFF42FF42, 0xFF8CFF8C, 0xFFD6FFD6, 0xFFFF0000, 0xFFFF4242, 0xFFFF8C8C, 0xFFFFD6D6, 0xFF0000FF, 0xFF4242FF, 0xFF8C8CFF, 0xFFD6D6FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF003100, 0xFF426342, 0xFF849C84, 0xFFC6D6C6, 0xFF00107B, 0xFF294AA5, 0xFF5A8CD6, 0xFF8CC6FF, 0xFF00B5D6, 0xFF39CEE7, 0xFF7BDEF7, 0xFFBDF7FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFFFF0000, 0xFF0000FF, 0xFF00FFFF, 0xFFFF4242, 0xFF4242FF, 0xFF42FFFF, 0xFFFF8C8C, 0xFF8C8CFF, 0xFF8CFFFF, 0xFFFFD6D6, 0xFFD6D6FF, 0xFFD6FFFF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF00FF00, 0xFFFF0000, 0xFFFF00FF, 0xFF42FF42, 0xFFFF4242, 0xFFFF42FF, 0xFF8CFF8C, 0xFFFF8C8C, 0xFFFF8CFF, 0xFFD6FFD6, 0xFFFFD6D6, 0xFFFFD6FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF0000FF, 0xFF007BFF, 0xFF00FFFF, 0xFF00FF84, 0xFF00FF00, 0xFF7B8400, 0xFFFF0000, 0xFFFF007B, 0xFFFF94FF, 0xFF00B5D6, 0xFF0010BD, 0xFF00105A, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF639410, 0xFF527B08, 0xFF398C10, 0xFF319C31, 0xFF4AA5CE, 0xFF3994CE, 0xFF4A8CBD, 0xFF318CD6, 0xFF4A73AD, 0xFF315A8C, 0xFF29426B, 0xFFFFEF84, 0xFFEFCE31, 0xFFC6A500, 0xFFFFFFFF,
	0xFFE7DED6, 0xFFDECEB5, 0xFFEFEFE7, 0xFFF7F7F7, 0xFF7B7384, 0xFF6B8C94, 0xFF637B84, 0xFF5A849C, 0xFFB59C73, 0xFF2929FF, 0xFF00FFFF, 0xFFFF2194, 0xFFBD9C00, 0xFF000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFEFEFF7, 0xFFDEDEE7, 0xFFCECED6, 0xFFB5B5C6, 0xFFA5A5B5, 0xFF9494A5, 0xFF84849C, 0xFF6B6B8C, 0xFF5A5A7B, 0xFF4A4A6B, 0xFF31315A, 0xFF21214A, 0xFF101042, 0xFF000031,
	0xFFFFFFFF, 0xFFEFEFEF, 0xFFDEDEDE, 0xFFCECECE, 0xFFB5B5B5, 0xFFA5A5A5, 0xFF949494, 0xFF848484, 0xFF6B6B6B, 0xFF5A5A5A, 0xFF4A4A4A, 0xFF313131, 0xFF212121, 0xFF101010, 0xFF000000,
	0xFF7B8CFF, 0xFF0000FF, 0xFF007BFF, 0xFF00FFFF, 0xFF008400, 0xFF00FF00, 0xFFFF0000, 0xFFFF9C00, 0xFFFF00D6, 0xFFFF6BFF, 0xFF00009C, 0xFF0094FF, 0xFF94BDFF, 0xFF000000, 0xFFFFFFFF
};

static void Draw(std::unique_ptr<PatternImage> &pImg, C2D_Image &img) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	GFX::DrawTop(true);
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9, WHITE, "LeafEdit - " + Lang::get("PALETTE_EDITOR"), 395, 0, font);
	if (img.subtex != nullptr) C2D_DrawImageAt(img, 125, 45, 0.5f, nullptr, 5, 5); // 160x160. 160/32 -> 5.
	GFX::DrawBottom(false);

	/* Drawing Palette. */
	for (int i = 0; i < 15; i++) {
		GFX::drawGrid(10 + i * 20, 120, 20, 20, WWPaletteColors[pImg->getPaletteColor(i)], C2D_Color32(20, 20, 20, 255));
	}

	Gui::DrawStringCentered(0, 80, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("PALETTE_INDEX") + ": " + std::to_string(pImg->getWWPaletteIndex() + 1), 310, 100, font);
	C3D_FrameEnd(0);
}

void Overlays::PaletteToolWW(std::unique_ptr<PatternImage> &pImg, C2D_Image &img) {
	bool exitOverlay = false;
	while(!exitOverlay) {
		
		Draw(pImg, img);
		u32 hRepeat = hidKeysDownRepeat();
		hidScanInput();

		/* Only allow actions, if alright. */
		if (pImg && img.subtex) {
			if (hRepeat & KEY_RIGHT) {
				if (pImg->getWWPaletteIndex() < 14) {
					pImg->setPaletteColor(pImg->getWWPaletteIndex() + 1, 0);
					C3D_FrameEnd(0);
					img = CoreUtils::patternImage(pImg, SaveType::WW);
				}
			}

			if (hRepeat & KEY_LEFT) {
				if (pImg->getWWPaletteIndex() > 0) {
					pImg->setPaletteColor(pImg->getWWPaletteIndex() - 1, 0);
					C3D_FrameEnd(0);
					img = CoreUtils::patternImage(pImg, SaveType::WW);
				}
			}
		}

		if (hidKeysDown() & KEY_A) {
			exitOverlay = true;
		}
	}
}