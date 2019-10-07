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

#include "common/config.hpp"
#include "common/utils.hpp"

#include "core/playerManagement.hpp"

#include "gui/keyboard.hpp"

#include "gui/screens/playerEditor.hpp"
#include "gui/screens/screenCommon.hpp"

#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"
#include "core/save/villager.h"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern Save* SaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

void PlayerEditor::Draw(void) const {
	if (screen == 0) {
		DrawSubMenu();
	} else if (screen == 1) {
		DrawMainEditor();
	} else if (screen == 2) {
		DrawPlayerEditor();
	} else if (screen == 3) {
//		DrawItemEditor();
	}
}

void PlayerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (screen == 0) {
		SubMenuLogic(hDown, hHeld);
	} else if (screen == 1) {
		MainEditorLogic(hDown, hHeld, touch);
	} else if (screen == 2) {
		PlayerEditorLogic(hDown, hHeld, touch);
	} else if (screen == 3) {
//		ItemEditorLogic();
	}
}

void PlayerEditor::DrawSubMenu(void) const {
	std::string activePlayer;
	std::string Player1Name;
	std::string Player2Name;
	std::string Player3Name;
	std::string Player4Name;

	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += "Player Selection";

	if (SaveFile->players[0]->Exists()) {
		Player1Name = StringUtils::UTF16toUTF8(SaveFile->players[0]->Name).c_str();
		Player1Name += "\n\n      ";
		if (SaveFile->players[0]->Gender == 0) {
			Player1Name += male;
		} else {
			Player1Name += female;
		}
	}

	if (SaveFile->players[1]->Exists()) {
		Player2Name = StringUtils::UTF16toUTF8(SaveFile->players[1]->Name).c_str();
		Player2Name += "\n\n      ";
		if (SaveFile->players[1]->Gender == 0) {
			Player2Name += male;
		} else {
			Player2Name += female;
		}
	}

	if (SaveFile->players[2]->Exists()) {
		Player3Name = StringUtils::UTF16toUTF8(SaveFile->players[2]->Name).c_str();
		Player3Name += "\n\n      ";
		if (SaveFile->players[2]->Gender == 0) {
			Player3Name += male;
		} else {
			Player3Name += female;
		}
	}

	if (SaveFile->players[3]->Exists()) {
		Player4Name = StringUtils::UTF16toUTF8(SaveFile->players[3]->Name).c_str();
		Player4Name += "\n\n      ";
		if (SaveFile->players[3]->Gender == 0) {
			Player4Name += male;
		} else {
			Player4Name += female;
		}
	}

	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, Config::barText, Title.c_str(), 400);
	u32 player = 0;
	for (u32 x = 0; x < 4; x++, player++) {
		if (SaveFile->players[player]->Exists()) {
			Gui::sprite(0, sprites_villagerBox_idx, 15 + x * 100, 93);
		}
	}

	Gui::DrawString(17 + 0 * 100, 106, 0.45f, Config::boxText, Player1Name.c_str(), 65);
	Gui::DrawString(17 + 1 * 100, 106, 0.45f, Config::boxText, Player2Name.c_str(), 65);
	Gui::DrawString(17 + 2 * 100, 106, 0.45f, Config::boxText, Player3Name.c_str(), 65);
	Gui::DrawString(17 + 3 * 100, 106, 0.45f, Config::boxText, Player4Name.c_str(), 65);

	if (selectedPlayer == 0)	Gui::drawAnimatedSelector(15 + 0 * 100, 93, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	else if (selectedPlayer == 1)	Gui::drawAnimatedSelector(15 + 1 * 100, 93, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	else if (selectedPlayer == 2)	Gui::drawAnimatedSelector(15 + 2 * 100, 93, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	else if (selectedPlayer == 3)	Gui::drawAnimatedSelector(15 + 3 * 100, 93, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));

	Gui::DrawBottom();
	activePlayer += Lang::misc[1];
	activePlayer += ": ";
	for (int i = 0; i < 3; i++) {
		if (selectedPlayer == i) {
			activePlayer += std::to_string(i+1);
		}
	}

	Gui::DrawString((320-Gui::GetStringWidth(0.8f, activePlayer.c_str()))/2, 212, 0.8f, Config::barText, activePlayer.c_str(), 320);
}

void PlayerEditor::SubMenuLogic(u32 hDown, u32 hHeld) {
	for (int player = 0; player < 4; player++) {
		if (SaveFile->players[player]->Exists())	maxPlayer = player;
	}

	if (hDown & KEY_RIGHT) {
		selectedPlayer++;
		if(selectedPlayer > maxPlayer)	selectedPlayer = 0;
	} else if (hDown & KEY_LEFT) {
		selectedPlayer--;
		if(selectedPlayer < 0)	selectedPlayer = maxPlayer;
	} else if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		if (selectedPlayer == 0)	cp = 0;
		else if (selectedPlayer == 1)	cp = 1;
		else if (selectedPlayer == 2)	cp = 2;
		else if (selectedPlayer == 3)	cp = 3;
		Selection = 0;
		screen = 1;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Select the Player, which you want to edit and press A.\nPress B to exit from this Screen.");
	}
}

