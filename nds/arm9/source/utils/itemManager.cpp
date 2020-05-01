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

#include "itemManager.hpp"

// Return color index of the ItemPalette array.
u8 ItemManager::getColor(ItemType item) {
	switch(item) {
		case ItemType::Empty:
			return 0x0; // Transparent.
		case ItemType::Furniture:
			return 0xF5;
		case ItemType::Gyroid:
			return 0xF6;
		case ItemType::Diary:
			return 0x0; // Isn't supported in LeafEdit.
		case ItemType::Clothes:
			return 0xEC;
		case ItemType::Song:
			return 0xEE;
		case ItemType::Paper:
			return 0xEA;
		case ItemType::Trash:
			return 0xF8; // Currently not in core for WW? Research.
		case ItemType::Shell:
			return 19;
		case ItemType::Fruit:
			return 0xF9; // Currently not in core for WW? Research.
		case ItemType::Turnip:
			return 0xF2;
		case ItemType::Catchable:
			return 0xED;
		case ItemType::QuestItem:
			return 0x0; // Isn't supported in LeafEdit.
		case ItemType::Item:
			return 0xF1;
		case ItemType::RaffleTicket:
			return 0x0; // Isn't supported in LeafEdit.
		case ItemType::WallpaperCarpet:
			return 0xEB;
		case ItemType::Fossil:
			return 0xF3;
		case ItemType::Tool:
			return 0xEF;
		case ItemType::Tree:
			return 0xE4;
		case ItemType::Weed:
			return 0xE3;
		case ItemType::Flower:
			return 0xE2;
		case ItemType::Rock:
			return 0xE8;
		case ItemType::MoneyRock:
			return 0xE9;
		case ItemType::Signboard:
			return 0x0; // Isn't supported in LeafEdit.
		case ItemType::Money:
			return 0xF0;
		case ItemType::HouseObject:
			return 0x0; // Isn't supported in LeafEdit.
		case ItemType::Building:
			return 0xF7;
		case ItemType::ParchedFlower:
			return 0xE5;
		case ItemType::WateredFlower:
			return 0xE6;
		case ItemType::Pattern:
			return 0xE7;
		case ItemType::WiltedFlower:
			return 0x0; // Not in the WW Core rn.
		case ItemType::Occupied:
			return 0xE1;
		case ItemType::Invalid:
			return 0xFA;
	}
	return 0x0; // Should not happen.
}