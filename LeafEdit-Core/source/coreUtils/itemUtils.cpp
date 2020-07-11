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
#include "LeafEditCore.h"
#include "stringUtils.hpp"
#include "types.hpp"

#include <fstream>
#include <map>
#include <string>
#include <unistd.h>
#include <vector>

std::map<u16, std::string> ItemDatabase;
std::vector<std::pair<u16, std::string>> itemDB;

// Load the Item Database.
void ItemUtils::LoadDatabase(SaveType save) {
	ItemDatabase.clear();
	itemDB.clear();

	std::string path; // Path.
	// If SaveType is Wild World -> Check.
	if (save == SaveType::WW) {
		#if defined(WW_ITEM_PATH)
			path = LANG_FOLDER WW_ITEM_PATH;
		#else
			return;
		#endif

	// If SaveType is New Leaf -> Check.
	} else if (save == SaveType::NL) {
		#if defined(NL_ITEM_PATH)
			path = LANG_FOLDER NL_ITEM_PATH;
		#else
			return;
		#endif

	// If SaveType is Welcome amiibo -> Check.
	} else if (save == SaveType::WA) {
		#if defined(WA_ITEM_PATH)
			path = LANG_FOLDER WA_ITEM_PATH;
		#else
			return;
		#endif

	// Nope sorry. Invalid SaveType.
	} else { return; }
	// Check if File even exist!
	if ((access(path.c_str(), F_OK) != 0)) {
		return;
		// Item File is found, load!
	} else {
		std::string currentLine;
		std::ifstream itemDatabase(path.c_str(), std::ifstream::in);

		std::string itemIdStr;
		std::string itemName;

		while (std::getline(itemDatabase, currentLine)) {
			if (currentLine.size() > 8 && currentLine.find("//") == std::string::npos) { // confirm we don't have any comments
				itemIdStr = currentLine.substr(2, 4); 	// skip the 0x hex specifier
				itemName = currentLine.substr(8, currentLine.size());

				// Convert itemIdStr to a u16
				u16 itemId = StringUtils::strToU16(itemIdStr);

				// Add item to the database
				ItemDatabase.insert(std::make_pair(itemId, itemName));
				itemDB.push_back(std::make_pair(itemId, itemName));
			}
		}

		itemDatabase.close();
	}
}

// Get an Item's name.
std::string ItemUtils::getName(u16 ID) {
	if (ItemDatabase.empty()) return "???";
	for (auto const& entry : ItemDatabase) {
		if (entry.first == ID) {
			return entry.second;
		}
	}

	return std::string("???");
}