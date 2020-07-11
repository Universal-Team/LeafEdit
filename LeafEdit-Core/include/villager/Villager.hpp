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

#ifndef _LEAFEDIT_CORE_VILLAGER_HPP
#define _LEAFEDIT_CORE_VILLAGER_HPP

#include "Item.hpp"
#include "types.hpp"

#include <memory>
#include <vector>

class Item;
class Villager {
protected:
	std::shared_ptr<u8[]> data;
	u32 offset; // Offset to the Villager.
public:
	virtual ~Villager() {}
	Villager(std::shared_ptr<u8[]> villagerData, u32 villagerOffset) : data(villagerData), offset(villagerOffset) {}
	Villager(const Villager& villager) = delete;
	Villager& operator=(const Villager& villager) = delete;
	virtual u32 getVillagerSize() const = 0;
	
	virtual u16 id() = 0;
	virtual void id(u16 v) = 0;
	virtual bool exist() = 0;
	virtual u8 personality() = 0;
	virtual void personality(u8 v) = 0;

	// Items.
	virtual std::unique_ptr<Item> song() = 0;
	virtual std::unique_ptr<Item> shirt() = 0;
	virtual std::unique_ptr<Item> wallpaper() = 0;
	virtual std::unique_ptr<Item> carpet() = 0;
	virtual std::unique_ptr<Item> umbrella() = 0;
	virtual std::unique_ptr<Item> furniture(int slot) = 0;
};

#endif