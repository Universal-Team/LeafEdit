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

#ifndef _LEAFEDIT_CORE_ITEMUTILS_HPP
#define _LEAFEDIT_CORE_ITEMUTILS_HPP

#include "types.hpp"

#include <string>
#include <vector>

struct ItemBin_s {
	u16 ItemIcon;
	u16 ItemPrice;
	u16 Unknown0;
	u16 RefurbishID;
	u16 SoundID;
	u8  Category;
	s8  Unknown2;
	s8  Unknown3;
	s8  Unknown4;
	s8  PriceMultiplier;
	u8  Unknown5;
	s8  ClothingGroup;
	u8  CanRefurbish___;
	u8  DinosaurGroup;
	s8  ArtGroup;
	u8  Unknown11;
	u8  Unknown12;
	u8  Unknown13;
	u8  Unknown14;
	u8  Unknown15;
	u8  Unknown16;
	u8  Unknown17;
	u8  Unknown18;
	u8  Unknown19;
	u8  Unknown20;
};

struct ItemKind_s {
	u8 Unknown1;
	u8 Unknown2;
	u8 Unknown3;
};

namespace ItemUtils {
	/* Read Database. */
	void LoadDatabase(SaveType save);
	std::string getName(u16 ID); // Get an Item's name.
	std::string getWWName(int category, u8 index);

	/* Item Kind stuff for AC:NL | AC:WA here. */
	void loadItemBins();
	void closeItemBins();
	FILE* GetItemBin(void);
	FILE* GetItemKindBin(void);
	s32 IsNormalItem(u16 itemID);
	ItemBin_s* GetItemBinSlot(u16 ItemID);
	ItemKind_s* GetItemKindSlot(u16 ItemID);
	u16 GetAxeDamageValue(u16 ItemID, u16 Flags = 0);
	u16 GetAxeDamageIcon(u16 ItemID, u16 Flags = 0);
	bool IsInvWhitelisted(u16 ItemID);
	u8 GetCategory(u16 ItemID);
	u16 GetIconID(u16 ItemID, u16 Flags = 0);
	s32 GetSpritesheetID(u16 ItemID, u16 Flags = 0);
}

#endif