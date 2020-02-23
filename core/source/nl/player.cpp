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

#include "pattern.hpp"
#include "player.hpp"
#include "types.hpp"

#include <string>

#ifdef _3DS
#include "gfx.hpp"
#include "jpeg.h"
#include "utils.hpp"
#include <citro2d.h>
extern C2D_SpriteSheet sprites;
#endif

Player::Player() { }

Player::~Player()
{
	// 3DS only.
#ifdef _3DS
	if (m_HasTPC && m_TPCPic.tex != nullptr) {
		C2DUtils::C2D_ImageDelete(this->m_TPCPic);
		m_TPCPic.tex = nullptr;
		m_TPCPic.subtex = nullptr;
		m_HasTPC = false;
	}

	if (this->m_TPCData != nullptr) {
		delete[] this->m_TPCData;
		this->m_TPCData = nullptr;
	}
#endif
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

	this->hairStyle = Save::Instance()->ReadU8(offset + 0x04);
	this->hairColor = Save::Instance()->ReadU8(offset + 0x05);
	this->face = Save::Instance()->ReadU8(offset + 0x06);
	this->eyeColor = Save::Instance()->ReadU8(offset + 0x07);


	this->Wallet = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6F08));
	this->BankAmount = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6b8c));
	this->IslandMedals = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6B9C));
	this->MeowCoupons = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x8D1C));

	this->TPCText = Save::Instance()->ReadString(offset + 0x6b38, 25);

	#ifdef _3DS
	if (this->Exists()) {
		this->RefreshTPC();
	}
	#endif

	for (int i = 0; i < 16; i++) {
		this->Pockets[i] = std::make_shared<Item>(offset + 0x6BD0 + i * sizeof(Item));
	}

	for (int i = 0; i < 180; i++) {
		this->Dresser[i] = std::make_shared<Item>(offset + 0x92f0 + i * sizeof(Item));
	}

	for (int i = 0; i < 40; i++) {
		this->IslandBox[i] = std::make_shared<Item>(offset + 0x6f10 + i * sizeof(Item));
	}

	for (int i = 0; i < 360; i++) {
		this->Storage[i] = std::make_shared<Item>((index*360)+ 0x07a778 + i * sizeof(Item));
	}

	for (u32 i = 0; i < 10; i++) {
		this->Patterns[i] = std::make_shared<Pattern>(this, i);
	}


	// Badges.
	for (int i = 0; i < 24; i++) {
		this->Badges[i] = Save::Instance()->ReadU8(offset + 0x569C + i);
		this->BadgeValues[i] = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x55DC + i*8));
	}
}

void Player::Write() {
	u32 encryptedInt = 0;
	u32 encryptionData = 0;

	// Always disable reset flag. TODO: Do we want this? Or should we make it a checkbox?
	this->SetHasReset(false);

	Save::Instance()->Write(this->m_offset + 0x55A6, this->PlayerId);
	Save::Instance()->Write(this->m_offset + 0x08, this->PlayerTan);
	Save::Instance()->Write(this->m_offset + 0x55A8, this->Name, 8);
	Save::Instance()->Write(this->m_offset + 0x55BA, this->Gender);
	Save::Instance()->Write(this->m_offset + 0x55BC, this->TownId);
	Save::Instance()->Write(this->m_offset + 0x55BE, this->TownName, 8);

	Save::Instance()->Write(this->m_offset + 0x04, this->hairStyle);
	Save::Instance()->Write(this->m_offset + 0x05, this->hairColor);
	Save::Instance()->Write(this->m_offset + 0x06, this->face);
	Save::Instance()->Write(this->m_offset + 0x07, this->eyeColor);

	// Was for Testing purpose of writing an Item to slot 1 of the pocket.
//	Save::Instance()->Write(this->m_offset + 0x6BD0 + 0 * 4, this->testItem); // Don't write Items for now.

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

	for(int i= 0; i < 24; i++){
		Save::Instance()->Write(this->m_offset + 0x569C + i, this->Badges[i]);
		this->BadgeValues[i].encrypt(encryptedInt, encryptionData);
		Save::Instance()->Write(this->m_offset + 0x55DC + i*8, encryptedInt);
		Save::Instance()->Write(this->m_offset + 0x55DC + i*8 + 4, encryptionData);
	}
}

#ifdef _3DS
u8* Player::RefreshTPC() {

	if (m_HasTPC && m_TPCPic.tex != nullptr) {
		C2DUtils::C2D_ImageDelete(this->m_TPCPic);
		m_TPCPic.tex = nullptr;
		m_TPCPic.subtex = nullptr;
	}

	if (Save::Instance()->ReadU32(this->m_offset + 0x5734) == 1) {
		if (Save::Instance()->ReadU16(this->m_offset + 0x5738) == 0xD8FF) { // 0xFFD8 = JPEG File Marker
			if (this->m_TPCData == nullptr)
			this->m_TPCData = new u8[0x1400];
			Save::Instance()->ReadArray(this->m_TPCData, this->m_offset + 0x5738, 0x1400);
			this->m_TPCPic = LoadPlayerPicture(this->m_TPCData);
			m_HasTPC = true;
		}
	}

	else { //No TPC
		if (this->m_TPCData != nullptr) {
			delete[] this->m_TPCData;
			this->m_TPCData = nullptr;
		}
		this->m_TPCPic = C2D_SpriteSheetGetImage(sprites, sprites_noTPC_idx);
		m_HasTPC = false;
	}

	return this->m_TPCData;
}
#endif

bool Player::Exists() {
	return Save::Instance()->ReadU16(this->m_offset + 0x55A6) != 0;
}

bool Player::HasReset() {
	return Save::Instance()->ReadU8(this->m_offset + 0x570A) & 2;
}

void Player::SetHasReset(bool reset) {
	u8 currentFlags = Save::Instance()->ReadU8(this->m_offset + 0x570A);
	if (reset) {
		Save::Instance()->Write(this->m_offset + 0x570A, static_cast<u8>(currentFlags | 2));
	}
	else {
		Save::Instance()->Write(this->m_offset + 0x570A, static_cast<u8>(currentFlags & ~2));
	}
}