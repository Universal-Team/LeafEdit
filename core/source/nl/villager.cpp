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

#include "villager.hpp"

Villager::Villager(const u32 offset, const u32 index) {
	this->m_offset = offset;
	this->m_index = index;

	// Read specific stuff.
	this->ID = Save::Instance()->ReadU16(offset + 0x0);
	this->personality = Save::Instance()->ReadU8(offset + 0x2);
	this->Catchphrase = Save::Instance()->ReadString(offset + 0x24C6, 11);
	this->status = Save::Instance()->ReadU8(offset + 24E4);
	this->TownName = Save::Instance()->ReadString(offset + 0x24EE, 9);

	// Read Items.
	this->Shirt = std::make_shared<Item>(offset + 0x246E);
	this->Song = std::make_shared<Item>(offset + 0x2472);
	this->Wallpaper = std::make_shared<Item>(offset + 0x2476);
	this->Carpet = std::make_shared<Item>(offset + 0x247A);
	this->Umbrella = std::make_shared<Item>(offset + 0x247E);
	// Read Furniture's.
	for (int i = 0; i < 16; i++) {
		this->Furniture[i] = std::make_shared<Item>(offset + 0x2482 + i * ITEM_SIZE);
	}
}

Villager::~Villager() { }

void Villager::Write() {
	// Write specific stuff.
	Save::Instance()->Write(this->m_offset + 0x0, this->ID);
	Save::Instance()->Write(this->m_offset + 0x2, this->personality);
	Save::Instance()->Write(this->m_offset + 0x24C6, this->Catchphrase, 11);
	Save::Instance()->Write(this->m_offset + 24E4, this->status);
	Save::Instance()->Write(this->m_offset + 0x24C6, this->TownName, 9);

	// Write Items.
	this->Shirt->Write();
	this->Song->Write();
	this->Wallpaper->Write();
	this->Carpet->Write();
	this->Umbrella->Write();
	// Write Furniture's.
	for (int i = 0; i < 16; i++) {
		this->Furniture[i]->Write();
	}
}