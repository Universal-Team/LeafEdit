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

#include "config.hpp"

std::string Config::currentRelease;
std::string Config::currentNightly;
bool Config::newStyle;

nlohmann::json configJson;

void Config::load() {
	FILE* file = fopen("sdmc:/LeafEdit/Settings.json", "r");
	if (file)	configJson = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	if (!configJson.contains("ReleaseVersion")) {
		Config::currentRelease = "";
	} else {
		Config::currentRelease = getString("ReleaseVersion");
	}

	if (!configJson.contains("NightlyVersion")) {
		Config::currentNightly = "";
	} else {
		Config::currentNightly = getString("NightlyVersion");
	}

	if (!configJson.contains("NewStyle")) {
		Config::newStyle = true;
	} else {
		Config::newStyle = getBool("NewStyle");
	}
}


void Config::save() {
	// Versions.
	Config::setString("ReleaseVersion", Config::currentRelease);
	Config::setString("NightlyVersion", Config::currentNightly);
	Config::setBool("NewStyle", Config::newStyle);

	FILE* file = fopen("sdmc:/LeafEdit/Settings.json", "w");
	if(file)	fwrite(configJson.dump(1, '\t').c_str(), 1, configJson.dump(1, '\t').size(), file);
	fclose(file);
}

// If no Settings File is found, set a default one. ;)
void Config::initializeNewConfig() {
	FILE* file = fopen("sdmc:/LeafEdit/Settings.json", "r");
	if(file)	configJson = nlohmann::json::parse(file, nullptr, false);
	fclose(file);
	setString("ReleaseVersion", "");
	setString("NightlyVersion", "");
	Config::setBool("NewStyle", true);
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