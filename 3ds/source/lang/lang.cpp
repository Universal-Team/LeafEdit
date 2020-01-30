#include "common/config.hpp"

#include "lang/lang.hpp"

#include <stdio.h>

nlohmann::json appJson;

std::vector<std::string> g_groups;
std::vector<std::string> g_badges;
std::vector<std::string> g_villagerDatabase;

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

void Lang::load(int lang) {
	loadToVector("romfs:/lang/"+langs[lang]+"/groups.txt", g_groups);
	loadToVector("romfs:/lang/"+langs[1]+"/badges.txt", g_badges);
	FILE* values;
	
	if (Config::getInt("LangLocation") == 1) {
		values = fopen(("sdmc:/LeafEdit/lang/"+langs[lang]+"/app.json").c_str(), "rt");
	} else {
		values = fopen(("romfs:/lang/"+langs[lang]+"/app.json").c_str(), "rt");
	}
	if(values)	appJson = nlohmann::json::parse(values, nullptr, false);
	fclose(values);
}

void Lang::loadVillager(int lang, bool isNewLeaf) {
	if (isNewLeaf)	loadToVector("romfs:/lang/"+langs[lang]+"/villager.txt", g_villagerDatabase);
	else	loadToVector("romfs:/lang/"+langs[lang]+"/wwVillager.txt", g_villagerDatabase);
}