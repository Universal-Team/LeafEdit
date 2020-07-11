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
#include "gfx.hpp"
#include "overlay.hpp"
#include "screenCommon.hpp"

#include <stack>

extern C2D_SpriteSheet GUI;
extern C2D_Font font;
extern std::unique_ptr<Config> config;

void GFX::DrawGUI(int img, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(GUI, img, x, y, ScaleX, ScaleY);
}

void GFX::DrawGUIBlend(int img, int x, int y, float ScaleX, float ScaleY, u32 color) {
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 0.5);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 0.5);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 0.5);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 0.5);
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(GUI, img), x, y, 0.5f,  &tint, ScaleX, ScaleY);
}

// Code from PKSM. https://github.com/FlagBrew/PKSM/blob/master/3ds/source/gui/gui.cpp#L73
Tex3DS_SubTexture _select_box(const C2D_Image& image, int x, int y, int endX, int endY) {
	Tex3DS_SubTexture tex = *image.subtex;
	if (x != endX) {
		int deltaX	= endX - x;
		float texRL	= tex.left - tex.right;
		tex.left	= tex.left - (float)texRL / tex.width * x;
		tex.right	= tex.left - (float)texRL / tex.width * deltaX;
		tex.width	= deltaX;
	}

	if (y != endY) {
		float texTB	= tex.top - tex.bottom;
		int deltaY	= endY - y;
		tex.top		= tex.top - (float)texTB / tex.height * y;
		tex.bottom	= tex.top - (float)texTB / tex.height * deltaY;
		tex.height	= deltaY;
	}

	return tex;
}

void GFX::DrawSelector(bool top, int y) {
	C2D_Image sprite = C2D_SpriteSheetGetImage(GUI, gui_selector_side_idx);
	Tex3DS_SubTexture tex = _select_box(sprite, 8, 0, 9, 21); // Get Height.

	if (top) {
		// Draw Sides.
		DrawGUI(gui_selector_side_idx, 0, y);
		DrawGUI(gui_selector_side_idx, 391, y, -1.0, 1.0);
		// Stretch the middle!
		C2D_DrawImageAt({sprite.tex, &tex}, 9, y, 0.5f, nullptr, 382.0, 1);
	} else {
		// Draw Sides.
		DrawGUI(gui_selector_side_idx, 0, y);
		DrawGUI(gui_selector_side_idx, 311, y, -1.0, 1.0);
		// Stretch the middle!
		C2D_DrawImageAt({sprite.tex, &tex}, 9, y, 0.5f, nullptr, 302.0, 1);
	}
}

