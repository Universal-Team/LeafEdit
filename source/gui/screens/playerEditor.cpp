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

// Player Stuff. p -> Player.
std::string pName;
std::string pWallet; 
std::string pTan; 
std::string pBank;
std::string pMedals;
std::string pCoupons;


extern bool touching(touchPosition touch, Structs::ButtonPos button);

void PlayerEditor::Draw(void) const
{
	if (currentPlayer == 1) {
		Player1Draw();
	} else if (currentPlayer == 2) {
		Player2Draw();
	} else if (currentPlayer == 3) {
		Player3Draw();
	} else if (currentPlayer == 4) {
		Player4Draw();
	}

	DrawCurrentPlayer();
	DrawBottom();
}

// This draws the Bottom Screen, because it is the same on every Player Screen.
void PlayerEditor::DrawBottom(void) const
{
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
		
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[1]))/2-65, playerButtons[0].y+10, 0.6f, BLACK, Lang::playerEditor[1], 130);

		// Display Wallet Amount. 
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[2]))/2-65, playerButtons[1].y+10, 0.6f, BLACK, Lang::playerEditor[2], 130);

		// Display current Tan Value.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[3]))/2-65, playerButtons[2].y+10, 0.6f, BLACK, Lang::playerEditor[3], 130);


		// Display Bank Amount.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[4]))/2+150-65, playerButtons[3].y+10, 0.6f, BLACK, Lang::playerEditor[4], 130);

		// Display Medal Amount. 
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[5]))/2+150-65, playerButtons[4].y+10, 0.6f, BLACK, Lang::playerEditor[5], 130);

		// Display Coupon Amount.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[6]))/2+150-65, playerButtons[5].y+10, 0.6f, BLACK, Lang::playerEditor[6], 130);

	} else if (currentPage == 2) {

		// Max Bank.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[7]))/2-65, playerButtons[0].y+10, 0.6f, BLACK, Lang::playerEditor[7], 130);

		// Max Medals. 
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[8]))/2-65, playerButtons[1].y+10, 0.6f, BLACK, Lang::playerEditor[8], 130);

		// Max Coupons.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[9]))/2-65, playerButtons[2].y+10, 0.6f, BLACK, Lang::playerEditor[9], 130);

		// Clear Bank.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[10]))/2+150-65+5, playerButtons[3].y+10, 0.6f, BLACK, Lang::playerEditor[10], 130);

		// Clear Medals.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[11]))/2+150-65, playerButtons[4].y+10, 0.6f, BLACK, Lang::playerEditor[11], 130);

		// Clear Coupons.
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::playerEditor[12]))/2+150-65, playerButtons[5].y+10, 0.6f, BLACK, Lang::playerEditor[12], 130);
	}
}

