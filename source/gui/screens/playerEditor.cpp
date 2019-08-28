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

// Player Stuff.
std::string player1Name; // Player Name [Mayor].
std::string player1Wallet; // Player Wallet Amount [Mayor].
std::string player1Tan; //Player Tan [Mayor].
std::string player1Bank;
std::string player1Medals;
std::string player1Coupons;

std::string player2Name; // Player Name [2].
std::string player2Wallet; // Player Wallet Amount [2].
std::string player2Tan; //Player Tan [2].
std::string player2Bank;
std::string player2Medals;
std::string player2Coupons;

std::string player3Name; // Player Name [3].
std::string player3Wallet; // Player Wallet Amount [3].
std::string player3Tan; //Player Tan [3].
std::string player3Bank;
std::string player3Medals;
std::string player3Coupons;

std::string player4Name; // Player Name [4].
std::string player4Wallet; // Player Wallet Amount [4].
std::string player4Tan; // Player Tan [4].
std::string player4Bank;
std::string player4Medals;
std::string player4Coupons;

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
}

void PlayerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (currentPlayer == 1) {
		Player1Logic(hDown, hHeld, touch);
	} else if (currentPlayer == 2) {
		Player2Logic(hDown, hHeld, touch);
	} else if (currentPlayer == 3) {
		Player3Logic(hDown, hHeld, touch);
	} else if (currentPlayer == 4) {
		Player4Logic(hDown, hHeld, touch);
	}
}

