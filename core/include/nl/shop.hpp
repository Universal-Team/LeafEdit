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

#pragma once

#ifndef SHOP_HPP
#define SHOP_HPP

#include "encryptedInt32.hpp"
#include "item.hpp"
#include "save.hpp"
#include "types.hpp"

#include <string>

class Pattern;
class Shop {
public:
	Shop(void);
	~Shop(void);
	void Write(void);

	// 0x06ADE0
	EncryptedInt32 PMPrice[6]; // 6 because of 6 Days.
	EncryptedInt32 AMPrice[6]; // 6 because of 6 Days.

	// TODO: Implement the pattern for this.
	//Pattern *PatternAbleSister[10]; // Able Sister Patterns. -> 0x62338 

	// Stores Inventory.
	Item *Retail = nullptr; // 0x668b4 [8]
	Item *TTStore = nullptr; // 0x62270 [24]
	Item *Gracie = nullptr; // 0x6acf8 [18]
	Item *NooksHome = nullptr; // 0x6ACCE [8]
	Item *Leif = nullptr; // 0x666f6 [11]
	Item *AbleSisterClothes = nullptr; // 0x622ec [5]
	Item *AbleSisterAccesories = nullptr; // 0x666b8 [7]
	Item *Kicks = nullptr; // 0x6682e [6]
	Item *Redd = nullptr; // 0x66730 [4]
	Item *IslandShop = nullptr; // 0x6adb8 [4]
	Item *MuseumShop = nullptr; // 0x6acbe [3]
	Item *LostFound = nullptr; // 0x621e2 [16]
	Item *ClubLol = nullptr; // 0x6ad92 [4]
	Item *Harvey = nullptr; // 0x06ae54 [2]

private:
};

#endif