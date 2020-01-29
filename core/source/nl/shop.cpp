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
	this->Retail = new Item[8];
	for (int i = 0; i < 8; i++) {
		this->Retail[i] = Item(0x668B4 + i * sizeof(Item));
	}

	this->TT = new Item[24];
	for (int i = 0; i < 24; i++) {
		this->TT[i] = Item(0x6ACCE + i * sizeof(Item));
	}

	this->Gracie = new Item[18];
	for (int i = 0; i < 18; i++) {
		this->Gracie[i] = Item(0x6ACF8 + i * sizeof(Item));
	}

	this->NooksHome = new Item[8];
	for (int i = 0; i < 8; i++) {
		this->NooksHome[i] = Item(0x62270 + i * sizeof(Item));
	}

	this->Leif = new Item[11];
	for (int i = 0; i < 11; i++) {
		this->Leif[i] = Item(0x666F6 + i * sizeof(Item));
	}

	this->AbleSisterClothes = new Item[5];
	for (int i = 0; i < 5; i++) {
		this->AbleSisterClothes[i] = Item(0x622EC + i * sizeof(Item));
	}

	this->AbleSisterAccesories = new Item[7];
	for (int i = 0; i < 7; i++) {
		this->AbleSisterAccesories[i] = Item(0x666B8 + i * sizeof(Item));
	}

	this->Kicks = new Item[6];
	for (int i = 0; i < 6; i++) {
		this->Kicks[i] = Item(0x6682E + i * sizeof(Item));
	}

	this->Redd = new Item[4];
	for (int i = 0; i < 4; i++) {
		this->Redd[i] = Item(0x66730 + i * sizeof(Item));
	}

	this->IslandShop = new Item[4];
	for (int i = 0; i < 4; i++) {
		this->IslandShop[i] = Item(0x6ADB8 + i * sizeof(Item));
	}

	this->MuseumShop = new Item[3];
	for (int i = 0; i < 3; i++) {
		this->MuseumShop[i] = Item(0x6ACBE + i * sizeof(Item));
	}

	this->LostFound = new Item[16];
	for (int i = 0; i < 16; i++) {
		this->LostFound[i] = Item(0x621E2 + i * sizeof(Item));
	}

	this->ClubLol = new Item[4];
	for (int i = 0; i < 4; i++) {
		this->ClubLol[i] = Item(0x6AD92 + i * sizeof(Item));
	}

	this->Harvey = new Item[2];
	for (int i = 0; i < 2; i++) {
		this->Harvey[i] = Item(0x06AE54 + i * sizeof(Item));
	}
}

// Delete all and set to nullptr.
Shop::~Shop() {
	if (this->Retail != nullptr) {
		delete[] this->Retail;
		this->Retail = nullptr;
	}

	if (this->TT != nullptr) {
		delete[] this->TT;
		this->TT = nullptr;
	}

	if (this->Gracie != nullptr) {
		delete[] this->Gracie;
		this->Gracie = nullptr;
	}

	if (this->NooksHome != nullptr) {
		delete[] this->NooksHome;
		this->NooksHome = nullptr;
	}

	if (this->Leif != nullptr) {
		delete[] this->Leif;
		this->Leif = nullptr;
	}

	if (this->AbleSisterClothes != nullptr) {
		delete[] this->AbleSisterClothes;
		this->AbleSisterClothes = nullptr;
	}

	if (this->AbleSisterAccesories != nullptr) {
		delete[] this->AbleSisterAccesories;
		this->AbleSisterAccesories = nullptr;
	}

	if (this->Kicks != nullptr) {
		delete[] this->Kicks;
		this->Kicks = nullptr;
	}

	if (this->Redd != nullptr) {
		delete[] this->Redd;
		this->Redd = nullptr;
	}

	if (this->IslandShop != nullptr) {
		delete[] this->IslandShop;
		this->IslandShop = nullptr;
	}

	if (this->MuseumShop != nullptr) {
		delete[] this->MuseumShop;
		this->MuseumShop = nullptr;
	}

	if (this->LostFound != nullptr) {
		delete[] this->LostFound;
		this->LostFound = nullptr;
	}

	if (this->ClubLol != nullptr) {
		delete[] this->ClubLol;
		this->ClubLol = nullptr;
	}

	if (this->Harvey != nullptr) {
		delete[] this->Harvey;
		this->Harvey = nullptr;
	}
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
}