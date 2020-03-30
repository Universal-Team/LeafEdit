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

#include "lang.hpp"

#include <stdio.h>

nlohmann::json appJson;

// New Leaf Vectors.
std::vector<std::string> g_badges;

// Vectors for Both versions.
std::vector<std::string> g_groups;
std::vector<std::string> g_villagerDatabase;
std::vector<std::string> g_personality;

// Wild World Vectors.
std::vector<std::string> g_wwFaceType;
std::vector<std::string> g_wwHairColor;
std::vector<std::string> g_wwHairStyle;

// Load a Text file or such to a Vector.
static void loadToVector(std::string path, std::vector<std::string> &vec) {
	char* line = NULL;
	size_t len = 0;
	vec.clear();

	FILE* in = fopen(path.c_str(), "r");
	while(__getline(&line, &len, in) != -1) {
		if(line[strlen(line)-1] == '\n')	line[strlen(line)-1] = '\0';
		vec.push_back(line);
	}
	fclose(in);
}

std::string Lang::get(const std::string &key) {
	if(!appJson.contains(key)) {
		return "MISSING: " + key;
	}
	return appJson.at(key).get_ref<const std::string&>();
}

std::string langs[] = {"de", "en", "es", "fr", "it", "lt", "pt", "jp"};

void Lang::loadWW(int lang) {
	loadToVector("romfs:/lang/"+langs[lang]+"/wwFaceType.txt", g_wwFaceType);
	loadToVector("romfs:/lang/"+langs[lang]+"/wwHairColor.txt", g_wwHairColor);
	loadToVector("romfs:/lang/"+langs[lang]+"/wwHairStyle.txt", g_wwHairStyle);
	loadToVector("romfs:/lang/"+langs[lang]+"/wwVillager.txt", g_villagerDatabase);
	loadToVector("romfs:/lang/"+langs[lang]+"/wwPersonalities.txt", g_personality);
}

void Lang::loadNL(int lang) {
	loadToVector("romfs:/lang/"+langs[1]+"/badges.txt", g_badges);
	loadToVector("romfs:/lang/"+langs[lang]+"/groups.txt", g_groups);
	loadToVector("romfs:/lang/"+langs[lang]+"/personalities.txt", g_personality);
	loadToVector("romfs:/lang/"+langs[lang]+"/villager.txt", g_villagerDatabase);
}

void Lang::load(int lang) {
	FILE* values;
	values = fopen(("romfs:/lang/"+langs[lang]+"/app.json").c_str(), "rt");
	if(values)	appJson = nlohmann::json::parse(values, nullptr, false);
	fclose(values);
}