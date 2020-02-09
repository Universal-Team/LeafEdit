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

#include "town.hpp"

Town::Town() {
	this->TownName = Save::Instance()->ReadString(0x0621BA, 8); // Works fine.
	this->NativeFruit = Save::Instance()->ReadU8(0x06223A); // Might be wrong?
	this->TimePlayed = Save::Instance()->ReadU16(0x0621b0); // Should be right?

	// *Only* first acre for now. TODO: Do it for every acre (20) -> 256x20 -> 5120 Items.
	this->MapItem = new Item[5120];
	for (int i = 0; i < 5120; i++) {
		this->MapItem[i] = Item(0x0534D8 + i * sizeof(Item));
	}
}

Town::~Town() {
	// Delete MapItem.
	if (this->MapItem != nullptr) {
		delete[] this->MapItem;
		this->MapItem = nullptr;
	}
}

void Town::Write() {
	Save::Instance()->Write(0x0621BA, this->TownName, 8);
	Save::Instance()->Write(0x06223A, this->NativeFruit);
	Save::Instance()->Write(0x0621b0, this->TimePlayed);
}