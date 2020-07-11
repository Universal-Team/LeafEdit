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

#include "itemUtils.hpp"
#include "ItemWW.hpp"
#include "saveUtils.hpp"

// Item ID.
u16 ItemWW::id() { return SaveUtils::Read<u16>(itemPointer(), 0); }
void ItemWW::id(u16 v) { SaveUtils::Write<u16>(itemPointer(), 0, v); }

// Item Flags. (Does not exist?)
u16 ItemWW::flags() { return 0; }
void ItemWW::flags(u16 v) { }

// Item Rotation. (Furniture / House)
int ItemWW::rotation() { return 0; }
void ItemWW::rotation(int Direction) { } // TODO.

// Item Name.
std::string ItemWW::name() { return ItemUtils::getName(this->id()); }

// Item Type / Category.
ItemType ItemWW::itemtype() {
	u16 ID = this->id(); // Only 1 time needed.

	if (ID == 0xFFF1) {
		return ItemType::Empty;
	} else if (ID == 0xF030) {
		return ItemType::Occupied;
	} else if (ID <= 0x1C) {
		return ItemType::Flower;
	} else if (ID >= 0x1D && ID <= 0x24) {
		return ItemType::Weed;
	} else if ((ID >= 0x25 && ID <= 0x6D) || (ID >= 0xC7 && ID <= 0xD3)) {
		return ItemType::Tree;
	} else if (ID >= 0x6E && ID <= 0x89) {
		return ItemType::ParchedFlower;
	} else if (ID >= 0x8A && ID <= 0xA5) {
		return ItemType::WateredFlower;
	} else if (ID >= 0xA7 && ID <= 0xC6) {
		return ItemType::Pattern;
	} else if ((ID >= 0xE3 && ID <= 0xE7) || (ID >= 0xED && ID <= 0xFB)) {
		return ItemType::Rock;
	} else if (ID >= 0xE8 && ID <= 0xEC) {
		return ItemType::MoneyRock;
	} else if (ID >= 0x1000 && ID <= 0x10FF) {
		return ItemType::Paper;
	} else if ((ID >= 0x1100 && ID <= 0x1143) || (ID >= 0x1144 && ID <= 0x1187)) {
		return ItemType::WallpaperCarpet; // Wallpaper then Carpet.
	} else if ((ID >= 0x11A8 && ID <= 0x12AF) || (ID >= 0x13A8 && ID <= 0x1457)) {
		return ItemType::Clothes; // Shirts, then hats 0x14s, & Glasses/Masks
	} else if (ID >= 0x12B0 && ID <= 0x131F) {
		return ItemType::Catchable;
	} else if (ID >= 0x1323 && ID <= 0x1368) {
		return ItemType::Song;
	} else if (ID >= 0x1369 && ID <= 0x139F) {
		return ItemType::Tool;
	} else if (ID >= 0x1492 && ID <= 0x14FD) {
		return ItemType::Money;
	} else if ((ID >= 0x14FE && ID <= 0x1530) || (ID >= 0x1542 && ID <= 0x1548) || (ID >= 0x155E && ID <= 0x156D)) {
		return ItemType::Item;
	} else if ((ID >= 0x1531 && ID <= 0x1541)) {
		return ItemType::Turnip;
	} else if (ID == 0x1549) {
		return ItemType::Fossil;
	} else if (ID >= 0x1554 && ID <= 0x155C) {
		return ItemType::Shell;
	} else if ((ID >= 0x3000 && ID <= 0x45D8) || (ID >= 0x47D8 && ID <= 0x4BA0)) {
		return ItemType::Furniture;
	} else if (ID >= 0x45DC && ID <= 0x47D4) {
		return ItemType::Gyroid;
	} else if ((ID >= 0x5000 && ID <= 0x5021) || ID == 0xF030 || ID == 0xF031) {
		return ItemType::Building; // F030-1 = Multispace furniture item
	} else {
		return ItemType::Invalid;
	}
}