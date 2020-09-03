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
#include "Player.hpp"
#include "playerManagement.hpp"
#include "Sav.hpp"
#include "spriteManagement.hpp"

#include <3ds.h>

extern std::shared_ptr<Sav> save;

/*
	Hair RGB Colors [NL / WA].

	Dark Brown -> 89, 58, 56.
	Light Brown -> 147, 89, 41.
	Orange -> 239, 87, 46.
	Light Blue -> 65, 166, 220.
	Gold -> 255, 231, 121.
	Light Green -> 139, 207, 98.
	Pink -> 238, 121, 139.
	White -> 255, 255, 255.
	Black -> 0, 0, 0.
	Auburn -> 65, 6, 1.
	Red -> 187, 12, 7.
	Dark Blue -> 0, 20, 73.
	Blonde -> 222, 167, 15.
	Dark Green -> 1, 90, 34.
	Light Purple -> 173, 117, 188.
	Ash Brown -> 122, 121, 90.
*/

/*
	Hair RGB Colors [WW].

	Dark Brown -> 128, 70, 27
	Light Brown -> 210, 105, 30
	Orange -> 255, 69, 0
	Blue -> 0, 191, 255
	Yellow -> 255, 216, 0
	Green -> 173, 255, 47
	Pink -> 255, 20, 147
	White -> 220, 220, 220
*/

/* Get Hair & Eye color. */
u32 PlayerManagement::getHairColor(u8 hairColor, SaveType save) {
	switch(save) {
		case SaveType::WW:
			switch(hairColor) {
				case 0:
					return C2D_Color32(128, 70, 27, 255);
				case 1:
					return C2D_Color32(210, 105, 30, 255);
				case 2:
					return C2D_Color32(255, 69, 0, 255);
				case 3:
					return C2D_Color32(0, 191, 255, 255);
				case 4:
					return C2D_Color32(255, 216, 0, 255);
				case 5:
					return C2D_Color32(173, 255, 47, 255);
				case 6:
					return C2D_Color32(255, 20, 147, 255);
				case 7:
					return C2D_Color32(220, 220, 220, 255);
				default:
					return C2D_Color32(0, 0, 0, 0);
			}

		case SaveType::NL:
		case SaveType::WA:
			switch(hairColor) {
				case 0:
					return C2D_Color32(89, 58, 56, 255);
				case 1:
					return C2D_Color32(147, 89, 41, 255);
				case 2:
					return C2D_Color32(239, 87, 46, 255);
				case 3:
					return C2D_Color32(65, 166, 220, 255);
				case 4:
					return C2D_Color32(255, 231, 121, 255);
				case 5:
					return C2D_Color32(139, 207, 98, 255);
				case 6:
					return C2D_Color32(238, 121, 139, 255);
				case 7:
					return C2D_Color32(255, 255, 255, 255);
				case 8:
					return C2D_Color32(0, 0, 0, 255);
				case 9:
					return C2D_Color32(65, 6, 1, 255);
				case 10:
					return C2D_Color32(187, 12, 7, 255);
				case 11:
					return C2D_Color32(0, 20, 73, 255);
				case 12:
					return C2D_Color32(222, 167, 15, 255);
				case 13:
					return C2D_Color32(1, 90, 34, 255);
				case 14:
					return C2D_Color32(173, 117, 188, 255);
				default:
					return C2D_Color32(0, 0, 0, 0);
			}

		case SaveType::UNUSED:
			return C2D_Color32(0, 0, 0, 0);
	}

	return C2D_Color32(0, 0, 0, 0);
}

/*
	Eye RGB Colors [NL / WA].

	0 -> 50, 54, 39.
	1 -> 205, 114, 70.
	2 -> 91, 151, 115.
	3 -> 109, 134, 128.
	4 -> 96, 128, 196.
	5 -> 63, 136, 189.
*/

/* Only available in New Leaf / Welcome Amiibo. */
u32 PlayerManagement::getEyeColor(u8 eyeColor) {
	switch(eyeColor) {
		case 0:
			return C2D_Color32(50, 54, 39, 255);
		case 1:
			return C2D_Color32(205, 114, 70, 255);
		case 2:
			return C2D_Color32(91, 151, 115, 255);
		case 3:
			return C2D_Color32(109, 134, 128, 255);
		case 4:
			return C2D_Color32(96, 128, 196, 255);
		case 5:
			return C2D_Color32(63, 136, 189, 255);
	}

	return C2D_Color32(63, 136, 189, 255); // Actually no real color. [6/7]
}