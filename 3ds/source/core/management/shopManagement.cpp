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

#include "common/utils.hpp"

#include "core/management/shopManagement.hpp"

#include "gui/screens/screenCommon.hpp"

// 8 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_Retail() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 8; num++) {
		Item* item = &Save::Instance()->shop[0]->Retail[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 24 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_TT() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 24; num++) {
		Item* item = &Save::Instance()->shop[0]->TTStore[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 18 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_Gracie() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 18; num++) {
		Item* item = &Save::Instance()->shop[0]->Gracie[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 8 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_NooksHome() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 8; num++) {
		Item* item = &Save::Instance()->shop[0]->NooksHome[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 11 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_Leif() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 11; num++) {
		Item* item = &Save::Instance()->shop[0]->Leif[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 5 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_AbleSisterClothes() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 5; num++) {
		Item* item = &Save::Instance()->shop[0]->AbleSisterClothes[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 7 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_AbleSisterAccesories() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 7; num++) {
		Item* item = &Save::Instance()->shop[0]->AbleSisterAccesories[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 6 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_Kicks() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 6; num++) {
		Item* item = &Save::Instance()->shop[0]->Kicks[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 4 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_Redd() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 4; num++) {
		Item* item = &Save::Instance()->shop[0]->Redd[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 4 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_IslandShop() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 4; num++) {
		Item* item = &Save::Instance()->shop[0]->IslandShop[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 3 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_MuseumShop() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 3; num++) {
		Item* item = &Save::Instance()->shop[0]->MuseumShop[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 16 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_LostFound() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 16; num++) {
		Item* item = &Save::Instance()->shop[0]->LostFound[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 4 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_ClubLol() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 4; num++) {
		Item* item = &Save::Instance()->shop[0]->ClubLol[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}

// 2 Slots.
std::vector<std::pair<std::string, s32>> ShopManagement::load_Harvey() {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 2; num++) {
		Item* item = &Save::Instance()->shop[0]->Harvey[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}