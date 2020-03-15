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

#ifndef ITEMCONTAINER_HPP
#define ITEMCONTAINER_HPP

#include "item.hpp"
#include "types.hpp"

#include <cstdio>
#include <string>

class Item;
class ItemContainer {
public:
	ItemContainer(const std::shared_ptr<Item> item);
	~ItemContainer();

	u16 returnItemID(); // Return Item ID.
	u16 returnItemFlag(); // Return Item Flag.
	u8 returnCategory(); // Return Item Category.
	s32 returnID(); // Return SpriteSheet ID.
	std::string returnName(); // Return Item name.
	bool returnWhiteList(); // Return if Item is in Inventory Whitelist.
	void Refresh();
private:
	u8 Category;
	s32 spritesheetID;
	std::string name;
	bool isWhiteList;
	std::shared_ptr<Item> m_item;
	u16 ItemID;
	u16 ItemFlag;
};

#endif