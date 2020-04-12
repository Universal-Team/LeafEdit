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

#include "encryptedInt32.hpp"
#include "Sav.hpp"
#include "saveUtils.hpp"
#include "scriptHelper.hpp"

#include <nds.h>

extern std::shared_ptr<Sav> save;

// Get String of the Script.
std::string ScriptHelper::getString(nlohmann::json json, const std::string &key, const std::string &key2) {
	if(!json.contains(key))						return "";
	if(!json.at(key).is_object())				return "";
	if(!json.at(key).contains(key2))			return "";
	if(!json.at(key).at(key2).is_string())		return "";

	return json.at(key).at(key2).get_ref<const std::string&>();
}

// Get int of the Script.
int ScriptHelper::getNum(nlohmann::json json, const std::string &key, const std::string &key2) {
	if(!json.contains(key))						return -1;
	if(!json.at(key).is_object())				return -1;
	if(!json.at(key).contains(key2))			return -1;
	if(!json.at(key).at(key2).is_number())		return -1;
	return json.at(key).at(key2).get_ref<const int64_t&>();
}
// Check for Validate.
bool ScriptHelper::checkIfValid(const std::string scriptFile, int Mode) {
	FILE* file = fopen(scriptFile.c_str(), "rt");
	if(!file) {
		printf("File not found\n");
		return false;
	}
	nlohmann::json json = nlohmann::json::parse(file, nullptr, false);
	fclose(file);
	// Check if "scriptInfo" exist.
	if (Mode == 0) {
		if (!json.contains("ScriptInfo"))	return false;
	} else if (Mode == 1) {
		if (!json.contains("ScriptContent"))	return false;
	}
	return true;
}
// Check if Version is correct for the Script usage. Do not execute if not. Right now this causes assert errors.
bool ScriptHelper::isCorrectVersion(const std::vector<ScriptInfo> &info, int entry) {
	switch(save->getType()) {
		case SaveType::WW:
			if (std::find(info[entry].Games.begin(), info[entry].Games.end(), "WW") != info[entry].Games.end()) return true;
			else return false;
		case SaveType::NL:
		case SaveType::WA:
		case SaveType::UNUSED:
			return false;
	}
	return false;
}

// Check if Version is proper for the Script.
bool ScriptHelper::checkVersion(const nlohmann::json &info, int entry) {
	std::vector<std::string> Games = info["ScriptContent"][entry]["Info"]["Games"].get<std::vector<std::string>>();
	switch(save->getType()) {
		case SaveType::WW:
			if (std::find(Games.begin(), Games.end(), "WW") != Games.end()) return true;
			else return false;
		case SaveType::NL:
		case SaveType::WA:
		case SaveType::UNUSED:
			return false;
	}
	return false;
}
// Check if Region is correct for the Script usage. Do not execute if not.
bool ScriptHelper::isCorrectRegion(const std::vector<ScriptInfo> &info, int entry) {
	if (save->getType() == SaveType::WW) {
		switch(save->getRegion()) {
			case WWRegion::EUR:
				if (std::find(info[entry].Regions.begin(), info[entry].Regions.end(), "EUR") != info[entry].Regions.end()) return true;
				if (std::find(info[entry].Regions.begin(), info[entry].Regions.end(), "USA") != info[entry].Regions.end()) return true;
				else return false;
			case WWRegion::JPN:
				if (std::find(info[entry].Regions.begin(), info[entry].Regions.end(), "JPN") != info[entry].Regions.end()) return true;
				else return false;
			case WWRegion::KOR:
				if (std::find(info[entry].Regions.begin(), info[entry].Regions.end(), "KOR") != info[entry].Regions.end()) return true;
				else return false;
		}
	}
	return false;
}

// Check if Region is correct for the Script usage.
bool ScriptHelper::checkRegion(const nlohmann::json &info, int entry) {
	if (save->getType() == SaveType::WW) {
		std::vector<std::string> Regions = info["ScriptContent"][entry]["Info"]["Regions"].get<std::vector<std::string>>();
		switch(save->getRegion()) {
			case WWRegion::EUR:
				if (std::find(Regions.begin(), Regions.end(), "EUR") != Regions.end()) return true;
				if (std::find(Regions.begin(), Regions.end(), "USA") != Regions.end()) return true;
				else return false;
			case WWRegion::JPN:
				if (std::find(Regions.begin(), Regions.end(), "JPN") != Regions.end()) return true;
				else return false;
			case WWRegion::KOR:
				if (std::find(Regions.begin(), Regions.end(), "KOR") != Regions.end()) return true;
				else return false;
		}
	}
	return false;
}