void PlayerEditor::Player1Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = "Player Name: ";
	PlayerName += " ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[0]->Name).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[0]->Wallet.value));
	std::string WalletAmount = "Wallet Amount: ";
	WalletAmount += " ";
	WalletAmount += Wallet.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[0]->PlayerTan));
	std::string TanPlayer = "Current Tan: ";
	TanPlayer += Tan.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[0]->BankAmount.value));
	std::string BankAmount = Lang::editor[2];
	BankAmount += " ";
	BankAmount += Bank.c_str();

	// Display the amount of medals.
	std::string Medals = std::to_string((SaveFile->players[0]->IslandMedals.value));
	std::string MedalsAmount = "Medals Amount: ";
	MedalsAmount += Medals.c_str();

	// Display the amount of Coupons.
	std::string Coupons = std::to_string((SaveFile->players[0]->MeowCoupons.value));
	std::string CouponsAmount = "Coupons Amount: ";
	CouponsAmount += Coupons.c_str();


	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);

		// Game Specific Things.
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 35, 0.8f, WHITE, PlayerName.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 60, 0.8f, WHITE, WalletAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 85, 0.8f, WHITE, TanPlayer.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 110, 0.8f, WHITE, BankAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, MedalsAmount.c_str()))/2, 135, 0.8f, WHITE, MedalsAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, CouponsAmount.c_str()))/2, 160, 0.8f, WHITE, CouponsAmount.c_str());


	DrawCurrentPlayer();

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);

	Gui::Draw_Rect(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, WHITE);
	Gui::Draw_Rect(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, WHITE);
	Gui::Draw_Rect(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, WHITE);
	Gui::Draw_Rect(playerButtons[3].x, playerButtons[3].y, playerButtons[3].w, playerButtons[3].h, WHITE);
	Gui::Draw_Rect(playerButtons[4].x, playerButtons[4].y, playerButtons[4].w, playerButtons[4].h, WHITE);
	Gui::Draw_Rect(playerButtons[5].x, playerButtons[5].y, playerButtons[5].w, playerButtons[5].h, WHITE);

	if (Selection == 0) {
		Gui::drawAnimatedSelector(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 1) {
		Gui::drawAnimatedSelector(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 2) {
		Gui::drawAnimatedSelector(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 3) {
		Gui::drawAnimatedSelector(playerButtons[3].x, playerButtons[3].y, playerButtons[3].w, playerButtons[3].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 4) {
		Gui::drawAnimatedSelector(playerButtons[4].x, playerButtons[4].y, playerButtons[4].w, playerButtons[4].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 5) {
		Gui::drawAnimatedSelector(playerButtons[5].x, playerButtons[5].y, playerButtons[5].w, playerButtons[5].h, .030f, C2D_Color32(0, 0, 0, 0));
	}


		// Display Player Name.
		Gui::DrawString(playerButtons[0].x+10, playerButtons[0].y+10, 0.65f, BLACK, "Player Name");

		// Display Wallet Amount. 
		Gui::DrawString(playerButtons[1].x+10, playerButtons[1].y+10, 0.65f, BLACK, "Wallet Amount");

		// Display current Tan Value.
		Gui::DrawString(playerButtons[2].x+10, playerButtons[2].y+10, 0.65f, BLACK, "Tan Value");


		// Display Bank Amount.
		Gui::DrawString(playerButtons[3].x+10, playerButtons[3].y+10, 0.65f, BLACK, "Bank Amount");

		// Display Medal Amount. 
		Gui::DrawString(playerButtons[4].x+10, playerButtons[4].y+10, 0.65f, BLACK, "Medal Amount");

		// Display Coupon Amount.
		Gui::DrawString(playerButtons[5].x+10, playerButtons[5].y+10, 0.65f, BLACK, "Coupon Amount");
}

void PlayerEditor::Player2Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = "Player Name: ";
	PlayerName += " ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[1]->Name).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[1]->Wallet.value));
	std::string WalletAmount = "Wallet Amount: ";
	WalletAmount += " ";
	WalletAmount += Wallet.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[1]->PlayerTan));
	std::string TanPlayer = "Current Tan: ";
	TanPlayer += Tan.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[1]->BankAmount.value));
	std::string BankAmount = Lang::editor[2];
	BankAmount += " ";
	BankAmount += Bank.c_str();

	// Display the amount of medals.
	std::string Medals = std::to_string((SaveFile->players[1]->IslandMedals.value));
	std::string MedalsAmount = "Medals Amount: ";
	MedalsAmount += Medals.c_str();

	// Display the amount of Coupons.
	std::string Coupons = std::to_string((SaveFile->players[1]->MeowCoupons.value));
	std::string CouponsAmount = "Coupons Amount: ";
	CouponsAmount += Coupons.c_str();


	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);

		// Game Specific Things.
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 35, 0.8f, WHITE, PlayerName.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 60, 0.8f, WHITE, WalletAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 85, 0.8f, WHITE, TanPlayer.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 110, 0.8f, WHITE, BankAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, MedalsAmount.c_str()))/2, 135, 0.8f, WHITE, MedalsAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, CouponsAmount.c_str()))/2, 160, 0.8f, WHITE, CouponsAmount.c_str());

	DrawCurrentPlayer();

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);

	Gui::Draw_Rect(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, WHITE);
	Gui::Draw_Rect(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, WHITE);
	Gui::Draw_Rect(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, WHITE);
	Gui::Draw_Rect(playerButtons[3].x, playerButtons[3].y, playerButtons[3].w, playerButtons[3].h, WHITE);
	Gui::Draw_Rect(playerButtons[4].x, playerButtons[4].y, playerButtons[4].w, playerButtons[4].h, WHITE);
	Gui::Draw_Rect(playerButtons[5].x, playerButtons[5].y, playerButtons[5].w, playerButtons[5].h, WHITE);

	if (Selection == 0) {
		Gui::drawAnimatedSelector(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 1) {
		Gui::drawAnimatedSelector(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 2) {
		Gui::drawAnimatedSelector(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 3) {
		Gui::drawAnimatedSelector(playerButtons[3].x, playerButtons[3].y, playerButtons[3].w, playerButtons[3].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 4) {
		Gui::drawAnimatedSelector(playerButtons[4].x, playerButtons[4].y, playerButtons[4].w, playerButtons[4].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 5) {
		Gui::drawAnimatedSelector(playerButtons[5].x, playerButtons[5].y, playerButtons[5].w, playerButtons[5].h, .030f, C2D_Color32(0, 0, 0, 0));
	}


		// Display Player Name.
		Gui::DrawString(playerButtons[0].x+10, playerButtons[0].y+10, 0.65f, BLACK, "Player Name");

		// Display Wallet Amount. 
		Gui::DrawString(playerButtons[1].x+10, playerButtons[1].y+10, 0.65f, BLACK, "Wallet Amount");

		// Display current Tan Value.
		Gui::DrawString(playerButtons[2].x+10, playerButtons[2].y+10, 0.65f, BLACK, "Tan Value");


		// Display Bank Amount.
		Gui::DrawString(playerButtons[3].x+10, playerButtons[3].y+10, 0.65f, BLACK, "Bank Amount");

		// Display Medal Amount. 
		Gui::DrawString(playerButtons[4].x+10, playerButtons[4].y+10, 0.65f, BLACK, "Medal Amount");

		// Display Coupon Amount.
		Gui::DrawString(playerButtons[5].x+10, playerButtons[5].y+10, 0.65f, BLACK, "Coupon Amount");
}


void PlayerEditor::Player3Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = "Player Name: ";
	PlayerName += " ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[2]->Name).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[2]->Wallet.value));
	std::string WalletAmount = "Wallet Amount: ";
	WalletAmount += " ";
	WalletAmount += Wallet.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[2]->PlayerTan));
	std::string TanPlayer = "Current Tan: ";
	TanPlayer += Tan.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[2]->BankAmount.value));
	std::string BankAmount = Lang::editor[2];
	BankAmount += " ";
	BankAmount += Bank.c_str();

	// Display the amount of medals.
	std::string Medals = std::to_string((SaveFile->players[2]->IslandMedals.value));
	std::string MedalsAmount = "Medals Amount: ";
	MedalsAmount += Medals.c_str();

	// Display the amount of Coupons.
	std::string Coupons = std::to_string((SaveFile->players[2]->MeowCoupons.value));
	std::string CouponsAmount = "Coupons Amount: ";
	CouponsAmount += Coupons.c_str();

	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);

		// Game Specific Things.
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 35, 0.8f, WHITE, PlayerName.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 60, 0.8f, WHITE, WalletAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 85, 0.8f, WHITE, TanPlayer.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 110, 0.8f, WHITE, BankAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, MedalsAmount.c_str()))/2, 135, 0.8f, WHITE, MedalsAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, CouponsAmount.c_str()))/2, 160, 0.8f, WHITE, CouponsAmount.c_str());

	DrawCurrentPlayer();

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);

	Gui::Draw_Rect(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, WHITE);
	Gui::Draw_Rect(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, WHITE);
	Gui::Draw_Rect(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, WHITE);
	Gui::Draw_Rect(playerButtons[3].x, playerButtons[3].y, playerButtons[3].w, playerButtons[3].h, WHITE);
	Gui::Draw_Rect(playerButtons[4].x, playerButtons[4].y, playerButtons[4].w, playerButtons[4].h, WHITE);
	Gui::Draw_Rect(playerButtons[5].x, playerButtons[5].y, playerButtons[5].w, playerButtons[5].h, WHITE);

	if (Selection == 0) {
		Gui::drawAnimatedSelector(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 1) {
		Gui::drawAnimatedSelector(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 2) {
		Gui::drawAnimatedSelector(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 3) {
		Gui::drawAnimatedSelector(playerButtons[3].x, playerButtons[3].y, playerButtons[3].w, playerButtons[3].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 4) {
		Gui::drawAnimatedSelector(playerButtons[4].x, playerButtons[4].y, playerButtons[4].w, playerButtons[4].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 5) {
		Gui::drawAnimatedSelector(playerButtons[5].x, playerButtons[5].y, playerButtons[5].w, playerButtons[5].h, .030f, C2D_Color32(0, 0, 0, 0));
	}


		// Display Player Name.
		Gui::DrawString(playerButtons[0].x+10, playerButtons[0].y+10, 0.65f, BLACK, "Player Name");

		// Display Wallet Amount. 
		Gui::DrawString(playerButtons[1].x+10, playerButtons[1].y+10, 0.65f, BLACK, "Wallet Amount");

		// Display current Tan Value.
		Gui::DrawString(playerButtons[2].x+10, playerButtons[2].y+10, 0.65f, BLACK, "Tan Value");


		// Display Bank Amount.
		Gui::DrawString(playerButtons[3].x+10, playerButtons[3].y+10, 0.65f, BLACK, "Bank Amount");

		// Display Medal Amount. 
		Gui::DrawString(playerButtons[4].x+10, playerButtons[4].y+10, 0.65f, BLACK, "Medal Amount");

		// Display Coupon Amount.
		Gui::DrawString(playerButtons[5].x+10, playerButtons[5].y+10, 0.65f, BLACK, "Coupon Amount");
}


void PlayerEditor::Player4Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = "Player Name: ";
	PlayerName += " ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[3]->Name).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[3]->Wallet.value));
	std::string WalletAmount = "Wallet Amount: ";
	WalletAmount += " ";
	WalletAmount += Wallet.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[3]->PlayerTan));
	std::string TanPlayer = "Current Tan: ";
	TanPlayer += Tan.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[3]->BankAmount.value));
	std::string BankAmount = Lang::editor[2];
	BankAmount += " ";
	BankAmount += Bank.c_str();

	// Display the amount of medals.
	std::string Medals = std::to_string((SaveFile->players[3]->IslandMedals.value));
	std::string MedalsAmount = "Medals Amount: ";
	MedalsAmount += Medals.c_str();

	// Display the amount of Coupons.
	std::string Coupons = std::to_string((SaveFile->players[3]->MeowCoupons.value));
	std::string CouponsAmount = "Coupons Amount: ";
	CouponsAmount += Coupons.c_str();

	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);

		// Game Specific Things.
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 35, 0.8f, WHITE, PlayerName.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 60, 0.8f, WHITE, WalletAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 85, 0.8f, WHITE, TanPlayer.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 110, 0.8f, WHITE, BankAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, MedalsAmount.c_str()))/2, 135, 0.8f, WHITE, MedalsAmount.c_str());
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, CouponsAmount.c_str()))/2, 160, 0.8f, WHITE, CouponsAmount.c_str());

	DrawCurrentPlayer();

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);

	Gui::Draw_Rect(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, WHITE);
	Gui::Draw_Rect(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, WHITE);
	Gui::Draw_Rect(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, WHITE);
	Gui::Draw_Rect(playerButtons[3].x, playerButtons[3].y, playerButtons[3].w, playerButtons[3].h, WHITE);
	Gui::Draw_Rect(playerButtons[4].x, playerButtons[4].y, playerButtons[4].w, playerButtons[4].h, WHITE);
	Gui::Draw_Rect(playerButtons[5].x, playerButtons[5].y, playerButtons[5].w, playerButtons[5].h, WHITE);

	if (Selection == 0) {
		Gui::drawAnimatedSelector(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 1) {
		Gui::drawAnimatedSelector(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 2) {
		Gui::drawAnimatedSelector(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 3) {
		Gui::drawAnimatedSelector(playerButtons[3].x, playerButtons[3].y, playerButtons[3].w, playerButtons[3].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 4) {
		Gui::drawAnimatedSelector(playerButtons[4].x, playerButtons[4].y, playerButtons[4].w, playerButtons[4].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 5) {
		Gui::drawAnimatedSelector(playerButtons[5].x, playerButtons[5].y, playerButtons[5].w, playerButtons[5].h, .030f, C2D_Color32(0, 0, 0, 0));
	}

		// Display Player Name.
		Gui::DrawString(playerButtons[0].x+10, playerButtons[0].y+10, 0.65f, BLACK, "Player Name");

		// Display Wallet Amount. 
		Gui::DrawString(playerButtons[1].x+10, playerButtons[1].y+10, 0.65f, BLACK, "Wallet Amount");

		// Display current Tan Value.
		Gui::DrawString(playerButtons[2].x+10, playerButtons[2].y+10, 0.65f, BLACK, "Tan Value");


		// Display Bank Amount.
		Gui::DrawString(playerButtons[3].x+10, playerButtons[3].y+10, 0.65f, BLACK, "Bank Amount");

		// Display Medal Amount. 
		Gui::DrawString(playerButtons[4].x+10, playerButtons[4].y+10, 0.65f, BLACK, "Medal Amount");

		// Display Coupon Amount.
		Gui::DrawString(playerButtons[5].x+10, playerButtons[5].y+10, 0.65f, BLACK, "Coupon Amount");
}

void PlayerEditor::DrawCurrentPlayer(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += Lang::editor[1];
	Title += " ";
	Title += Lang::editor[6];

	std::string activePlayer = "Current Player: ";
	if (currentPlayer == 1) {
		activePlayer += "1";
	} else if (currentPlayer == 2) {
		activePlayer += "2";
	} else if (currentPlayer == 3) {
		activePlayer += "3";
	} else if (currentPlayer == 4) {
		activePlayer += "4";
	}

	std::string currentPages = "Current Page: ";
	if (currentPage == 1) {
		currentPages += "1";
	} else if (currentPage == 2) {
		currentPages += "2";
	}

	currentPages += " / 1";

	Gui::DrawString(225, 215, 0.8f, WHITE, activePlayer.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str());
	Gui::DrawString(0, 215, 0.8f, WHITE, currentPages.c_str());
}


void PlayerEditor::Player1Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 5)	Selection++;
//	} else if (hDown & KEY_RIGHT) {
//		if(currentPage <2) currentPage++;
//	} else if (hDown & KEY_LEFT) {
//		if(currentPage > 1)	currentPage--;
	}

	if (hDown & KEY_A) {
		if (currentPage == 1) {
				switch(Selection) {
					case 0: {
							PlayerManagement::PlayerName(0, player1Name);
							break;
					}   case 1:
							PlayerManagement::PlayerWallet(0, player1Wallet);
							break;
				 		case 2: {
							PlayerManagement::PlayerTan(0, player1Tan);
							break;
					 	} case 3: {
							PlayerManagement::PlayerBank(0, player1Bank);
							break;
						 } case 4: {
							PlayerManagement::PlayerMedals(0, player1Medals);
							break;
						 } case 5: {
							PlayerManagement::PlayerCoupons(0, player1Coupons);
							break;
						 }
					}
			}
		}

		if (hDown & KEY_TOUCH) {
			if (touching(touch, playerButtons[0])) {
				PlayerManagement::PlayerName(0, player1Name);
		} else if (touching(touch, playerButtons[1])) {
				PlayerManagement::PlayerWallet(0, player1Wallet);
		} else if (touching(touch, playerButtons[2])) {
				PlayerManagement::PlayerTan(0, player1Tan);
		} else if (touching(touch, playerButtons[3])) {
				PlayerManagement::PlayerBank(0, player1Bank);
		} else if (touching(touch, playerButtons[4])) {
				PlayerManagement::PlayerMedals(0, player1Medals);
		} else if (touching(touch, playerButtons[5])) {
				PlayerManagement::PlayerCoupons(0, player1Coupons);
		}
		}

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_R) {
		if (SaveFile->players[1]->Exists()) {
			currentPlayer++;
		}
	}
}


