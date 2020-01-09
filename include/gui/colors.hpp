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

#ifndef COLORS_HPP
#define COLORS_HPP

#include <citro2d.h>

// Select Colors.
#define selectedColor C2D_Color32(38, 130, 40, 255)
#define unselectedColor C2D_Color32(38, 70, 40, 180)

// Default colors i guess.
#define DIM C2D_Color32(0, 0, 0, 190)
#define WHITE C2D_Color32(255, 255, 255, 255)
#define BLACK C2D_Color32(0, 0, 0, 255)

// New Green tones.
extern u32 colorType; // Dark / Light mode.
#define DARKER_GREEN				C2D_Color32(14, 73, 32, 255)
#define LIGHT_GREEN					C2D_Color32(23, 145, 53, 255)
#define LIGHTER_GREEN				C2D_Color32(23, 121, 53, 255)
#define NEW_GREEN					C2D_Color32(3,192,107, 255)

typedef u32 Color;

#endif