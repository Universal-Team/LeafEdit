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

#include "graphics/gui.hpp"

#include "management/villagerManagement.hpp"

#include "screens/screenCommon.hpp"

#include <nds.h>

void VillagerManagement::DrawVillager(u8 villagerID, int x, int y, bool top) {
	// 144 is the current exception, since we don't have the sprite for "Champ".
	if (villagerID > 149 || villagerID == 144) {
		villagerID = 150; // 150 -> EMPTY.
	}

	Image villagerData = loadImage("/graphics/villagers/" + std::to_string(villagerID) + ".gfx"); // Load sprite.
	drawImage(x, y, villagerData, top, true, 0x20); // Draw sprite.
}