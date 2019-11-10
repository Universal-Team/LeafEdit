/*
*   This file is part of LeafEdit
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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

#include "common/config.hpp"
#include "common/json.hpp"

#include "gui/colors.hpp"

#include "lang/lang.hpp"

#include <3ds.h>
#include <string>
#include <unistd.h>


// Config stuff for the Sprites.
int Config::barText;
int Config::buttonText;
int Config::bgText;
int Config::boxText;
int Config::fileBrowseText;
int Config::MessageText;
int Config::helpMsg;

int Config::Color1;
int Config::Color2;
int Config::Color3;


nlohmann::json configJson;

void Config::load() {
	FILE* file = fopen("sdmc:/LeafEdit/Settings.json", "r");
	if(file)	configJson = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	Config::Color1 = ColorHelper::getColor(getString("Color1"));
	Config::Color2 = ColorHelper::getColor(getString("Color2"));
	Config::Color3 = ColorHelper::getColor(getString("Color3"));
	Config::barText = ColorHelper::getColor(getString("TextColor"));
	Config::buttonText = ColorHelper::getColor(getString("TextColor"));
	Config::bgText = ColorHelper::getColor(getString("TextColor"));
	Config::boxText = ColorHelper::getColor(getString("TextColor"));
	Config::fileBrowseText = ColorHelper::getColor(getString("TextColor"));
	Config::MessageText = ColorHelper::getColor(getString("TextColor"));
	Config::helpMsg = ColorHelper::getColor(getString("TextColor"));
}


void Config::save() {
	FILE* file = fopen("sdmc:/LeafEdit/Settings.json", "w");
	if(file)	fwrite(configJson.dump(1, '\t').c_str(), 1, configJson.dump(1, '\t').size(), file);
	fclose(file);
}

// If no Settings File is found, set a default one. ;)
void Config::initializeNewConfig() {
	FILE* file = fopen("sdmc:/LeafEdit/Settings.json", "r");
	if(file)	configJson = nlohmann::json::parse(file, nullptr, false);
	fclose(file);
	setString("Color1", "#547239");
	setString("Color2", "#a8c060");
	setString("Color3", "#2c4d26");
	setString("TextColor", "#ffffff");
	setBool("updateCheck", false);
	setBool("update", false);
	setBool("Logging", false);
	setInt("lang", 1);
	setInt("LangLocation", 0);
	save();
}


bool Config::getBool(const std::string &key) {
	if(!configJson.contains(key)) {
		return false;
	}
	return configJson.at(key).get_ref<const bool&>();
}
void Config::setBool(const std::string &key, bool v) {
	configJson[key] = v;
}

int Config::getInt(const std::string &key) {
	if(!configJson.contains(key)) {
		return 0;
	}
	return configJson.at(key).get_ref<const int64_t&>();
}
void Config::setInt(const std::string &key, int v) {
	configJson[key] = v;
}

std::string Config::getString(const std::string &key) {
	if(!configJson.contains(key)) {
		return "";
	}
	return configJson.at(key).get_ref<const std::string&>();
}
void Config::setString(const std::string &key, const std::string &v) {
	configJson[key] = v;
}

int Config::getLang(const std::string &key) {
	if(!configJson.contains(key)) {
		return 1;
	}
	return configJson.at(key).get_ref<const int64_t&>();
}