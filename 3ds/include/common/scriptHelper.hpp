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

#ifndef SCRIPTHELPER_HPP
#define SCRIPTHELPER_HPP

#include "json.hpp"

#include <3ds.h>
#include <string>

// Script Struct which contains all information. :P
struct ScriptInfo {
	std::string Author;
	std::string Description;
	std::vector<std::string> Games;
	std::vector<std::string> Regions;
	std::string Title;
	bool Valid;
};

#define SCRIPT_VERSION	1

namespace ScriptHelper {
	// Getter.
	std::string getString(nlohmann::json json, const std::string &key, const std::string &key2);
	int getNum(nlohmann::json json, const std::string &key, const std::string &key2);
	// Compare's.
	bool checkIfValid(const std::string scriptFile, int Mode = 0);
	// General Script check.
	bool isCorrectVersion(const std::vector<ScriptInfo> &info, int entry);
	bool isCorrectRegion(const std::vector<ScriptInfo> &info, int entry);
	// Entry Check.
	bool checkVersion(const nlohmann::json &info, int entry);
	bool checkRegion(const nlohmann::json &info, int entry);
	void run(const nlohmann::json &json, int Selection);
}

#endif