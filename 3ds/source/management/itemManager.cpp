/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
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

#include "common.hpp"
#include "gfx.hpp"
#include "itemManager.hpp"
#include "itemUtils.hpp"
#include <vector>

extern std::vector<std::pair<u16, std::string>> itemDB;

	// All Colors.
	u32 ItemManager::Pattern, ItemManager::Building, ItemManager::MoneyRock, ItemManager::Furniture, ItemManager::Gyroid, ItemManager::Clothes,
	ItemManager::Song, ItemManager::Paper, ItemManager::Trash, ItemManager::Shell,
	ItemManager::Fruit, ItemManager::Turnip, ItemManager::Catchable, ItemManager::Item, ItemManager::WallpaperCarpet, ItemManager::Fossil,
	ItemManager::Tool, ItemManager::Tree, ItemManager::Weed, ItemManager::Flower, ItemManager::Rock,
	ItemManager::Money, ItemManager::ParchedFlower, ItemManager::WateredFlower, ItemManager::WiltedFlower, ItemManager::Occupied, ItemManager::Invalid;

void ItemManager::loadColors() {
	Furniture		= C2D_Color32(99, 226, 90, 255);
	Gyroid			= C2D_Color32(218, 155, 80, 255);
	Clothes			= C2D_Color32(83, 143, 185, 255);
	Song			= C2D_Color32(181, 237, 196, 255);
	Paper			= C2D_Color32(181, 237, 234, 255);
	Trash			= C2D_Color32(119, 136, 89, 255);
	Shell			= C2D_Color32(252, 203, 211, 255);
	Fruit			= C2D_Color32(225, 177, 225, 255);
	Turnip			= C2D_Color32(199, 187, 175, 255);
	Catchable		= C2D_Color32(198, 230, 101, 255);
	Item			= C2D_Color32(252, 181, 52, 255);
	WallpaperCarpet = C2D_Color32(172, 102, 102, 255);
	Fossil			= C2D_Color32(116, 100, 89, 255);
	Tool			= C2D_Color32(153, 153, 153, 255);
	Tree			= C2D_Color32(161, 106, 67, 255);
	Weed			= C2D_Color32(52, 152, 52, 255);
	Flower			= C2D_Color32(237, 133, 196, 255);
	Rock			= C2D_Color32(52, 52, 52, 255);
	MoneyRock		= C2D_Color32(180, 89, 89, 255);
	Money			= C2D_Color32(252, 252, 52, 255);
	Building		= C2D_Color32(119, 119, 119, 255);
	ParchedFlower	= C2D_Color32(180, 133, 52, 255);
	WateredFlower	= C2D_Color32(52, 177, 177, 255);
	Pattern			= C2D_Color32(172, 172, 252, 255);
	WiltedFlower	= C2D_Color32(112, 112, 92, 255);
	Occupied		= C2D_Color32(165, 165, 165, 255);
	Invalid			= C2D_Color32(252, 52, 52, 255);
}


u32 ItemManager::getColor(ItemType item) {
	switch(item) {
		case ItemType::Empty:
			return C2D_Color32(0, 0, 0, 0); // Transparent.
		case ItemType::Furniture:
			return Furniture;
		case ItemType::Gyroid:
			return Gyroid;
		case ItemType::Diary:
			return C2D_Color32(0, 0, 0, 0); // Isn't supported in LeafEdit.
		case ItemType::Clothes:
			return Clothes;
		case ItemType::Song:
			return Song;
		case ItemType::Paper:
			return Paper;
		case ItemType::Trash:
			return Trash;
		case ItemType::Shell:
			return Shell;
		case ItemType::Fruit:
			return Fruit;
		case ItemType::Turnip:
			return Turnip;
		case ItemType::Catchable:
			return Catchable;
		case ItemType::QuestItem:
			return C2D_Color32(0, 0, 0, 0); // Isn't supported in LeafEdit.
		case ItemType::Item:
			return Item;
		case ItemType::RaffleTicket:
			return C2D_Color32(0, 0, 0, 0); // Isn't supported in LeafEdit.
		case ItemType::WallpaperCarpet:
			return WallpaperCarpet;
		case ItemType::Fossil:
			return Fossil;
		case ItemType::Tool:
			return Tool;
		case ItemType::Tree:
			return Tree;
		case ItemType::Weed:
			return Weed;
		case ItemType::Flower:
			return Flower;
		case ItemType::Rock:
			return Rock;
		case ItemType::MoneyRock:
			return MoneyRock;
		case ItemType::Signboard:
			return C2D_Color32(0, 0, 0, 0); // Isn't supported in LeafEdit.
		case ItemType::Money:
			return Money;
		case ItemType::HouseObject:
			return C2D_Color32(0, 0, 0, 0); // Isn't supported in LeafEdit.
		case ItemType::Building:
			return Building;
		case ItemType::ParchedFlower:
			return ParchedFlower;
		case ItemType::WateredFlower:
			return WateredFlower;
		case ItemType::Pattern:
			return Pattern;
		case ItemType::WiltedFlower:
			return WiltedFlower;
		case ItemType::Occupied:
			return Occupied;
		case ItemType::Invalid:
			return Invalid;
	}
	return C2D_Color32(0, 0, 0, 0); // Should not happen.
}

