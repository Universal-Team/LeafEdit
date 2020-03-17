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

#include "wwPlayer.hpp"
#include "wwsave.hpp"

WWPlayer::WWPlayer() { }

WWPlayer::~WWPlayer() { }

WWPlayer::WWPlayer(u32 offset, u32 index) {
	this->m_offset = offset;
	this->m_index = index;

	this->Gender = WWSave::Instance()->ReadU8(offset + 0x228A); // Gender.
	this->Bells = WWSave::Instance()->ReadU32(offset + 0x1B40); // Bells from the Wallet.
	this->BankAmount = WWSave::Instance()->ReadU32(offset + 0x21E4); // Bank Amount.
	this->Name = WWSave::Instance()->ReadString(offset + 0x2282, 7, false);
	this->NookPoints = WWSave::Instance()->ReadU16(offset + 0x2208); // Is correct.

	u8 testType = WWSave::Instance()->ReadU8(offset + 0x223C);
	this->HairType = testType >> 4;
	this->FaceType = testType & 0xF;

	u8 testType2 = WWSave::Instance()->ReadU8(offset + 0x223D);
	this->HairColor = testType2 & 0xF;
	this->TAN = testType2 >> 4;

	for (int i = 0; i < 15; i++) {
		this->Pocket[i] = std::make_shared<WWItem>(offset + 0x1B22 + i * WWITEM_SIZE);
	}
	for (int i = 0; i < 90; i++) {
		this->Dresser[i] = std::make_shared<WWItem>(0x15430 + 0xB4 * m_index + i * WWITEM_SIZE);
	}
	// 8 Pattern exist for a Player.
	for (int i = 0; i < 8; i++) {
		this->Pattern[i] = std::make_shared<WWPattern>(this, i);
	}
}

void WWPlayer::Write() {
	WWSave::Instance()->Write(this->m_offset + 0x1B40, this->Bells);
	WWSave::Instance()->Write(this->m_offset + 0x21E4, this->BankAmount);
	WWSave::Instance()->Write(this->m_offset + 0x2282, this->Name, 7, false);
	WWSave::Instance()->Write(this->m_offset + 0x228A, this->Gender);

	// Write HairColor & TAN.
	u8 testType = (this->HairColor & 0xF) + (this->TAN >> 4);
	WWSave::Instance()->Write(this->m_offset + 0x223D, testType);
	// Write HairType & FaceType.
	u8 testType2 = (this->FaceType & 0xF) + (this->HairType >> 4);
	WWSave::Instance()->Write(this->m_offset + 0x223C, testType2);
	
	// Write Pocket Items.
	for (int i = 0; i < 15; i++) {
		this->Pocket[i]->Write();
	}
	// Write Dresser Items.
	for (int i = 0; i < 90; i++) {
		this->Dresser[i]->Write();
	}
}

bool WWPlayer::Exists() {
	return WWSave::Instance()->ReadU16(this->m_offset + 0x2280) != 0;
}