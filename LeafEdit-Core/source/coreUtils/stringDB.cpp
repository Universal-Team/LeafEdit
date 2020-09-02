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

#include "stringDB.hpp"
#include "stringUtils.hpp"
#include <vector>

std::vector<std::tuple<u16, std::string, std::string>> itemDB;
std::vector<std::string> itemCategories;
std::vector<std::tuple<u16, std::string, std::string>> villagerDB;
std::vector<std::string> villagerSpecies;

#define WW_ITEM_PATH "romfs:/lang/strings/ww/items.txt"
#define NL_ITEM_PATH "romfs:/lang/strings/nl/items.txt"
#define WA_ITEM_PATH "romfs:/lang/strings/wa/items.txt"

#define WW_VILLAGER_PATH "romfs:/lang/strings/ww/villager.txt"
#define NL_VILLAGER_PATH "romfs:/lang/strings/nl/villager.txt"
#define WA_VILLAGER_PATH "romfs:/lang/strings/wa/villager.txt"

/* Load the Item Database. */
void StringDB::LoadItemDatabase(SaveType save) {
	itemDB.clear();
	itemCategories.clear();

	std::string path; // Path.
	switch(save) {
		case SaveType::WW:
			path = WW_ITEM_PATH;
			break;
		case SaveType::NL:
			path = NL_ITEM_PATH;
			break;
		case SaveType::WA:
			path = WA_ITEM_PATH;
			break;
		case SaveType::UNUSED:
			return;
			break;
	}

	std::string currentLine;
	std::ifstream itemDatabase(path.c_str(), std::ifstream::in);

	std::string itemIdStr;
	std::string categoryName;
	std::string itemName;

	while (std::getline(itemDatabase, currentLine)) {
		/* Push category. */
		if (currentLine.find("//") == 0) {
			categoryName = currentLine.substr(2);
			itemCategories.push_back({categoryName});

		/* confirm we don't have any comments. */
		} else if (currentLine.size() > 8 && currentLine.find("//") == std::string::npos) { 
			itemIdStr = currentLine.substr(2, 4); // skip the 0x hex specifier.
			itemName = currentLine.substr(8, currentLine.size());

			/* Convert itemIdStr to a u16. */
			u16 itemId = StringUtils::strToU16(itemIdStr);

			/* Add item to the database. */
			itemDB.push_back(std::make_tuple(itemId, itemName, categoryName));
		}
	}

	itemDatabase.close();
}

/* Load the Villager Database. */
void StringDB::LoadVillagerDatabase(SaveType save) {
	villagerDB.clear();

	std::string path; // Path.
	switch(save) {
		case SaveType::WW:
			path = WW_VILLAGER_PATH;
			break;
		case SaveType::NL:
			path = NL_VILLAGER_PATH;
			break;
		case SaveType::WA:
			path = WA_VILLAGER_PATH;
			break;
		case SaveType::UNUSED:
			return;
			break;
	}

	std::string currentLine;
	std::ifstream villagerDatabase(path.c_str(), std::ifstream::in);

	std::string villagerIdStr;
	std::string categoryName;
	std::string villagerName;

	while (std::getline(villagerDatabase, currentLine)) {
		/* Push category. */
		if (currentLine.find("//") == 0) {
			categoryName = currentLine.substr(2);
			villagerSpecies.push_back({categoryName});

		/* confirm we don't have any comments. */
		} else if (currentLine.size() > 8 && currentLine.find("//") == std::string::npos) { 
			villagerIdStr = currentLine.substr(2, 4); // skip the 0x hex specifier.
			villagerName = currentLine.substr(8, currentLine.size());

			/* Convert villagerIdStr to a u16. */
			u16 villagerID = StringUtils::strToU16(villagerIdStr);

			/* Add villager to the database. */
			villagerDB.push_back(std::make_tuple(villagerID, villagerName, categoryName));
		}
	}

	villagerDatabase.close();
}

/* Searching. */
std::vector<std::tuple<u16, std::string, std::string>> StringDB::searchTuple(std::string searchResult, std::string searchCategory, std::vector<std::tuple<u16, std::string, std::string>> &searchType) {
	std::vector<std::tuple<u16, std::string, std::string>> temp, tempEnd;

	/* Push categories first. */
	if (searchCategory != "") {
		for (int i = 0; i < (int)searchType.size(); i++) {
			if (std::get<2>(searchType[i]).find(searchCategory) != std::string::npos) {
				temp.push_back({searchType[i]});
			}
		}

	} else {
		temp = searchType;
	}


	/* Push search result. */
	if (temp.size() > 0) {
		for (int i = 0; i < (int)temp.size(); i++) {
			if (std::get<1>(temp[i]).find(searchResult) != std::string::npos) {
				tempEnd.push_back({temp[i]});
			}
		}

	} else {
		return searchType;
	}

	return tempEnd;
}