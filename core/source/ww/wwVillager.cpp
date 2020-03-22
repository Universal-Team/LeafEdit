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

WWVillager::WWVillager(const u32 offset, const u32 index) {
	this->m_offset = offset;
	this->m_index = index;

	// Read specific stuff.
	this->ID = WWSave::Instance()->ReadU8(offset + 0x6CB);
	this->personality = WWSave::Instance()->ReadU8(offset + 0x6CA);
	this->CatchPhrase = WWSave::Instance()->ReadString(offset + 0x6DE, 10, false);
	this->HouseCoordinates = WWSave::Instance()->ReadInt(offset + 0x6E8); // Is that correct?

	// Read Items.
	this->Song = std::make_shared<WWItem>(offset + 0x6D0); // Check this.
	this->Shirt = std::make_shared<WWItem>(offset + 0x6EC);
	this->Wallpaper = std::make_shared<WWItem>(offset + 0x6CC);
	this->Carpet = std::make_shared<WWItem>(offset + 0x6CE);

	for (int i = 0; i < 10; i++) {
		this->Furniture[i] = std::make_shared<WWItem>(offset + 0x6AC + i * WWITEM_SIZE);
	}
}

WWVillager::~WWVillager() { }

void WWVillager::Write() {
	// Write specific stuff.
	WWSave::Instance()->Write(this->m_offset + 0x6CB, this->ID);
	WWSave::Instance()->Write(this->m_offset + 0x6CA, this->personality);
//	WWSave::Instance()->Write(this->m_offset + 0x6DE, this->CatchPhrase, 10, false); // Overwrites House Coordinates(?) TODO: Figure out.

	// Write Items.
	this->Song->Write();
	this->Shirt->Write();
	this->Wallpaper->Write();
	this->Carpet->Write();
	// Write Furniture's.
	for (int i = 0; i < 10; i++) {
		this->Furniture[i]->Write();
	}
}