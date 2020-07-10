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

#include "PlayerWA.hpp"
#include "saveUtils.hpp"
#include "stringUtils.hpp"

#include <cstring>

// Face.
u8 PlayerWA::face() {
	return playerPointer()[0x06];
}
void PlayerWA::face(u8 v) {
	playerPointer()[0x06] = v;
}

// Tan.
u16 PlayerWA::tan() {
	return SaveUtils::Read<u16>(playerPointer(), 0x08);
}
void PlayerWA::tan(u16 v) {
	SaveUtils::Write<u16>(playerPointer(), 0x08, v);
}

// Gender.
u8 PlayerWA::gender() {
	return playerPointer()[0x55BA];
}
void PlayerWA::gender(u8 v) {
	playerPointer()[0x55BA] = v;
}

// HairStyle.
u8 PlayerWA::hairstyle() {
	return playerPointer()[0x04];
}
void PlayerWA::hairstyle(u8 v) {
	playerPointer()[0x04] = v;
}

// HairColor.
u8 PlayerWA::haircolor() {
	return playerPointer()[0x05];
}
void PlayerWA::haircolor(u8 v) {
	playerPointer()[0x05] = v;
}

// EyeColor.
u8 PlayerWA::eyecolor() {
	return playerPointer()[0x07];
}
void PlayerWA::eyecolor(u8 v) {
	playerPointer()[0x07] = v;
}

// Badges.
u8 PlayerWA::badge(int badge) {
	return playerPointer()[0x569C + badge];
}
void PlayerWA::badge(int badge, u8 v) {
	playerPointer()[0x569C + badge] = v;
}

// Player ID.
u16 PlayerWA::playerid() {
	return SaveUtils::Read<u16>(playerPointer(), 0x55A6);
}
void PlayerWA::playerid(u16 v) {
	SaveUtils::Write<u16>(playerPointer(), 0x55A6, v);
}

// Town ID.
u16 PlayerWA::townid() {
	return SaveUtils::Read<u16>(playerPointer(), 0x55BC);
}
void PlayerWA::townid(u16 v) {
	SaveUtils::Write<u16>(playerPointer(), 0x55BC, v);
}

// Town Name.
std::u16string PlayerWA::townname() {
	return StringUtils::UTF8toUTF16("?");
}
void PlayerWA::townname(std::u16string v) { }

// Player Exist.
bool PlayerWA::exist() {
	return SaveUtils::Read<u16>(playerPointer(), 0x55A6) != 0;
}

// Player Name.
std::u16string PlayerWA::name() {
	return StringUtils::ReadNLString(playerPointer(), 0x55A8, 8, u'\uFFFF');
}
void PlayerWA::name(std::u16string v) {
	StringUtils::WriteNLString(playerPointer(), v, 0x55A8, 8);
}

// Wallet Amount.
u32 PlayerWA::wallet() {
	this->walletValue = EncryptedInt32(SaveUtils::Read<u64>(playerPointer(), 0x6F08));
	return walletValue.value;
}
void PlayerWA::wallet(u32 v) {
	this->walletValue.value = v; // Set Value.
	u32 encryptedInt = 0, encryptionData = 0;
	this->walletValue.encrypt(encryptedInt, encryptionData);
	SaveUtils::Write<u32>(playerPointer(), 0x6F08, encryptedInt);
	SaveUtils::Write<u32>(playerPointer(), 0x6F0C, encryptionData);
}

// Bank Amount.
u32 PlayerWA::bank() {
	this->bankValue = EncryptedInt32(SaveUtils::Read<u64>(playerPointer(), 0x6B8C));
	return bankValue.value;
}
void PlayerWA::bank(u32 v) {
	this->bankValue.value = v; // Set Value.
	u32 encryptedInt = 0, encryptionData = 0;
	this->walletValue.encrypt(encryptedInt, encryptionData);
	SaveUtils::Write<u32>(playerPointer(), 0x6B8C, encryptedInt);
	SaveUtils::Write<u32>(playerPointer(), 0x6B90, encryptionData);
}

// Island Medals.
u32 PlayerWA::islandmedals() {
	this->islandValue = EncryptedInt32(SaveUtils::Read<u64>(playerPointer(), 0x6B9C));
	return islandValue.value;
}
void PlayerWA::islandmedals(u32 v) {
	this->islandValue.value = v; // Set Value.
	u32 encryptedInt = 0, encryptionData = 0;
	this->islandValue.encrypt(encryptedInt, encryptionData);
	SaveUtils::Write<u32>(playerPointer(), 0x6B9C, encryptedInt);
	SaveUtils::Write<u32>(playerPointer(), 0x6BA0, encryptionData);
}

// Coupons.
u32 PlayerWA::coupons() {
	this->couponValue = EncryptedInt32(SaveUtils::Read<u64>(playerPointer(), 0x8D1C));
	return couponValue.value;
}
void PlayerWA::coupons(u32 v) {
	this->couponValue.value = v; // Set Value.
	u32 encryptedInt = 0, encryptionData = 0;
	this->couponValue.encrypt(encryptedInt, encryptionData);
	SaveUtils::Write<u32>(playerPointer(), 0x8D1C, encryptedInt);
	SaveUtils::Write<u32>(playerPointer(), 0x8D20, encryptionData);
}

// Player Pocket.
std::unique_ptr<Item> PlayerWA::pocket(int slot) {
	if (slot > 15)	return nullptr;
	return std::make_unique<ItemWA>(data, offset + 0x6BD0 + slot * 4);
}

// Player Dresser.
std::unique_ptr<Item> PlayerWA::dresser(int slot) {
	if (slot > 179)	return nullptr;
	return std::make_unique<ItemWA>(data, offset + 0x92F0 + slot * 4);
}

// Player Islandbox.
std::unique_ptr<Item> PlayerWA::islandbox(int slot) {
	if (slot > 39)	return nullptr;
	return std::make_unique<ItemWA>(data, offset + 0x6F10 + slot * 4);
}

// Player Storage.
std::unique_ptr<Item> PlayerWA::storage(int slot) {
	if (slot > 359)	return nullptr;
	return std::make_unique<ItemWA>(data, (Index*360) + 0x07A778 + slot * 4);
}

// Player Pattern.
std::unique_ptr<Pattern> PlayerWA::pattern(int slot) {
	if (slot > 9)	return nullptr;
	return std::make_unique<PatternWA>(data, offset + 0x2C + slot * 0x870);
}

// TPC Image.
u8* PlayerWA::tpcImage() {
	u8 *TPCBuffer = nullptr;
	if (SaveUtils::Read<u32>(data.get(), offset + 0x5734) == 1) {
		if (SaveUtils::Read<u16>(data.get(), offset + 0x5738) == 0xD8FF) { // 0xFFD8 = JPEG File Marker
			TPCBuffer = new u8[0x1400];
			// Put the data to buffer.
			memcpy(TPCBuffer, data.get() + offset + 0x5738, 0x1400);
		}
	}
	return TPCBuffer;
}