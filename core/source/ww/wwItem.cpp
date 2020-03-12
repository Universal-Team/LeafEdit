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

#include "wwItem.hpp"
#include "wwsave.hpp"

#include <fstream>
#include <map>
#include <string>

extern std::map<u16, std::string> g_itemDatabase;

WWItem::WWItem(const u32 offset) {
	m_ID = WWSave::Instance()->ReadU16(offset);
	m_offset = offset;
}

WWItem::~WWItem() { }

void WWItem::Write() {
	WWSave::Instance()->Write(m_offset, m_ID);
}

u16 WWItem::GetItemID() {
	return this->m_ID;
}

std::string WWItem::GetName(void) {
	for (auto const& entry : g_itemDatabase) {
		if (entry.first == this->m_ID) {
			return entry.second;
		}
	}
	return std::string("???");
}