void PlayerEditor::DrawMainEditor(void) const {
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += Lang::editor[1];

	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, Config::barText, Title.c_str(), 400);

	Gui::DrawBottom();


	if (Selection == 0) {
		Gui::Draw_ImageBlend(0, sprites_button_idx, mainButtons[0].x, mainButtons[0].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, mainButtons[1].x, mainButtons[1].y);
		Gui::sprite(0, sprites_button_idx, mainButtons[2].x, mainButtons[2].y);

	} else if (Selection == 1) {
		Gui::sprite(0, sprites_button_idx, mainButtons[0].x, mainButtons[0].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, mainButtons[1].x, mainButtons[1].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, mainButtons[2].x, mainButtons[2].y);

	} else if (Selection == 2) {
		Gui::sprite(0, sprites_button_idx, mainButtons[0].x, mainButtons[0].y);
		Gui::sprite(0, sprites_button_idx, mainButtons[1].x, mainButtons[1].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, mainButtons[2].x, mainButtons[2].y, selectedColor);
	}

	Gui::sprite(0, sprites_back_idx, mainButtons[3].x, mainButtons[3].y);

	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::editor[1]))/2, mainButtons[0].y+10, 0.6f, Config::buttonText, Lang::editor[1], 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Items"))/2, mainButtons[1].y+10, 0.6f, Config::buttonText, "Items", 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, "WIP"))/2, mainButtons[2].y+10, 0.6f, Config::buttonText, "WIP", 140);
}


void PlayerEditor::MainEditorLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	} else if (hDown & KEY_TOUCH && touching(touch, mainButtons[3])) {
		screen = 0;
	}

	if (hDown & KEY_B) {
		screen = 0;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Select Player, to edit the Player Stuff.\nSelect Items, to edit the Items. (Not Implemented yet)\nSelect WIP to ?. (Not implemented yet)\nPress B to exit from this Screen.");
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						Selection = 0;
						screen = 2;
						break;
				}	case 1:
						Msg::NotImplementedYet();
						//screen = 3;
						break;
					case 2: {
						Msg::NotImplementedYet();
						//screen = 4;
						break;
				}
			}
	}
}

