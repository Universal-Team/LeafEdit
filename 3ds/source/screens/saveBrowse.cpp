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

#include "buildingManagement.hpp"
#include "editor.hpp"
#include "init.hpp"
#include "itemManagement.hpp"
#include "offsets.hpp"
#include "saveBrowse.hpp"
#include "screenCommon.hpp"
#include "utils.hpp"
#include "villagerManagement.hpp"
#include "wwsave.hpp"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern std::vector<std::string> g_villagerDatabase;
std::string selectedSaveFolderEditor = "";
WWSave* WWSaveFile; // Wild World Savefile.
Save* SaveFile; // New Leaf Savefile.
int saveType; // Save type.

extern bool touching(touchPosition touch, Structs::ButtonPos button);

SaveBrowse::SaveBrowse() {
	dirContents.clear();
	std::string customPath;
	customPath += "sdmc:/LeafEdit/Towns/";
	chdir(customPath.c_str());
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, {"sav", "dat"});
	for(uint i=0;i<dirContentsTemp.size();i++) {
	  dirContents.push_back(dirContentsTemp[i]);
	}
}


void SaveBrowse::Draw(void) const {
	if (BrowseMode == 1) {
		DrawBrowse();
	}
}

void SaveBrowse::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (BrowseMode == 1) {
		BrowseLogic(hDown, hHeld);
	}
}

void SaveBrowse::DrawBrowse(void) const
{
	GFX::DrawFileBrowseBG();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, Lang::get("SELECT_A_SAVE"), 395);
	Gui::DrawStringCentered(0, 218, 0.8f, WHITE, Lang::get("REFRESH"), 390);
	std::string dirs;
	for (uint i=(selectedSave<5) ? 0 : selectedSave-5;i<dirContents.size()&&i<((selectedSave<5) ? 6 : selectedSave+1);i++) {
		if (i == selectedSave) {
			dirs += "> " + dirContents[i].name + "\n\n";
		} else {
			dirs += dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}
	Gui::DrawString(26, 32, 0.65f, WHITE, dirs.c_str(), 360);
	GFX::DrawFileBrowseBG(false);
}


void SaveBrowse::BrowseLogic(u32 hDown, u32 hHeld) {
	if (keyRepeatDelay)	keyRepeatDelay--;

	if (dirChanged) {
		dirContents.clear();
		std::vector<DirEntry> dirContentsTemp;
		getDirectoryContents(dirContentsTemp, {"sav", "dat"});
		for(uint i=0;i<dirContentsTemp.size();i++) {
			dirContents.push_back(dirContentsTemp[i]);
		}
		dirChanged = false;
	}


	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK") + "\n" + Lang::get("REFRESH"));
	}

	if (hDown & KEY_A) {
		if (dirContents[selectedSave].isDirectory) {
			chdir(dirContents[selectedSave].name.c_str());
			selectedSave = 0;
			dirChanged = true;
		} else {
				// Wild World.
			if ((strcasecmp(dirContents[selectedSave].name.substr(dirContents[selectedSave].name.length()-3, 3).c_str(), "sav") == 0)) {
				PrepareWW(dirContents[selectedSave].name);
				// New Leaf | Welcome Amiibo.
			} else if ((strcasecmp(dirContents[selectedSave].name.substr(dirContents[selectedSave].name.length()-3, 3).c_str(), "dat") == 0)) {
				PrepareNL(dirContents[selectedSave].name);
			}
		}
	}

	if (hHeld & KEY_UP) {
		if (selectedSave > 0 && !keyRepeatDelay) {
			selectedSave--;
			keyRepeatDelay = 6;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedSave < dirContents.size()-1) {
			selectedSave++;
			keyRepeatDelay = 6;
		}
	} else if (hDown & KEY_B) {
		char path[PATH_MAX];
		getcwd(path, PATH_MAX);
		if(strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
			if(Msg::promptMsg(Lang::get("RETURN_MAINMENU"))) {
				Gui::screenBack();
				return;
			}
		} else {
			chdir("..");
			selectedSave = 0;
			dirChanged = true;
		}
	} else if (hDown & KEY_START) {
		dirChanged = true;
	}
}

// Prepare functions.
void SaveBrowse::PrepareWW(const std::string &savePath) {
	if(access(savePath.c_str(), F_OK) == 0) {
		std::string prompt = Lang::get("WILDWORLD_SAVE_DETECTED") + "\n" + Lang::get("LOAD_THIS_SAVE");
		if(Msg::promptMsg(prompt.c_str())) {
			selectedSaveFolderEditor = savePath;
			const char *save = selectedSaveFolderEditor.c_str();
			saveType = 1; 
			WWSaveFile = WWSave::Initialize(save, true);
			Msg::DisplayMsg(Lang::get("PREPARING_EDITOR"));
			Init::loadWWSheets();
			// Clear Villager then reload, so we can avoid having double names.
			g_villagerDatabase.clear();
			Lang::loadVillager(1, false);
			Gui::setScreen(std::make_unique<Editor>());
		}
	}
}

void SaveBrowse::PrepareNL(const std::string &savePath) {
	if(access(savePath.c_str(), F_OK) == 0) {
		std::string prompt = Lang::get("NEWLEAF_SAVE_DETECTED") + "\n" + Lang::get("LOAD_THIS_SAVE");
		if(Msg::promptMsg(prompt.c_str())) {
			selectedSaveFolderEditor = savePath;
			const char *save = selectedSaveFolderEditor.c_str();
			saveType = 0;
			SaveFile = Save::Initialize(save, true);
			// Check, if SaveFile has correct size.
			if (SaveFile->GetSaveSize() != SIZE_SAVE) {
				Msg::DisplayWarnMsg(Lang::get("SAVE_INCORRECT_SIZE"));
				SaveFile->Close();
				return;
			}
			Msg::DisplayMsg(Lang::get("PREPARING_EDITOR"));
			Init::loadNLSheets();
			BuildingManagement::loadDatabase();
			ItemManagement::LoadDatabase(Config::getLang("Lang"));
			// Clear Villager then reload, so we can avoid having double names.
			g_villagerDatabase.clear();
			Lang::loadVillager(1, true);
			Utils::createBackup();
			Gui::setScreen(std::make_unique<Editor>());
		}
	}
}