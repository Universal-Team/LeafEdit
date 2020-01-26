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

#include "wwVillager.hpp"

WWVillager::WWVillager() { }

WWVillager::~WWVillager() {
	// Checking if a pointer is null is not necessary for delete. It will verify the memory is valid before attempting to delete it.
	if (this->m_villagerData != nullptr) {
		delete this->m_villagerData;
	}
}

WWVillager::WWVillager(const u32 offset, const u32 index) : m_villagerData(new Villager_s), m_offset(offset), m_index(index) {
	WWSave::Instance()->ReadArray(reinterpret_cast<u8 *>(this->m_villagerData), offset, sizeof(Villager_s));
}

u32 WWVillager::GetOffset() const {
	return this->m_offset;
}

u32 WWVillager::GetIndex() const {
	return this->m_index;
}

u16 WWVillager::GetId() const {
	return this->m_villagerData->Id;
}

void WWVillager::SetId(const u16 id) {
	this->m_villagerData->Id = id;
}


void WWVillager::Write() {
	WWSave::Instance()->Write(m_offset, reinterpret_cast<u8 *>(this->m_villagerData), sizeof(Villager_s));
}