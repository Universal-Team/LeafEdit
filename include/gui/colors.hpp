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

#ifndef COLORS_HPP
#define COLORS_HPP

#include <citro2d.h>
#include <citro3d.h>
#include <sstream>
#include <string>

/**
 * @brief Creates a 8 byte RGBA color
 * @param r red component of the color
 * @param g green component of the color
 * @param b blue component of the color
 * @param a alpha component of the color
 */
#define RGBA8(r, g, b, a) ((((r)&0xFF)<<0) | (((g)&0xFF)<<8) | (((b)&0xFF)<<16) | (((a)&0xFF)<<24))

#define WHITE C2D_Color32(255, 255, 255, 255)

#define BLACK C2D_Color32(0, 0, 0, 255)

#define GRAY C2D_Color32(127, 127, 127, 255)

#define DARKGRAY C2D_Color32(70, 70, 70, 255)

#define TRANSPARENT C2D_Color32(0, 0, 0, 0)

#define GREEN C2D_Color32(0, 140, 0, 255)

#define DIM C2D_Color32(0, 0, 0, 190)

#define RED C2D_Color32(255, 0, 0, 255)

#define SelectorBlue C2D_Color32(0, 191, 255, 255)

#define BarGreen C2D_Color32(0, 200, 10, 255)

#define selectedColor C2D_Color32(180, 180, 180, 255)

#define green1 C2D_Color32(0, 34, 23, 255) // -15261184
#define green2 C2D_Color32(38, 77, 52, 255) // -13012396
#define green3 C2D_Color32(84, 114, 57, 255) // -13349594
#define green4 C2D_Color32(168, 192, 96, 255) // -10436440
#define green5 C2D_Color32(192, 216, 120, 255) // -8857408
#define greentext C2D_Color32(239, 255, 173, 255) // -5373969
typedef u32 Color;

namespace ColorHelper {
	int getColorValue(int color, int bgr);
	std::string getColorName(int color, int bgr);
}

#endif
