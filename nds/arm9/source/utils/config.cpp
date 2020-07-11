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

#include "config.hpp"
#include "flashcard.hpp"

#include <nds.h>
#include <string>
#include <unistd.h>

// Used to add missing stuff for the JSON.
void Config::addMissingThings() { }

// In case it doesn't exist.
void Config::initialize() {
	// Create through fopen "Write".
	FILE *file = fopen(sdFound() ? "sd:/_nds/LeafEdit/Settings.json" : "fat:/_nds/LeafEdit/Settings.json", "w");
	// Set default values.
	this->setInt("KeyboardDirections", 0);
	this->setInt("KeyboardGroupAmount", 0);
	this->setInt("KeyboardXPos", 0);
	this->setInt("KeyboardLayout", 0);
	this->setInt("Language", 1);
	this->setBool("Create_Backups", true);
	this->setInt("Version", this->configVersion);

	// Write to file.
	const std::string dump = this->json.dump(1, '\t');
	fwrite(dump.c_str(), 1, this->json.dump(1, '\t').size(), file);
	fclose(file); // Now we have the file and can properly access it.
}

Config::Config() {
	if (access(sdFound() ? "sd:/_nds/LeafEdit/Settings.json" : "fat:/_nds/LeafEdit/Settings.json", F_OK) != 0) {
		this->initialize();
	}

	FILE* file = fopen(sdFound() ? "sd:/_nds/LeafEdit/Settings.json" : "fat:/_nds/LeafEdit/Settings.json", "r");
	this->json = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	if (!this->json.contains("Version")) {
		// Let us create a new one.
		this->initialize();
	}

	// Here we add the missing things.
	if (this->json["Version"] < this->configVersion) {
		this->addMissingThings();
	}

	if (!this->json.contains("Language")) {
		this->language(1);
	} else {
		this->language(this->getInt("Language"));
	}

	if (!this->json.contains("KeyboardDirections")) {
		this->kbDirections(0);
	} else {
		this->kbDirections(this->getInt("KeyboardDirections"));
	}

	if (!this->json.contains("KeyboardGroupAmount")) {
		this->kbGroupAmount(0);
	} else {
		this->kbGroupAmount(this->getInt("KeyboardGroupAmount"));
	}


	if (!this->json.contains("KeyboardXPos")) {
		this->kbXPos(0);
	} else {
		this->kbXPos(this->getInt("KeyboardXPos"));
	}

	if (!this->json.contains("KeyboardLayout")) {
		this->kbLayout(0);
	} else {
		this->kbLayout(this->getInt("KeyboardLayout"));
	}

	if (!this->json.contains("Create_Backups")) {
		this->createBackups(true);
	} else {
		this->createBackups(this->getBool("Create_Backups"));
	}

	if (!this->json.contains("Version")) {
		this->version(this->configVersion);
	} else {
		this->version(this->getInt("Version"));
	}

	this->changesMade = false; // No changes made yet.
}


// Write to config if changesMade.
void Config::save() {
	if (this->changesMade) {
		FILE *file = fopen(sdFound() ? "sd:/_nds/LeafEdit/Settings.json" : "fat:/_nds/LeafEdit/Settings.json", "w");
		// Set values.
		this->setInt("KeyboardDirections", this->kbDirections());
		this->setInt("KeyboardGroupAmount", this->kbGroupAmount());
		this->setInt("KeyboardXPos", this->kbXPos());
		this->setInt("KeyboardLayout", this->kbLayout());
		this->setInt("Language", this->language());
		this->setBool("Create_Backups", this->createBackups());
		this->setInt("Version", this->version());
		// Write changes to file.
		const std::string dump = this->json.dump(1, '\t');
		fwrite(dump.c_str(), 1, this->json.dump(1, '\t').size(), file);
		fclose(file);
	}
}


bool Config::getBool(const std::string &key) {
	if (!this->json.contains(key)) {
		return false;
	}

	return this->json.at(key).get_ref<const bool&>();
}

void Config::setBool(const std::string &key, bool v) { this->json[key] = v; }

int Config::getInt(const std::string &key) {
	if (!this->json.contains(key)) {
		return 0;
	}

	return this->json.at(key).get_ref<const int64_t&>();
}

void Config::setInt(const std::string &key, int v) { this->json[key] = v; }

std::string Config::getString(const std::string &key) {
	if (!this->json.contains(key)) {
		return "";
	}

	return this->json.at(key).get_ref<const std::string&>();
}

void Config::setString(const std::string &key, const std::string &v) { this->json[key] = v; }