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

#ifndef _LEAFEDIT_CORE_VILLAGER_WA_HPP
#define _LEAFEDIT_CORE_VILLAGER_WA_HPP

#include "Item.hpp"
#include "ItemWA.hpp"
#include "Villager.hpp"
#include "types.hpp"

#include <memory>
#include <vector>

class Item;
class ItemWA;
class VillagerWA : public Villager {
protected:
	std::shared_ptr<u8[]> data;
	u32 offset;
public:
	virtual ~VillagerWA() {}
	VillagerWA(std::shared_ptr<u8[]> villagerData, u32 villagerOffset) :
			Villager(villagerData, villagerOffset), data(villagerData), offset(villagerOffset) { }
			
	u32 getVillagerSize() const override { return 0x2518; }

	u16 id() const override;
	void id(u16 v) override;
	bool exist() const override;
	u8 personality() const override;
	void personality(u8 v) override;

	/* Items. */
	std::unique_ptr<Item> song() const override;
	std::unique_ptr<Item> shirt() const override;
	std::unique_ptr<Item> wallpaper() const override;
	std::unique_ptr<Item> carpet() const override;
	std::unique_ptr<Item> umbrella() const override;
	
	/* Only for AC:WW. */
	u8 songWW() const override { return 0; };
	void songWW(u8 sng) override { };
	u8 wallpaperWW() const override { return 0; };
	void wallpaperWW(u8 wlp) override { };
	u8 carpetWW() const override { return 0; };
	void carpetWW(u8 crp) override { };
	u8 umbrellaWW() const override { return 0; };
	void umbrellaWW(u8 umbr) override { };

	std::unique_ptr<Item> furniture(int slot) const override;
private:
	u8* villagerPointer() const {
		return data.get() + offset;
	}
};

#endif