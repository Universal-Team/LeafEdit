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

#ifndef GUI_HPP
#define GUI_HPP

#include "spriteID.h"

#include "gui/colors.hpp"

#include "gui/screens/screen.hpp"

#include <3ds.h>
#include <stack>

namespace Gui
{
	// Init and Exit of the GUI.
	Result init(void);
	Result loadSheets(); // Load Spritesheets.
	Result unloadSheets(); // Unload Spritesheets.
	void exit(void);

	// Screen and MainLoops.
	void mainLoop(u32 hDown, u32 hHeld, touchPosition touch);
	void setScreen(std::unique_ptr<Screen> screen);
	void screenBack(void);
	C3D_RenderTarget* target(gfxScreen_t t);
	void ScreenDraw(C3D_RenderTarget * screen);

	// Clear Text.
	void clearTextBufs(void);

	// Sprite Drawing, but also with Scaling Feature!
	void sprite(int sheet, int key, int x, int y, float ScaleX = 1, float ScaleY = 1);

	void Draw_ImageBlend(int sheet, int key, int x, int y, u32 color, float ScaleX = 1, float ScaleY = 1);

	void DrawArrow(int x, int y, float rotation = 0);

	// float xPos -> X Position of the Selector.
	// float yPos -> Y Position of the Selector.
	// float Width -> The Width of the Selector.
	// float Height -> The Height of the Selector.
	// float speed -> The speed of the Animation. For example : ".030f/.030".
	// u32 colour -> The color Value for the Selector. example : C2D_Color32(0, 0, 0, 0) for Transparency.
	void drawAnimatedSelector(float xPos, float yPos, float Width, float Height, float speed, u32 colour);

	// Misc.
	bool Draw_Rect(float x, float y, float w, float h, u32 color);

	// Text / String Functions.
	void DrawStringCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth = 0, int maxHeight = 0);
	void DrawString(float x, float y, float size, u32 color, std::string Text, int maxWidth = 0, int maxHeight = 0);
	void GetStringSize(float size, float *width, float *height, std::string Text);
	float GetStringWidth(float size, std::string Text);
	float GetStringHeight(float size, std::string Text);

	// GUI Functions.
	void DrawTop(void);
	void DrawBottom(void);
	void DrawFileBrowseBG(bool isTop = true);
}

void DrawSprite(C2D_SpriteSheet sheet, size_t imgindex, int x, int y, float ScaleX = 1, float ScaleY = 1);

#endif