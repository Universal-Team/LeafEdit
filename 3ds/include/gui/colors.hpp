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

// Default colors i guess.
#define DIM C2D_Color32(0, 0, 0, 190)
#define WHITE C2D_Color32(255, 255, 255, 255)
#define BLACK C2D_Color32(0, 0, 0, 255)

// Green tones.
#define DARKER_GREEN				C2D_Color32(14, 73, 32, 255)
#define LIGHT_GREEN					C2D_Color32(23, 121, 53, 255)
#define LIGHTER_GREEN				C2D_Color32(23, 145, 53, 255)
#define SELECTED_GREEN				C2D_Color32(38, 130, 40, 255)
#define UNSELECTED_GREEN			C2D_Color32(38, 70, 40, 180)

// Blue Tones.
#define DARKER_BLUE					C2D_Color32(0, 0, 205, 255)
#define LIGHT_BLUE					C2D_Color32(70, 130, 180, 255)
#define LIGHTER_BLUE				C2D_Color32(70, 130, 155, 255)
#define SELECTED_BLUE				C2D_Color32(30, 0, 130, 255)
#define UNSELECTED_BLUE				C2D_Color32(30, 0, 70, 180)

// Deep Blue.
#define DARKER_DEEPBLUE				C2D_Color32(14, 32, 73, 255)
#define LIGHT_DEEPBLUE				C2D_Color32(23, 53, 121, 255)
#define LIGHTER_DEEPBLUE			C2D_Color32(23, 53, 145, 255)
#define SELECTED_DEEPBLUE			C2D_Color32(38, 40, 140, 255)
#define UNSELECTED_DEEPBLUE			C2D_Color32(38, 40, 70, 180)

// Deep Red.
#define DARKER_DEEPRED				C2D_Color32(73, 32, 14, 255)
#define LIGHT_DEEPRED				C2D_Color32(121, 53, 23, 255)
#define LIGHTER_DEEPRED				C2D_Color32(145, 53, 23, 255)
#define SELECTED_DEEPRED			C2D_Color32(140, 40, 38, 255)
#define UNSELECTED_DEEPRED			C2D_Color32(70, 40, 38, 180)

extern u32 DARKER_COLOR, LIGHT_COLOR, LIGHTER_COLOR, SELECTED_COLOR, UNSELECTED_COLOR; // C010r Typ3s.
typedef u32 Color;

#endif