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
#include "SavNL.hpp"

/* Get Player data. */
std::unique_ptr<Player> SavNL::player(int player, int index) const {
	if (player > 3 || index > 3) return nullptr; // Player goes out of scope.

	return std::make_unique<PlayerNL>(this->dataPointer, 0xA0 + (player * 0x9F10), index);
}

/* Get Villager data. */
std::unique_ptr<Villager> SavNL::villager(int villager) const {
	if (villager > 9) return nullptr; // Villager goes out of scope.

	return std::make_unique<VillagerNL>(this->dataPointer, 0x027d10 + (villager * 0x24f8));
}

/* Get Town data. */
std::unique_ptr<Town> SavNL::town() const {
	return std::make_unique<TownNL>(this->dataPointer);
}

/* Get Island data. */
std::unique_ptr<Island> SavNL::island() const {
	return std::make_unique<IslandNL>(this->dataPointer);
}

std::unique_ptr<Shop> SavNL::shop() const {
	return std::make_unique<ShopNL>(this->dataPointer, 0);
}

/* Last call before writing to file. Update Checksum. */
void SavNL::Finish(void) {
	Checksum::FixNLCRC32s(this->savePointer());
}