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

#ifndef _LEAFEDIT_CORE_ITEM_HPP
#define _LEAFEDIT_CORE_ITEM_HPP

#include "types.hpp"

#include <map>
#include <memory>
#include <vector>

extern std::map<u16, std::string> ItemDatabase;

// Type of the Item.
enum class ItemType {
	Empty,
	Furniture,
	Gyroid,
	Diary,
	Clothes,
	Song,
	Paper,
	Trash,
	Shell,
	Fruit,
	Turnip,
	Catchable,
	QuestItem,
	Item,
	RaffleTicket,
	WallpaperCarpet,
	Fossil,
	Tool,
	Tree,
	Weed,
	Flower,
	Rock,
	MoneyRock,
	Signboard,
	Money,
	HouseObject,
	Building,
	ParchedFlower,
	WateredFlower,
	Pattern,
	WiltedFlower,
	Occupied,
	Invalid
};

class Item {
protected:
	std::shared_ptr<u8[]> data;
	u32 Offset;
public:
	virtual ~Item() {}
	Item(std::shared_ptr<u8[]> dt, u32 offset) : data(dt), Offset(offset) {}
	Item(const Item& item) = delete;
	Item& operator=(const Item& item) = delete;

	virtual u32 maxItems() const = 0; // Get Item Amount.
	virtual u16 id() = 0; // Return ID.
	virtual void id(u16 v) = 0;
	virtual u16 flags() = 0; // Return Flags.
	virtual void flags(u16 v) = 0;
	virtual std::string name() = 0;
	virtual ItemType itemtype() = 0;
	virtual int rotation() = 0;
	virtual void rotation(int Direction) = 0;
};

#endif