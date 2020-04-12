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

#include "scriptHelper.hpp"
#include "scriptScreen.hpp"

#include <unistd.h>

extern bool touching(touchPosition touch, Structs::ButtonPos button);

// Return if Scripts are found.
bool ScriptScreen::returnIfExist() {
	dirContents.clear();
	chdir("sdmc:/LeafEdit/Scripts/");
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, {"json"});
	for(uint i=0;i<dirContentsTemp.size();i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}
	if (dirContents.size() == 0) {
		return false;
	}
	return true;
}

ScriptScreen::ScriptScreen() {
	refreshList();
}

void ScriptScreen::Draw(void) const
{
	if (Mode == 0)	DrawList();
	else if (Mode == 1)	DrawEntries();
}
void ScriptScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0)	ListLogic(hDown, hHeld, touch);
	else if (Mode == 1)	EntryLogic(hDown, hHeld, touch);
}

std::vector<std::string> ScriptScreen::parseObjects(std::string Name) {
	FILE* file = fopen(Name.c_str(), "rt");
	if(!file) {
		printf("File not found\n");
		return {{""}};
	}
	nlohmann::json json = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	std::vector<std::string> objs;
	for(auto it = json.begin();it != json.end(); it++) {
		if(it.key() != "ScriptInfo") {
			objs.push_back(it.key());
		}
	}
	return objs;
}

// Parse Informations.
ScriptInfo ScriptScreen::parseInfos(const std::string fileName) {
	FILE* file = fopen(fileName.c_str(), "rt");
	if(!file) {
		printf("File not found\n");
		return {""};
	}
	nlohmann::json json = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	ScriptInfo info;
	info.Author		 = ScriptHelper::getString(json, "ScriptInfo", "Author");
	info.Description = ScriptHelper::getString(json, "ScriptInfo", "Description");
	info.Games = json["ScriptInfo"]["Games"].get<std::vector<std::string>>();
	info.Regions = json["ScriptInfo"]["Regions"].get<std::vector<std::string>>();
	info.Title		 = ScriptHelper::getString(json, "ScriptInfo", "Title");
	int ver = ScriptHelper::getNum(json, "ScriptInfo", "Version");
	if (ver == SCRIPT_VERSION) {
		info.Valid = true;
	} else {
		info.Valid = false;
	}
	return info;
}
// Opens a JSON File and parses it.
nlohmann::json ScriptScreen::openScriptFile() {
	FILE* file = fopen(currentFile.c_str(), "rt");
	nlohmann::json jsonFile;
	if(file)	jsonFile = nlohmann::json::parse(file, nullptr, false);
	fclose(file);
	return jsonFile;
}
// Refresh the Scriptlist and return scriptFound state.
void ScriptScreen::refreshList() {
	if (returnIfExist() == true) {
		Msg::DisplayMsg("Refreshing list...");
		dirContents.clear();
		scriptInfo.clear();
		chdir("sdmc:/LeafEdit/Scripts/");
		getDirectoryContents(dirContents, {"json"});
		for(uint i=0;i<dirContents.size();i++) {
			scriptInfo.push_back(parseInfos(dirContents[i].name));
		}
		Selection = 0;
		Mode = 0;
		ScriptsFound = true;
	} else {
		Selection = 0;
		Mode = 0;
		ScriptsFound = false;
	}
}

// Script Selection list.
void ScriptScreen::DrawList(void) const {
	if (ScriptsFound) {
		std::string line1;
		std::string line2;
		std::string scriptAmount = std::to_string(Selection +1) + " | " + std::to_string(scriptInfo.size());
		GFX::DrawTop();
		Gui::DrawStringCentered(0, -2, 0.9f, WHITE, scriptInfo[Selection].Title, 395);
		Gui::DrawString(397-Gui::GetStringWidth(0.7f, scriptAmount), 239-Gui::GetStringHeight(0.7f, scriptAmount), 0.7f, BLACK, scriptAmount);

		Gui::DrawStringCentered(0, 50, 0.9f, BLACK, "Author: " + scriptInfo[Selection].Author, 395);
		Gui::DrawStringCentered(0, 80, 0.9f, BLACK, "Game: " + scriptInfo[Selection].Games[0], 395);
		Gui::DrawStringCentered(0, 110, 0.9f, BLACK, "Region: " + scriptInfo[Selection].Regions[0], 395);
		Gui::DrawStringCentered(0, 140, 0.9f, BLACK, "Desc: " + scriptInfo[Selection].Description, 395);

		GFX::DrawBottom();
		for(int i=0;i<3 && i<(int)scriptInfo.size();i++) {
			Gui::Draw_Rect(0, 40+(i*57), 320, 45, DARKER_COLOR);
			line1 = scriptInfo[screenPos + i].Title;
			line2 = scriptInfo[screenPos + i].Author;
			if(screenPos + i == Selection) {
				Gui::Draw_Rect(0, 40+(i*57), 320, 45, LIGHTER_COLOR);
			}
			Gui::DrawStringCentered(0, 38+(i*57), 0.7f, WHITE, line1, 310);
			Gui::DrawStringCentered(0, 62+(i*57), 0.7f, WHITE, line2, 310);
		}
	} else {
		GFX::DrawTop();
		Gui::DrawStringCentered(0, -2, 0.8f, WHITE, "No Scripts found!", 395);
		GFX::DrawBottom();
	}
}

