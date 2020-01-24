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

#include "common/common.hpp" // For the TID's.
#include "common/config.hpp"

#include "core/management/townManagement.hpp"

#include "gui/screens/screenCommon.hpp"
#include "gui/screens/townManager.hpp"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern FS_MediaType currentMedia;
extern u64 currentID;
extern u32 currentLowID;
extern u32 currentHighID;
extern u32 currentUniqueID;
std::string selectedSaveFolder = "";
std::string currentBackup = "";

extern bool WelcomeAmiibo;
extern bool isROMHack; // For Welcome Luxury.

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void TownManager::Draw(void) const
{
	if (screenMode == 0) {
		DrawSubMenu();
	} else if (screenMode == 1 || screenMode == 2 || screenMode == 3) {
		DrawBrowse();
	}
}

void TownManager::DrawSubMenu(void) const
{
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("TOWN_MANAGER"), 390);
	Gui::DrawBottom();

	for (int i = 0; i < 5; i++) {
		Gui::Draw_Rect(townButtons[i].x, townButtons[i].y, townButtons[i].w, townButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			Gui::drawAnimatedSelector(townButtons[i].x, townButtons[i].y, townButtons[i].w, townButtons[i].h, .030f, SELECTED_COLOR);
		}
	}
	// Launch a Town from a Backup or just start the game.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("LAUNCH_TOWN")))/2-80+17.5, 0.8, WHITE, Lang::get("LAUNCH_TOWN"), 130, 25);
	// Backup the Save from the installed Title / Gamecard.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("BACKUP_TOWN")))/2-20+17.5, 0.8, WHITE, Lang::get("BACKUP_TOWN"), 130, 25);
	// Restore a Backuped save.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("RESTORE_TOWN")))/2+75-17.5, 0.8, WHITE, Lang::get("RESTORE_TOWN"), 130, 25);
	// Delete Save from Installed Title / Gamecard.
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("DELETE_TOWN")))/2-80+17.5, 0.8, WHITE, Lang::get("DELETE_TOWN"), 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("DELETE_BACKUP")))/2-20+17.5, 0.8, WHITE, Lang::get("DELETE_BACKUP"), 130, 25);
}


void TownManager::Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (screenMode == 0) {
		SelectionLogic(hDown, hHeld);

		if (hDown & KEY_B) {
			Gui::screenBack();
			return;
		}

		if (hDown & KEY_TOUCH) {
			if (touching(touch, townButtons[0])) {
				screenMode = 2;
				dirChanged = true;
			} else if (touching(touch, townButtons[1])) {
				if (Msg::promptMsg(Lang::get("BACKUP_TOWN_POPUP"))) {
					TownManagement::BackupTown(currentID, currentMedia, currentLowID, currentHighID);
				}
			} else if (touching(touch, townButtons[2])) {
				screenMode = 1;
				dirChanged = true;
			} else if (touching(touch, townButtons[3])) {
				if (Msg::promptMsg(Lang::get("DELETE_GAME_SAVE"))) {
					TownManagement::CreateNewTown(currentMedia, currentID, currentLowID, currentHighID, currentUniqueID);
				}
			} else if (touching(touch, townButtons[4])) {
				screenMode = 3;
				dirChanged = true;
			}
		}

		if (hDown & KEY_A) {
			switch(Selection) {
				case 0:
					screenMode = 2;
					dirChanged = true;
					break;
				case 1:
					if (Msg::promptMsg(Lang::get("BACKUP_TOWN_POPUP"))) {
						TownManagement::BackupTown(currentID, currentMedia, currentLowID, currentHighID);
					}
					break;
				case 2:
					screenMode = 1;
					dirChanged = true;
					break;
				case 3:
					if (Msg::promptMsg(Lang::get("DELETE_GAME_SAVE"))) {
						TownManagement::CreateNewTown(currentMedia, currentID, currentLowID, currentHighID, currentUniqueID);
					}
					break;
				case 4:
					screenMode = 3;
					dirChanged = true;
					break;
			}
		}
	} else if (screenMode == 1 || screenMode == 2 || screenMode == 3) {
		BrowseLogic(hDown, hHeld);
	}
}

void TownManager::SelectionLogic(u32 hDown, u32 hHeld)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	}
	if (hDown & KEY_DOWN) {
		if(Selection < 4)	Selection++;
	}
	if (hDown & KEY_RIGHT) {
		if (Selection == 0)	Selection = 3;
		else if (Selection == 1)	Selection = 4;
	}
	if (hDown & KEY_LEFT) {
		if (Selection == 3)	Selection = 0;
		else if (Selection == 4)	Selection = 1;
	}
}

