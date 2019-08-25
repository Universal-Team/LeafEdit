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
#include "common/utils.hpp"

#include "gui/keyboard.hpp"

#include "gui/screens/editor.hpp"
#include "gui/screens/mainMenu.hpp"
#include "gui/screens/screenCommon.hpp"

#include "core/save/player.h"
#include "core/save/save.h"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>


extern u64 currentID;
std::string selectedSaveFolderEditor = "";
Save* SaveFile;

void Editor::Draw(void) const
{
	if (EditorMode == 1) {
		DrawBrowse();
	} else if (EditorMode == 2) {
		DrawSubMenu();
	}
}

void Editor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (EditorMode == 1) {
		BrowseLogic(hDown, hHeld);
	} else if (EditorMode == 2) {
		SubMenuLogic(hDown, hHeld);
	}
}

void Editor::DrawSubMenu(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += "Editor";

	// Display First Player Name.
	std::string PlayerName = "Player Name: ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[0]->Name).c_str();

	// Display Town Name.
	std::string TownName = "Town Name: ";
	TownName += StringUtils::UTF16toUTF8(SaveFile->players[0]->TownName).c_str();

	std::string Wallet = std::to_string((SaveFile->players[0]->Wallet.value));
	std::string WalletAmount = "Wallet Amount: ";
	WalletAmount += Wallet.c_str();

	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str());

	// Game Specific Things.
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 100, 0.8f, WHITE, PlayerName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, TownName.c_str()))/2, 130, 0.8f, WHITE, TownName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 160, 0.8f, WHITE, WalletAmount.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);


	if (Selection == 0) {
		Gui::drawAnimatedSelector(editorButtons[0].x, editorButtons[0].y, editorButtons[0].w, editorButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
		Gui::Draw_Rect(editorButtons[1].x, editorButtons[1].y, editorButtons[1].w, editorButtons[1].h, GREEN);
		Gui::Draw_Rect(editorButtons[2].x, editorButtons[2].y, editorButtons[2].w, editorButtons[2].h, GREEN);

	} else if (Selection == 1) {
		Gui::Draw_Rect(editorButtons[0].x, editorButtons[0].y, editorButtons[0].w, editorButtons[0].h, GREEN);
		Gui::drawAnimatedSelector(editorButtons[1].x, editorButtons[1].y, editorButtons[1].w, editorButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
		Gui::Draw_Rect(editorButtons[2].x, editorButtons[2].y, editorButtons[2].w, editorButtons[2].h, GREEN);

	} else if (Selection == 2) {
		Gui::Draw_Rect(editorButtons[0].x, editorButtons[0].y, editorButtons[0].w, editorButtons[0].h, GREEN);
		Gui::Draw_Rect(editorButtons[1].x, editorButtons[1].y, editorButtons[1].w, editorButtons[1].h, GREEN);
		Gui::drawAnimatedSelector(editorButtons[2].x, editorButtons[2].y, editorButtons[2].w, editorButtons[2].h, .030f, C2D_Color32(0, 0, 0, 0));
	}

	Gui::DrawString((320-Gui::GetStringWidth(0.6f, "WIP"))/2, editorButtons[0].y+10, 0.6f, WHITE, "WIP");
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, "WIP"))/2, editorButtons[1].y+10, 0.6f, WHITE, "WIP");
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, "WIP"))/2, editorButtons[2].y+10, 0.6f, WHITE, "WIP");
}

void Editor::SubMenuLogic(u32 hDown, u32 hHeld)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	} else if (hDown & KEY_B) {
		EditorMode = 1;
		selectedSaveFolderEditor = "";
		SaveFile->Close();
	}
}


void Editor::DrawBrowse(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Select a Save for the Editor."))/2, 2, 0.72f, WHITE, "Select a Save for the Editor.");

	std::string dirs;
	for (uint i=(selectedSave<5) ? 0 : selectedSave-5;i<dirContents.size()&&i<((selectedSave<5) ? 6 : selectedSave+1);i++) {
		(i == selectedSave);

		if (selectedSave == 0) {
			Gui::drawAnimatedSelector(0, 28, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSave == 1) {
			Gui::drawAnimatedSelector(0, 58, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSave == 2) {
			Gui::drawAnimatedSelector(0, 91, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSave == 3) {
			Gui::drawAnimatedSelector(0, 125, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSave == 4) {
			Gui::drawAnimatedSelector(0, 156, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSave == 5) {
			Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			dirs +=  dirContents[i].name + "\n\n";
		} else {
			Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			dirs +=  dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}

	Gui::DrawString(26, 32, 0.53f, WHITE, dirs.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}


void Editor::BrowseLogic(u32 hDown, u32 hHeld) { 
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

		if(hDown & KEY_A) {
			std::string prompt = "Do you want to load this Save?";
			if(Msg::promptMsg(prompt.c_str())) {
				selectedSaveFolderEditor = "/LeafEdit/Towns/Welcome-Amiibo/";
				selectedSaveFolderEditor += dirContents[selectedSave].name.c_str();
				selectedSaveFolderEditor += "/garden_plus.dat";
				const char *save = selectedSaveFolderEditor.c_str();
				SaveFile = Save::Initialize(save, true);
				EditorMode = 2;
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
		if(Msg::promptMsg("Do you want to return to the Main Menu?")) {
			Gui::screenBack();
			return;
		}
	} else if (hDown & KEY_SELECT) {
		dirChanged = true;
	}
}