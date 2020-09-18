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

extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

static const u32 NLPaletteColors[] = {
	0xFFFFEEFF, 0xFFAA99FF, 0xFF9955EE, 0xFFAA66FF, 0xFF6600FF, 0xFF7744BB, 0xFF5500CC, 0xFF330099, 0xFF332255, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFCCBBFF, 0xFF7777FF, 0xFF1133DD, 0xFF4455FF, 0xFF0000FF, 0xFF6666CC, 0xFF4444BB, 0xFF0000BB, 0xFF222288, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFEEEEEE,
	0xFFBBCCDD, 0xFF66CCFF, 0xFF2266DD, 0xFF22AAFF, 0xFF0066FF, 0xFF5588BB, 0xFF0044DD, 0xFF0044BB, 0xFF113366, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFDDDDDD,
	0xFFDDEEFF, 0xFFCCDDFF, 0xFFAACCFF, 0xFF88BBFF, 0xFF88AAFF, 0xFF6688DD, 0xFF4466BB, 0xFF335599, 0xFF224488, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFCCCCCC,
	0xFFFFCCFF, 0xFFFF88EE, 0xFFDD66CC, 0xFFCC88BB, 0xFFFF00CC, 0xFF996699, 0xFFAA0088, 0xFF770055, 0xFF440033, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFBBBBBB,
	0xFFFFBBFF, 0xFFFF99FF, 0xFFBB22DD, 0xFFEE55FF, 0xFFCC00FF, 0xFF775588, 0xFF9900BB, 0xFF660088, 0xFF440055, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFAAAAAA,
	0xFF99BBDD, 0xFF77AACC, 0xFF334477, 0xFF4477AA, 0xFF003399, 0xFF223377, 0xFF002255, 0xFF001133, 0xFF001122, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF999999,
	0xFFCCFFFF, 0xFF77FFFF, 0xFF22DDDD, 0xFF00FFFF, 0xFF00DDFF, 0xFF00AACC, 0xFF009999, 0xFF007788, 0xFF005555, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF888888,
	0xFFFFBBDD, 0xFFEE99BB, 0xFFCC3366, 0xFFFF5599, 0xFFFF0066, 0xFF884455, 0xFF990044, 0xFF660022, 0xFF331122, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF777777,
	0xFFFFBBBB, 0xFFFF9988, 0xFFAA3333, 0xFFEE5533, 0xFFFF0000, 0xFF883333, 0xFFAA0000, 0xFF661111, 0xFF220000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF666666,
	0xFFBBEE99, 0xFF77CC66, 0xFF116622, 0xFF33AA44, 0xFF338800, 0xFF557755, 0xFF005522, 0xFF223311, 0xFF112200, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF555555,
	0xFFBBFFDD, 0xFF88FFCC, 0xFF55AA88, 0xFF88DDAA, 0xFF00FF88, 0xFF99BBAA, 0xFF00BB66, 0xFF009955, 0xFF006633, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF444444,
	0xFFFFDDBB, 0xFFFFCC77, 0xFF995533, 0xFFFF9966, 0xFFFF7711, 0xFFAA7744, 0xFF774422, 0xFF772200, 0xFF441100, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF333333,
	0xFFFFFFAA, 0xFFFFFF55, 0xFFBB8800, 0xFFCCBB55, 0xFFFFCC00, 0xFFAA9944, 0xFF886600, 0xFF554400, 0xFF332200, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF222222,
	0xFFEEFFCC, 0xFFDDEEAA, 0xFFAACC33, 0xFFBBEE55, 0xFFCCFF00, 0xFFAAAA77, 0xFF99AA00, 0xFF778800, 0xFF334400, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000,
	0xFFAAFFAA, 0xFF77FF77, 0xFF44DD66, 0xFF00FF00, 0xFF22DD22, 0xFF55BB55, 0xFF00BB00, 0xFF008800, 0xFF224422, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

static const std::vector<Structs::ButtonPos> paletteGrid = {
	{10, 120, 20, 20},
	{30, 120, 20, 20},
	{50, 120, 20, 20},

	{70, 120, 20, 20},
	{90, 120, 20, 20},
	{110, 120, 20, 20},

	{130, 120, 20, 20},
	{150, 120, 20, 20},
	{170, 120, 20, 20},

	{190, 120, 20, 20},
	{210, 120, 20, 20},
	{230, 120, 20, 20},

	{250, 120, 20, 20},
	{270, 120, 20, 20},
	{290, 120, 20, 20}
};

static void Draw(std::unique_ptr<PatternImage> &pImg, C2D_Image &img, int index) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	GFX::DrawTop(true);
	Gui::DrawStringCentered(0, -2, 0.9, WHITE, "LeafEdit - " + Lang::get("PALETTE_EDITOR"), 395, 0, font);
	if (img.tex) C2D_DrawImageAt(img, 125, 45, 0.5f, nullptr, 5, 5); // 160x160. 160/32 -> 5.
	GFX::DrawBottom(false);

	/* Drawing Palette. */
	for (int i = 0; i < 15; i++) {
		if (i == index) {
			GFX::drawGrid(paletteGrid[i].x, paletteGrid[i].y, paletteGrid[i].w, paletteGrid[i].h, NLPaletteColors[pImg->getPaletteColor(i)], C2D_Color32(180, 0, 0, 255));

		} else {
			GFX::drawGrid(paletteGrid[i].x, paletteGrid[i].y, paletteGrid[i].w, paletteGrid[i].h, NLPaletteColors[pImg->getPaletteColor(i)], C2D_Color32(20, 20, 20, 255));
		}
	}

	C3D_FrameEnd(0);
}