void GFX::DrawBtn(int x, int y, int xLength, int yLength) {
	// Tint for the old style.
	C2D_ImageTint tint;
	if (!config->newStyle()) {
		C2D_SetImageTint(&tint, C2D_TopLeft, C2D_Color32(38, 130, 40, 255), 0.5);
		C2D_SetImageTint(&tint, C2D_TopRight, C2D_Color32(38, 130, 40, 255), 0.5);
		C2D_SetImageTint(&tint, C2D_BotLeft, C2D_Color32(38, 130, 40, 255), 0.5);
		C2D_SetImageTint(&tint, C2D_BotRight, C2D_Color32(38, 130, 40, 255), 0.5);
	}

	C2D_Image sprite = C2D_SpriteSheetGetImage(GUI, gui_button_corner_idx);
	Tex3DS_SubTexture tex;
	
	// Corners.
	if (!config->newStyle()) {
		DrawGUIBlend(gui_button_corner_idx, x, y, 1, 1, C2D_Color32(23, 121, 53, 255));
		DrawGUIBlend(gui_button_corner_idx, x + xLength - 14, y, -1.0, 1.0, C2D_Color32(23, 121, 53, 255));
		DrawGUIBlend(gui_button_corner_idx, x, y + yLength - 14, 1.0, -1.0, C2D_Color32(23, 121, 53, 255));
		DrawGUIBlend(gui_button_corner_idx, x + xLength - 14, y + yLength - 14, -1.0, -1.0, C2D_Color32(23, 121, 53, 255));
	} else {
		DrawGUI(gui_button_corner_idx, x, y);
		DrawGUI(gui_button_corner_idx, x + xLength - 14, y, -1.0, 1.0);
		DrawGUI(gui_button_corner_idx, x, y + yLength - 14, 1.0, -1.0);
		DrawGUI(gui_button_corner_idx, x + xLength - 14, y + yLength - 14, -1.0, -1.0);
	}

	// Height draw.
	tex = _select_box(sprite, 0, 11, 14, 12); // Get Height.
	if (!config->newStyle()) {
		C2D_DrawImageAt({sprite.tex, &tex}, x, y + 14, 0.5f, &tint, 1, yLength - 28);
		C2D_DrawImageAt({sprite.tex, &tex}, x + xLength - 14, y + 14, 0.5f, &tint, -1, yLength - 28);
	} else {
		C2D_DrawImageAt({sprite.tex, &tex}, x, y + 14, 0.5f, nullptr, 1, yLength - 28);
		C2D_DrawImageAt({sprite.tex, &tex}, x + xLength - 14, y + 14, 0.5f, nullptr, -1, yLength - 28);
	}

	// Width draw.
	tex = _select_box(sprite, 11, 0, 12, 14); // Get Width.
	if (!config->newStyle()) {
		C2D_DrawImageAt({sprite.tex, &tex}, x + 14, y, 0.5f, &tint, xLength - 28, 1);
		C2D_DrawImageAt({sprite.tex, &tex}, x + 14, y + yLength - 14, 0.5f, &tint, xLength - 28, -1);
	} else {
		C2D_DrawImageAt({sprite.tex, &tex}, x + 14, y, 0.5f, nullptr, xLength - 28, 1);
		C2D_DrawImageAt({sprite.tex, &tex}, x + 14, y + yLength - 14, 0.5f, nullptr, xLength - 28, -1);	
	}

	// And now the middle!
	tex = _select_box(sprite, 11, 11, 12, 12); // Get Corner pixel.
	if (!config->newStyle()) {
		C2D_DrawImageAt({sprite.tex, &tex}, x + 14, y + 14, 0.5f, &tint, xLength - 28, yLength - 28);
	} else {
		C2D_DrawImageAt({sprite.tex, &tex}, x + 14, y + 14, 0.5f, nullptr, xLength - 28, yLength - 28);
	}
}

void GFX::DrawTop(bool useBars, bool fullscreen) {
	Gui::ScreenDraw(Top);
	if (!config->newStyle()) {
		if (!fullscreen) {
			Gui::Draw_Rect(0, 0, 400, 25, C2D_Color32(14, 73, 32, 255));
			Gui::Draw_Rect(0, 25, 400, 190, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 215, 400, 25, C2D_Color32(14, 73, 32, 255));
		} else {
			Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(23, 121, 53, 255));
		}
	} else {
		if (!fullscreen) {
			// Draw Tiled BG.
			DrawGUI(gui_tileBG_idx, 0, 0);
			DrawGUI(gui_tileBG_idx, 135, 0);
			DrawGUI(gui_tileBG_idx, 270, 0);
			DrawGUI(gui_tileBG_idx, 0, 132);
			DrawGUI(gui_tileBG_idx, 135, 132);
			DrawGUI(gui_tileBG_idx, 270, 132);
			// Draw grass bar on bottom.
			DrawGUI(gui_bar_idx, 0, 215);
			if (useBars) {
				// Draw Text bar, so Text is readable.
				DrawGUI(gui_top_bar_idx, 0, 0);
			}
		} else {
			// Just solid.
			Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(215, 178, 111, 255)); // Draw BG based on Tile's color.
		}
	}
}

