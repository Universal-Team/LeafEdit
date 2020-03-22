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

#include "pattern.hpp"
#include "shop.hpp"

// Initialize everything of the shops.
Shop::Shop() {
	// Initialize Turnip prices.
	for (int i = 0; i < 6; i++) {
		this->AMPrice[i] = EncryptedInt32(Save::Instance()->ReadU64(0x06ADE0+i*16));
		this->PMPrice[i] = EncryptedInt32(Save::Instance()->ReadU64(0x06ADE0+i*16+8));
	}

	// Initialize shops.
	for (int i = 0; i < 8; i++) {
		this->Retail[i] = std::make_shared<Item>(0x668B4 + i * sizeof(Item));
	}

	for (int i = 0; i < 8; i++) {
		this->NooksHome[i] = std::make_shared<Item>(0x6ACCE + i * sizeof(Item));
	}

	for (int i = 0; i < 18; i++) {
		this->Gracie[i] = std::make_shared<Item>(0x6ACF8 + i * sizeof(Item));
	}

	for (int i = 0; i < 24; i++) {
		this->TTStore[i] = std::make_shared<Item>(0x62270 + i * sizeof(Item));
	}

	for (int i = 0; i < 11; i++) {
		this->Leif[i] = std::make_shared<Item>(0x666F6 + i * sizeof(Item));
	}

	for (int i = 0; i < 5; i++) {
		this->AbleSisterClothes[i] = std::make_shared<Item>(0x622EC + i * sizeof(Item));
	}

	for (int i = 0; i < 7; i++) {
		this->AbleSisterAccesories[i] = std::make_shared<Item>(0x666B8 + i * sizeof(Item));
	}

	for (int i = 0; i < 6; i++) {
		this->Kicks[i] = std::make_shared<Item>(0x6682E + i * sizeof(Item));
	}

	for (int i = 0; i < 4; i++) {
		this->Redd[i] = std::make_shared<Item>(0x66730 + i * sizeof(Item));
	}

	for (int i = 0; i < 4; i++) {
		this->IslandShop[i] = std::make_shared<Item>(0x6ADB8 + i * sizeof(Item));
	}

	for (int i = 0; i < 3; i++) {
		this->MuseumShop[i] = std::make_shared<Item>(0x6ACBE + i * sizeof(Item));
	}

	for (int i = 0; i < 16; i++) {
		this->LostFound[i] = std::make_shared<Item>(0x621E2 + i * sizeof(Item));
	}

	for (int i = 0; i < 4; i++) {
		this->ClubLol[i] = std::make_shared<Item>(0x6AD92 + i * sizeof(Item));
	}

	for (int i = 0; i < 2; i++) {
		this->Harvey[i] = std::make_shared<Item>(0x06AE54 + i * sizeof(Item));
	}

	// Able sister pattern.
	for (u32 i = 0; i < 8; i++) {
		this->Patterns[i] = std::make_shared<Pattern>(0x62338 + i * 0x870, i);
	}
}

// Delete all and set to nullptr.
Shop::~Shop() {
}

void Shop::Write() {
	u32 encryptedInt = 0;
	u32 encryptionData = 0;

	for (int i = 0; i < 6; i++) {
		this->AMPrice[i].encrypt(encryptedInt, encryptionData);
		Save::Instance()->Write(0x06ADE0+i*16, encryptedInt);
		Save::Instance()->Write(0x06ADE0+i*16+4, encryptionData);
	}
	for (int i = 0; i < 6; i++) {
		this->PMPrice[i].encrypt(encryptedInt, encryptionData);
		Save::Instance()->Write(0x06ADE0+i*16+8, encryptedInt);
		Save::Instance()->Write(0x06ADE0+i*16+8+4, encryptionData);
	}

	for (u32 i = 0; i < 8; i++) {
		//this->Patterns[i]->Write();
	}
}