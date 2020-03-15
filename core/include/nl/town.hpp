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

#pragma once

#ifndef TOWN_HPP
#define TOWN_HPP

#include "buildingArray.hpp"
#include "item.hpp"
#include "save.hpp"
#include "types.hpp"

#include <string>

class BuildingArray;
class Item;
class Town {
public:
	Town(void);
	~Town(void);
	void Write(void);

	std::u16string TownName; // 0x0621BA [8]
	u8 NativeFruit; // 0x06223A
	u16 TimePlayed; // 0x06223E

	std::shared_ptr<Item> MapItem[5120];
	u8 FullAcres[42];
	std::shared_ptr<BuildingArray> townBuildings[58];
private:
	u32 m_offset; // Needed?
};

#endif