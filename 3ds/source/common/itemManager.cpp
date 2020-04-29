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

#include "common.hpp"
#include "itemManager.hpp"

u32 ItemManager::getColor(ItemType item) {
	switch(item) {
		case ItemType::Empty:
			return C2D_Color32(0, 0, 0, 0); // Transparent.
		case ItemType::Furniture:
			return Config::Furniture;
		case ItemType::Gyroid:
			return Config::Gyroid;
		case ItemType::Diary:
			return C2D_Color32(0, 0, 0, 0); // Isn't supported in LeafEdit.
		case ItemType::Clothes:
			return Config::Clothes;
		case ItemType::Song:
			return Config::Song;
		case ItemType::Paper:
			return Config::Paper;
		case ItemType::Trash:
			return Config::Trash;
		case ItemType::Shell:
			return Config::Shell;
		case ItemType::Fruit:
			return Config::Fruit;
		case ItemType::Turnip:
			return Config::Turnip;
		case ItemType::Catchable:
			return Config::Catchable;
		case ItemType::QuestItem:
			return C2D_Color32(0, 0, 0, 0); // Isn't supported in LeafEdit.
		case ItemType::Item:
			return Config::Item;
		case ItemType::RaffleTicket:
			return C2D_Color32(0, 0, 0, 0); // Isn't supported in LeafEdit.
		case ItemType::WallpaperCarpet:
			return Config::WallpaperCarpet;
		case ItemType::Fossil:
			return Config::Fossil;
		case ItemType::Tool:
			return Config::Tool;
		case ItemType::Tree:
			return Config::Tree;
		case ItemType::Weed:
			return Config::Weed;
		case ItemType::Flower:
			return Config::Flower;
		case ItemType::Rock:
			return Config::Rock;
		case ItemType::MoneyRock:
			return Config::MoneyRock;
		case ItemType::Signboard:
			return C2D_Color32(0, 0, 0, 0); // Isn't supported in LeafEdit.
		case ItemType::Money:
			return Config::Money;
		case ItemType::HouseObject:
			return C2D_Color32(0, 0, 0, 0); // Isn't supported in LeafEdit.
		case ItemType::Building:
			return Config::Building;
		case ItemType::ParchedFlower:
			return Config::ParchedFlower;
		case ItemType::WateredFlower:
			return Config::WateredFlower;
		case ItemType::Pattern:
			return Config::Pattern;
		case ItemType::WiltedFlower:
			return Config::WiltedFlower;
		case ItemType::Occupied:
			return Config::Occupied;
		case ItemType::Invalid:
			return Config::Invalid;
	}
	return C2D_Color32(0, 0, 0, 0); // Should not happen.
}