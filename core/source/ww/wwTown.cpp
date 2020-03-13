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

#include "wwTown.hpp"
#include "wwsave.hpp"

WWTown::WWTown() {
	this->Name = WWSave::Instance()->ReadString(0x0004, 7, false);
	this->Debt = WWSave::Instance()->ReadU32(0xFAE8);
	// Read Town Map Items.
	for (int i = 0; i < 4096; i++) {
		this->MapItems[i] = std::make_shared<WWItem>(0xC354 + i * WWITEM_SIZE);
	}
	// Read full Acres.
	for (int i = 0; i < 36; i++) {
		this->FullAcres[i] = WWSave::Instance()->ReadU8(0xC330 + i * WWACRE_SIZE);
	}
}

WWTown::~WWTown() { }

void WWTown::Write() {
	WWSave::Instance()->Write(0x0004, this->Name, 7, false);
	WWSave::Instance()->Write(0xFAE8, this->Debt);
	// Write Town Map Items.
	for (int i = 0; i < 4096; i++) {
		this->MapItems[i]->Write();
	}
	// Write full Acres.
	for (int i = 0; i < 36; i++) {
		WWSave::Instance()->Write(0xC330 + i * WWACRE_SIZE, this->FullAcres[i]);
	}
}