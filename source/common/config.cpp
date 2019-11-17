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

int Config::Color1;
int Config::Color2;
int Config::Color3;
int Config::TxtColor;
int Config::lang;


nlohmann::json configJson;

void Config::load() {
	FILE* file = fopen("sdmc:/LeafEdit/Settings.json", "r");
	if(file)	configJson = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

		if(!configJson.contains("BarColor")) {
			Config::Color1 = green2;
		} else {
			Config::Color1 = getInt("BarColor");
		}

		if(!configJson.contains("TopBgColor")) {
			Config::Color2 = green4;
		} else {
			Config::Color2 = getInt("TopBgColor");
		}

		if(!configJson.contains("BottomBgColor")) {
			Config::Color3 = green3;
		} else {
			Config::Color3 = getInt("BottomBgColor");
		}

		if(!configJson.contains("TextColor")) {
			Config::TxtColor = WHITE;
		} else {
			Config::TxtColor = getInt("TextColor");
		}

		if(!configJson.contains("Lang")) {
			Config::lang = 1;
		} else {
			Config::lang = getInt("Lang");
		}
}


void Config::save() {
	Config::setInt("BarColor", Config::Color1);
	Config::setInt("TopBgColor", Config::Color2);
	Config::setInt("BottomBgColor", Config::Color3);
	Config::setInt("TextColor", Config::TxtColor);
	Config::setInt("Lang", Config::lang);
	FILE* file = fopen("sdmc:/LeafEdit/Settings.json", "w");
	if(file)	fwrite(configJson.dump(1, '\t').c_str(), 1, configJson.dump(1, '\t').size(), file);
	fclose(file);
}

// If no Settings File is found, set a default one. ;)
void Config::initializeNewConfig() {
	FILE* file = fopen("sdmc:/LeafEdit/Settings.json", "r");
	if(file)	configJson = nlohmann::json::parse(file, nullptr, false);
	fclose(file);
	setInt("BarColor", green2);
	setInt("TopBgColor", green4);
	setInt("BottomBgColor", green3);
	setInt("TextColor", WHITE);
	setInt("Lang", 1);
	if(file)	fwrite(configJson.dump(1, '\t').c_str(), 1, configJson.dump(1, '\t').size(), file);
	fclose(file);
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