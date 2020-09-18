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

/* Grasstype. */
u8 TownWA::grasstype() const {
	return this->townPointer()[0x053481];
}
void TownWA::grasstype(u8 v) {
	SaveUtils::Write<u8>(this->townPointer(), 0x053481, v);
}

/* Town Name. */
std::u16string TownWA::name() const {
	return StringUtils::ReadUTF16String(this->townPointer(), 0x0621BA, 8);
}
void TownWA::name(std::u16string v) {
	StringUtils::WriteUTF16String(this->townPointer(), v, 0x0621BA, 8);
}

/* Town Acre. */
std::unique_ptr<Acre> TownWA::acre(int Acre) const {
	if (Acre > 41) return nullptr; // Acre Index goes out of scope.

	return std::make_unique<AcreWA>(this->data, 0x053484 + Acre *2);
}

/* Town Item. */
std::unique_ptr<Item> TownWA::item(u32 index) const {
	if (index > 5119) return nullptr; // Item Index goes out of scope.

	return std::make_unique<ItemWA>(this->data, 0x0534D8 + index * 4);
}

/* Return if Town exist. */
bool TownWA::exist() const {
	return true; // TODO?
}

/* Turnip prices. */
u32 TownWA::turnipPrices(bool isAM, int day) const {
	if (day > 5) return 0; // Out of scope.

	this->v_turnipPrices[isAM ? day : 6 + day] = EncryptedInt32(SaveUtils::Read<u64>(this->townPointer(), isAM ? 0x06ADE0 + day * 16 : 0x06ADE0 + day * 16 + 8));
	return this->v_turnipPrices[isAM ? day : 6 + day].value;
}
void TownWA::turnipPrices(bool isAM, int day, u32 v) {
	if (day > 5) return; // Out of scope.

	this->v_turnipPrices[isAM ? day : 6 + day].value = v; // Set Value.
	u32 encryptedInt = 0, encryptionData = 0;
	this->v_turnipPrices[isAM ? day : 6 + day].encrypt(encryptedInt, encryptionData);
	SaveUtils::Write<u32>(this->townPointer(), isAM ? 0x06ADE0 + day * 16 : 0x06ADE0 + day * 16 + 8, encryptedInt);
	SaveUtils::Write<u32>(this->townPointer(), isAM ? 0x06ADE0 + day * 16 + 0x4 : 0x06ADE0 + day * 16 + 8 + 0x4, encryptionData);
}

/* Town flag. */
std::unique_ptr<Pattern> TownWA::townflag() const {
	return std::make_unique<PatternWA>(this->data, 0x70F1C);
}

/* Item buried. */
bool TownWA::itemBuried(int index) const {
	if (index > 5119) return false;

	return (this->townPointer()[(0x0534D8 + index * 4) + 3]) == 0x80; // That is basically Item Flag 2.. but I don't want to init the item class there.
}

void TownWA::itemBuried(int index, bool buried) {
	if (index > 5119) return;

	/* Check, if already buried or not. */
	if (this->itemBuried(index) == buried) return; // No need to set again.

	std::unique_ptr<Item> item = this->item(index);

	if (item->itemtype() == ItemType::Empty) return; // Do not allow buried on empty spots.

	/* Write buried state. */
	item->flag2(buried ? 0x80 : 0x0);
}