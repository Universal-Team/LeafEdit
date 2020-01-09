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

#include "core/management/scriptManagement.hpp"

#include "gui/gui.hpp"

#include "gui/screens/scripts.hpp"

#include <3ds.h>

#include <algorithm>
#include <fstream>
#include <regex>
#include <unistd.h>

#define ENTRIES_PER_SCREEN 3

// Information like Title and Author and Description.
struct Info {
	std::string title;
	std::string author;
	std::string Description;
	std::string Mode;
};
nlohmann::json jsonFile;
std::string selectedScript = "";

Info parseInfo(std::string fileName) {
	FILE* file = fopen(fileName.c_str(), "rt");
	if(!file) {
		printf("File not found.\n");
		return {"", ""};
	}
	nlohmann::json json = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	Info info;
	info.title = ScriptManagement::getString(json, "scriptInfo", "title");
	info.author = ScriptManagement::getString(json, "scriptInfo", "author");
	info.Description = ScriptManagement::getString(json, "scriptInfo", "description");
	info.Mode = ScriptManagement::getString(json, "scriptInfo", "type");
	return info;
}

nlohmann::json openScriptFile(std::string currentFile) {
	FILE* file = fopen(currentFile.c_str(), "rt");
	nlohmann::json jsonFile;
	if(file)	jsonFile = nlohmann::json::parse(file, nullptr, false);
	fclose(file);
	return jsonFile;
}

// Get the Entries.
std::vector<std::string> parseObjects(std::string fileName) {
	FILE* file = fopen(fileName.c_str(), "rt");
	if(!file) {
		printf("File not found.\n");
		return {{""}};
	}
	nlohmann::json json = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	std::vector<std::string> objs;
	for(auto it = json.begin();it != json.end(); it++) {
		if(it.key() != "scriptInfo") {
			objs.push_back(it.key());
		}
	}
	return objs;
}

std::vector<Info> scriptInformation;
std::vector<std::string> entryInformation;

Scripts::Scripts() {
	dirContents.clear();
	chdir("romfs:/scripts/");
	getDirectoryContents(dirContents, {"json"});
	for(uint i=0;i<dirContents.size();i++) {
		scriptInformation.push_back(parseInfo(dirContents[i].name));
	}
}

// 0 -> SD Card., 1..2..3..4...-> ROMFS.
void Scripts::refresh(int location) {
	scriptInformation.clear();
	dirContents.clear();
	if (location == 0) {
		chdir("sdmc:/LeafEdit/scripts/");
	} else {
		chdir("romfs:/scripts/");
	}
	getDirectoryContents(dirContents, {"json"});
	for(uint i=0;i<dirContents.size();i++) {
		scriptInformation.push_back(parseInfo(dirContents[i].name));
	}
}


void Scripts::DrawSubMenu(void) const {
	if (scriptInformation.size() != 0) {
		std::string line1;
		std::string line2;
		std::string scriptAmount = std::to_string(selection +1) + " / " + std::to_string(scriptInformation.size());
		Gui::DrawTop();
		if (scriptLocation == 0) {
			Gui::DrawStringCentered(0, 2, 0.7f, WHITE, Lang::get("MODE_SD"), 390);
		} else {
			Gui::DrawStringCentered(0, 2, 0.7f, WHITE, Lang::get("MODE_ROMFS"), 390);
		}
		Gui::DrawStringCentered(0, 80, 0.7f, WHITE, Lang::get("TITLE") + std::string(scriptInformation[selection].title), 390);
		Gui::DrawStringCentered(0, 100, 0.7f, WHITE, Lang::get("AUTHOR") + std::string(scriptInformation[selection].author), 390);
		Gui::DrawStringCentered(0, 120, 0.7f, WHITE, Lang::get("DESCRIPTION") + std::string(scriptInformation[selection].Description), 390);
		if (scriptInformation[selection].Mode != "MISSING: scriptInfo.type") {
			Gui::DrawStringCentered(0, 215, 0.7f, WHITE, Lang::get("SCRIPT_TYPE") + std::string(scriptInformation[selection].Mode), 390);
		}
		Gui::DrawString(397-Gui::GetStringWidth(0.6f, scriptAmount), 237-Gui::GetStringHeight(0.6f, scriptAmount), 0.6f, WHITE, scriptAmount);
		Gui::DrawBottom();
		for(int i=0;i<ENTRIES_PER_SCREEN && i<(int)scriptInformation.size();i++) {
			line1 = scriptInformation[screenPos + i].title;
			line2 = scriptInformation[screenPos + i].author;
			if(screenPos + i == selection) {
				Gui::Draw_Rect(0, 40+(i*57), 320, 45, selectedColor);
			} else {
				Gui::Draw_Rect(0, 40+(i*57), 320, 45, unselectedColor);
			}
			Gui::DrawStringCentered(0, 38+(i*57), 0.7f, WHITE, line1, 320);
			Gui::DrawStringCentered(0, 62+(i*57), 0.7f, WHITE, line2, 320);
		}
	} else {
		Gui::DrawTop();
		if (scriptLocation == 0) {
			Gui::DrawStringCentered(0, 2, 0.7f, WHITE, Lang::get("MODE_SD"), 390);
		} else {
			Gui::DrawStringCentered(0, 2, 0.7f, WHITE, Lang::get("MODE_ROMFS"), 390);
		}
		Gui::DrawStringCentered(0, 215, 0.7f, WHITE, Lang::get("NO_SCRIPTS_FOUND"), 390);
		Gui::DrawBottom();
	}
}