void PlayerEditor::DrawPlayerEditor(void) const {
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += "Player Selection";

	// Display First Player Name.
	std::string PlayerName = Lang::playerEditor[1];
	PlayerName += ": ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[cp]->Name).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[cp]->Wallet.value));
	std::string WalletAmount = Lang::playerEditor[2];
	WalletAmount += ": ";
	WalletAmount += Wallet.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[cp]->PlayerTan));
	std::string TanPlayer = Lang::playerEditor[3];
	TanPlayer += ": ";
	TanPlayer += Tan.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[cp]->BankAmount.value));
	std::string BankAmount = Lang::playerEditor[4];
	BankAmount += ": ";
	BankAmount += Bank.c_str();

	// Display the amount of medals.
	std::string Medals = std::to_string((SaveFile->players[cp]->IslandMedals.value));
	std::string MedalsAmount = Lang::playerEditor[5];
	MedalsAmount += ": ";
	MedalsAmount += Medals.c_str();

	// Display the amount of Coupons.
	std::string Coupons = std::to_string((SaveFile->players[cp]->MeowCoupons.value));
	std::string CouponsAmount = Lang::playerEditor[6];
	CouponsAmount += ": ";
	CouponsAmount += Coupons.c_str();

	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, Config::barText, Title.c_str(), 400);

		// Game Specific Things.
		Gui::sprite(0, sprites_topbox_idx, 40, 37);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 35, 0.8f, Config::boxText, PlayerName.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 65);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 63, 0.8f, Config::boxText, WalletAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 93);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 91, 0.8f, Config::boxText, TanPlayer.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 121);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 119, 0.8f, Config::boxText, BankAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 149);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, MedalsAmount.c_str()))/2, 147, 0.8f, Config::boxText, MedalsAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 177);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, CouponsAmount.c_str()))/2, 175, 0.8f, Config::boxText, CouponsAmount.c_str(), 400);

		// Player Bottom.
		Gui::DrawBottom();

		Gui::sprite(0, sprites_button_idx, playerButtons[0].x, playerButtons[0].y);
		Gui::sprite(0, sprites_button_idx, playerButtons[1].x, playerButtons[1].y);
		Gui::sprite(0, sprites_button_idx, playerButtons[2].x, playerButtons[2].y);
		Gui::sprite(0, sprites_button_idx, playerButtons[3].x, playerButtons[3].y);
		Gui::sprite(0, sprites_button_idx, playerButtons[4].x, playerButtons[4].y);
		Gui::sprite(0, sprites_button_idx, playerButtons[5].x, playerButtons[5].y);



		if (Selection == 0) {
			Gui::Draw_ImageBlend(0, sprites_button_idx, playerButtons[0].x, playerButtons[0].y, selectedColor);
		} else if (Selection == 1) {
			Gui::Draw_ImageBlend(0, sprites_button_idx, playerButtons[1].x, playerButtons[1].y, selectedColor);
		} else if (Selection == 2) {
			Gui::Draw_ImageBlend(0, sprites_button_idx, playerButtons[2].x, playerButtons[2].y, selectedColor);
		} else if (Selection == 3) {
			Gui::Draw_ImageBlend(0, sprites_button_idx, playerButtons[3].x, playerButtons[3].y, selectedColor);
		} else if (Selection == 4) {
			Gui::Draw_ImageBlend(0, sprites_button_idx, playerButtons[4].x, playerButtons[4].y, selectedColor);
		} else if (Selection == 5) {
			Gui::Draw_ImageBlend(0, sprites_button_idx, playerButtons[5].x, playerButtons[5].y, selectedColor);
		}


	if (currentPage == 1) {
		// Display Player Name.

		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[1]))/2-65, playerButtons[0].y+10, 0.6f, Config::buttonText, Lang::playerEditor[1], 130);

		// Display Wallet Amount.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[2]))/2-65, playerButtons[1].y+10, 0.6f, Config::buttonText, Lang::playerEditor[2], 130);

		// Display current Tan Value.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[3]))/2-65, playerButtons[2].y+10, 0.6f, Config::buttonText, Lang::playerEditor[3], 130);


		// Display Bank Amount.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[4]))/2+150-65, playerButtons[3].y+10, 0.6f, Config::buttonText, Lang::playerEditor[4], 130);

		// Display Medal Amount.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[5]))/2+150-65, playerButtons[4].y+10, 0.6f, Config::buttonText, Lang::playerEditor[5], 130);

		// Display Coupon Amount.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[6]))/2+150-65, playerButtons[5].y+10, 0.6f, Config::buttonText, Lang::playerEditor[6], 130);

	} else if (currentPage == 2) {

		// Max Bank.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[7]))/2-65, playerButtons[0].y+10, 0.6f, Config::buttonText, Lang::playerEditor[7], 130);

		// Max Medals.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[8]))/2-65, playerButtons[1].y+10, 0.6f, Config::buttonText, Lang::playerEditor[8], 130);

		// Max Coupons.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[9]))/2-65, playerButtons[2].y+10, 0.6f, Config::buttonText, Lang::playerEditor[9], 130);

		// Clear Bank.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[10]))/2+150-65+5, playerButtons[3].y+10, 0.6f, Config::buttonText, Lang::playerEditor[10], 130);

		// Clear Medals.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[11]))/2+150-65, playerButtons[4].y+10, 0.6f, Config::buttonText, Lang::playerEditor[11], 130);

		// Clear Coupons.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[12]))/2+150-65, playerButtons[5].y+10, 0.6f, Config::buttonText, Lang::playerEditor[12], 130);
	}
}