void PlayerEditor::Player2Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 5)	Selection++;
//	} else if (hDown & KEY_RIGHT) {
//		if(currentPage <2) currentPage++;
//	} else if (hDown & KEY_LEFT) {
//		if(currentPage > 1)	currentPage--;
	}

	if (hDown & KEY_A) {
		if (currentPage == 1) {
				switch(Selection) {
					case 0: {
							PlayerManagement::PlayerName(1, player2Name);
							break;
					}   case 1:
							PlayerManagement::PlayerWallet(1, player2Wallet);
							break;
				 		case 2: {
							PlayerManagement::PlayerTan(1, player2Tan);
							break;
					 	} case 3: {
							PlayerManagement::PlayerBank(1, player2Bank);
							break;
						 } case 4: {
							PlayerManagement::PlayerMedals(1, player2Medals);
							break;
						 } case 5: {
							PlayerManagement::PlayerCoupons(1, player2Coupons);
							break;
						 }
					}
			}
		}


		if (hDown & KEY_TOUCH) {
			if (touching(touch, playerButtons[0])) {
				PlayerManagement::PlayerName(1, player2Name);
		} else if (touching(touch, playerButtons[1])) {
				PlayerManagement::PlayerWallet(1, player2Wallet);
		} else if (touching(touch, playerButtons[2])) {
				PlayerManagement::PlayerTan(1, player2Tan);
		} else if (touching(touch, playerButtons[3])) {
				PlayerManagement::PlayerBank(1, player2Bank);
		} else if (touching(touch, playerButtons[4])) {
				PlayerManagement::PlayerMedals(1, player2Medals);
		} else if (touching(touch, playerButtons[5])) {
				PlayerManagement::PlayerCoupons(1, player2Coupons);
		}
		}


	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_L) {
		if (SaveFile->players[0]->Exists()) {
			currentPlayer--;
		}
	}

	if (hDown & KEY_R) {
		if (SaveFile->players[2]->Exists()) {
			currentPlayer++;
		}
	}
}