void Scripts::DrawSingleObject(void) const {
	std::string info;
	std::string entryAmount = std::to_string(selection2+1) + " / " + std::to_string(entryInformation.size());
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.7f, WHITE, selectedScript, 400);
	Gui::DrawStringCentered(0, 70, 0.7f, WHITE, std::string(scriptInformation[selection].Description), 390);
	Gui::DrawString(397-Gui::GetStringWidth(0.6f, entryAmount), 237-Gui::GetStringHeight(0.6f, entryAmount), 0.6f, WHITE, entryAmount);
	Gui::DrawBottom();
	for(int i=0;i<ENTRIES_PER_SCREEN && i<(int)entryInformation.size();i++) {
		info = entryInformation[screenPos2 + i];
		if(screenPos2 + i == selection2) {
			Gui::Draw_Rect(0, 40+(i*57), 320, 45, selectedColor);
		} else {
			Gui::Draw_Rect(0, 40+(i*57), 320, 45, unselectedColor);
		}
		Gui::DrawStringCentered(0, 50+(i*57), 0.7f, WHITE, info, 320);
	}
}


void Scripts::Draw(void) const {
	if (mode == 0) {
		DrawSubMenu();
	} else if (mode == 1) {
		DrawSingleObject();
	}
}

void Scripts::subMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (keyRepeatDelay)	keyRepeatDelay--;
	if (hDown & KEY_B) {
		scriptInformation.clear();
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_X) {
		selection = 0;
		if (scriptLocation == 0) {
			scriptLocation = 1;
			refresh(1);
		} else {
			scriptLocation = 0;
			refresh(0);
		}
	}

	if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selection < (int)scriptInformation.size()-1) {
			selection++;
		} else {
			selection = 0;
		}
		if (fastMode == true) {
			keyRepeatDelay = 3;
		} else if (fastMode == false) {
			keyRepeatDelay = 6;
		}
	}

	if (hHeld & KEY_UP && !keyRepeatDelay) {
		if (selection > 0) {
			selection--;
		} else {
			selection = (int)scriptInformation.size()-1;
		}
		if (fastMode == true) {
			keyRepeatDelay = 3;
		} else if (fastMode == false) {
			keyRepeatDelay = 6;
		}
	}

	if (hDown & KEY_A) {
		if (dirContents[selection].isDirectory) {
		} else if (scriptInformation.size() != 0) {
			if (ScriptManagement::checkIfValid(dirContents[selection].name) == true) {
				selectedScript = scriptInformation[selection].title;
				jsonFile = openScriptFile(dirContents[selection].name);
				entryInformation = parseObjects(dirContents[selection].name);

				if (scriptInformation[selection].Mode == "Player") {
					executeMode = 0;
				} else if (scriptInformation[selection].Mode == "Villager") {
					executeMode = 1;
				} else if (scriptInformation[selection].Mode == "Custom") {
					executeMode = 2;
				} else {
					executeMode = 3;
				}

				selection2 = 0;
				mode = 1;
			}
		}
	}

	if(selection < screenPos) {
		screenPos = selection;
	} else if (selection > screenPos + ENTRIES_PER_SCREEN - 1) {
		screenPos = selection - ENTRIES_PER_SCREEN + 1;
	}
}

void Scripts::selectLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (keyRepeatDelay)	keyRepeatDelay--;
	if (hDown & KEY_B) {
		entryInformation.clear();
		mode = 0;
	}

	if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selection2 < (int)entryInformation.size()-1) {
			selection2++;
		} else {
			selection2 = 0;
		}
		if (fastMode == true) {
			keyRepeatDelay = 3;
		} else if (fastMode == false) {
			keyRepeatDelay = 6;
		}
	}

	if (hHeld & KEY_UP && !keyRepeatDelay) {
		if (selection2 > 0) {
			selection2--;
		} else {
			selection2 = (int)entryInformation.size()-1;
		}
		if (fastMode == true) {
			keyRepeatDelay = 3;
		} else if (fastMode == false) {
			keyRepeatDelay = 6;
		}
	}

	if (hDown & KEY_A) {
		if (entryInformation.size() != 0) {
			if (executeMode == 0) {
				ScriptManagement::executePlayer(jsonFile, entryInformation[selection2]);
			} else if (executeMode == 1) {
				//ScriptManagement::executeVillager(jsonFile, entryInformation[selection2]);
				Msg::NotImplementedYet();
			} else if (executeMode == 2) {
				//ScriptManagement::executeCustom(jsonFile, entryInformation[selection2]);
				Msg::NotImplementedYet();
			} else if (executeMode == 3) {
				Msg::DisplayWarnMsg(Lang::get("INVALID_TYPE"));
			}
		}
	}

	if(selection2 < screenPos2) {
		screenPos2 = selection2;
	} else if (selection2 > screenPos2 + ENTRIES_PER_SCREEN - 1) {
		screenPos2 = selection2 - ENTRIES_PER_SCREEN + 1;
	}
}


void Scripts::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	gspWaitForVBlank();
	if (mode == 0) {
		subMenuLogic(hDown, hHeld, touch);
	} else if (mode == 1) {
		selectLogic(hDown, hHeld, touch);
	}

	if (hDown & KEY_R) {
		fastMode = true;
	}
	if (hDown & KEY_L) {
		fastMode = false;
	}
}