#include "config.hpp"
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
	if (Config::getInt("LangLocation") == 1) {
		values = fopen(("sdmc:/LeafEdit/lang/"+langs[lang]+"/app.json").c_str(), "rt");
	} else {
		values = fopen(("romfs:/lang/"+langs[lang]+"/app.json").c_str(), "rt");
	}
	if(values)	appJson = nlohmann::json::parse(values, nullptr, false);
	fclose(values);
}