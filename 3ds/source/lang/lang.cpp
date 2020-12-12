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

#include "lang.hpp"
#include "stringDB.hpp"

#include <stdio.h>

nlohmann::json appJson;

/* New Leaf Vectors. */
std::vector<std::string> g_badges;
std::vector<std::string> g_EyeColor;

/* Vectors for Both versions. */
std::vector<std::string> g_FaceType;
std::vector<std::string> g_HairColor;
std::vector<std::string> g_HairStyle;
std::vector<std::string> g_personality;


/* Load a Text file or such to a Vector. */
void Lang::loadToVector(std::string path, std::vector<std::string> &vec) {
	char *line = NULL;
	size_t len = 0;
	vec.clear();

	FILE *in = fopen(path.c_str(), "r");
	while(__getline(&line, &len, in) != -1) {
		if (line[strlen(line) - 1] == '\n') line[strlen(line)-1] = '\0';
		vec.push_back(line);
	}

	fclose(in);
}

std::string Lang::get(const std::string &key) {
	if (!appJson.contains(key)) return "";

	return appJson.at(key).get_ref<const std::string&>();
}

const std::string langs[] = { "de", "en", "es", "fr", "it", "lt", "pt", "jp" };

void Lang::loadGameStrings(int lang, SaveType save) {
	switch (save) {
		case SaveType::WW:
			loadToVector("romfs:/lang/strings/ww/en/facetype.txt", g_FaceType);
			loadToVector("romfs:/lang/strings/ww/en/haircolor.txt", g_HairColor);
			loadToVector("romfs:/lang/strings/ww/en/hairstyle.txt", g_HairStyle);
			loadToVector("romfs:/lang/strings/ww/en/personalities.txt", g_personality);
			break;

		case SaveType::NL:
			loadToVector("romfs:/lang/strings/nl/en/badges.txt", g_badges);
			loadToVector("romfs:/lang/strings/nl/en/facetype.txt", g_FaceType);
			loadToVector("romfs:/lang/strings/nl/en/haircolor.txt", g_HairColor);
			loadToVector("romfs:/lang/strings/nl/en/hairstyle.txt", g_HairStyle);
			loadToVector("romfs:/lang/strings/nl/en/personalities.txt", g_personality);
			break;

		case SaveType::WA:
			loadToVector("romfs:/lang/strings/wa/en/badges.txt", g_badges);
			loadToVector("romfs:/lang/strings/wa/en/facetype.txt", g_FaceType);
			loadToVector("romfs:/lang/strings/wa/en/haircolor.txt", g_HairColor);
			loadToVector("romfs:/lang/strings/wa/en/hairstyle.txt", g_HairStyle);
			loadToVector("romfs:/lang/strings/wa/en/personalities.txt", g_personality);
			break;

		case SaveType::HHD:
		case SaveType::UNUSED:
			break;
	}

	StringDB::LoadItemDatabase(save);
	StringDB::LoadVillagerDatabase(save);
}

void Lang::load(int lang) {
	FILE *values;
	values = fopen(("romfs:/lang/" + langs[lang] + "/app.json").c_str(), "rt");
	if (values) appJson = nlohmann::json::parse(values, nullptr, false);
	fclose(values);
}