// cp -> Current Player.
void PlayerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Selection / Page.
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
			if(Selection < 5)	Selection++;
	} else if (hDown & KEY_RIGHT) {
		if(currentPage <2) currentPage++;
		Selection = 0;
	} else if (hDown & KEY_LEFT) {
		if(currentPage > 1)	currentPage--;
		Selection = 0;
	}

	// Player Switching.
		if (hDown & KEY_R) {
			if (cp < 3) {
				if (SaveFile->players[cp+1]->Exists()) {
					currentPlayer++;
					cp++;
					// Clear the strings.
					pName = "";
					pWallet = ""; 
					pTan = ""; 
					pBank = "";
					pMedals = "";
					pCoupons = "";
				}
			}
		} else if (hDown & KEY_L) {
			if (cp > 0) {
				if (SaveFile->players[cp-1]->Exists()) {
					currentPlayer--;
					cp--;
					// Clear the strings.
					pName = "";
					pWallet = ""; 
					pTan = ""; 
					pBank = "";
					pMedals = "";
					pCoupons = "";
					}
				}
		}

	// Selection A / Touch.
	if (hDown & KEY_A) {
		if (currentPage == 1) {
				switch(Selection) {
					case 0: {
							PlayerManagement::PlayerName(cp);
							break;
					}	case 1:
							PlayerManagement::PlayerWallet(cp);
							break;
						case 2: {
							PlayerManagement::PlayerTan(cp);
							break;
						} case 3: {
							PlayerManagement::PlayerBank(cp);
							break;
						} case 4: {
							PlayerManagement::PlayerMedals(cp);
							break;
						} case 5: {
							PlayerManagement::PlayerCoupons(cp);
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
					PlayerManagement::PlayerName(cp);
			} else if (touching(touch, playerButtons[1])) {
					PlayerManagement::PlayerWallet(cp);
			} else if (touching(touch, playerButtons[2])) {
					PlayerManagement::PlayerTan(cp);
			} else if (touching(touch, playerButtons[3])) {
					PlayerManagement::PlayerBank(cp);
			} else if (touching(touch, playerButtons[4])) {
					PlayerManagement::PlayerMedals(cp);
			} else if (touching(touch, playerButtons[5])) {
					PlayerManagement::PlayerCoupons(cp);
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
		Gui::screenBack();
		return;
	}
}

void PlayerEditor::Player1Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = Lang::playerEditor[1];
	PlayerName += ": ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[0]->Name).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[0]->Wallet.value));
	std::string WalletAmount = Lang::playerEditor[2];
	WalletAmount += ": ";
	WalletAmount += Wallet.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[0]->PlayerTan));
	std::string TanPlayer = Lang::playerEditor[3];
	TanPlayer += ": ";
	TanPlayer += Tan.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[0]->BankAmount.value));
	std::string BankAmount = Lang::playerEditor[4];
	BankAmount += ": ";
	BankAmount += Bank.c_str();

	// Display the amount of medals.
	std::string Medals = std::to_string((SaveFile->players[0]->IslandMedals.value));
	std::string MedalsAmount = Lang::playerEditor[5];
	MedalsAmount += ": ";
	MedalsAmount += Medals.c_str();

	// Display the amount of Coupons.
	std::string Coupons = std::to_string((SaveFile->players[0]->MeowCoupons.value));
	std::string CouponsAmount = Lang::playerEditor[6];
	CouponsAmount += ": ";
	CouponsAmount += Coupons.c_str();


	Gui::DrawTop();

		// Game Specific Things.
		Gui::sprite(0, sprites_topbox_idx, 40, 37);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 35, 0.8f, WHITE, PlayerName.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 65);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 63, 0.8f, WHITE, WalletAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 93);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 91, 0.8f, WHITE, TanPlayer.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 121);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 119, 0.8f, WHITE, BankAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 149);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, MedalsAmount.c_str()))/2, 147, 0.8f, WHITE, MedalsAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 177);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, CouponsAmount.c_str()))/2, 175, 0.8f, WHITE, CouponsAmount.c_str(), 400);
}

void PlayerEditor::Player2Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = Lang::playerEditor[1];
	PlayerName += ": ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[1]->Name).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[1]->Wallet.value));
	std::string WalletAmount = Lang::playerEditor[2];
	WalletAmount += ": ";
	WalletAmount += Wallet.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[1]->PlayerTan));
	std::string TanPlayer = Lang::playerEditor[3];
	TanPlayer += ": ";
	TanPlayer += Tan.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[1]->BankAmount.value));
	std::string BankAmount = Lang::playerEditor[4];
	BankAmount += ": ";
	BankAmount += Bank.c_str();

	// Display the amount of medals.
	std::string Medals = std::to_string((SaveFile->players[1]->IslandMedals.value));
	std::string MedalsAmount = Lang::playerEditor[5];
	MedalsAmount += ": ";
	MedalsAmount += Medals.c_str();

	// Display the amount of Coupons.
	std::string Coupons = std::to_string((SaveFile->players[1]->MeowCoupons.value));
	std::string CouponsAmount = Lang::playerEditor[6];
	CouponsAmount += ": ";
	CouponsAmount += Coupons.c_str();


	Gui::DrawTop();

		// Game Specific Things.
		Gui::sprite(0, sprites_topbox_idx, 40, 37);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 35, 0.8f, WHITE, PlayerName.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 65);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 63, 0.8f, WHITE, WalletAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 93);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 91, 0.8f, WHITE, TanPlayer.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 121);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 119, 0.8f, WHITE, BankAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 149);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, MedalsAmount.c_str()))/2, 147, 0.8f, WHITE, MedalsAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 177);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, CouponsAmount.c_str()))/2, 175, 0.8f, WHITE, CouponsAmount.c_str(), 400);
}

