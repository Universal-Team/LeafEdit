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

#include "checksum.hpp"
#include "SavWW.hpp"

#include <cstring>

/* Get Player data. */
std::unique_ptr<Player> SavWW::player(int player, int index) const {
	if (player > 3 || index > 3) return nullptr; // Player goes out of scope.

	switch (this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return std::make_unique<PlayerWW>(this->dataPointer, 0x000C + (player * 0x228C), this->region, index);

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return std::make_unique<PlayerWW>(this->dataPointer, 0x000C + (player * 0x1D10), this->region, index);

		case WWRegion::KOR_REV1:
			return std::make_unique<PlayerWW>(this->dataPointer, 0x0014 + (player * 0x249C), this->region, index);

		case WWRegion::UNKNOWN:
			return nullptr;
	}

	return nullptr; // Unknwon region.
}

/* Get Villager data. */
std::unique_ptr<Villager> SavWW::villager(int villager) const {
	if (villager > 7) return nullptr; // Villager goes out of scope.

	switch (this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return std::make_unique<VillagerWW>(this->dataPointer, 0x8A3C + (villager * 0x700), this->region);

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return std::make_unique<VillagerWW>(this->dataPointer, 0x744C + (villager * 0x5C0), this->region);

		case WWRegion::KOR_REV1:
			return std::make_unique<VillagerWW>(this->dataPointer, 0x928C + (villager * 0x7EC), this->region);

		case WWRegion::UNKNOWN:
			return nullptr;
	}

	return nullptr; // Unknown region.
}

/* Get Town data. */
std::unique_ptr<Town> SavWW::town() const {
	return std::make_unique<TownWW>(this->dataPointer, this->region);
}

/* Get Island data. (Does not exist.) */
std::unique_ptr<Island> SavWW::island() const {
	return nullptr;
}

std::unique_ptr<Shop> SavWW::shop() const {
	return std::make_unique<ShopWW>(this->dataPointer, 0, this->region);
}

/* Last call before writing to file. Update Checksum. */
void SavWW::Finish(void) {
	switch (this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			Checksum::UpdateWWChecksum(this->region, this->savePointer(), reinterpret_cast<u16*>(this->savePointer()), 0x15FE0 / sizeof(u16));
			memcpy(this->savePointer() + 0x15FE0, this->savePointer(), 0x15FE0); // Copy SaveData to the second save copy.
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			Checksum::UpdateWWChecksum(this->region, this->savePointer(), reinterpret_cast<u16*>(this->savePointer()), 0x12224 / sizeof(u16));
			memcpy(this->savePointer() + 0x12224, this->savePointer(), 0x12224); // Copy SaveData to the second save copy.
			break;

		case WWRegion::KOR_REV1:
			Checksum::UpdateWWChecksum(this->region, this->savePointer(), reinterpret_cast<u16*>(this->savePointer()), 0x173FC / sizeof(u16));
			memcpy(this->savePointer() + 0x173FC, this->savePointer(), 0x173FC); // Copy SaveData to the second save copy.
			break;
			
		case WWRegion::UNKNOWN:
			break;
	}
}