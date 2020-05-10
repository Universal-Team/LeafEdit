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

#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP

#include "common.hpp"
#include "Item.hpp"

#include <3ds.h>
#include <string>

namespace ItemManager {
	u32 getColor(ItemType item);
	void loadColors();
	// Getter's.
	int getIndex(const u16 &v);
	int getIndexString(const int &current, const std::string &v);

	// All Colors.
	extern u32 Pattern, Building, MoneyRock, Furniture, Gyroid, Clothes, Song, Paper, Trash, Shell,
	Fruit, Turnip, Catchable, Item, WallpaperCarpet, Fossil, Tool, Tree, Weed, Flower, Rock,
	Money, ParchedFlower, WateredFlower, WiltedFlower, Occupied, Invalid;
}

#endif