static const std::vector<Structs::ButtonPos> colorGrid = {
	{100, 80, 20, 20},
	{140, 80, 20, 20},
	{180, 80, 20, 20},

	{100, 120, 20, 20},
	{140, 120, 20, 20},
	{180, 120, 20, 20},

	{100, 160, 20, 20},
	{140, 160, 20, 20},
	{180, 160, 20, 20}
};

static void DrawPaletteSelection(int colorGroup, int selection) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	GFX::DrawBottom(false);

	Gui::DrawStringCentered(0, 50, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("COLOR_GROUP") + ": " + std::to_string(colorGroup + 1) + " | 17", 310, 100, font);

	if (colorGroup < 16) {
		for (int i = 0; i < 9; i++) {
			if (i == selection) {
				GFX::drawGrid(colorGrid[i].x, colorGrid[i].y, colorGrid[i].w, colorGrid[i].h, NLPaletteColors[(i + (colorGroup * 16))], C2D_Color32(180, 0, 0, 255));

			} else {
				GFX::drawGrid(colorGrid[i].x, colorGrid[i].y, colorGrid[i].w, colorGrid[i].h, NLPaletteColors[(i + (colorGroup * 16))], C2D_Color32(0, 0, 0, 255));
			}
		}

	} else {
		for (int i = 0; i < 15; i++) {
			if (i == selection) {
				GFX::drawGrid(paletteGrid[i].x, paletteGrid[i].y, paletteGrid[i].w, paletteGrid[i].h, NLPaletteColors[(15 + (colorGroup * i))], C2D_Color32(180, 0, 0, 255));

			} else {
				GFX::drawGrid(paletteGrid[i].x, paletteGrid[i].y, paletteGrid[i].w, paletteGrid[i].h, NLPaletteColors[(15 + (colorGroup * i))], C2D_Color32(20, 20, 20, 255));
			}
		}
	}

	C3D_FrameEnd(0);
}

void Overlays::PaletteToolNL(std::unique_ptr<PatternImage> &pImg, C2D_Image &img) {
	bool exitOverlay = false;
	bool selectColor = false;
	int group = 0, groupSelection = 0, index = 0;

	while(!exitOverlay) {
		
		if (!selectColor) Draw(pImg, img, index);
		else DrawPaletteSelection(group, groupSelection);
		touchPosition touch;
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();
		hidScanInput();
		hidTouchRead(&touch);

		if (pImg && img.subtex) {
			if (selectColor) {

				if (hRepeat & KEY_R) {
					if (group < 16) {
						group++;
						groupSelection = 0;
					}
				}

				if (hDown & KEY_TOUCH) {
					if (group < 16) {
						for (int i = 0; i < 9; i++) {
							if (iconTouch(touch, colorGrid[i])) {
								pImg->setPaletteColor(index, (i + (group * 16)));
								C3D_FrameEnd(0);
								img = CoreUtils::patternImage(pImg, SaveType::NL);
								groupSelection = 0;
								group = 0;
								selectColor = false;
								break;
							}
						}

					} else {
						for (int i = 0; i < 15; i++) {
							if (iconTouch(touch, paletteGrid[i])) {
								pImg->setPaletteColor(index, (15 + (group * i)));
								C3D_FrameEnd(0);
								img = CoreUtils::patternImage(pImg, SaveType::NL);
								groupSelection = 0;
								group = 0;
								selectColor = false;
								break;
							}
						}
					}
				}

				if (hRepeat & KEY_L) {
					if (group > 0) {
						group--;
						groupSelection = 0;
					}
				}

				if (hRepeat & KEY_RIGHT) {
					if (group < 16) {
						if (groupSelection < 2 || groupSelection < 5 || groupSelection < 8) {
							groupSelection++;
						}

					} else {
						if (groupSelection < 14) groupSelection++;
					}
				}

				if (hRepeat & KEY_LEFT) {
					if (group < 16) {
						if (groupSelection > 0 || groupSelection > 3 || groupSelection > 6) {
							groupSelection--;
						}
						
					} else {
						if (groupSelection > 0) groupSelection--;
					}
				}

				if (hRepeat & KEY_UP) {
					if (group < 16) {
						if (groupSelection > 2) groupSelection -= 3;
					}
				}

				if (hRepeat & KEY_DOWN) {
					if (group < 16) {
						if (groupSelection < 6) groupSelection += 3;
					}
				}

				if (hDown & KEY_A) {
					if (group < 16) pImg->setPaletteColor(index, (groupSelection + (group * 16)));
					else pImg->setPaletteColor(index, (15 + (group * groupSelection)));
					C3D_FrameEnd(0);
					img = CoreUtils::patternImage(pImg, SaveType::NL);
					groupSelection = 0;
					group = 0;
					selectColor = false;
				}
				
			} else {
				if (hRepeat & KEY_RIGHT) {
					if (index < 14) index++;
				}

				if (hDown & KEY_TOUCH) {
					for (int i = 0; i < 15; i++) {
						if (iconTouch(touch, paletteGrid[i])) {
							index = i;
							selectColor = true;
							break;
						}
					}
				}

				if (hRepeat & KEY_LEFT) {
					if (index > 0) index--;
				}

				if (hDown & KEY_A) {
					selectColor = true;
				}
			}
		}

		if (hDown & KEY_B) {
			exitOverlay = true;
		}
	}
}