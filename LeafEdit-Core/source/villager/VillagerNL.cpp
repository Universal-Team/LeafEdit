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
#include "VillagerNL.hpp"

// Villager ID.
u16 VillagerNL::id() {
	return SaveUtils::Read<u16>(villagerPointer(), 0x0);
}
void VillagerNL::id(u16 v) {
	SaveUtils::Write<u16>(villagerPointer(), 0x0, v);
}

// Check if the Villager exist.
bool VillagerNL::exist() {
	if (this->id() == 0xFFFF)	return false;
	return true;
}

// Villager Personality.
u8 VillagerNL::personality() {
	return villagerPointer()[0x2];
}
void VillagerNL::personality(u8 v) {
	villagerPointer()[0x2] = v;
}

// Villager Song.
std::unique_ptr<Item> VillagerNL::song() {
	return std::make_unique<ItemNL>(data, offset + 0x2452);
}

// Villager Shirt.
std::unique_ptr<Item> VillagerNL::shirt() {
	return std::make_unique<ItemNL>(data, offset + 0x244E);
}

// Villager Wallpaper.
std::unique_ptr<Item> VillagerNL::wallpaper() {
	return std::make_unique<ItemNL>(data, offset + 0x2456);
}

// Villager Carpet.
std::unique_ptr<Item> VillagerNL::carpet() {
	return std::make_unique<ItemNL>(data, offset + 0x245A);
}

// Villager Umbrella.
std::unique_ptr<Item> VillagerNL::umbrella() {
	return std::make_unique<ItemNL>(data, offset + 0x245E);
}

// Villager Furniture.
std::unique_ptr<Item> VillagerNL::furniture(int slot) {
	if (slot > 15)	return nullptr;
	return std::make_unique<ItemNL>(data, offset + 0x2462 + slot * 4);
}