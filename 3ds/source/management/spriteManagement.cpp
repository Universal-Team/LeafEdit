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
#include "Sav.hpp"
#include "spriteManagement.hpp"

#include <3ds.h>

// Include all SpriteSheets which are used for the editor.
extern C2D_SpriteSheet Acres, Items, Players, Villager, Villager2;

extern std::shared_ptr<Sav> save;

// Draws the Hair sprite from a Player.
void SpriteManagement::DrawHair(u8 hair, int x, int y, float ScaleX, float ScaleY) {
	if (save != nullptr) {
		switch(save->getType()) {
			case SaveType::WW:
				Gui::DrawSprite(Players, 56 + hair, x, y, ScaleX, ScaleY); // Hair & Face are the same on AC:WW.
				break;
			case SaveType::NL:
			case SaveType::WA:
				Gui::DrawSprite(Players, hair, x, y, ScaleX, ScaleY); // Hair starts at 0.
				break;
			case SaveType::UNUSED:
				break;
		}
	}
}

// Draws the Face sprite from a Player.
void SpriteManagement::DrawFace(u16 Gender, u8 face, int x, int y, float ScaleX, float ScaleY) {
	if (save != nullptr) {
		switch(save->getType()) {
			case SaveType::WW:
				Gui::DrawSprite(Players, 56 + face, x, y, ScaleX, ScaleY); // Hair & Face are the same on AC:WW.
				break;
			case SaveType::NL:
			case SaveType::WA:
				// Face starts at 32.
				if (Gender == 1) {
					Gui::DrawSprite(Players, 32 + face +12, x, y, ScaleX, ScaleY);
				} else {
					Gui::DrawSprite(Players, 32 + face, x, y, ScaleX, ScaleY);
				}
				break;
			case SaveType::UNUSED:
				break;
		}
	}
}

// Draws the Item Sprite.
void SpriteManagement::DrawItem(u8 itemCategory, int x, int y, float ScaleX, float ScaleY) {
	if (save != nullptr) {
		switch(save->getType()) {
			case SaveType::WW:
				break; // No Sprites for AC:WW implemented.
			case SaveType::NL:
			case SaveType::WA:
				Gui::DrawSprite(Items, itemCategory, x, y, ScaleX, ScaleY);
				break;
			case SaveType::UNUSED:
				break;
		}
	}
}

// Draws the Acre Sprite.
void SpriteManagement::DrawAcres(u8 acreID, int x, int y, float ScaleX, float ScaleY) {
	if (save != nullptr) {
		switch(save->getType()) {
			case SaveType::WW:
				Gui::DrawSprite(Acres, 219 + acreID, x, y, ScaleX, ScaleY); // Acre starts at 219 here.
				break;
			case SaveType::NL:
				// AC:NL has a bit different exception there.
				if (acreID < 155)	Gui::DrawSprite(Acres, acreID, x, y, ScaleX, ScaleY); // Normal handling.
				else if (acreID > 154 && acreID < 165)	Gui::DrawSprite(Acres, acreID + 1, x, y, ScaleX, ScaleY); // +1 cause one skipped Acre.
				else if (acreID > 164)	Gui::DrawSprite(Acres, acreID + 2, x, y, ScaleX, ScaleY); // +2 cause two skipped Acre.
				break;
			case SaveType::WA:
				Gui::DrawSprite(Acres, acreID, x, y, ScaleX, ScaleY);
				break;
			case SaveType::UNUSED:
				break;
		}
	}
}

// AC:WW Villager Index, which point to the correct SpriteSheet ID.
const std::array<int, 150> wwVillagerIndex = {
	0, 1, 2, 3, 7, 8, 9, 10, 21, 22, 23, 24, 25,
	35, 36, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
	64, 65, 66, 67, 68, 69, 80, 81, 82, 89, 90, 93, 94,
	111, 112, 113, 114, 115, 116, 117, 118, 127, 128, 129,
	130, 131, 132, 133, 134, 135, 136, 144, 145, 146, 147, 148,
	155, 156, 157, 158, 159, 160, 161, 162, 173, 174, 180, 181, 182,
	199,
	// Second SpriteSheet -- 78
	6, 7, 8, 9, 10, 22, 23, 24, 31, 32, 39, 40, 54, 55, 56, 57, 58, 59,
	69, 70, 73, 74, 83, 84, 85, 92, 93, 94, 95, 96, 97, 105, 106, 107, 108, 109,
	121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 142, 143, 148, 149,
	161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 181, 182, 188, 189, 190, 191,
	200, 46, 47, 48, 49, 50
};

