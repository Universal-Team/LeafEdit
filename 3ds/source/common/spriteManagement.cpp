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

#include "common.hpp"
#include "Sav.hpp"
#include "spriteManagement.hpp"

#include <3ds.h>

// Include all SpriteSheets which are used for the editor.
extern C2D_SpriteSheet Acres;
extern C2D_SpriteSheet Items;
extern C2D_SpriteSheet Players;
extern C2D_SpriteSheet Villager;
extern C2D_SpriteSheet Villager2;

extern std::shared_ptr<Sav> save;

// Draws the Hair sprite from a Player.
void SpriteManagement::DrawHair(u8 hair, int x, int y, float ScaleX, float ScaleY) {
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

// Draws the Face sprite from a Player.
void SpriteManagement::DrawFace(u8 face, int x, int y, float ScaleX, float ScaleY) {
	switch(save->getType()) {
		case SaveType::WW:
			Gui::DrawSprite(Players, 56 + face, x, y, ScaleX, ScaleY); // Hair & Face are the same on AC:WW.
			break;
		case SaveType::NL:
		case SaveType::WA:
			Gui::DrawSprite(Players, 32 + face, x, y, ScaleX, ScaleY); // Face starts at 32.
			break;
		case SaveType::UNUSED:
			break;
	}
}

// Draws the Item Sprite.
void SpriteManagement::DrawItem(u8 itemCategory, int x, int y, float ScaleX, float ScaleY) {
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

// Draws the Acre Sprite.
void SpriteManagement::DrawAcres(u8 acreID, int x, int y, float ScaleX, float ScaleY) {
	switch(save->getType()) {
		case SaveType::WW:
			Gui::DrawSprite(Acres, 219 + acreID, x, y, ScaleX, ScaleY); // Acre starts at 219 here.
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

// AC:WW Villager Index, which point to the correct SpriteSheet ID.
const std::array<int, 150> wwVillagerIndex = {
	0, 1, 2, 3, 8, 9, 10, 11, 22, 23, 24, 25, 26,
	36, 37, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
	52, 53, 54, 65, 66, 67, 68, 69, 70, 81, 82, 83,
	90, 91, 94, 95, 112, 113, 114, 115, 116, 117, 118,
	119, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137,
	145, 146, 147, 148, 149, 156, 157, 158, 159, 160, 161,
	174, 175, 182, 182, 183, 200, 7, 8, 9, 10, 11, 23, 24, 25,
	32, 33, 40, 41, 55, 56, 57, 58, 59, 60, 70, 71, 74, 75,
	84, 85, 86, 93, 94, 95, 96, 97, 98, 106, 107, 108, 109, 110,
	122, 123, 124, 125, 126, 127, 128, 129, 130, 143, 144, 149,
	150, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 182, 183,
	189, 190, 191, 192, 200, 147, 148, 149, 150, 151
};

void SpriteManagement::DrawVillager(u16 villagerID, int x, int y, float ScaleX, float ScaleY) {
	switch(save->getType()) {
		case SaveType::WW:
			// Display empty Villager.
			if (villagerID > 150) {
				Gui::DrawSprite(Villager2, 199, x, y, ScaleX, ScaleY); // Display empty Villager.
				break;
			}
			if (villagerID > 77)	Gui::DrawSprite(Villager2, wwVillagerIndex[villagerID], x, y, ScaleX, ScaleY);
			else 					Gui::DrawSprite(Villager, wwVillagerIndex[villagerID], x, y, ScaleX, ScaleY);
			break;
		// Pretty sure AC:NL needs a special handling for the Villagers, cause the SpriteSheet has AC:WA special ones(?) TODO!
		case SaveType::NL:
		case SaveType::WA:
			 // Display empty Villager.
			if (villagerID > 399) {
				Gui::DrawSprite(Villager2, 199, x, y, ScaleX, ScaleY);
				break;
			}
			if (villagerID < 200) {
				Gui::DrawSprite(Villager, villagerID, x, y, ScaleX, ScaleY);
			}
			else {
				Gui::DrawSprite(Villager2, villagerID - 200, x, y, ScaleX, ScaleY);
			}
			break;
		case SaveType::UNUSED:
			break;
	}
}

// Draws the Badge Sprite.
void SpriteManagement::DrawBadge(u8 badge, int x, int y, float ScaleX, float ScaleY) {
	switch(save->getType()) {
		case SaveType::WW:
			break; // Don't exist here.
		case SaveType::NL:
		case SaveType::WA:
			Gui::DrawSprite(Items, 394 + badge, x, y, ScaleX, ScaleY); // Start at 394.
			break;
		case SaveType::UNUSED:
			break;
	}
}