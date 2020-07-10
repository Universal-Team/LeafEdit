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

#include "saveUtils.hpp"
#include "stringUtils.hpp"
#include "TownWA.hpp"

// Grasstype.
u8 TownWA::grasstype() {
	return townPointer()[0x053481];
}
void TownWA::grasstype(u8 v) {
	townPointer()[0x053481] = v;
}

// Town Name.
std::u16string TownWA::name() {
	return StringUtils::ReadNLString(townPointer(), 0x0621BA, 8, u'\uFFFF');
}
void TownWA::name(std::u16string v) {
	StringUtils::WriteNLString(townPointer(), v, 0x0621BA, 8);
}

// Town Acre.
std::unique_ptr<Acre> TownWA::acre(int Acre) {
	if (Acre > 41)	return nullptr; // Acre Index goes out of scope.
	return std::make_unique<AcreWA>(data, 0x053484 + Acre *2);
}

// Town Item.
std::unique_ptr<Item> TownWA::item(u32 index) {
	if (index > 5119)	return nullptr; // Item Index goes out of scope.
	return std::make_unique<ItemWA>(data, 0x0534D8 + index * 4);
}

// Return if Town exist.
bool TownWA::exist() {
	return true; // TODO?
}

// Turnip prices.
u32 TownWA::turnipPrices(bool isAM, int day) {
	if (day > 5) return 0; // Out of scope.
	this->v_turnipPrices[isAM ? day : 6 + day] = EncryptedInt32(SaveUtils::Read<u64>(townPointer(), isAM ? 0x06ade0 + day * 16 : 0x06ade0 + day * 16 + 8));
	return this->v_turnipPrices[isAM ? day : 6 + day].value;
}
void TownWA::turnipPrices(bool isAM, int day, u32 v) {
	if (day > 5) return; // Out of scope.
	this->v_turnipPrices[isAM ? day : 6 + day].value = v; // Set Value.
	u32 encryptedInt = 0, encryptionData = 0;
	this->v_turnipPrices[isAM ? day : 6 + day].encrypt(encryptedInt, encryptionData);
	SaveUtils::Write<u32>(townPointer(), isAM ? 0x06ade0 + day * 16 : 0x06ade0 + day * 16 + 8, encryptedInt);
	SaveUtils::Write<u32>(townPointer(), isAM ? 0x06ade0 + day * 16 + 0x4 : 0x06ade0 + day * 16 + 8 + 0x4, encryptionData);
}