void PlayerEditor::Player3Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 5)	Selection++;
//	} else if (hDown & KEY_RIGHT) {
//		if(currentPage <2) currentPage++;
//	} else if (hDown & KEY_LEFT) {
//		if(currentPage > 1)	currentPage--;
	}

	if (hDown & KEY_A) {
		if (currentPage == 1) {
				switch(Selection) {
					case 0: {
							PlayerManagement::PlayerName(2, player3Name);
							break;
					}   case 1:
							PlayerManagement::PlayerWallet(2, player3Wallet);
							break;
				 		case 2: {
							PlayerManagement::PlayerTan(2, player3Tan);
							break;
					 	} case 3: {
							PlayerManagement::PlayerBank(2, player3Bank);
							break;
						 } case 4: {
							PlayerManagement::PlayerMedals(2, player3Medals);
							break;
						 } case 5: {
							PlayerManagement::PlayerCoupons(2, player3Coupons);
							break;
						 }
					}
			}
		}


		if (hDown & KEY_TOUCH) {
			if (touching(touch, playerButtons[0])) {
				PlayerManagement::PlayerName(2, player3Name);
		} else if (touching(touch, playerButtons[1])) {
				PlayerManagement::PlayerWallet(2, player3Wallet);
		} else if (touching(touch, playerButtons[2])) {
				PlayerManagement::PlayerTan(2, player3Tan);
		} else if (touching(touch, playerButtons[3])) {
				PlayerManagement::PlayerBank(2, player3Bank);
		} else if (touching(touch, playerButtons[4])) {
				PlayerManagement::PlayerMedals(2, player3Medals);
		} else if (touching(touch, playerButtons[5])) {
				PlayerManagement::PlayerCoupons(2, player3Coupons);
		}
		}


	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_L) {
		if (SaveFile->players[1]->Exists()) {
			currentPlayer--;
		}
	}

	if (hDown & KEY_R) {
		if (SaveFile->players[3]->Exists()) {
			currentPlayer++;
		}
	}
}