// Get the index of the current Item for the selection.
int ItemManager::getIndex(const u16 &v) {
	if (v == itemDB[0].first || v >= 0xFFF1) {
		return 0;
	}

	int index = -1, min = 0, mid = 0, max = itemDB.size();
	while (min <= max) {
		mid = min + (max - min) / 2;
		if (itemDB[mid].first == v) {
			index = mid;
			break;
		}

		if (itemDB[mid].first < v) {
			min = mid + 1;
		} else {
			max = mid - 1;
		}
	}

	return index >= 0 ? index : 0;
}

// Get the index of the current Item for the selection.
int ItemManager::getIndexString(const int &current, const std::string &v) {
	if (v == "") {
		return current;
	}

	if (v == itemDB[0].second) {
		return 0;
	}

	int index = -1, min = 0, mid = 0, max = itemDB.size();
	while (min <= max) {
		mid = min + (max - min) / 2;
		if (itemDB[mid].second == v) {
			index = mid;
			break;
		} if (itemDB[mid].second < v) {
			min = mid + 1;
		} else {
			max = mid - 1;
		}
	}

	return index >= 0 ? index : 0;
}

u16 ItemManager::selectItem(u16 currentID) {
	std::string itemList;
	int selection = getIndex(currentID);
	int keyRepeatDelay = 0;
	while (1) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, BLACK);
		C2D_TargetClear(Bottom, BLACK);
		GFX::DrawFileBrowseBG(true);
		Gui::DrawStringCentered(0, -2 + barOffset, 0.9, WHITE, "Old Item: " + ItemUtils::getName(currentID), 390);

		for (int i=(selection<8) ? 0 : (int)selection-8;i<(int)itemDB.size()&&i<(((int)selection<8) ? 9 : (int)selection+1);i++) {
			itemList += itemDB[i].second + "\n";
		}
		
		for (uint i=0;i<((itemDB.size()<9) ? 9-itemDB.size() : 0);i++) {
			itemList += "\n";
		}

		// Selector Logic.
		if (selection < 9)	GFX::DrawSelector(true, 24 + ((int)selection * 21));
		else				GFX::DrawSelector(true, 24 + (8 * 21));
		Gui::DrawString(5, 25, 0.85f, BLACK, itemList, 360);
		Gui::DrawStringCentered(0, 217, 0.9f, WHITE, std::to_string(selection + 1) + " | " + std::to_string(itemDB.size()), 395);
		GFX::DrawBottom();
		C3D_FrameEnd(0);

		hidScanInput();
		if (keyRepeatDelay)	keyRepeatDelay--;
		if (hidKeysHeld() & KEY_DOWN && !keyRepeatDelay) {
			if (selection < (int)itemDB.size()-1)	selection++;
			else	selection = 0;
			keyRepeatDelay = 4;
		}

		if (hidKeysHeld() & KEY_UP && !keyRepeatDelay) {
			if (selection > 0)	selection--;
			else if (selection == 0)	selection = (int)itemDB.size()-1;
			keyRepeatDelay = 4;
		}

		if (hidKeysDown() & KEY_A) {
			return itemDB[selection].first;
		}

		if (hidKeysDown() & KEY_B) {
			return currentID;
		}
	}
}