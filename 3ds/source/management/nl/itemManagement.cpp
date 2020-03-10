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

#include "itemManagement.hpp"
#include "screenCommon.hpp"
#include "utils.hpp"

#include <fstream>

extern C2D_SpriteSheet Items;

extern std::map<u16, std::string> g_itemDatabase;
//std::map<std::string, std::map<u16, std::string>> g_sortedItemDatabase;

// Draw the Item sprite.
void ItemManagement::DrawItem(u16 ItemID, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(Items, ItemID, x, y, ScaleX, ScaleY);
}

std::string itemLanguages[] = {"de", "en", "es", "fr", "it", "jp", "lt", "pt"};

void ItemManagement::LoadDatabase(int lang) {
	g_itemDatabase.clear();
	//g_sortedItemDatabase.clear();

	std::string currentLine;
	std::ifstream itemDatabase("romfs:/lang/" + itemLanguages[lang] + "/items.txt", std::ifstream::in);
	std::string itemIdStr;
	std::string itemName;
	std::string sectionName = "None";

	while (std::getline(itemDatabase, currentLine))
	{
		if (currentLine.find("//") == 0)
		{
			sectionName = currentLine.substr(2);
		}
		else if (currentLine.size() > 8 && currentLine.find("//") == std::string::npos)
		{											// confirm we don't have any comments
			itemIdStr = currentLine.substr(2, 4); 	// skip the 0x hex specifier
			itemName = currentLine.substr(8, currentLine.size());

			// Convert itemIdStr to a u16
			u16 itemId = strToU16(itemIdStr);

			// Add item to the database
			g_itemDatabase.insert(std::make_pair(itemId, itemName));
			//g_sortedItemDatabase[sectionName][itemId] = itemName;
		}
	}

	itemDatabase.close();
}

Item it;
// Load | Unload Itembins.
void ItemManagement::loadItems() {
	it.LoadItemBins();
}

void ItemManagement::unloadItems() {
	it.UnloadItemBins();
}