void TownManager::DrawBrowse(void) const
{
	Gui::DrawFileBrowseBG();
	if (screenMode == 1) {
		Gui::DrawStringCentered(0, 2, 0.9f, WHITE, Lang::get("SELECT_TOWN_RESTORE"), 390);
	} else if (screenMode == 2) {
		Gui::DrawStringCentered(0, 2, 0.9f, WHITE, Lang::get("SELECT_TOWN_LAUNCH"), 390);
	} else if (screenMode == 3) {
		Gui::DrawStringCentered(0, 2, 0.9f, WHITE, Lang::get("SELECT_BACKUP_DELETE"), 390);
	}

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
	Gui::DrawFileBrowseBG(false);
}

void TownManager::BrowseLogic(u32 hDown, u32 hHeld) {
	if (keyRepeatDelay)	keyRepeatDelay--;

	if (dirChanged) {
		dirContents.clear();
		std::string customPath = "sdmc:/LeafEdit/Towns/";

		// Check for the game.
		if (isROMHack == true) {
			customPath += "Welcome-Luxury/";
		} else if (isROMHack == false) {
			if (currentID == OldJPN || currentID == OldUSA || currentID == OldEUR || currentID == OldKOR) {
				if (WelcomeAmiibo == false) {
					customPath += "Old/";
				} else if (WelcomeAmiibo == true) {
					customPath += "Welcome-Amiibo/";
				}
			} else {
				if (currentID == WelcomeAmiiboUSA || currentID == WelcomeAmiiboEUR || currentID == WelcomeAmiiboJPN || currentID == WelcomeAmiiboKOR) {
					customPath += "Welcome-Amiibo/";
				}
			}
		}

		chdir(customPath.c_str());
		std::vector<DirEntry> dirContentsTemp;
		getDirectoryContents(dirContentsTemp);
		for(uint i=0;i<dirContentsTemp.size();i++) {
			dirContents.push_back(dirContentsTemp[i]);
		}
		dirChanged = false;
	}

	if (screenMode == 1) {
		if(hDown & KEY_A) {
			if (dirContents.size() == 0) {
				Msg::DisplayWarnMsg(Lang::get("WHAT_YOU_DO"));
			} else {
				std::string prompt = Lang::get("RESTORE_TOWN_POPUP");
				prompt += "\n\n";
				prompt += "'";
				prompt += dirContents[selectedSave].name;
				prompt += "'";
				if(Msg::promptMsg(prompt.c_str())) {
					selectedSaveFolder = dirContents[selectedSave].name.c_str();
					TownManagement::RestoreTown(currentID, currentMedia, currentLowID, currentHighID, currentUniqueID, selectedSaveFolder);
					selectedSaveFolder = "";
					screenMode = 0;
				}
			}
		}

	} else if (screenMode == 2) {
		if(hDown & KEY_A) {
			if (dirContents.size() == 0) {
				Msg::DisplayWarnMsg(Lang::get("WHAT_YOU_DO"));
			} else {
				std::string prompt = Lang::get("LAUNCH_TOWN_POPUP_A");
				prompt += "\n\n";
				prompt += "'";
				prompt += dirContents[selectedSave].name;
				prompt += "'";
				if(Msg::promptMsg(prompt.c_str())) {
					selectedSaveFolder = dirContents[selectedSave].name.c_str();
					if(Msg::promptMsg(Lang::get("BACKUP_BEFORE_RESTORE"))) {
						TownManagement::BackupTown(currentID, currentMedia, currentLowID, currentHighID);
					}
					TownManagement::RestoreTown(currentID, currentMedia, currentLowID, currentHighID, currentUniqueID, selectedSaveFolder);
					selectedSaveFolder = "";
					TownManagement::LaunchTown(currentMedia, currentID);
				}
			}
		}

	} else if (screenMode == 3) {
		if (hDown & KEY_A) {
			if (dirContents.size() == 0) {
				Msg::DisplayWarnMsg(Lang::get("WHAT_YOU_DO"));
			} else {
				if(Msg::promptMsg(Lang::get("DELETE_BACKUP_MSG"))) {
					currentBackup += dirContents[selectedSave].name.c_str(); // Get the actual Backup Folder.
					TownManagement::DeleteBackup(currentID, currentBackup.c_str()); // We delete the Backup now.
					currentBackup = ""; // We reset the Backup Folder.
					dirChanged = true; // We want to refresh the list after it.
				}
			}
		}
	}

	if (screenMode == 2) {
		if (hDown & KEY_X) {
			if(Msg::promptMsg(Lang::get("LAUNCH_TOWN_POPUP_X"))) {
				TownManagement::LaunchTown(currentMedia, currentID);
			}
		} else if (hHeld & KEY_SELECT) {
			Msg::HelperBox("Press X to launch just the current Mediatype.\nSelect a Backup and Press A to restore and launch it.\nPress Start to refresh the FileList.\nPress B to exit from this Screen.");
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
		if(Msg::promptMsg(Lang::get("CANCEL"))) {
			screenMode = 0;
		}
	} else if (hDown & KEY_START) {
		dirChanged = true;
	}
}