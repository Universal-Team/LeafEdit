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

#ifndef GFX_HPP
#define GFX_HPP

#include "gui.hpp"
#include "spriteIDs.h"

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <string>
#include <vector>

// Default colors i guess.
#define DIM C2D_Color32(0, 0, 0, 190)
#define WHITE C2D_Color32(255, 255, 255, 255)
#define BLACK C2D_Color32(0, 0, 0, 255)
#define TRANSPARENT C2D_Color32(0, 0, 0, 0)

// Green tones.
#define DARKER_GREEN				C2D_Color32(14, 73, 32, 255)
#define LIGHT_GREEN					C2D_Color32(23, 121, 53, 255)
#define LIGHTER_GREEN				C2D_Color32(23, 145, 53, 255)
#define SELECTED_GREEN				C2D_Color32(38, 130, 40, 255)
#define UNSELECTED_GREEN			C2D_Color32(38, 70, 40, 180)

extern u32 DARKER_COLOR, LIGHT_COLOR, LIGHTER_COLOR, SELECTED_COLOR, UNSELECTED_COLOR;

// Button Type.
struct ButtonType {
	int x;
	int y;
	int xLength;
	int yLength;
	std::string Text;
};

namespace GFX {
	// Draw Sprites.
	void DrawGUI(int img, int x, int y, float ScaleX = 1, float ScaleY = 1);
	void DrawGUIBlend(int img, int x, int y, float ScaleX = 1, float ScaleY = 1, u32 color = C2D_Color32(0, 0, 0, 180));
	void DrawSelector(bool top, int y);
	void DrawBtn(int x, int y, int xLength, int yLength);
	// Draw basic GUI.
	void DrawTop(bool useBars = true, bool fullscreen = false);
	void DrawBottom(bool fullscreen = false);
	void DrawFileBrowseBG(bool isTop = true);

	// Selection from list.
	int ListSelection(int current, const std::vector<std::string> list, const std::string Msg);

	void DrawButton(const ButtonType button, float TextSize = 0.9f);

	void drawGrid(float xPos, float yPos, float Width, float Height, u32 itemColor, u32 gridColor = C2D_Color32(80, 80, 80, 100));

	void Main(u32 hDown, u32 hHeld, touchPosition touch);
}

#endif