void GFX::DrawBottom(bool fullscreen) {
	Gui::ScreenDraw(Bottom);
	if (!config->newStyle()) {
		if (!fullscreen) {
			Gui::Draw_Rect(0, 0, 400, 25, C2D_Color32(14, 73, 32, 255));
			Gui::Draw_Rect(0, 25, 400, 190, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 215, 400, 25, C2D_Color32(14, 73, 32, 255));
		} else {
			Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(23, 121, 53, 255));
		}
	} else {
		DrawGUI(gui_tileBG_idx, 0, 0);
		DrawGUI(gui_tileBG_idx, 135, 0);
		DrawGUI(gui_tileBG_idx, 270, 0);
		DrawGUI(gui_tileBG_idx, 0, 132);
		DrawGUI(gui_tileBG_idx, 135, 132);
		DrawGUI(gui_tileBG_idx, 270, 132);
		if (!fullscreen) {
			DrawGUI(gui_bar_idx, 0, 0, 1, -1);
			DrawGUI(gui_bar_idx, 0, 215);
		}
	}
}

void GFX::DrawFileBrowseBG(bool isTop) {
	if (!config->newStyle()) {
		if (isTop) {
			Gui::ScreenDraw(Top);
			Gui::Draw_Rect(0, 0, 400, 24, C2D_Color32(14, 73, 32, 255));
			Gui::Draw_Rect(0, 24, 400, 21, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 45, 400, 21, C2D_Color32(23, 145, 53, 255));
			Gui::Draw_Rect(0, 66, 400, 21, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 87, 400, 21, C2D_Color32(23, 145, 53, 255));
			Gui::Draw_Rect(0, 108, 400, 21, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 129, 400, 21, C2D_Color32(23, 145, 53, 255));
			Gui::Draw_Rect(0, 150, 400, 21, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 171, 400, 21, C2D_Color32(23, 145, 53, 255));
			Gui::Draw_Rect(0, 192, 400, 21, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 213, 400, 27, C2D_Color32(14, 73, 32, 255));
		} else {
			Gui::ScreenDraw(Bottom);
			Gui::Draw_Rect(0, 0, 320, 24, C2D_Color32(14, 73, 32, 255));
			Gui::Draw_Rect(0, 24, 320, 21, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 45, 320, 21, C2D_Color32(23, 145, 53, 255));
			Gui::Draw_Rect(0, 66, 320, 21, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 87, 320, 21, C2D_Color32(23, 145, 53, 255));
			Gui::Draw_Rect(0, 108, 320, 21, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 129, 320, 21, C2D_Color32(23, 145, 53, 255));
			Gui::Draw_Rect(0, 150, 320, 21, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 171, 320, 21, C2D_Color32(23, 145, 53, 255));
			Gui::Draw_Rect(0, 192, 320, 21, C2D_Color32(23, 121, 53, 255));
			Gui::Draw_Rect(0, 213, 320, 27, C2D_Color32(14, 73, 32, 255));
		}
	} else {
		if (isTop) {
			Gui::ScreenDraw(Top);
			// Draw Bakground.
			Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
			// Draw FileBrowse Bars.
			Gui::Draw_Rect(0, 45, 400, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
			Gui::Draw_Rect(0, 87, 400, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
			Gui::Draw_Rect(0, 129, 400, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
			Gui::Draw_Rect(0, 171, 400, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
			// Draw Textbox bars.
			DrawGUI(gui_top_bar_idx, 0, 0);
			DrawGUI(gui_bottom_bar_idx, 0, 208);
		} else {
			Gui::ScreenDraw(Bottom);
			// Draw Background.
			Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
			// Draw FileBrowse Bars.
			Gui::Draw_Rect(0, 24, 320, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
			Gui::Draw_Rect(0, 67, 320, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
			Gui::Draw_Rect(0, 109, 320, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
			Gui::Draw_Rect(0, 152, 320, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
			Gui::Draw_Rect(0, 194, 320, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
			// Draw grass Bars.
			DrawGUI(gui_bar_idx, 0, 0, 1, -1);
			DrawGUI(gui_bar_idx, 0, 215);
		}
	}
}

void DrawList(int selection, const std::vector<std::string> List, const std::string Msg) {
	std::string lists;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawFileBrowseBG(true);
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, Msg, 400, 0, font);

	for (int i=(selection<8) ? 0 : (uint)selection-8;i<(int)List.size()&&i<((selection<8) ? 9 : selection+1);i++) {
		lists += List[i] + "\n";
	}

	for (uint i=0;i<((List.size()<9) ? 9-List.size() : 0);i++) {
		lists += "\n";
	}

	// Selector Logic.
	if (selection < 9)	GFX::DrawSelector(true, 24 + ((int)selection * 21));
	else				GFX::DrawSelector(true, 24 + (8 * 21));
	Gui::DrawString(5, 25, 0.85f, BLACK, lists, 360, 0, font);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, std::to_string(selection + 1) + " | " + std::to_string(List.size()), 395, 0, font);
	GFX::DrawFileBrowseBG(false);
	C3D_FrameEnd(0);
}

int GFX::ListSelection(int current, const std::vector<std::string> list, const std::string Msg) {
	s32 selection = current;
	int keyRepeatDelay = 0;
	while (1) {
		// Draw List.
		DrawList(selection, list, Msg);
		hidScanInput();
		if (keyRepeatDelay)	keyRepeatDelay--;
		if (hidKeysHeld() & KEY_DOWN && !keyRepeatDelay) {
			if (selection < (int)list.size()-1)	selection++;
			else	selection = 0;
			keyRepeatDelay = 6;
		}

		if (hidKeysHeld() & KEY_UP && !keyRepeatDelay) {
			if (selection > 0)	selection--;
			else if (selection == 0)	selection = (int)list.size()-1;
			keyRepeatDelay = 6;
		}

		if (hidKeysDown() & KEY_A) {
			return selection;
		}

		if (hidKeysDown() & KEY_B) {
			return -1;
		}
	}
}

// Draw a Button and draw Text on it.
void GFX::DrawButton(const ButtonType button, float TextSize) {
	DrawBtn(button.x, button.y, button.xLength, button.yLength);
	// Draw String.
	Gui::DrawStringCentered(button.x - 160 + (button.xLength/2), button.y + (button.yLength/2) - 10, TextSize, BLACK, Lang::get(button.Text), button.xLength-17, button.yLength-5, font);
}

// Special Grid for Items. I need to do this here instead of using the Universal-Core one.
void GFX::drawGrid(float xPos, float yPos, float Width, float Height, u32 itemColor, u32 gridColor) {
	static constexpr int w	= 1;
	C2D_DrawRectSolid(xPos, yPos, 0.5, Width, Height, itemColor);

	// Grid part.
	C2D_DrawRectSolid(xPos, yPos, 0.5, Width, w, gridColor); // top
	C2D_DrawRectSolid(xPos, yPos + w, 0.5, w, Height - 2 * w, gridColor); // left
	C2D_DrawRectSolid(xPos + Width - w, yPos + w, 0.5, w, Height - 2 * w, gridColor); // right
	C2D_DrawRectSolid(xPos, yPos + Height - w, 0.5, Width, w, gridColor); // bottom
}

// Main Logic.
std::unique_ptr<Overlay> overlay;

// Main Logic.
void GFX::Main(u32 hDown, u32 hHeld, touchPosition touch) {
	// Make sure it's not nullptr.
	if (overlay != nullptr) {
		// Make sure it's used.
		if (overlay->isUsed) {
			GFX::DrawTop();
			Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
			overlay->DrawOverlayTop();
			GFX::DrawBottom();
			Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
			overlay->DrawOverlayBottom();
			overlay->Logic(hDown, hHeld, touch);
		} else {
			Gui::DrawScreen(true);
			Gui::ScreenLogic(hDown, hHeld, touch, doFade, true);
		}
	} else {
		Gui::DrawScreen(true);
		Gui::ScreenLogic(hDown, hHeld, touch, doFade, true);
	}
}