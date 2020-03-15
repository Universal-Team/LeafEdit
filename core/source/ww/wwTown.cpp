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

	// Only Town Map Acres.
	this->MapAcres[0] = this->FullAcres[7];
	this->MapAcres[1] = this->FullAcres[8];
	this->MapAcres[2] = this->FullAcres[9];
	this->MapAcres[3] = this->FullAcres[10];

	this->MapAcres[4] = this->FullAcres[13];
	this->MapAcres[5] = this->FullAcres[14];
	this->MapAcres[6] = this->FullAcres[15];
	this->MapAcres[7] = this->FullAcres[16];

	this->MapAcres[8] = this->FullAcres[19];
	this->MapAcres[9] = this->FullAcres[20];
	this->MapAcres[10] = this->FullAcres[21];
	this->MapAcres[11] = this->FullAcres[22];

	this->MapAcres[12] = this->FullAcres[25];
	this->MapAcres[13] = this->FullAcres[26];
	this->MapAcres[14] = this->FullAcres[27];
	this->MapAcres[15] = this->FullAcres[28];
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