void PlayerEditor::Player4Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 5)	Selection++;
//	} else if (hDown & KEY_RIGHT) {
//		if(currentPage <2) currentPage++;
//	} else if (hDown & KEY_LEFT) {
//		if(currentPage > 1)	currentPage--;
	}

	if (hDown & KEY_A) {
		if (currentPage == 1) {
				switch(Selection) {
					case 0: {
							PlayerManagement::PlayerName(3, player4Name);
							break;
					}   case 1:
							PlayerManagement::PlayerWallet(3, player4Wallet);
							break;
				 		case 2: {
							PlayerManagement::PlayerTan(3, player4Tan);
							break;
					 	} case 3: {
							PlayerManagement::PlayerBank(3, player4Bank);
							break;
						 } case 4: {
							PlayerManagement::PlayerMedals(3, player4Medals);
							break;
						 } case 5: {
							PlayerManagement::PlayerCoupons(3, player4Coupons);
							break;
						 }
					}
			}
		}

		if (hDown & KEY_TOUCH) {
			if (touching(touch, playerButtons[0])) {
				PlayerManagement::PlayerName(3, player4Name);
		} else if (touching(touch, playerButtons[1])) {
				PlayerManagement::PlayerWallet(3, player4Wallet);
		} else if (touching(touch, playerButtons[2])) {
				PlayerManagement::PlayerTan(3, player4Tan);
		} else if (touching(touch, playerButtons[3])) {
				PlayerManagement::PlayerBank(3, player4Bank);
		} else if (touching(touch, playerButtons[4])) {
				PlayerManagement::PlayerMedals(3, player4Medals);
		} else if (touching(touch, playerButtons[5])) {
				PlayerManagement::PlayerCoupons(3, player4Coupons);
		}
		}


	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_L) {
		if (SaveFile->players[2]->Exists()) {
			currentPlayer--;
		}
	}
}