void PlayerEditor::PlayerEditorLogic(u32 hDown, u32 hHeld, touchPosition touch) {

	// Selection / Page.
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
			if(Selection < 5)	Selection++;
	} else if (hDown & KEY_R) {
		if(currentPage <2) currentPage++;
		Selection = 0;
	} else if (hDown & KEY_L) {
		if(currentPage > 1)	currentPage--;
		Selection = 0;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Press L/R to switch the Editing Pages.\nTouch or press A to edit the selected option.\nPress B to exit from this Screen.");
	}

	// Selection A / Touch.
	if (hDown & KEY_A) {
		if (currentPage == 1) {
				switch(Selection) {
					case 0: {
							SaveFile->players[cp]->Name = Input::getu16String(8, Lang::playerEditor[0].c_str());
							break;
					}	case 1:
							SaveFile->players[cp]->Wallet.value = Input::getu32(5, 99999);
							break;
						case 2: {
							SaveFile->players[cp]->PlayerTan = Input::getu32(2, 15);
							break;
						} case 3: {
							SaveFile->players[cp]->BankAmount.value = Input::getu32(9, 999999999);
							break;
						} case 4: {
							SaveFile->players[cp]->IslandMedals.value = Input::getu32(4, 9999);
							break;
						} case 5: {
							SaveFile->players[cp]->MeowCoupons.value = Input::getu32(4, 9999);
							break;
						}
					}

			} else if (currentPage == 2) {
				switch(Selection) {
					case 0: {
							PlayerManagement::PlayerMaxBank(cp, pBank);
							break;
					}	case 1:
							PlayerManagement::PlayerMaxMedals(cp, pMedals);
							break;
						case 2: {
							PlayerManagement::PlayerMaxCoupons(cp, pCoupons);
							break;
						} case 3: {
							PlayerManagement::PlayerClearBank(cp, pBank);
							break;
						} case 4: {
							PlayerManagement::PlayerClearMedals(cp, pMedals);
							break;
						} case 5: {
							PlayerManagement::PlayerClearCoupons(cp, pCoupons);
							break;
						}
					}
			}
	}

		if (hDown & KEY_TOUCH) {
			if (currentPage == 1) {
				if (touching(touch, playerButtons[0])) {
					SaveFile->players[cp]->Name = Input::getu16String(8, Lang::playerEditor[0].c_str());
			} else if (touching(touch, playerButtons[1])) {
					SaveFile->players[cp]->Wallet.value = Input::getu32(5, 99999);
			} else if (touching(touch, playerButtons[2])) {
					SaveFile->players[cp]->PlayerTan = Input::getu32(2, 15);
			} else if (touching(touch, playerButtons[3])) {
					SaveFile->players[cp]->BankAmount.value = Input::getu32(9, 999999999);
			} else if (touching(touch, playerButtons[4])) {
					SaveFile->players[cp]->IslandMedals.value = Input::getu32(4, 9999);
			} else if (touching(touch, playerButtons[5])) {
					SaveFile->players[cp]->MeowCoupons.value = Input::getu32(4, 9999);
			}

			} else if (currentPage == 2) {
				if (touching(touch, playerButtons[0])) {
					PlayerManagement::PlayerMaxBank(cp, pBank);
			} else if (touching(touch, playerButtons[1])) {
					PlayerManagement::PlayerMaxMedals(cp, pMedals);
			} else if (touching(touch, playerButtons[2])) {
					PlayerManagement::PlayerMaxCoupons(cp, pCoupons);
			} else if (touching(touch, playerButtons[3])) {
					PlayerManagement::PlayerClearBank(cp, pBank);
			} else if (touching(touch, playerButtons[4])) {
					PlayerManagement::PlayerClearMedals(cp, pMedals);
			} else if (touching(touch, playerButtons[5])) {
					PlayerManagement::PlayerClearCoupons(cp, pCoupons);
			}
			}
		}

	if (hDown & KEY_B) {
		Selection = 0;
		screen = 1;
	}
}