void ScriptScreen::DrawEntries(void) const {
	std::string line1;
	std::string scriptAmount = std::to_string(Selection +1) + " | " + std::to_string((int)scriptJson.at("ScriptContent").size());
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, std::string(scriptJson["ScriptInfo"]["Title"]), 395);
	Gui::DrawString(397-Gui::GetStringWidth(0.7f, scriptAmount), 239-Gui::GetStringHeight(0.7f, scriptAmount), 0.7f, BLACK, scriptAmount);

	Gui::DrawStringCentered(0, 50, 0.9f, BLACK, "Title: " + std::string(scriptJson["ScriptContent"][Selection]["Info"]["Title"]), 395);
	Gui::DrawStringCentered(0, 80, 0.9f, BLACK, "Desc: " + std::string(scriptJson["ScriptContent"][Selection]["Info"]["Description"]), 395);

	GFX::DrawBottom();
	for(int i=0;i<3 && i<(int)scriptJson.at("ScriptContent").size();i++) {
		Gui::Draw_Rect(0, 40+(i*57), 320, 45, DARKER_COLOR);
		line1 = scriptJson["ScriptContent"][screenPos + i]["Info"]["Title"];
		if(screenPos + i == Selection) {
			Gui::Draw_Rect(0, 40+(i*57), 320, 45, LIGHTER_COLOR);
		}
		Gui::DrawStringCentered(0, 50+(i*57), 0.7f, WHITE, line1, 310);
	}
}

void ScriptScreen::ListLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (keyRepeatDelay)	keyRepeatDelay--;

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (Selection < (int)scriptInfo.size()-1) {
			Selection++;
		} else {
			Selection = 0;
		}
		if (fastMode == true) {
			keyRepeatDelay = 8;
		} else if (fastMode == false){
			keyRepeatDelay = 8;
		}
	}
	if (hHeld & KEY_UP && !keyRepeatDelay) {
		if (Selection > 0) {
			Selection--;
		} else {
			Selection = (int)scriptInfo.size()-1;
		}
		if (fastMode == true) {
			keyRepeatDelay = 8;
		} else if (fastMode == false){
			keyRepeatDelay = 8;
		}
	}

	if (hDown & KEY_A) {
		if (ScriptsFound) {
			// Make sure this is not a Directory.
			if (!dirContents[Selection].isDirectory) {
				// Make sure the vector's size is not 0.
				if (scriptInfo.size() != 0) {
					// Make sure the Script is not invalid!
					if (ScriptHelper::checkIfValid(dirContents[Selection].name, 1)) {
						// Check for Correct Version.
						if (ScriptHelper::isCorrectVersion(scriptInfo, Selection)) {
							// Check for Correct Region.
							if (ScriptHelper::isCorrectRegion(scriptInfo, Selection)) {
								currentFile = dirContents[Selection].name;
								scriptJson = openScriptFile();
								scriptEntries = parseObjects(currentFile);
								Selection = 0;
								Mode = 1;
							} else {
								Msg::DisplayWaitMsg("Region not supported.");
							}
						} else {
							Msg::DisplayWaitMsg("Game not supported.");
						}
					}
				}
			}
		}
	}
	// Update Screen when needed.
	if(Selection < screenPos) {
		screenPos = Selection;
	} else if (Selection > screenPos + 3 - 1) {
		screenPos = Selection - 3 + 1;
	}
}

void ScriptScreen::EntryLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (keyRepeatDelay)	keyRepeatDelay--;

	if (hDown & KEY_B) {
		Selection = 0;
		refreshList(); // Refresh Script list.
		Mode = 0;
		scriptEntries.clear();
	}

		// Go one entry up.
	if (hHeld & KEY_UP && !keyRepeatDelay) {
		if (Selection > 0) {
			Selection--;
		} else {
			Selection = (int)scriptJson.at("ScriptContent").size()-1;
		}
		if (fastMode == true) {
			keyRepeatDelay = 8;
		} else if (fastMode == false){
			keyRepeatDelay = 8;
		}
	}

		// Go one entry down.
	if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (Selection < (int)scriptJson.at("ScriptContent").size()-1) {
			Selection++;
		} else {
			Selection = 0;
		}
		if (fastMode == true) {
			keyRepeatDelay = 8;
		} else if (fastMode == false){
			keyRepeatDelay = 8;
		}
	}

	if (hDown & KEY_A) {
		// Check Game.
		if (ScriptHelper::checkVersion(scriptJson, Selection)) {
			// Check Region.
			if (ScriptHelper::checkRegion(scriptJson, Selection)) {
				// Prompt.
				if (Msg::promptMsg("Do you like to execute this Entry?")) {
					// Run Entry.
					ScriptHelper::run(scriptJson, Selection);
				}
			} else {
				Msg::DisplayWaitMsg("Region not supported.");
			}
		} else {
			Msg::DisplayWaitMsg("Game not supported.");
		}
	} 
	
	if(Selection < screenPos) {
		screenPos = Selection;
	} else if (Selection > screenPos + 3 - 1) {
		screenPos = Selection - 3 + 1;
	}
}