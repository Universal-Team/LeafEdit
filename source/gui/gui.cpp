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

#include "common/config.hpp"

#include "gui/gui.hpp"

#include "gui/screens/screenCommon.hpp"

#include <3ds.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stack>

C3D_RenderTarget* top;
C3D_RenderTarget* bottom;

bool editorSheetHasLoaded = false;

C2D_SpriteSheet Acres;
C2D_SpriteSheet Faces;
C2D_SpriteSheet Hairs;
C2D_SpriteSheet Items;
C2D_SpriteSheet sprites;
C2D_SpriteSheet Villager;
C2D_SpriteSheet Villager2;

C2D_TextBuf sizeBuf;
C2D_Font systemFont;
std::stack<std::unique_ptr<Screen>> screens;
bool currentScreen = false;

// Clear Text.
void Gui::clearTextBufs(void)
{
	C2D_TextBufClear(sizeBuf);
}

// Draw a Sprite from the Sheet, but blended.
void Gui::Draw_ImageBlend(int sheet, int key, int x, int y, u32 color, float ScaleX, float ScaleY)
{
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 0.5);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 0.5);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 0.5);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 0.5);

	if (sheet == 0) { // Sprites.
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f, &tint, ScaleX, ScaleY);
	} else if (sheet == 1) { // villagers.
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(Villager, key), x, y, 0.5f, &tint, ScaleX, ScaleY);
	} else if (sheet == 2) { // villagers2.
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(Villager2, key), x, y, 0.5f, &tint, ScaleX, ScaleY);
	} else if (sheet == 3) { // Items.
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(Items, key), x, y, 0.5f, &tint, ScaleX, ScaleY);
	} else if (sheet == 4) { // Acres.
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(Acres, key), x, y, 0.5f, &tint, ScaleX, ScaleY);
	}
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
	sprites = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	systemFont = C2D_FontLoadSystem(CFG_REGION_USA);
	/*
		Was for testing the Villager stuff.
	Villager	= C2D_SpriteSheetLoad("romfs:/gfx/villagers.t3x");
	Villager2	= C2D_SpriteSheetLoad("romfs:/gfx/villagers2.t3x");
	*/
	return 0;
}

// Loading the Sheets.
Result Gui::loadSheets() {
	if (editorSheetHasLoaded == false) {
		editorSheetHasLoaded = true;
		Acres	= C2D_SpriteSheetLoad("romfs:/gfx/acres.t3x");
		Faces	= C2D_SpriteSheetLoad("romfs:/gfx/faces.t3x");
		Hairs	= C2D_SpriteSheetLoad("romfs:/gfx/hairs.t3x");
		Items	= C2D_SpriteSheetLoad("romfs:/gfx/items.t3x");
		Villager	= C2D_SpriteSheetLoad("romfs:/gfx/villagers.t3x");
		Villager2	= C2D_SpriteSheetLoad("romfs:/gfx/villagers2.t3x");
	}
	return 0;
}

// Unload all Sheets.
Result Gui::unloadSheets() {
	if (editorSheetHasLoaded == true) {
		editorSheetHasLoaded = false;
		C2D_SpriteSheetFree(Acres);
		C2D_SpriteSheetFree(Faces);
		C2D_SpriteSheetFree(Hairs);
		C2D_SpriteSheetFree(Items);
		C2D_SpriteSheetFree(Villager);
		C2D_SpriteSheetFree(Villager2);
	}
	return 0;
}

// Exit the whole GUI.
void Gui::exit(void)
{
	C2D_SpriteSheetFree(sprites);
	C2D_TextBufDelete(sizeBuf);
	C2D_Fini();
	C3D_Fini();
}

// Draw a Sprite from the Sheet.
void Gui::sprite(int sheet, int key, int x, int y, float ScaleX, float ScaleY)
{
	if (sheet == 0) { // Sprites.
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f, NULL, ScaleX, ScaleY);
	} else if (sheet == 1) { // villagers.
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(Villager, key), x, y, 0.5f, NULL, ScaleX, ScaleY);
	} else if (sheet == 2) { // villagers2.
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(Villager2, key), x, y, 0.5f, NULL, ScaleX, ScaleY);
	} else if (sheet == 3) { // Items.
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(Items, key), x, y, 0.5f, NULL, ScaleX, ScaleY);
	} else if (sheet == 4) { // Acres.
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(Acres, key), x, y, 0.5f, NULL, ScaleX, ScaleY);
	}
}

