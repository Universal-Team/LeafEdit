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

#include "core/management/buildingManagement.hpp"

#include <fstream>
#include <map>

extern std::map<u16, std::string> g_buildingDatabase;

std::string buildLanguages[] = {"de", "en", "es", "fr", "it", "jp", "lt", "pt"};

void BuildingManagement::loadDatabase() {
	g_buildingDatabase.clear();

	std::string currentLine;
	std::ifstream buildDatabase("romfs:/lang/" + buildLanguages[1] + "/building.txt", std::ifstream::in);
	std::string buildIdStr;
	std::string buildName;

	while (std::getline(buildDatabase, currentLine))
	{
		if (currentLine.size() > 6)
		{
			buildIdStr = currentLine.substr(2, 2); 	// skip the 0x hex specifier
			buildName = currentLine.substr(6, currentLine.size());

			// Convert buildIdStr to a u16
			u16 buildID = strToU16(buildIdStr);

			// Add building to the database
			g_buildingDatabase.insert(std::make_pair(buildID, buildName));
		}
	}

	buildDatabase.close();
}