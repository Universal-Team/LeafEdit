/*
MIT License
This file is part of NLTK
Copyright (c) 2018-2019 Slattz, Cuyler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <3ds.h>
#include <string>
#include "core/save/player.h"


Player::Player() { }

Player::~Player()
{
	if (this->Pockets != nullptr) {
		delete[] this->Pockets;
		this->Pockets = nullptr;
	}
}

Player::Player(u32 offset, u32 index) {
	this->m_offset = offset;
	this->m_index = index;

	this->PlayerId = Save::Instance()->ReadU16(offset + 0x55A6);
	this->PlayerTan = Save::Instance()->ReadU16(offset + 0x08);
	this->Name = Save::Instance()->ReadString(offset + 0x55A8, 8);
	this->TownName = Save::Instance()->ReadString(offset + 0x55BE, 8);
	this->Gender = Save::Instance()->ReadU16(offset + 0x55BA);
	this->TownId = Save::Instance()->ReadU16(offset + 0x55BC);

	this->Wallet = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6F08));
	this->BankAmount = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6b8c));
	this->IslandMedals = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6B9C));
	this->MeowCoupons = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x8D1C));

	this->Pockets = new Item[16];

	for (int i = 0; i < 16; i++) {
		this->Pockets[i] = Item(offset + 0x6BD0 + i * 4);
	}
}

void Player::Write() {
	u32 encryptedInt = 0;
	u32 encryptionData = 0;

	Save::Instance()->Write(this->m_offset + 0x55A6, this->PlayerId);
	Save::Instance()->Write(this->m_offset + 0x08, this->PlayerTan);
	Save::Instance()->Write(this->m_offset + 0x55A8, this->Name, 8);
	Save::Instance()->Write(this->m_offset + 0x55BA, this->Gender);
	Save::Instance()->Write(this->m_offset + 0x55BC, this->TownId);
	Save::Instance()->Write(this->m_offset + 0x55BE, this->TownName, 8);

	this->BankAmount.encrypt(encryptedInt, encryptionData);
	Save::Instance()->Write(this->m_offset + 0x6b8c, encryptedInt);
	Save::Instance()->Write(this->m_offset + 0x6b90, encryptionData);

	this->Wallet.encrypt(encryptedInt, encryptionData);
	Save::Instance()->Write(this->m_offset + 0x6F08, encryptedInt);
	Save::Instance()->Write(this->m_offset + 0x6F0C, encryptionData);

	this->IslandMedals.encrypt(encryptedInt, encryptionData);
	Save::Instance()->Write(this->m_offset + 0x6B9C, encryptedInt);
	Save::Instance()->Write(this->m_offset + 0x6BA0, encryptionData);

	this->MeowCoupons.encrypt(encryptedInt, encryptionData);
	Save::Instance()->Write(this->m_offset + 0x8D1C, encryptedInt);
	Save::Instance()->Write(this->m_offset + 0x8D20, encryptionData);
}

bool Player::Exists() {
	return Save::Instance()->ReadU16(this->m_offset + 0x55A6) != 0;
}