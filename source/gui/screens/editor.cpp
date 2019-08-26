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
#include "gui/screens/playerEditor.hpp"
#include "gui/screens/screenCommon.hpp"

#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>


extern u64 currentID;
std::string selectedSaveFolderEditor = "";
extern bool WelcomeAmiibo;
Save* SaveFile;

// Player Stuff.
std::string player1Name;
std::string player1Wallet;

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
	Title += Lang::editor[6];

	// Display First Player Name.
	std::string PlayerName = Lang::editor[5];
	PlayerName += " ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[0]->Name).c_str();


	// Display Town Name.
	std::string TownName = Lang::editor[4];
	TownName += " ";
	TownName += StringUtils::UTF16toUTF8(SaveFile->players[0]->TownName).c_str();

	// Display the Amount of Bells inside the Wallet. [NEW]
	std::string Wallet = std::to_string((SaveFile->players[0]->Wallet.value));
	std::string WalletAmount = Lang::editor[3];
	WalletAmount += " ";
	WalletAmount += Wallet.c_str();

	// Display the Amount of Bells inside the Wallet. [OLD]
	std::string WalletOld = std::to_string((SaveFile->players[0]->WalletOld.value));
	std::string WalletAmountOld = Lang::editor[3];
	WalletAmountOld += " ";
	WalletAmountOld += WalletOld.c_str();



	// Display the Amount of Bells from the Bank. [OLD]
	std::string BankOld = std::to_string((SaveFile->players[0]->BankAmountOld.value));
	std::string BankAmountOld = Lang::editor[2];
	BankAmountOld += " ";
	BankAmountOld += BankOld.c_str();

	// Display the Amount of Bells from the Bank. [NEW]
	std::string Bank = std::to_string((SaveFile->players[0]->BankAmount.value));
	std::string BankAmount = Lang::editor[2];
	BankAmount += " ";
	BankAmount += Bank.c_str();



	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str());

	// Game Specific Things.
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 70, 0.8f, WHITE, PlayerName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, TownName.c_str()))/2, 100, 0.8f, WHITE, TownName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 130, 0.8f, WHITE, WalletAmount.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 160, 0.8f, WHITE, BankAmount.c_str());

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

	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::editor[1]))/2, editorButtons[0].y+10, 0.6f, WHITE, Lang::editor[1]);
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

			// Only write something to the Save, because we don't want to write Data with nothing inside it!
/*		if (player1Name != "" || player1Wallet != "") {
			if (Msg::promptMsg(Lang::editor[0])) {
				std::vector<u32> m_PlayerIdReferences = EditorUtils::findPlayerReferences(SaveFile->players[0]);
				SaveFile->players[0]->Name = StringUtils::UTF8toUTF16(player1Name.c_str());
				for (u32 offset : m_PlayerIdReferences) {

					if (player1Name != "") {
						SaveFile->Write(offset + 2, SaveFile->players[0]->Name, 8); // Name Player 1.
					}

					if (player1Wallet != "") {
						SaveFile->players[0]->Wallet.value = static_cast<u32>(std::stoi(player1Wallet.c_str())); // Wallet Amount Player 1.
					}

					SaveFile->Commit(false);
				}
			}
		} */
		EditorMode = 1;
		selectedSaveFolderEditor = "";
		SaveFile->Close();
		player1Name = "";
		player1Wallet = "";
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						Gui::setScreen(std::make_unique<PlayerEditor>());
						break;
				}   case 1:
						break;
				 	case 2: {
						break;
					 }
			}
	}
}


void Editor::DrawBrowse(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::editor[7]))/2, 2, 0.72f, WHITE, Lang::editor[7]);

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

		// Check, if the current ID is the old one.
		if (currentID == OldJPN || currentID == OldUSA || currentID == OldEUR || currentID == OldKOR) {
			if (WelcomeAmiibo == false) {
				customPath += "Old/";
			} else if (WelcomeAmiibo == true) {
				customPath += "Welcome-Amiibo/";
			}
		}

		if (currentID == WelcomeAmiiboUSA || currentID == WelcomeAmiiboEUR || currentID == WelcomeAmiiboJPN || currentID == WelcomeAmiiboKOR) {
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
			std::string prompt = Lang::editor[8];
			if(Msg::promptMsg(prompt.c_str())) {

			selectedSaveFolderEditor = "/LeafEdit/Towns/";
		// Check, if the current ID is the old one.
		if (currentID == OldJPN || currentID == OldUSA || currentID == OldEUR || currentID == OldKOR) {
			if (WelcomeAmiibo == false) {
				selectedSaveFolderEditor += "Old/";
				selectedSaveFolderEditor += dirContents[selectedSave].name.c_str();
				selectedSaveFolderEditor += "/garden.dat";

			} else if (WelcomeAmiibo == true) {
				selectedSaveFolderEditor += "Welcome-Amiibo/";
				selectedSaveFolderEditor += dirContents[selectedSave].name.c_str();
				selectedSaveFolderEditor += "/garden_plus.dat";
			}
		}

		if (currentID == WelcomeAmiiboUSA || currentID == WelcomeAmiiboEUR || currentID == WelcomeAmiiboJPN || currentID == WelcomeAmiiboKOR) {
			selectedSaveFolderEditor += "Welcome-Amiibo/";
			selectedSaveFolderEditor += dirContents[selectedSave].name.c_str();
			selectedSaveFolderEditor += "/garden_plus.dat";
		}

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
		if(Msg::promptMsg(Lang::editor[9])) {
			Gui::screenBack();
			return;
		}
	} else if (hDown & KEY_SELECT) {
		dirChanged = true;
	}
}