void Gui::DrawStringCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth, int maxHeight) {
    float heightScale;
    if(maxHeight == 0) {
        heightScale = size;
    } else {
        heightScale = std::min(size, size*(maxHeight/Gui::GetStringHeight(size, Text)));
    }

	Gui::DrawString((currentScreen ? 200 : 160)+x-((maxWidth == 0 ? (int)Gui::GetStringWidth(size, Text) : std::min(maxWidth, (int)Gui::GetStringWidth(size, Text)))/2), y, size, color, Text, maxWidth, heightScale);
}

// Draw String or Text.
void Gui::DrawString(float x, float y, float size, u32 color, std::string Text, int maxWidth, int maxHeight) {
    C2D_Text c2d_text;
    C2D_TextFontParse(&c2d_text, systemFont, sizeBuf, Text.c_str());
    C2D_TextOptimize(&c2d_text);

    float heightScale;
    if(maxHeight == 0) {
        heightScale = size;
    } else {
        heightScale = std::min(size, size*(maxHeight/Gui::GetStringHeight(size, Text)));
    }

    if(maxWidth == 0) {
        C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, size, heightScale, color);
    } else {
        C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, std::min(size, size*(maxWidth/Gui::GetStringWidth(size, Text))), heightScale, color);
    }
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
void Gui::setScreen(std::unique_ptr<Screen> screen)
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
	currentScreen = screen == top ? 1 : 0;
}

void Gui::drawAnimatedSelector(float xPos, float yPos, float Width, float Height, float speed, u32 colour)
{
	static constexpr int w     = 2;
	static float timer         = 0.0f;
	float highlight_multiplier = fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
	u8 r                       = SELECTED_COLOR & 0xFF;
	u8 g                       = (SELECTED_COLOR >> 8) & 0xFF;
	u8 b                       = (SELECTED_COLOR >> 16) & 0xFF;
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


void DrawSprite(C2D_SpriteSheet sheet, size_t imgindex, int x, int y, float ScaleX, float ScaleY)
{
	C2D_Image img = C2D_SpriteSheetGetImage(sheet, imgindex);
	C2D_DrawImageAt(img, x, y, 0.5f, NULL, ScaleX, ScaleY);
}

// Basic GUI Functions.

void Gui::DrawTop(void) {
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, DARKER_COLOR);
	Gui::Draw_Rect(0, 30, 400, 180, LIGHT_COLOR);
	Gui::Draw_Rect(0, 210, 400, 30, DARKER_COLOR);
}

void Gui::DrawBottom(void) {
	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, DARKER_COLOR);
	Gui::Draw_Rect(0, 30, 320, 180, LIGHT_COLOR);
	Gui::Draw_Rect(0, 210, 320, 30, DARKER_COLOR);
}

void Gui::DrawFileBrowseBG(bool isTop) {
	if (isTop == true) {
		Gui::ScreenDraw(top);
		Gui::Draw_Rect(0, 0, 400, 27, DARKER_COLOR);
		Gui::Draw_Rect(0, 27, 400, 31, LIGHT_COLOR);
		Gui::Draw_Rect(0, 58, 400, 31, LIGHTER_COLOR);
		Gui::Draw_Rect(0, 89, 400, 31, LIGHT_COLOR);
		Gui::Draw_Rect(0, 120, 400, 31, LIGHTER_COLOR);
		Gui::Draw_Rect(0, 151, 400, 31, LIGHT_COLOR);
		Gui::Draw_Rect(0, 182, 400, 31, LIGHTER_COLOR);
		Gui::Draw_Rect(0, 213, 400, 27, DARKER_COLOR);
	} else {
		Gui::ScreenDraw(bottom);
		Gui::Draw_Rect(0, 0, 320, 27, DARKER_COLOR);
		Gui::Draw_Rect(0, 27, 320, 31, LIGHT_COLOR);
		Gui::Draw_Rect(0, 58, 320, 31, LIGHTER_COLOR);
		Gui::Draw_Rect(0, 89, 320, 31, LIGHT_COLOR);
		Gui::Draw_Rect(0, 120, 320, 31, LIGHTER_COLOR);
		Gui::Draw_Rect(0, 151, 320, 31, LIGHT_COLOR);
		Gui::Draw_Rect(0, 182, 320, 31, LIGHTER_COLOR);
		Gui::Draw_Rect(0, 213, 320, 27, DARKER_COLOR);
	}
}