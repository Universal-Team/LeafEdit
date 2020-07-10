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

#ifndef _LEAFEDIT_CORE_VILLAGER_WW_HPP
#define _LEAFEDIT_CORE_VILLAGER_WW_HPP

#include "Item.hpp"
#include "ItemWW.hpp"
#include "Villager.hpp"
#include "types.hpp"

#include <memory>
#include <vector>

class Item;
class ItemWW;
class VillagerWW : public Villager {
protected:
	std::shared_ptr<u8[]> data;
	WWRegion region;
	u32 offset; // Offset to the Villager.
public:
	virtual ~VillagerWW() {}
	VillagerWW(std::shared_ptr<u8[]> villagerData, u32 villagerOffset, WWRegion Region) : Villager(villagerData, villagerOffset), data(villagerData), region(Region), offset(villagerOffset) { }
	u32 getVillagerSize() const override {
		switch(this->region) {
			case WWRegion::USA_REV0:
			case WWRegion::USA_REV1:
			case WWRegion::EUR_REV1:
				return 0x700;
			case WWRegion::JPN_REV0:
			case WWRegion::JPN_REV1:
				return 0x5C0;
			case WWRegion::KOR_REV1:
				return 0x7EC;
			case WWRegion::UNKNOWN:
				return 0;
		}
		return 0;
	}

	u16 id() override;
	void id(u16 v) override;
	bool exist() override;
	u8 personality() override;
	void personality(u8 v) override;
	
	// Items.
	std::unique_ptr<Item> song() override;
	std::unique_ptr<Item> shirt() override;
	std::unique_ptr<Item> wallpaper() override;
	std::unique_ptr<Item> carpet() override;
	std::unique_ptr<Item> umbrella() override;
	std::unique_ptr<Item> furniture(int slot) override;
private:
	u8* villagerPointer() const {
		return data.get() + offset;
	}
};

#endif