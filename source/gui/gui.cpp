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

#include "common/config.hpp"

#include "gui/gui.hpp"

#include "gui/screens/screenCommon.hpp"

#include <3ds.h>
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
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f);
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
float Gui::GetStringWidth(float size, std::string Text) {
	float width = 0;
	GetStringSize(size, &width, NULL, Text);
	return width;
}

// Get String or Text Size.
void Gui::GetStringSize(float size, float *width, float *height, std::string Text) {
	C2D_Text c2d_text;
    C2D_TextFontParse(&c2d_text, systemFont, sizeBuf, Text.c_str());
	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}


// Get String or Text Height.
float Gui::GetStringHeight(float size, std::string Text) {
	float height = 0;
	GetStringSize(size, NULL, &height, Text.c_str());
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

void Gui::drawAnimatedSelector(float xPos, float yPos, float Width, float Height, float speed, u32 colour)
{
    static constexpr int w     = 2;
    static float timer         = 0.0f;
    float highlight_multiplier = fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
    u8 r                       = C2D_Color32(0, 191, 255, 255) & 0xFF;
    u8 g                       = (C2D_Color32(0, 191, 255, 255) >> 8) & 0xFF;
    u8 b                       = (C2D_Color32(0, 191, 255, 255) >> 16) & 0xFF;
    u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);

    // BG Color for the Selector.
    C2D_DrawRectSolid(xPos, yPos, 0.5, Width, Height, colour); // Black.

    // Animated Selector part.
    C2D_DrawRectSolid(xPos, yPos, 0.5, Width, w, color);                      // top
    C2D_DrawRectSolid(xPos, yPos + w, 0.5, w, Height - 2 * w, color);          // left
    C2D_DrawRectSolid(xPos + Width - w, yPos + w, 0.5, w, Height - 2 * w, color); // right
    C2D_DrawRectSolid(xPos, yPos + Height - w, 0.5, Width, w, color);             // bottom

    timer += speed; // Speed of the animation. Example : .030f / .030
}