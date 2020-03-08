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

#ifndef WWVILLAGER_HPP
#define WWVILLAGER_HPP

#include "types.hpp"
#include "wwsave.hpp"

#include <string>

class WWVillager {
public:
	WWVillager(void);
	~WWVillager(void);
	WWVillager(const u32 offset, const u32 index);

	enum Personality : u8 {
		Lazy, Jock, Cranky, Normal, Peppy, Snooty, NotSet
	};

	#pragma pack(push, 1)
	struct Villager_s {
		u8 Unknown[0x6CA]; // 0 - 0x6C9.
		WWVillager::Personality personality; // 0x6CA
		u8 ID; // 0x6CB.
		u8 Unknown2[0x34]; // Until 0x700.
	};

	#pragma pack(pop)
	u32 GetOffset(void) const;
	u32 GetIndex(void) const;
	// ID.
	u8 GetId(void) const;
	void SetId(const u8);
	u8 GetPersonality() const;
	void SetPersonality(const u8);
	void Write(void);

private:
	Villager_s* m_villagerData;

	u32 m_offset;
	u32 m_index;
};

#endif