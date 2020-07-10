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
#include "SavWA.hpp"

// Get Player data.
std::unique_ptr<Player> SavWA::player(int player, int index) {
	if (player > 3 || index > 3)	return nullptr; // Player goes out of scope.
	return std::make_unique<PlayerWA>(dataPointer, 0xA0 + (player * 0xA480), index);
}

// Get Villager data.
std::unique_ptr<Villager> SavWA::villager(int villager) {
	if (villager > 9)	return nullptr; // Villager goes out of scope.
	return std::make_unique<VillagerWA>(dataPointer, 0x0292D0 + (villager * 0x2518));
}

// Get Town data.
std::unique_ptr<Town> SavWA::town() {
	return std::make_unique<TownWA>(dataPointer);
}

// Get Island data.
std::unique_ptr<Island> SavWA::island() {
	return std::make_unique<IslandWA>(dataPointer);
}

// Last call before writing to file. Update Checksum.
void SavWA::Finish(void) {
	Checksum::FixCRC32s(this->savePointer());
}