void PlayerEditor::Player3Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = Lang::playerEditor[1];
	PlayerName += ": ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[2]->Name).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[2]->Wallet.value));
	std::string WalletAmount = Lang::playerEditor[2];
	WalletAmount += ": ";
	WalletAmount += Wallet.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[2]->PlayerTan));
	std::string TanPlayer = Lang::playerEditor[3];
	TanPlayer += ": ";
	TanPlayer += Tan.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[2]->BankAmount.value));
	std::string BankAmount = Lang::playerEditor[4];
	BankAmount += ": ";
	BankAmount += Bank.c_str();

	// Display the amount of medals.
	std::string Medals = std::to_string((SaveFile->players[2]->IslandMedals.value));
	std::string MedalsAmount = Lang::playerEditor[5];
	MedalsAmount += ": ";
	MedalsAmount += Medals.c_str();

	// Display the amount of Coupons.
	std::string Coupons = std::to_string((SaveFile->players[2]->MeowCoupons.value));
	std::string CouponsAmount = Lang::playerEditor[6];
	CouponsAmount += ": ";
	CouponsAmount += Coupons.c_str();


	Gui::DrawTop();

		// Game Specific Things.
		Gui::sprite(0, sprites_topbox_idx, 40, 37);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 35, 0.8f, WHITE, PlayerName.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 65);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 63, 0.8f, WHITE, WalletAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 93);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 91, 0.8f, WHITE, TanPlayer.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 121);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 119, 0.8f, WHITE, BankAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 149);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, MedalsAmount.c_str()))/2, 147, 0.8f, WHITE, MedalsAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 177);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, CouponsAmount.c_str()))/2, 175, 0.8f, WHITE, CouponsAmount.c_str(), 400);
}


void PlayerEditor::Player4Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = Lang::playerEditor[1];
	PlayerName += ": ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[3]->Name).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[3]->Wallet.value));
	std::string WalletAmount = Lang::playerEditor[2];
	WalletAmount += ": ";
	WalletAmount += Wallet.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[3]->PlayerTan));
	std::string TanPlayer = Lang::playerEditor[3];
	TanPlayer += ": ";
	TanPlayer += Tan.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[3]->BankAmount.value));
	std::string BankAmount = Lang::playerEditor[4];
	BankAmount += ": ";
	BankAmount += Bank.c_str();

	// Display the amount of medals.
	std::string Medals = std::to_string((SaveFile->players[3]->IslandMedals.value));
	std::string MedalsAmount = Lang::playerEditor[5];
	MedalsAmount += ": ";
	MedalsAmount += Medals.c_str();

	// Display the amount of Coupons.
	std::string Coupons = std::to_string((SaveFile->players[3]->MeowCoupons.value));
	std::string CouponsAmount = Lang::playerEditor[6];
	CouponsAmount += ": ";
	CouponsAmount += Coupons.c_str();


	Gui::DrawTop();

		// Game Specific Things.
		Gui::sprite(0, sprites_topbox_idx, 40, 37);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 35, 0.8f, WHITE, PlayerName.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 65);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 63, 0.8f, WHITE, WalletAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 93);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 91, 0.8f, WHITE, TanPlayer.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 121);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 119, 0.8f, WHITE, BankAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 149);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, MedalsAmount.c_str()))/2, 147, 0.8f, WHITE, MedalsAmount.c_str(), 400);

		Gui::sprite(0, sprites_topbox_idx, 40, 177);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, CouponsAmount.c_str()))/2, 175, 0.8f, WHITE, CouponsAmount.c_str(), 400);
}

void PlayerEditor::DrawCurrentPlayer(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += Lang::editor[1];
	Title += " ";
	Title += Lang::editor[2];

	std::string activePlayer = Lang::misc[1];
	activePlayer += ": ";

	for (int i = 1; i < 5; i++) {
		if (currentPlayer == i) {
			activePlayer += std::to_string(i);
		}
	}

	std::string currentPages = Lang::misc[0];
	currentPages += ": ";

	for (int i = 1; i < 3; i++) {
		if (currentPage == i) {
			currentPages += std::to_string(i);
		}
	}

	currentPages += " / 2";
	std::string endresult = currentPages;
	endresult += " ";
	endresult += activePlayer;

	Gui::DrawString((400-Gui::GetStringWidth(0.8f, endresult.c_str()))/2, 212, 0.8f, WHITE, endresult.c_str(), 400);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str(), 400);
}