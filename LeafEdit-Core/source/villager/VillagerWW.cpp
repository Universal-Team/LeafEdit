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
#include "VillagerWW.hpp"

/* Villager ID. */
u16 VillagerWW::id() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return villagerPointer()[0x6CB];

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return villagerPointer()[0x595];

		case WWRegion::KOR_REV1:
			return villagerPointer()[0x7A7];

		case WWRegion::UNKNOWN:
			return 0;
	}

	return 0;
}
void VillagerWW::id(u16 v) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x6CB, (u8)v);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x595, (u8)v);
			break;

		case WWRegion::KOR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x7A7, (u8)v);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

/* Check if the Villager exist. */
bool VillagerWW::exist() const {
	if (this->id() == 0xFF)	return false;
	return true;
}

/* Villager Personality. */
u8 VillagerWW::personality() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return villagerPointer()[0x6CA];

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return villagerPointer()[0x594];

		case WWRegion::KOR_REV1:
			return villagerPointer()[0x7A6];

		case WWRegion::UNKNOWN:
			return 0;
	}

	return 0;
}
void VillagerWW::personality(u8 v) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x6CA, v);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x594, v);
			break;

		case WWRegion::KOR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x7A6, v);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

/* Villager Song. This needs still to be researched. */
u8 VillagerWW::songWW() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return this->villagerPointer()[0x6D0];

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return this->villagerPointer()[0x59A];

		case WWRegion::KOR_REV1:
			return this->villagerPointer()[0x7AC];

		case WWRegion::UNKNOWN:
			return 0;
	}

	return 0;
}
void VillagerWW::songWW(u8 sng) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x6D0, sng);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x59A, sng);
			break;

		case WWRegion::KOR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x7AC, sng);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

/* Villager Shirt. */
std::unique_ptr<Item> VillagerWW::shirt() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return std::make_unique<ItemWW>(this->data, this->offset + 0x6EC);

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return std::make_unique<ItemWW>(this->data, this->offset + 0x5AE);

		case WWRegion::KOR_REV1:
			return std::make_unique<ItemWW>(this->data, this->offset + 0x7CA);

		case WWRegion::UNKNOWN:
			return nullptr;
	}

	return nullptr;
}

/* Villager Wallpaper. */
u8 VillagerWW::wallpaperWW() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return this->villagerPointer()[0x6EE];

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return this->villagerPointer()[0x5B0];

		case WWRegion::KOR_REV1:
			return this->villagerPointer()[0x7CC];

		case WWRegion::UNKNOWN:
			return 0;
	}

	return 0;
}
void VillagerWW::wallpaperWW(u8 wlp) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x6EE, wlp);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x5B0, wlp);
			break;

		case WWRegion::KOR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x7CC, wlp);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

/* Villager Carpet. */
u8 VillagerWW::carpetWW() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return this->villagerPointer()[0x6EF];

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return this->villagerPointer()[0x5B1];

		case WWRegion::KOR_REV1:
			return this->villagerPointer()[0x7CD];

		case WWRegion::UNKNOWN:
			return 0;
	}

	return 0;
}
void VillagerWW::carpetWW(u8 crp) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x6EF, crp);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x5B1, crp);
			break;

		case WWRegion::KOR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x7CD, crp);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

/* Villager Umbrella. */
u8 VillagerWW::umbrellaWW() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return this->villagerPointer()[0x6F4];

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return this->villagerPointer()[0x544];

		case WWRegion::KOR_REV1:
			return this->villagerPointer()[0x7D2];

		case WWRegion::UNKNOWN:
			return 0;
	}

	return 0;
}
void VillagerWW::umbrellaWW(u8 umbr) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x6F4, umbr);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x544, umbr);
			break;

		case WWRegion::KOR_REV1:
			SaveUtils::Write<u8>(this->villagerPointer(), 0x7D2, umbr);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

/* Villager Furniture. */
std::unique_ptr<Item> VillagerWW::furniture(int slot) const {
	if (slot > 9) return nullptr;

	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return std::make_unique<ItemWW>(this->data, this->offset + 0x6AC + slot * 2);

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return std::make_unique<ItemWW>(this->data, this->offset + 0x578 + slot * 2);

		case WWRegion::KOR_REV1:
			return std::make_unique<ItemWW>(this->data, this->offset + 0x784 + slot * 2);

		case WWRegion::UNKNOWN:
			return nullptr;
	}
	
	return nullptr;
}