// TODO: IMPROVEMENTS!!!
void ScriptHelper::run(const nlohmann::json &json, int Selection) {
	for(int i=0;i<(int)json.at("ScriptContent").at(Selection).at("Script").size();i++) {
		std::string type = json.at("ScriptContent").at(Selection).at("Script").at(i).at("type");
		if(type == "WriteU8") {
			bool missing = false;
			std::string Offset;
			u32 offset; u8 data;
			if(json.at("ScriptContent").at(Selection).at("Script").at(i).contains("Offset"))	Offset = json.at("ScriptContent").at(Selection).at("Script").at(i).at("Offset");
			else missing = true;
			if(json.at("ScriptContent").at(Selection).at("Script").at(i).contains("Data"))	data = json.at("ScriptContent").at(Selection).at("Script").at(i).at("Data");
			else missing = true;
			offset = (u32)std::stoi(Offset, 0, 16);
			if (!missing)	save->savePointer()[offset] = data; // Write U8.

		} else if(type == "WriteU16") {
			bool missing = false;
			std::string Offset;
			u32 offset; u16 data;
			if(json.at("ScriptContent").at(Selection).at("Script").at(i).contains("Offset"))	Offset = json.at("ScriptContent").at(Selection).at("Script").at(i).at("Offset");
			else missing = true;
			if(json.at("ScriptContent").at(Selection).at("Script").at(i).contains("Data"))	data = json.at("ScriptContent").at(Selection).at("Script").at(i).at("Data");
			else missing = true;
			offset = std::stoi(Offset, 0, 16);
			if (!missing)	SaveUtils::Write<u16>(save->savePointer(), offset, data); // Write U16.

		} else if(type == "WriteU32") {
			bool missing = false;
			std::string Offset;
			u32 offset; u32 data;
			if(json.at("ScriptContent").at(Selection).at("Script").at(i).contains("Offset"))	Offset = json.at("ScriptContent").at(Selection).at("Script").at(i).at("Offset");
			else missing = true;
			if(json.at("ScriptContent").at(Selection).at("Script").at(i).contains("Data"))	data = json.at("ScriptContent").at(Selection).at("Script").at(i).at("Data");
			else missing = true;
			offset = std::stoi(Offset, 0, 16);
			if (!missing)	SaveUtils::Write<u32>(save->savePointer(), offset, data); // Write U32.

		} else if(type == "WriteU64") {
			bool missing = false;
			std::string Offset;
			u32 offset; u64 data;
			if(json.at("ScriptContent").at(Selection).at("Script").at(i).contains("Offset"))	Offset = json.at("ScriptContent").at(Selection).at("Script").at(i).at("Offset");
			else missing = true;
			if(json.at("ScriptContent").at(Selection).at("Script").at(i).contains("Data"))	data = json.at("ScriptContent").at(Selection).at("Script").at(i).at("Data");
			else missing = true;
			offset = (u32)std::stoi(Offset, 0, 16);
			if (!missing)	SaveUtils::Write<u64>(save->savePointer(), offset, data); // Write U64.

		} else if(type == "WriteENC32") {
			bool missing = false;
			std::string Offset;
			u32 offset; EncryptedInt32 data1; u32 data; u32 ENC1, ENC2;

			if(json.at("ScriptContent").at(Selection).at("Script").at(i).contains("Offset"))	Offset = json.at("ScriptContent").at(Selection).at("Script").at(i).at("Offset");
			else missing = true;
			if(json.at("ScriptContent").at(Selection).at("Script").at(i).contains("Data"))	data = json.at("ScriptContent").at(Selection).at("Script").at(i).at("Data");
			else missing = true;
			// Get Data etc.
			offset = (u32)std::stoi(Offset, 0, 16);
			if(!missing)	data1 = EncryptedInt32(SaveUtils::Read<u64>(save->savePointer(), offset));
			// Actual Set & Write part.
			if(!missing)	data1.value = data;
			if(!missing)	data1.encrypt(ENC1, ENC2); // Encrypt.
			if (!missing) {
				SaveUtils::Write<u32>(save->savePointer(), offset, ENC1);
				SaveUtils::Write<u32>(save->savePointer(), offset + 0x04, ENC2);
			}
		}
	}
}