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

#include "common/json.hpp"

#include "core/management/scriptManagement.hpp"

#include "core/save/offsets.h"
#include "core/save/save.h"

#include "gui/msg.hpp"

#include "lang/lang.hpp"

extern Save* SaveFile;

// Get String of the Script.
std::string ScriptManagement::getString(nlohmann::json json, const std::string &key, const std::string &key2) {
	if(!json.contains(key))	return "MISSING: " + key;
	if(!json.at(key).is_object())	return "NOT OBJECT: " + key;

	if(!json.at(key).contains(key2))	return "MISSING: " + key + "." + key2;
	if(!json.at(key).at(key2).is_string())	return "NOT STRING: " + key + "." + key2;

	return json.at(key).at(key2).get_ref<const std::string&>();
}

// Get int of the Script.
int ScriptManagement::getNum(nlohmann::json json, const std::string &key, const std::string &key2) {
	if(!json.contains(key))	return 0;
	if(!json.at(key).is_object())	return 0;

	if(!json.at(key).contains(key2))	return 0;
	if(!json.at(key).at(key2).is_number())	return 0;
	return json.at(key).at(key2).get_ref<const int64_t&>();
}

// json -> The JSON file.
// choice -> Selected Function.
void ScriptManagement::executeScript(nlohmann::json &json, std::string choice) {
	for(int i=0;i<(int)json.at(choice).size();i++) {
		std::string type = json.at(choice).at(i).at("type");

		if (type == "Wallet") {
			bool missing = false;	int player;	u32 amount;
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("amount") && json.at(choice).at(i).at("amount").is_number()
				&& json.at(choice).at(i).at("amount") < 100000 && json.at(choice).at(i).at("amount") > -1)

			amount = json.at(choice).at(i).at("amount");
			else	missing = true;

			if (!missing)	SaveFile->players[player-1]->Wallet.value = amount;
		}
	}
	Msg::DisplayWarnMsg(Lang::get("DONE"));
}

bool ScriptManagement::checkIfValid(std::string scriptFile) {
	FILE* file = fopen(scriptFile.c_str(), "rt");
	if(!file) {
		printf("File not found\n");
		return false;
	}
	nlohmann::json json = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	if (!json.contains("scriptInfo"))	return false;
	
	return true;
}