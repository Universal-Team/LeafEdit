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

	// Load Town Map Items.
	for (int i = 0; i < 5120; i++) {
		this->MapItem[i] = std::make_shared<Item>(0x0534D8 + i * ITEM_SIZE);
	}

	// Read Town Acres.
	for (int i = 0; i < 42; i++) {
		this->FullAcres[i] = Save::Instance()->ReadU8(0x053484 + i * 2);
	}

	// Read Town Buildings.
	for (int i = 0; i < 58; i++) {
		this->townBuildings[i] = std::make_shared<BuildingArray>(0x80 + 0x4BE08 + i * 4);
	}
}

Town::~Town() {
}

void Town::Write() {
	Save::Instance()->Write(0x0621BA, this->TownName, 8);
	Save::Instance()->Write(0x06223A, this->NativeFruit);
	Save::Instance()->Write(0x0621b0, this->TimePlayed);

	// Write Map Items.
	for (int i = 0; i < 5120; i++) {
		this->MapItem[i]->Write();
	}

	// Write Acres.
	for (int i = 0; i < 42; i++) {
		Save::Instance()->Write(0x053484 + i * 2, this->FullAcres[i]);
	}

	// Write Town Buildings.
	for (int i = 0; i < 58; i++) {
		this->townBuildings[i]->Write();
	}
}