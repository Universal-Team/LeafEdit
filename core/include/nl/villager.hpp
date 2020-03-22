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

#ifndef VILLAGER_HPP
#define VILLAGER_HPP

#include "item.hpp"
#include "save.hpp"
#include "types.hpp"

#include <memory>
#include <string>

class Villager {
public:
	~Villager(void);
	Villager(const u32 offset, const u32 index);

	enum Personality : u8 {
		Lazy, Jock, Cranky, Smug, Normal, Peppy, Snooty, Uchi, NotSet
	};

	u16 ID; // 0 - 1 (2)
	u8 personality; // 2 (1)
	u8 Unknown1[0x246B]; // 3 - 246D
	std::shared_ptr<Item> Shirt; // 246E - 2471
	std::shared_ptr<Item> Song; // 2472 - 2475
	std::shared_ptr<Item> Wallpaper; // 2476 - 2479
	std::shared_ptr<Item> Carpet; // 247A - 247D
	std::shared_ptr<Item> Umbrella; // 247E - 2481
	std::shared_ptr<Item> Furniture[16]; // 2482 - 24C1
	u32 Unknown2; // 24C2 - 24C5
	std::u16string Catchphrase; // Last character is null terminator | 24C6 - 24DB
	u8 Unknown3[8]; // 24DC - 24E3
	u8 status; // Also general flags? | 24E4
	u8 Unknown4[9]; // 24E5 - 24ED
	std::u16string TownName; // Last character is null terminator | 24EE - 24FF
	u8 Unknown5[0x18]; // 2500 - 2517

	void Write(void);

private:
	u32 m_offset;
	u32 m_index;
};

#endif