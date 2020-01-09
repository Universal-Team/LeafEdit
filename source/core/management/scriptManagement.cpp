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
#include "common/utils.hpp"

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
void ScriptManagement::executePlayer(nlohmann::json &json, std::string choice) {
	for(int i=0;i<(int)json.at(choice).size();i++) {
		std::string type = json.at(choice).at(i).at("type");

		if (type == "Wallet") {
			bool missing = false;	int player = 1;	u32 amount = 0;
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

		else if (type == "Bank") {
			bool missing = false;	int player = 1;	u32 amount = 0;
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("amount") && json.at(choice).at(i).at("amount").is_number()
				&& json.at(choice).at(i).at("amount") < 1000000000 && json.at(choice).at(i).at("amount") > -1)

			amount = json.at(choice).at(i).at("amount");
			else	missing = true;

			if (!missing)	SaveFile->players[player-1]->BankAmount.value = amount;
		}

		else if (type == "TAN") {
			bool missing = false;	int player = 1;	u32 value = 0;
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("value") && json.at(choice).at(i).at("value").is_number()
				&& json.at(choice).at(i).at("value") < 16 && json.at(choice).at(i).at("value") > -1)

			value = json.at(choice).at(i).at("value");
			else	missing = true;

			if (!missing)	SaveFile->players[player-1]->PlayerTan = value;
		}

		else if (type == "Medals") {
			bool missing = false;	int player = 1;	u32 amount = 0;
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("amount") && json.at(choice).at(i).at("amount").is_number()
				&& json.at(choice).at(i).at("amount") < 10000 && json.at(choice).at(i).at("amount") > -1)

			amount = json.at(choice).at(i).at("amount");
			else	missing = true;

			if (!missing)	SaveFile->players[player-1]->IslandMedals.value = amount;
		}

		else if (type == "Coupons") {
			bool missing = false;	int player = 1;	u32 amount = 0;
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("amount") && json.at(choice).at(i).at("amount").is_number()
				&& json.at(choice).at(i).at("amount") < 10000 && json.at(choice).at(i).at("amount") > -1)

			amount = json.at(choice).at(i).at("amount");
			else	missing = true;

			if (!missing)	SaveFile->players[player-1]->MeowCoupons.value = amount;
		}

		else if (type == "Name") {
			bool missing = false;	int player = 1;	std::u16string name = StringUtils::UTF8toUTF16("");
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("name") && json.at(choice).at(i).at("name").is_string()) {
				std::string strName = json.at(choice).at(i).at("name");
				name = StringUtils::UTF8toUTF16(strName.c_str());
			} else {
				missing = true;
			}

			if (!missing)	SaveFile->players[player-1]->Name = name;
		}

		else if (type == "HairStyle") {
			bool missing = false;	int player = 1;	u8 style = 1;
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("style") && json.at(choice).at(i).at("style").is_number())
				style = json.at(choice).at(i).contains("style");
			else	missing = true;

			if (!missing)	SaveFile->players[player-1]->hairStyle = style;
		}

		else if (type == "HairColor") {
			bool missing = false;	int player = 1;	u8 color = 1;
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("color") && json.at(choice).at(i).at("color").is_number())
				color = json.at(choice).at(i).contains("color");
			else	missing = true;

			if (!missing)	SaveFile->players[player-1]->hairColor = color;
		}

		else if (type == "EyeColor") {
			bool missing = false;	int player = 1;	u8 color = 1;
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("color") && json.at(choice).at(i).at("color").is_number())
				color = json.at(choice).at(i).contains("color");
			else	missing = true;

			if (!missing)	SaveFile->players[player-1]->eyeColor = color;
		}

		else if (type == "Face") {
			bool missing = false;	int player = 1;	u8 face = 1;
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("face") && json.at(choice).at(i).at("face").is_number())
				face = json.at(choice).at(i).contains("face");
			else	missing = true;

			if (!missing)	SaveFile->players[player-1]->face = face;
		}

		else if (type == "Gender") {
			bool missing = false;	int player = 1;	u16 gender = 0; // 0 -> Male.
			if(json.at(choice).at(i).contains("player") && json.at(choice).at(i).at("player").is_number()
				&& json.at(choice).at(i).at("player") < 5 && json.at(choice).at(i).at("player") > 0)

			player = json.at(choice).at(i).at("player");
			else	missing = true;

			if(json.at(choice).at(i).contains("gender") && json.at(choice).at(i).at("gender").is_boolean()
				&& json.at(choice).at(i).contains("gender") == true)
				gender = 1;
			else	missing = true;

			if (!missing)	SaveFile->players[player-1]->Gender = gender;
		}

	}
	Msg::DisplayWarnMsg(Lang::get("DONE"));
}

void ScriptManagement::executeVillager(nlohmann::json &json, std::string choice) {
	for(int i=0;i<(int)json.at(choice).size();i++) {
		std::string type = json.at(choice).at(i).at("type");

		if (type == "Replace") {
			bool missing = false;	int villager = 1;	u16 ID = 0;
			if(json.at(choice).at(i).contains("villager") && json.at(choice).at(i).at("villager").is_number()
				&& json.at(choice).at(i).at("villager") < 11 && json.at(choice).at(i).at("villager") > 0)

			villager = json.at(choice).at(i).at("villager");
			else	missing = true;

			if(json.at(choice).at(i).contains("ID") && json.at(choice).at(i).at("ID").is_number()
				&& json.at(choice).at(i).at("ID") < 399 && json.at(choice).at(i).at("ID") > -1)

			ID = json.at(choice).at(i).at("ID");
			else	missing = true;

			if (!missing)	SaveFile->villagers[villager-1]->SetId(ID);
		}

		else if (type == "Boxed") {
			bool missing = false;	int villager = 1;	u8 isBoxed = 1;
			if(json.at(choice).at(i).contains("villager") && json.at(choice).at(i).at("villager").is_number()
				&& json.at(choice).at(i).at("villager") < 11 && json.at(choice).at(i).at("villager") > 0)

			villager = json.at(choice).at(i).at("villager");
			else	missing = true;

			if(json.at(choice).at(i).contains("boxed") && json.at(choice).at(i).at("boxed").is_number()
				&& json.at(choice).at(i).at("boxed") < 2 && json.at(choice).at(i).at("boxed") > -1)

			isBoxed = json.at(choice).at(i).at("boxed");
			else	missing = true;

			if (!missing)	SaveFile->villagers[villager-1]->setStatus(isBoxed);
		}
	}
	Msg::DisplayWarnMsg(Lang::get("DONE"));
}

void ScriptManagement::executeCustom(nlohmann::json &json, std::string choice) {
	for(int i=0;i<(int)json.at(choice).size();i++) {
		std::string type = json.at(choice).at(i).at("type");
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