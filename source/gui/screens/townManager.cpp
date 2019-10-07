	/*
*   This file is part of LeafEdit
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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

#include "core/townManagement.hpp"

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
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += Lang::mainMenu[0];
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, Config::barText, Title.c_str(), 400);

	Gui::DrawBottom();


	if (Selection == 0) {
		Gui::Draw_ImageBlend(0, sprites_button_idx, townButtons[0].x, townButtons[0].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, townButtons[1].x, townButtons[1].y);
		Gui::sprite(0, sprites_button_idx, townButtons[2].x, townButtons[2].y);
		Gui::sprite(0, sprites_button_idx, townButtons[3].x, townButtons[3].y);
		Gui::sprite(0, sprites_button_idx, townButtons[4].x, townButtons[4].y);

	} else if (Selection == 1) {
		Gui::sprite(0, sprites_button_idx, townButtons[0].x, townButtons[0].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, townButtons[1].x, townButtons[1].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, townButtons[2].x, townButtons[2].y);
		Gui::sprite(0, sprites_button_idx, townButtons[3].x, townButtons[3].y);
		Gui::sprite(0, sprites_button_idx, townButtons[4].x, townButtons[4].y);

	} else if (Selection == 2) {
		Gui::sprite(0, sprites_button_idx, townButtons[0].x, townButtons[0].y);
		Gui::sprite(0, sprites_button_idx, townButtons[1].x, townButtons[1].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, townButtons[2].x, townButtons[2].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, townButtons[3].x, townButtons[3].y);
		Gui::sprite(0, sprites_button_idx, townButtons[4].x, townButtons[4].y);

	} else if (Selection == 3) {
		Gui::sprite(0, sprites_button_idx, townButtons[0].x, townButtons[0].y);
		Gui::sprite(0, sprites_button_idx, townButtons[1].x, townButtons[1].y);
		Gui::sprite(0, sprites_button_idx, townButtons[2].x, townButtons[2].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, townButtons[3].x, townButtons[3].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, townButtons[4].x, townButtons[4].y);

	} else if (Selection == 4) {
		Gui::sprite(0, sprites_button_idx, townButtons[0].x, townButtons[0].y);
		Gui::sprite(0, sprites_button_idx, townButtons[1].x, townButtons[1].y);
		Gui::sprite(0, sprites_button_idx, townButtons[2].x, townButtons[2].y);
		Gui::sprite(0, sprites_button_idx, townButtons[3].x, townButtons[3].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, townButtons[4].x, townButtons[4].y, selectedColor);
	}

		// Launch a Town from a Backup or just start the game.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::townmanager[0]))/2-70+5, townButtons[0].y+10, 0.6f, Config::buttonText, Lang::townmanager[0], 130);

		// Backup the Save from the installed Title / Gamecard. 
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::townmanager[1]))/2-70+5, townButtons[1].y+10, 0.6f, Config::buttonText, Lang::townmanager[1], 130);

		// Restore a Backuped save.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::townmanager[2]))/2-70+5, townButtons[2].y+10, 0.6f, Config::buttonText, Lang::townmanager[2], 130);


		// Delete Save from Installed Title / Gamecard.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Delete Town"))/2+150-70+5, townButtons[3].y+10, 0.6f, Config::buttonText, "Delete Town", 130);

		Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Delete Backup"))/2+150-70+5, townButtons[4].y+10, 0.6f, Config::buttonText, "Delete Backup", 130);
}


void TownManager::Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (screenMode == 0) {
	SelectionLogic(hDown, hHeld);

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						screenMode = 2;
						dirChanged = true;
						break;
				}	case 1:
						if (Msg::promptMsg(Lang::messages2[2])) {
						TownManagement::BackupTown(currentID, currentMedia, currentLowID, currentHighID);
						playChange();
						}
						break;
				 	case 2: {
						screenMode = 1;
						dirChanged = true;
						break;
					 } case 3: {
						if (Msg::promptMsg("Would you like, to delete this Game's Town?\nYour Game will start directly after it.")) {
							TownManagement::CreateNewTown(currentMedia, currentID, currentLowID, currentHighID, currentUniqueID);
						}
						break;
					 } case 4: {
						screenMode = 3;
						dirChanged = true;
						break;
					 }
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
	} else if (hDown & KEY_DOWN) {
		if(Selection < 4)	Selection++;
	}  else if (hHeld & KEY_SELECT) {
        Msg::HelperBox("Select Launch Town, to launch a Town from your SD Card.\nSelect Backup Town, to backup a Town from the selected Mediatype.\nSelect Restore Town, to restore a save from your SD Card.\nSelect Delete Town, to delete your current Mediatypes SaveFile.\nSelect Delete Backup, to delete an unneded Backup from the SD Card.\nPress B to exit from this Screen.");
    }
}

void TownManager::DrawBrowse(void) const
{
	Gui::ScreenDraw(top);
	Gui::sprite(0, sprites_top_topbar_idx, 0, 0);
	Gui::sprite(0, sprites_fbBgTop_idx, 0, 27);
	Gui::sprite(0, sprites_top_bottombar_idx, 0, 213);

	if (screenMode == 1) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::townmanager[3]))/2, 2, 0.72f, Config::barText, Lang::townmanager[3], 400);
	} else if (screenMode == 2) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::townmanager[4]))/2, 2, 0.72f, Config::barText, Lang::townmanager[4], 400);
	} else if (screenMode == 3) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Select a Backup to delete."))/2, 2, 0.72f, Config::barText, "Select a Backup to delete.", 400);
	}

	Gui::DrawString((400-Gui::GetStringWidth(0.60f, Lang::messages2[9]))/2, 216, 0.60f, Config::barText, Lang::messages2[9], 400);

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

	Gui::DrawString(26, 32, 0.51f, Config::fileBrowseText, dirs.c_str(), 400);

	Gui::DrawString(0, 2, 0.65f, WHITE, selectedSaveFolder.c_str(), 400);

	Gui::ScreenDraw(bottom);
	Gui::sprite(0, sprites_bottom_topbar_idx, 0, 0);
	Gui::sprite(0, sprites_fbBgBottom_idx, 0, 27);
	Gui::sprite(0, sprites_bottom_bottombar_idx, 0, 213);
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
				Msg::DisplayWarnMsg("What are you trying to do? :P");
			} else {
				std::string prompt = Lang::messages2[3];
				prompt += "\n\n";
				prompt += "'";
				prompt += dirContents[selectedSave].name;
				prompt += "'";
				if(Msg::promptMsg(prompt.c_str())) {
					selectedSaveFolder = dirContents[selectedSave].name.c_str();
					TownManagement::RestoreTown(currentID, currentMedia, currentLowID, currentHighID, currentUniqueID, selectedSaveFolder);
					selectedSaveFolder = "";
					screenMode = 0;
					playChange();
				}
			}
		} else if (hHeld & KEY_SELECT) {
			Msg::HelperBox("Select a Backup, which you like to restore and press A.\nPress Start to refresh the FileList.\nPress B to exit from this Screen.");
		}

	} else if (screenMode == 2) {
		if(hDown & KEY_A) {
			if (dirContents.size() == 0) {
				Msg::DisplayWarnMsg("What are you trying to do? :P");
			} else {
				std::string prompt = Lang::messages2[4];
				prompt += "\n\n";
				prompt += "'";
				prompt += dirContents[selectedSave].name;
				prompt += "'";
				if(Msg::promptMsg(prompt.c_str())) {
					selectedSaveFolder = dirContents[selectedSave].name.c_str();
					if(Msg::promptMsg(Lang::messages2[10])) {
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
				Msg::DisplayWarnMsg("What are you trying to do? :P");
			} else {
				if(Msg::promptMsg("Would you like, to delete this Backup?")) {
					currentBackup += dirContents[selectedSave].name.c_str(); // Get the actual Backup Folder.
					TownManagement::DeleteBackup(currentID, currentBackup.c_str()); // We delete the Backup now.
					currentBackup = ""; // We reset the Backup Folder.
					dirChanged = true; // We want to refresh the list after it.
					playChange();
				}
			}
		} else if (hHeld & KEY_SELECT) {
			Msg::HelperBox("Select the unneded Backup and press A to delete it.\nPress Start to refresh the FileList.\nPress B to exit from this Screen.");
		}
	}

		if (screenMode == 2) {
			if (hDown & KEY_X) {
				if(Msg::promptMsg(Lang::messages2[5])) {
					TownManagement::LaunchTown(currentMedia, currentID);
					playChange();
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
		if(Msg::promptMsg(Lang::messages[7])) {
			screenMode = 0;
		}
	} else if (hDown & KEY_START) {
		dirChanged = true;
	}
}