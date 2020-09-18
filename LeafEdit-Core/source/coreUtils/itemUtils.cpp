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
#include "stringDB.hpp"
#include "stringUtils.hpp"
#include "types.hpp"

#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>

extern std::vector<std::tuple<u16, std::string, std::string>> itemDB;
extern std::vector<std::string> itemCategories;

// Get an Item's name.
std::string ItemUtils::getName(u16 ID) {
	if (itemDB.empty()) return "???"; // Database empty.
	for (auto const& entry : itemDB) {
		if (std::get<0>(entry) == ID) {
			return std::get<1>(entry);
		}
	}

	return std::string("???");
}

// Get an WW's index Item's name.
std::string ItemUtils::getWWName(int category, u8 index) {
	std::vector<std::tuple<u16, std::string, std::string>> temp = StringDB::searchTuple("", {itemCategories[category]}, itemDB, true);

	if (temp.empty()) return "???"; // Database empty.
	if (temp.size() >= index) return std::get<1>(temp[index]);

	return std::string("???");
}

/* Item Kind stuff here, AC:NL / AC:WA. */
FILE *g_ItemBin = nullptr;
FILE *g_ItemKindBin = nullptr;
/* Real Max Item ID is 0x372B (excludes wrap paper) but for checks, ID -= 0x2000 from orig item ID. */
static constexpr u16 maxID = 0x172B;

/* This table lists the icons in the same order as the game, except it has been adapted for our spritesheet instead. */
static constexpr s32 SpritesheetIDTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 234, 235, 236, 225, 226, 227, 228, 229, 230, 231, 232, 233, 237, 238, 239, 241, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 1, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 266, 379, 339, 242, 240, 393 };

void ItemUtils::loadItemBins() {
	g_ItemBin = fopen("romfs:/ItemBins/Item.bin", "rb");
	g_ItemKindBin = fopen("romfs:/ItemBins/Kind.bin", "rb");
}

void ItemUtils::closeItemBins() {
	if (g_ItemBin) fclose(g_ItemBin);
	if (g_ItemKindBin) fclose(g_ItemKindBin);
}

FILE* ItemUtils::GetItemBin(void) {
	return g_ItemBin;
}

FILE* ItemUtils::GetItemKindBin(void) {
	return g_ItemKindBin;
}

s32 ItemUtils::IsNormalItem(u16 itemID) {
	u16 ID = itemID & 0x7FFF;
	s32 chk = ID - 0x2000; // To cover items lower than 0x2000 (Enviroment Items).

	if (chk < maxID) return chk;

	else return -1;
}

ItemBin_s* ItemUtils::GetItemBinSlot(u16 ItemID) {
	ItemBin_s *ItemSlot = new ItemBin_s;
	s32 chk = IsNormalItem(ItemID);

	if (chk < 0 || chk > maxID) {
		delete ItemSlot;
		return nullptr;

	} else if (g_ItemBin) {
		/* chk <= maxID. */
		fseek(g_ItemBin, sizeof(ItemBin_s)*chk, SEEK_SET);
		if ((fread(reinterpret_cast<void*>(ItemSlot), 1, sizeof(ItemBin_s), g_ItemBin)) >= 0) return ItemSlot;
	}

	delete ItemSlot;
	return nullptr;
}

ItemKind_s* ItemUtils::GetItemKindSlot(u16 ItemID) {
	ItemKind_s *KindSlot = new ItemKind_s;
	ItemBin_s *ItemSlot = GetItemBinSlot(ItemID);

	if (!ItemSlot) {
		delete KindSlot;
		return nullptr;
	}

	u8 ItemCategory = ItemSlot->Category;
	delete ItemSlot;

	if (ItemCategory >= 0x9B) {
		delete KindSlot;
		return nullptr;

	} else if (g_ItemKindBin) {
		fseek(g_ItemKindBin, sizeof(ItemBin_s)* ItemCategory, SEEK_SET);
		if ((fread(reinterpret_cast<void *>(KindSlot), 1, sizeof(ItemKind_s), g_ItemKindBin)) >= 0) return KindSlot;
	}

	delete KindSlot;
	return nullptr;
}

u16 ItemUtils::GetAxeDamageValue(u16 ItemID, u16 Flags) {
	/* Used for Normal Axe and Silver Axe. */
	if (ItemID != 0x334D && ItemID != 0x334E) return 0;

	return (Flags & 0xF);
}

u16 ItemUtils::GetAxeDamageIcon(u16 ItemID, u16 Flags) {
	static constexpr u8 AxeDamageTable[] = {0, 0, 0, 1, 1, 1, 2, 2};
	u16 DamageValue = GetAxeDamageValue(ItemID, Flags);
	if (DamageValue >= 8) return ItemID;

	return ItemID + AxeDamageTable[DamageValue];
}

bool ItemUtils::IsInvWhitelisted(u16 ItemID) {
	u16 ID = ItemID;
	ID &= 0x6000;
	if (ID == 0x4000) return true; // If item is wrapping paper / Any item ID between 0x4000 <-> 0x5FFF.

	ItemKind_s *KindSlot = GetItemKindSlot(ID);

	if (!KindSlot) return false;

	u32 Unk = !(KindSlot->Unknown2 & 0x10);
	if (Unk == static_cast<u32>(-1)) return true;

	return false;
}

u8 ItemUtils::GetCategory(u16 ItemID) {
	ItemBin_s* ItemSlot = GetItemBinSlot(ItemID);
	if (!ItemSlot) return 0x9B;

	u8 category = ItemSlot->Category;
	delete ItemSlot;
	if (category >= 0x9B) return 0;

	return category;
}

u16 ItemUtils::GetIconID(u16 ItemID, u16 Flags) {

	ItemBin_s* ItemSlot = GetItemBinSlot(ItemID);
	if (!ItemSlot) return 0;

	u16 IconID = ItemSlot->ItemIcon;
	delete ItemSlot;

	if (IconID >= 0x1FB) return 0;

	u8 category = GetCategory(ItemID);
	if (category == 0xE && (ItemID == 0x334D || ItemID == 0x334E)) {
		IconID = GetAxeDamageIcon(IconID, Flags);
	}

	return IconID;
}

s32 ItemUtils::GetSpritesheetID(u16 ItemID, u16 Flags) {
	if (ItemID == 0x7FFE) return -1;

	u16 iconID = 0;
	if (g_ItemBin) iconID = GetIconID(ItemID, Flags);

	return SpritesheetIDTable[iconID];
}