std::array<int, 333> nlVillagerIndex = {
	0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16,
	18, 19, 20, 21, 22, 23, 24, 25, 28, 30, 31, 32, 33, 35,
	36, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
	51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 64, 65,
	66, 67, 68, 69, 70, 73, 75, 76, 77, 78, 80, 81, 82, 83,
	84, 86, 87, 88, 89, 90, 92, 93, 94, 96, 97, 98, 99,
	100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 111,
	112, 113, 114, 115, 116, 117, 118, 119, 122, 123, 124,
	125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
	136, 137, 139, 141, 142, 143, 144, 145, 146, 147, 148,
	150, 152, 153, 155, 156, 157, 158, 159, 160, 161, 162,
	164, 166, 167, 169, 170, 171, 172, 173, 174, 176, 177,
	178, 179, 180, 181, 182, 185, 186, 187, 188, 190, 191,
	192, 193, 194, 195, 196, 197, 199,
	// Second SpriteSheet -- 168
	0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18,
	19, 22, 23, 24, 25, 27, 28, 29, 30, 31, 32, 34, 35, 36, 37,
	38, 39, 40, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
	55, 56, 57, 58, 59, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
	71, 73, 74, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90,
	91, 92, 93, 94, 95, 96, 97, 98, 100, 101, 102, 103, 104, 105,
	106, 107, 108, 109, 110, 111, 114, 115, 116, 117, 118, 119,
	121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 133, 134,
	136, 137, 138, 139, 140, 142, 143, 146, 147, 148, 149, 150,
	153, 154, 155, 156, 157, 158, 159, 161, 162, 163, 164, 165,
	166, 167, 168, 169, 170, 171, 172, 175, 176, 177, 178, 181,
	182, 184, 185, 186, 187, 188, 189, 190, 191, 193, 194, 196,
	197
};

void SpriteManagement::DrawVillager(u16 villagerID, int x, int y, float ScaleX, float ScaleY) {
	if (save != nullptr) {
		switch(save->getType()) {
			case SaveType::WW:
				// Display empty Villager.
				if (villagerID > 150) {
					Gui::DrawSprite(Villager2, 199, x, y, ScaleX, ScaleY); // Display empty Villager.
					break;
				}

				if (villagerID < 78) {
					Gui::DrawSprite(Villager, wwVillagerIndex[villagerID], x, y, ScaleX, ScaleY);
				} else {
					Gui::DrawSprite(Villager2, wwVillagerIndex[villagerID], x, y, ScaleX, ScaleY);
				}

				break;
			// Pretty sure AC:NL needs a special handling for the Villagers, cause the SpriteSheet has AC:WA special ones(?) TODO!
			case SaveType::NL:
				if (villagerID > 333) {
					Gui::DrawSprite(Villager2, 199, x, y, ScaleX, ScaleY);
					break;
				}

				if (villagerID < 168) {
					Gui::DrawSprite(Villager, nlVillagerIndex[villagerID], x, y, ScaleX, ScaleY);
				} else {
					Gui::DrawSprite(Villager2, nlVillagerIndex[villagerID], x, y, ScaleX, ScaleY);
				}

				break;
			case SaveType::WA:
				 // Display empty Villager.
				if (villagerID > 399) {
					Gui::DrawSprite(Villager2, 199, x, y, ScaleX, ScaleY);
					break;
				}

				if (villagerID < 200) {
					Gui::DrawSprite(Villager, villagerID, x, y, ScaleX, ScaleY);
				} else {
					Gui::DrawSprite(Villager2, villagerID - 200, x, y, ScaleX, ScaleY);
				}

				break;
			case SaveType::UNUSED:
				break;
		}
	}
}

// Draws the Badge Sprite.
void SpriteManagement::DrawBadge(u8 badgeGroup, u8 badge, int x, int y, float ScaleX, float ScaleY) {
	if (save != nullptr) {
		switch(save->getType()) {
			case SaveType::WW:
				break; // Don't exist here.
			case SaveType::NL:
			case SaveType::WA:
				if (badge != 0) {
					Gui::DrawSprite(Items, 394 + 3*badgeGroup + badge - 1, x, y, ScaleX, ScaleY);
				} else {
					Gui::DrawSprite(Items, 394 + 72, x, y, ScaleX, ScaleY); // 72.. the blank one.
				}
				break;
			case SaveType::UNUSED:
				break;
		}	
	}
}