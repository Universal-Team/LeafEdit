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

#ifndef _LEAFEDIT_CONFIG_HPP
#define _LEAFEDIT_CONFIG_HPP

#include "json.hpp"

#include <nds.h>
#include <string>

class Config {
public:
	Config();
	void save();
	void initialize();
	void addMissingThings();

	// Language.
	int language() { return this->v_language; }
	void language(int v) { this->v_language = v; if (!this->changesMade) this->changesMade = true; }
	// Do Backups.
	bool createBackups() { return this->v_createBackups; }
	void createBackups(bool v) { this->v_createBackups = v; if (!this->changesMade) this->changesMade = true; }
	// Keyboard Layout.
	int kbLayout() { return this->v_kbLayout; }
	void kbLayout(int v) { this->v_kbLayout = v; if (!this->changesMade) this->changesMade = true; }
	// Keyboard X Position.
	int kbXPos() { return this->v_kbXPos; }
	void kbXPos(int v) { this->v_kbXPos = v; if (!this->changesMade) this->changesMade = true; }
	// Keyboard Directions.
	int kbDirections() { return this->v_kbDirections; }
	void kbDirections(int v) { this->v_kbDirections = v; if (!this->changesMade) this->changesMade = true; }
	// Keyboard Group Amount.
	int kbGroupAmount() { return this->v_kbGroupAmount; }
	void kbGroupAmount(int v) { this->v_kbGroupAmount = v; if (!this->changesMade) this->changesMade = true; }
	// Version.
	int version() { return this->v_version; }
	void version(int v) { this->v_version = v; if (!this->changesMade) this->changesMade = true; }

	// Mainly helper.
	bool getBool(const std::string &key);
	void setBool(const std::string &key, bool v);
	int getInt(const std::string &key);
	void setInt(const std::string &key, int v);
	std::string getString(const std::string &key);
	void setString(const std::string &key, const std::string &v);
private:
	nlohmann::json json; // Our private JSON file.
	bool changesMade = false;

	// variables for the config.
	bool v_createBackups;
	int v_language, v_kbGroupAmount, v_kbLayout, v_kbXPos, v_kbDirections, v_version;

	int configVersion = 1; // Update this by changes.
};

#endif