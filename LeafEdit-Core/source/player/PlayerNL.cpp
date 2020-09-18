/*
*   This file is part of LeafEdit-Core
*   Copyright (C) 2020 Universal-Team
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

#include "PlayerNL.hpp"
#include "saveUtils.hpp"
#include "stringUtils.hpp"

#include <cstring>

/* Face. */
u8 PlayerNL::face() const {
	return this->playerPointer()[0x06];
}
void PlayerNL::face(u8 v) {
	SaveUtils::Write<u8>(this->playerPointer(), 0x06, v);
}

/* Tan. */
u16 PlayerNL::tan() const {
	return SaveUtils::Read<u16>(this->playerPointer(), 0x08);
}
void PlayerNL::tan(u16 v) {
	SaveUtils::Write<u16>(this->playerPointer(), 0x08, v);
}

/* Gender. */
u8 PlayerNL::gender() const {
	return this->playerPointer()[0x55BA];
}
void PlayerNL::gender(u8 v) {
	SaveUtils::Write<u8>(this->playerPointer(), 0x55BA, v);
}

/* HairStyle. */
u8 PlayerNL::hairstyle() const {
	return this->playerPointer()[0x04];
}
void PlayerNL::hairstyle(u8 v) {
	SaveUtils::Write<u8>(this->playerPointer(), 0x04, v);
}

/* HairColor. */
u8 PlayerNL::haircolor() const {
	return this->playerPointer()[0x05];
}
void PlayerNL::haircolor(u8 v) {
	SaveUtils::Write<u8>(this->playerPointer(), 0x05, v);
}

/* EyeColor. */
u8 PlayerNL::eyecolor() const {
	return this->playerPointer()[0x07];
}
void PlayerNL::eyecolor(u8 v) {
	SaveUtils::Write<u8>(this->playerPointer(), 0x07, v);
}

/* Badges. */
u8 PlayerNL::badge(int badge) const {
	return this->playerPointer()[0x569C + badge];
}
void PlayerNL::badge(int badge, u8 v) {
	SaveUtils::Write<u8>(this->playerPointer(), 0x569C + badge, v);
}

/* Player ID. */
u16 PlayerNL::playerid() const {
	return SaveUtils::Read<u16>(this->playerPointer(), 0x55A6);
}
void PlayerNL::playerid(u16 v) {
	SaveUtils::Write<u16>(this->playerPointer(), 0x55A6, v);
}

/* Town ID. */
u16 PlayerNL::townid() const {
	return SaveUtils::Read<u16>(this->playerPointer(), 0x55BC);
}
void PlayerNL::townid(u16 v) {
	SaveUtils::Write<u16>(this->playerPointer(), 0x55BC, v);
}

/* Town Name. */
std::u16string PlayerNL::townname() const {
	return StringUtils::UTF8toUTF16("?");
}
void PlayerNL::townname(std::u16string v) { }

/* Player Exist. */
bool PlayerNL::exist() const {
	return SaveUtils::Read<u16>(this->playerPointer(), 0x55A6) != 0;
}

/* Player Name. */
std::u16string PlayerNL::name() const {
	return StringUtils::ReadUTF16String(this->playerPointer(), 0x55A8, 8);
}
void PlayerNL::name(std::u16string v) {
	StringUtils::WriteUTF16String(this->playerPointer(), v, 0x55A8, 8);
}

/* Wallet Amount. */
u32 PlayerNL::wallet() const {
	this->walletValue = EncryptedInt32(SaveUtils::Read<u64>(this->playerPointer(), 0x6E38));
	return walletValue.value;
}
void PlayerNL::wallet(u32 v) {
	this->walletValue.value = v; // Set Value.
	u32 encryptedInt = 0, encryptionData = 0;
	this->walletValue.encrypt(encryptedInt, encryptionData);
	SaveUtils::Write<u32>(this->playerPointer(), 0x6E38, encryptedInt);
	SaveUtils::Write<u32>(this->playerPointer(), 0x6E3C, encryptionData);
}

/* Bank Amount. */
u32 PlayerNL::bank() const {
	this->bankValue = EncryptedInt32(SaveUtils::Read<u64>(this->playerPointer(), 0x6B6C));
	return bankValue.value;
}
void PlayerNL::bank(u32 v) {
	this->bankValue.value = v; // Set Value.
	u32 encryptedInt = 0, encryptionData = 0;
	this->bankValue.encrypt(encryptedInt, encryptionData);
	SaveUtils::Write<u32>(this->playerPointer(), 0x6B6C, encryptedInt);
	SaveUtils::Write<u32>(this->playerPointer(), 0x6B70, encryptionData);
}

/* Island Medals. */
u32 PlayerNL::islandmedals() const {
	this->islandValue = EncryptedInt32(SaveUtils::Read<u64>(this->playerPointer(), 0x6B7C));
	return islandValue.value;
}
void PlayerNL::islandmedals(u32 v) {
	this->islandValue.value = v; // Set Value.
	u32 encryptedInt = 0, encryptionData = 0;
	this->islandValue.encrypt(encryptedInt, encryptionData);
	SaveUtils::Write<u32>(this->playerPointer(), 0x6B7C, encryptedInt);
	SaveUtils::Write<u32>(this->playerPointer(), 0x6B80, encryptionData);
}

/* Coupons. */
u32 PlayerNL::coupons() const {
	return 0; // Does not exist here.
}
void PlayerNL::coupons(u32 v) { }

/* Player Letters. */
std::unique_ptr<Letter> PlayerNL::letter(int slot) const {
	if (slot > 9) return nullptr;

	return std::make_unique<LetterNL>(data, offset + 0x6C38 + (0x280 * slot));
}

/* Player Pocket. */
std::unique_ptr<Item> PlayerNL::pocket(int slot) const {
	if (slot > 15) return nullptr;

	return std::make_unique<ItemNL>(data, offset + 0x6BB0 + slot * 4);
}

/* Player Dresser. */
std::unique_ptr<Item> PlayerNL::dresser(int slot) const {
	if (slot > 179) return nullptr;

	return std::make_unique<ItemNL>(data, offset + 0x8E18 + slot * 4);
}

/* Player Islandbox. */
std::unique_ptr<Item> PlayerNL::islandbox(int slot) const {
	if (slot > 39) return nullptr;
	
	return std::make_unique<ItemNL>(data, offset + 0x6E40 + slot * 4);
}

/* Player Pattern. */
std::unique_ptr<Pattern> PlayerNL::pattern(int slot) const {
	if (slot > 9) return nullptr;

	return std::make_unique<PatternNL>(data, offset + 0x2C + slot * 0x870);
}

/* TPC Image. */
u8* PlayerNL::tpcImage() const {
	u8 *TPCBuffer = nullptr;

	if (SaveUtils::Read<u32>(data.get(), offset + 0x5720) == 1) {
		if (SaveUtils::Read<u16>(data.get(), offset + 0x5724) == 0xD8FF) { // 0xFFD8 = JPEG File Marker.
			TPCBuffer = new u8[0x1400];
			
			/* Put the data to buffer. */
			memcpy(TPCBuffer, data.get() + offset + 0x5724, 0x1400);
		}
	}
	
	return TPCBuffer;
}