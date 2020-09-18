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
#include "ItemWA.hpp"
#include "saveUtils.hpp"

/* Item ID. */
u16 ItemWA::id() const { return SaveUtils::Read<u16>(this->itemPointer(), 0); }
void ItemWA::id(u16 v) { SaveUtils::Write<u16>(this->itemPointer(), 0, v); }

/* Item Flags. */
u16 ItemWA::flags() const { return SaveUtils::Read<u16>(this->itemPointer(), 2); }
void ItemWA::flags(u16 v) { SaveUtils::Write<u16>(this->itemPointer(), 2, v); }

u8 ItemWA::flag1() const { return SaveUtils::Read<u8>(this->itemPointer(), 2); }
void ItemWA::flag1(u8 v) { SaveUtils::Write<u8>(this->itemPointer(), 2, v); }

u8 ItemWA::flag2() const { return SaveUtils::Read<u8>(this->itemPointer(), 3); }
void ItemWA::flag2(u8 v) { SaveUtils::Write<u8>(this->itemPointer(), 3, v); }

/* Item Rotation. (Furniture / House) */
FurnitureDirection ItemWA::rotation() const { return FurnitureDirection::None; }
void ItemWA::rotation(FurnitureDirection Direction) { }

/* Item Name. */
std::string ItemWA::name() const { return ItemUtils::getName(this->id()); }

/* Item Type / Category. */
ItemType ItemWA::itemtype() const {
	u16 ID = this->id(); // Only 1 time needed.

	if (ID == 0x7FFE) {
		return ItemType::Empty;

	} else if ((ID >= 0x0005 && ID <= 0x007B) || (ID >= 0x0080 && ID <= 0x0097)) {
		return ItemType::Tree;

	} else if ((ID >= 0x007C && ID <= 0x007F) || (ID >= 0x00C9 && ID <= 0x00CD) || ID == 0x00F8) {
		return ItemType::Weed;

	} else if (ID == 0x009D) {
		return ItemType::Pattern;

	} else if (ID >= 0x0098 && ID <= 0x009C) {
		return ItemType::Rock;

	} else if (ID >= 0x009F && ID <= 0x00C8) {
		return ItemType::Flower;

	} else if (ID >= 0x00CE && ID <= 0x00F7) {
		return ItemType::WiltedFlower;

	} else if (ID == 0x202A) {
		return ItemType::Fossil;

	} else if (ID >= 0x20AC && ID <= 0x2117) {
		return ItemType::Money;

	} else if (ID >= 0x212B && ID <= 0x223E) {
		return ItemType::Song;

	} else if (ID >= 0x223F && ID <= 0x2282) {
		return ItemType::Paper;

	} else if (ID >= 0x2283 && ID <= 0x228D) {
		return ItemType::Turnip;

	} else if (ID >= 0x228E && ID <= 0x234B) {
		return ItemType::Catchable;

	} else if (ID >= 0x234C && ID <= 0x2492) {
		return ItemType::WallpaperCarpet;

	} else if (ID >= 0x2495 && ID <= 0x295B) {
		return ItemType::Clothes;

	} else if (ID >= 0x295C && ID <= 0x29DE) {
		return ItemType::Gyroid;

	} else if (ID >= 0x334C && ID <= 0x338B) {
		return ItemType::Tool;

	} else if (ID >= 0x209F && ID <= 0x20A0) {
		return ItemType::Item;

	} else if (ID >= 0x208C && ID <= 0x2095) {
		return ItemType::Shell;

	} else if (ID == 0x7FFC) {
		return ItemType::Occupied;
		
	} else {
		return ItemType::Furniture;
	}
}