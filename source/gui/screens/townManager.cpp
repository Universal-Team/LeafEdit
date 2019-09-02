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

void TownManager::Draw(void) const
{
	if (screenMode == 0) {
		DrawSubMenu();
	} else if (screenMode == 1 || screenMode == 2) {
		DrawBrowse();
	}
}

void TownManager::DrawSubMenu(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += Lang::mainMenu[0];
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);


	if (Selection == 0) {
		Gui::drawAnimatedSelector(townButtons[0].x, townButtons[0].y, townButtons[0].w, townButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
		Gui::Draw_Rect(townButtons[1].x, townButtons[1].y, townButtons[1].w, townButtons[1].h, GREEN);
		Gui::Draw_Rect(townButtons[2].x, townButtons[2].y, townButtons[2].w, townButtons[2].h, GREEN);

	} else if (Selection == 1) {
		Gui::Draw_Rect(townButtons[0].x, townButtons[0].y, townButtons[0].w, townButtons[0].h, GREEN);
		Gui::drawAnimatedSelector(townButtons[1].x, townButtons[1].y, townButtons[1].w, townButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
		Gui::Draw_Rect(townButtons[2].x, townButtons[2].y, townButtons[2].w, townButtons[2].h, GREEN);

	} else if (Selection == 2) {
		Gui::Draw_Rect(townButtons[0].x, townButtons[0].y, townButtons[0].w, townButtons[0].h, GREEN);
		Gui::Draw_Rect(townButtons[1].x, townButtons[1].y, townButtons[1].w, townButtons[1].h, GREEN);
		Gui::drawAnimatedSelector(townButtons[2].x, townButtons[2].y, townButtons[2].w, townButtons[2].h, .030f, C2D_Color32(0, 0, 0, 0));
	}

	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::townmanager[0]))/2, townButtons[0].y+10, 0.6f, WHITE, Lang::townmanager[0]);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::townmanager[1]))/2, townButtons[1].y+10, 0.6f, WHITE, Lang::townmanager[1]);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::townmanager[2]))/2, townButtons[2].y+10, 0.6f, WHITE, Lang::townmanager[2]);
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
						}
						break;
				 	case 2: {
							screenMode = 1;
							dirChanged = true;
						break;
					 }
				}
			}
		} else if (screenMode == 1 || screenMode == 2) {
			BrowseLogic(hDown, hHeld);
		}
}

void TownManager::SelectionLogic(u32 hDown, u32 hHeld)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	}
}

void TownManager::DrawBrowse(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
	if (screenMode == 1) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::townmanager[3]))/2, 2, 0.72f, WHITE, Lang::townmanager[3]);
	} else if (screenMode == 2) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::townmanager[4]))/2, 2, 0.72f, WHITE, Lang::townmanager[4]);
	}

	Gui::DrawString((400-Gui::GetStringWidth(0.60f, Lang::messages2[9]))/2, 218, 0.60f, WHITE, Lang::messages2[9]);

	std::string dirs;
	for (uint i=(selectedSave<5) ? 0 : selectedSave-5;i<dirContents.size()&&i<((selectedSave<5) ? 6 : selectedSave+1);i++) {
		(i == selectedSave);

		if (selectedSave == 0) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 28, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 28, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 28, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSave == 1) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 58, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 58, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 58, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSave == 2) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 91, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 91, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 91, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSave == 3) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 125, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 125, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 125, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSave == 4) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 156, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 156, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 156, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSave == 5) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			dirs +=  dirContents[i].name + "\n\n";
		} else {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			dirs +=  dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}

	if (Config::selector == 1) {
		Gui::DrawString(26, 32, 0.53f, WHITE, dirs.c_str());
	} else if (Config::selector == 2 || Config::selector == 3) {
		Gui::DrawString(26, 32, 0.53f, BLACK, dirs.c_str());
	}

	Gui::DrawString(0, 2, 0.65f, WHITE, selectedSaveFolder.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}

void TownManager::BrowseLogic(u32 hDown, u32 hHeld) { 
	if (keyRepeatDelay)	keyRepeatDelay--;

			if (dirChanged) {
            dirContents.clear();
		std::string customPath = "sdmc:/LeafEdit/Towns/";

		// EUR.
		if (currentID == OldEUR && Config::update == 1) {
			customPath += "Welcome-Amiibo/";
		} else if (currentID == OldEUR  && Config::update == 0) {
			customPath += "Old/";
		}

		// USA.
		if (currentID == OldUSA  && Config::update == 1) {
			customPath += "Welcome-Amiibo/";
		} else if (currentID == OldUSA  && Config::update == 0) {
			customPath += "Old/";
		}

		// JPN.
		if (currentID == OldJPN  && Config::update == 1) {
			customPath += "Welcome-Amiibo/";
		} else if (currentID == OldJPN  && Config::update == 0) {
			customPath += "Old/";
		}

		if (currentID == WelcomeAmiiboUSA || currentID == WelcomeAmiiboEUR || currentID == WelcomeAmiiboJPN) {
			customPath += "Welcome-Amiibo/";
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
			std::string prompt = Lang::messages2[3];
			prompt += "\n\n";
			prompt += dirContents[selectedSave].name;
			if(Msg::promptMsg(prompt.c_str())) {
				selectedSaveFolder = dirContents[selectedSave].name.c_str();
				TownManagement::RestoreTown(currentID, currentMedia, currentLowID, currentHighID, currentUniqueID, selectedSaveFolder);
				selectedSaveFolder = "";
				screenMode = 0;
			}
		}
	} else if (screenMode == 2) {
		if(hDown & KEY_A) {
			std::string prompt = Lang::messages2[4];
			prompt += "\n\n";
			prompt += dirContents[selectedSave].name;
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

		if (screenMode == 2) {
			if (hDown & KEY_X) {
				if(Msg::promptMsg(Lang::messages2[5])) {
					TownManagement::LaunchTown(currentMedia, currentID);
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
		if(Msg::promptMsg(Lang::messages[7])) {
			screenMode = 0;
		}
	} else if (hDown & KEY_SELECT) {
		dirChanged = true;
	}
}