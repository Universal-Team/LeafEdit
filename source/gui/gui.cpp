/*
*   This file is part of LeafEdit
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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

#include <3ds.h>
#include "gui/gui.hpp"
#include "gui/screens/screenCommon.hpp"
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>
#include <stack>

C3D_RenderTarget* top;
C3D_RenderTarget* bottom;

static C2D_SpriteSheet sprites;

C2D_TextBuf sizeBuf;
C2D_Font systemFont;
std::stack<std::unique_ptr<SCREEN>> screens;

// Clear Text.
void Gui::clearTextBufs(void)
{
    C2D_TextBufClear(sizeBuf);
}

// Draw a blended Image. (Maybe useful for later?)
void Gui::Draw_ImageBlend(int key, int x, int y, u32 color)
{
    C2D_ImageTint tint;
    C2D_SetImageTint(&tint, C2D_TopLeft, color, 0.5);
    C2D_SetImageTint(&tint, C2D_TopRight, color, 0.5);
    C2D_SetImageTint(&tint, C2D_BotLeft, color, 0.5);
    C2D_SetImageTint(&tint, C2D_BotRight, color, 0.5);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f, &tint);
}

// Initialize GUI.
Result Gui::init(void)
{
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    sizeBuf = C2D_TextBufNew(4096);
    sprites    = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    systemFont = C2D_FontLoadSystem(CFG_REGION_USA);
    return 0;
}

// Exit GUI.
void Gui::exit(void)
{
    if (sprites)
    {
        C2D_SpriteSheetFree(sprites);
    }
    C2D_TextBufDelete(sizeBuf);
    C2D_Fini();
    C3D_Fini();
}

// Draw a normal Sprite from the Spritesheet.
void Gui::sprite(int key, int x, int y)
{
    if (key == sprites_res_null_idx)
    {
        return;
    }
    // standard case
    else
    {
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f);
    }
}

// Display a Message, which needs to be confirmed with A/B.
bool Gui::promptMsg2(std::string promptMsg)
{
    Gui::clearTextBufs();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLACK);
    C2D_TargetClear(bottom, BLACK);
    Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    Gui::DrawString((400-Gui::Draw_GetStringWidth(0.72f, promptMsg.c_str()))/2, 50, 0.72f, WHITE, promptMsg.c_str());
    Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
	C3D_FrameEnd(0);

    while(1)
    {
		gspWaitForVBlank();
		hidScanInput();
		if(hidKeysDown() & KEY_A) {
			return true;
		} else if(hidKeysDown() & KEY_B) {
			return false;
		}
    }

}

bool Gui::promptMsg(std::string msg) {
	return Gui::promptMsg2(msg);
}

// Displays a Message for 2 Seconds. Good for warnings like invalid Language.
void Gui::DisplayWarnMsg(std::string Text)
{
    Gui::clearTextBufs();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLACK);
    C2D_TargetClear(bottom, BLACK);
    Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    Gui::DrawString((400-Gui::Draw_GetStringWidth(0.72f, Text.c_str()))/2, 2, 0.72f, WHITE, Text.c_str());
    Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
	C3D_FrameEnd(0);
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

// Display a Message, which can be skipped with A.
void Gui::DisplayWaitMsg(std::string waitMsg, ...)
{
    Gui::clearTextBufs();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLACK);
    C2D_TargetClear(bottom, BLACK);
    Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    //Gui::DrawString((400-Gui::Draw_GetStringWidth(0.72f, Lang::Continue))/2, 2, 0.72f, WHITE, Lang::Continue);
    Gui::DrawString((400-Gui::Draw_GetStringWidth(0.72f, waitMsg.c_str()))/2, 50, 0.72f, WHITE, waitMsg.c_str());
    Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
	C3D_FrameEnd(0);

	while(1)
    {
		hidScanInput();
		if(hidKeysDown() & KEY_A)
			break;
    }

}

// Draw String or Text.
void Gui::DrawString(float x, float y, float size, u32 color, std::string Text)
{
	C2D_Text c2d_text;
    C2D_TextFontParse(&c2d_text, systemFont, sizeBuf, Text.c_str());
	C2D_TextOptimize(&c2d_text);
	C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, size, size, color);
}


// Get String or Text Width.
float Gui::Draw_GetStringWidth(float size, std::string Text) {
	float width = 0;
	Draw_GetStringSize(size, &width, NULL, Text);
	return width;
}

// Get String or Text Size.
void Gui::Draw_GetStringSize(float size, float *width, float *height, std::string Text) {
	C2D_Text c2d_text;
    C2D_TextFontParse(&c2d_text, systemFont, sizeBuf, Text.c_str());
	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}


// Get String or Text Height.
float Gui::Draw_GetStringHeight(float size, std::string Text) {
	float height = 0;
	Draw_GetStringSize(size, NULL, &height, Text.c_str());
	return height;
}

// Draw a Rectangle.
bool Gui::Draw_Rect(float x, float y, float w, float h, u32 color) {
	return C2D_DrawRectSolid(x, y, 0.5f, w, h, color);
}

// Mainloop the GUI.
void Gui::mainLoop(u32 hDown, u32 hHeld, touchPosition touch) {
	screens.top()->Draw();
	screens.top()->Logic(hDown, hHeld, touch);
}

// Set the current Screen.
void Gui::setScreen(std::unique_ptr<SCREEN> screen)
{
    screens.push(std::move(screen));
}

// Go a Screen back.
void Gui::screenBack()
{
    screens.pop();
}

// Select, on which Screen should be drawn.
void Gui::ScreenDraw(C3D_RenderTarget * screen)
{
    C2D_SceneBegin(screen);
}

void Gui::drawSelector(float x, float y)
{
    static constexpr int w     = 2;
    static float timer         = 0.0f;
    float highlight_multiplier = fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
    u8 r                       = RED & 0xFF;
    u8 g                       = (RED >> 8) & 0xFF;
    u8 b                       = (RED >> 16) & 0xFF;
    u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);

    Gui::Draw_Rect(x, y, 50, 50, C2D_Color32(255, 255, 255, 100));
    Gui::Draw_Rect(x, y, 50, w, color);                      // top
    Gui::Draw_Rect(x, y + w, w, 50 - 2 * w, color);          // left
    Gui::Draw_Rect(x + 50 - w, y + w, w, 50 - 2 * w, color); // right
    Gui::Draw_Rect(x, y + 50 - w, 50, w, color);             // bottom

    timer += .025f;
}