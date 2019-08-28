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

std::string player2Name; // Player Name [2].
std::string player2Wallet; // Player Wallet Amount [2].
std::string player2Tan; //Player Tan [2].

std::string player3Name; // Player Name [3].
std::string player3Wallet; // Player Wallet Amount [3].
std::string player3Tan; //Player Tan [3].

std::string player4Name; // Player Name [4].
std::string player4Wallet; // Player Wallet Amount [4].
std::string player4Tan; // Player Tan [4].

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
	std::string PlayerName = Lang::editor[5];
	PlayerName += " ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[0]->Name).c_str();

	// Display Town Name.
	std::string TownName = Lang::editor[4];
	TownName += " ";
	TownName += StringUtils::UTF16toUTF8(SaveFile->players[0]->TownName).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[0]->Wallet.value));
	std::string WalletAmount = Lang::editor[3];
	WalletAmount += " ";
	WalletAmount += Wallet.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[0]->BankAmount.value));
	std::string BankAmount = Lang::editor[2];
	BankAmount += " ";
	BankAmount += Bank.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[0]->PlayerTan));
	std::string TanPlayer = "Current Tan: ";
	TanPlayer += Tan.c_str();

	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);

	// Game Specific Things.
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 40, 0.8f, WHITE, PlayerName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, TownName.c_str()))/2, 70, 0.8f, WHITE, TownName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 100, 0.8f, WHITE, WalletAmount.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 130, 0.8f, WHITE, BankAmount.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 160, 0.8f, WHITE, TanPlayer.c_str());
	DrawCurrentPlayer();

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);

	Gui::Draw_Rect(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, WHITE);
	Gui::Draw_Rect(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, WHITE);
	Gui::Draw_Rect(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, WHITE);

	if (Selection == 0) {
		Gui::drawAnimatedSelector(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 1) {
		Gui::drawAnimatedSelector(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 2) {
		Gui::drawAnimatedSelector(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, .030f, C2D_Color32(0, 0, 0, 0));
	}


	// Display Player Name.
	Gui::DrawString(0, playerButtons[0].y+10, 0.6f, WHITE, Lang::editor[5]);
	Gui::DrawString(playerButtons[0].x+30, playerButtons[0].y+10, 0.72f, BLACK, player1Name.c_str());

	// Display Wallet Amount of the first Player. 
	Gui::DrawString(0, playerButtons[1].y+10, 0.6f, WHITE, Lang::editor[3]);
	Gui::DrawString(playerButtons[1].x+30, playerButtons[1].y+10, 0.72f, BLACK, player1Wallet.c_str());

	// Display current Tan Value.
	Gui::DrawString(0, playerButtons[2].y+10, 0.6f, WHITE, "Tan Value: ");
	Gui::DrawString(playerButtons[2].x+30, playerButtons[2].y+10, 0.72f, BLACK, player1Tan.c_str());
}

void PlayerEditor::Player2Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = Lang::editor[5];
	PlayerName += " ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[1]->Name).c_str();

	// Display Town Name.
	std::string TownName = Lang::editor[4];
	TownName += " ";
	TownName += StringUtils::UTF16toUTF8(SaveFile->players[1]->TownName).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[1]->Wallet.value));
	std::string WalletAmount = Lang::editor[3];
	WalletAmount += " ";
	WalletAmount += Wallet.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[1]->BankAmount.value));
	std::string BankAmount = Lang::editor[2];
	BankAmount += " ";
	BankAmount += Bank.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[1]->PlayerTan));
	std::string TanPlayer = "Current Tan: ";
	TanPlayer += Tan.c_str();

	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);

	// Game Specific Things.
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 40, 0.8f, WHITE, PlayerName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, TownName.c_str()))/2, 70, 0.8f, WHITE, TownName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 100, 0.8f, WHITE, WalletAmount.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 130, 0.8f, WHITE, BankAmount.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 160, 0.8f, WHITE, TanPlayer.c_str());
	DrawCurrentPlayer();

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);

	Gui::Draw_Rect(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, WHITE);
	Gui::Draw_Rect(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, WHITE);
	Gui::Draw_Rect(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, WHITE);

	if (Selection == 0) {
		Gui::drawAnimatedSelector(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 1) {
		Gui::drawAnimatedSelector(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 2) {
		Gui::drawAnimatedSelector(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, .030f, C2D_Color32(0, 0, 0, 0));
	}


	// Display Player Name.
	Gui::DrawString(0, playerButtons[0].y+10, 0.6f, WHITE, Lang::editor[5]);
	Gui::DrawString(playerButtons[0].x+30, playerButtons[0].y+10, 0.72f, BLACK, player2Name.c_str());

	// Display Wallet Amount of the first Player. 
	Gui::DrawString(0, playerButtons[1].y+10, 0.6f, WHITE, Lang::editor[3]);
	Gui::DrawString(playerButtons[1].x+30, playerButtons[1].y+10, 0.72f, BLACK, player2Wallet.c_str());

		// Display current Tan Value.
	Gui::DrawString(0, playerButtons[2].y+10, 0.6f, WHITE, "Tan Value: ");
	Gui::DrawString(playerButtons[2].x+30, playerButtons[2].y+10, 0.72f, BLACK, player2Tan.c_str());
}


void PlayerEditor::Player3Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = Lang::editor[5];
	PlayerName += " ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[2]->Name).c_str();

	// Display Town Name.
	std::string TownName = Lang::editor[4];
	TownName += " ";
	TownName += StringUtils::UTF16toUTF8(SaveFile->players[2]->TownName).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[2]->Wallet.value));
	std::string WalletAmount = Lang::editor[3];
	WalletAmount += " ";
	WalletAmount += Wallet.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[2]->BankAmount.value));
	std::string BankAmount = Lang::editor[2];
	BankAmount += " ";
	BankAmount += Bank.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[2]->PlayerTan));
	std::string TanPlayer = "Current Tan: ";
	TanPlayer += Tan.c_str();

	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);

	// Game Specific Things.
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 40, 0.8f, WHITE, PlayerName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, TownName.c_str()))/2, 70, 0.8f, WHITE, TownName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 100, 0.8f, WHITE, WalletAmount.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 130, 0.8f, WHITE, BankAmount.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 160, 0.8f, WHITE, TanPlayer.c_str());
	DrawCurrentPlayer();

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);

	Gui::Draw_Rect(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, WHITE);
	Gui::Draw_Rect(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, WHITE);
	Gui::Draw_Rect(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, WHITE);

	if (Selection == 0) {
		Gui::drawAnimatedSelector(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 1) {
		Gui::drawAnimatedSelector(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 2) {
		Gui::drawAnimatedSelector(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, .030f, C2D_Color32(0, 0, 0, 0));
	}


	// Display Player Name.
	Gui::DrawString(0, playerButtons[0].y+10, 0.6f, WHITE, Lang::editor[5]);
	Gui::DrawString(playerButtons[0].x+30, playerButtons[0].y+10, 0.72f, BLACK, player3Name.c_str());

	// Display Wallet Amount of the first Player. 
	Gui::DrawString(0, playerButtons[1].y+10, 0.6f, WHITE, Lang::editor[3]);
	Gui::DrawString(playerButtons[1].x+30, playerButtons[1].y+10, 0.72f, BLACK, player3Wallet.c_str());

	// Display current Tan Value.
	Gui::DrawString(0, playerButtons[2].y+10, 0.6f, WHITE, "Tan Value: ");
	Gui::DrawString(playerButtons[2].x+30, playerButtons[2].y+10, 0.72f, BLACK, player3Tan.c_str());
}


void PlayerEditor::Player4Draw(void) const
{

	// Display First Player Name.
	std::string PlayerName = Lang::editor[5];
	PlayerName += " ";
	PlayerName += StringUtils::UTF16toUTF8(SaveFile->players[3]->Name).c_str();

	// Display Town Name.
	std::string TownName = Lang::editor[4];
	TownName += " ";
	TownName += StringUtils::UTF16toUTF8(SaveFile->players[3]->TownName).c_str();

	// Display the Amount of Bells inside the Wallet.
	std::string Wallet = std::to_string((SaveFile->players[3]->Wallet.value));
	std::string WalletAmount = Lang::editor[3];
	WalletAmount += " ";
	WalletAmount += Wallet.c_str();

	// Display the Amount of Bells from the Bank.
	std::string Bank = std::to_string((SaveFile->players[3]->BankAmount.value));
	std::string BankAmount = Lang::editor[2];
	BankAmount += " ";
	BankAmount += Bank.c_str();

	// Display the current Tan of the Player.
	std::string Tan = std::to_string((SaveFile->players[3]->PlayerTan));
	std::string TanPlayer = "Current Tan: ";
	TanPlayer += Tan.c_str();

	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);

	// Game Specific Things.
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, PlayerName.c_str()))/2, 40, 0.8f, WHITE, PlayerName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, TownName.c_str()))/2, 70, 0.8f, WHITE, TownName.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, WalletAmount.c_str()))/2, 100, 0.8f, WHITE, WalletAmount.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, BankAmount.c_str()))/2, 130, 0.8f, WHITE, BankAmount.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, TanPlayer.c_str()))/2, 160, 0.8f, WHITE, TanPlayer.c_str());
	DrawCurrentPlayer();

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);

	Gui::Draw_Rect(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, WHITE);
	Gui::Draw_Rect(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, WHITE);
	Gui::Draw_Rect(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, WHITE);

	if (Selection == 0) {
		Gui::drawAnimatedSelector(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 1) {
		Gui::drawAnimatedSelector(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 2) {
		Gui::drawAnimatedSelector(playerButtons[2].x, playerButtons[2].y, playerButtons[2].w, playerButtons[2].h, .030f, C2D_Color32(0, 0, 0, 0));
	}


	// Display Player Name.
	Gui::DrawString(0, playerButtons[0].y+10, 0.6f, WHITE, Lang::editor[5]);
	Gui::DrawString(playerButtons[0].x+30, playerButtons[0].y+10, 0.72f, BLACK, player4Name.c_str());

	// Display Wallet Amount of the 4. Player. 
	Gui::DrawString(0, playerButtons[1].y+10, 0.6f, WHITE, Lang::editor[3]);
	Gui::DrawString(playerButtons[1].x+30, playerButtons[1].y+10, 0.72f, BLACK, player4Wallet.c_str());

	// Display current Tan Value.
	Gui::DrawString(0, playerButtons[2].y+10, 0.6f, WHITE, "Tan Value: ");
	Gui::DrawString(playerButtons[2].x+30, playerButtons[2].y+10, 0.72f, BLACK, player4Tan.c_str());
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
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, activePlayer.c_str()))/2, 215, 0.8f, WHITE, activePlayer.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str());
}


void PlayerEditor::Player1Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						player1Name = Input::getLine(8, Lang::playerEditor[0]);
						SaveFile->players[0]->Name = StringUtils::UTF8toUTF16(player1Name.c_str());
						break;
				}   case 1:
						player1Wallet = Input::Numpad(5, "Please type in the Wallet Value.");
						SaveFile->players[0]->Wallet.value = static_cast<u32>(std::stoi(player1Wallet.c_str()));
						break;
				 	case 2: {
						player1Tan = Input::Numpad(2, "Please type in the Tan Value.");
						SaveFile->players[0]->PlayerTan = static_cast<u16>(std::stoi(player1Tan.c_str()));
						break;
					 }
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
		if(Selection < 2)	Selection++;
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						player2Name = Input::getLine(8, Lang::playerEditor[0]);
						SaveFile->players[1]->Name = StringUtils::UTF8toUTF16(player2Name.c_str());
						break;
				}   case 1:
						player2Wallet = Input::Numpad(5, "Please type in the Wallet Value.");
						SaveFile->players[1]->Wallet.value = static_cast<u32>(std::stoi(player2Wallet.c_str()));
						break;
				 	case 2: {
						player2Tan = Input::Numpad(2, "Please type in the Tan Value.");
						SaveFile->players[1]->PlayerTan = static_cast<u16>(std::stoi(player2Tan.c_str()));
						break;
					 }
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
		if(Selection < 2)	Selection++;
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						player3Name = Input::getLine(8, Lang::playerEditor[0]);
						SaveFile->players[2]->Name = StringUtils::UTF8toUTF16(player3Name.c_str());
						break;
				}   case 1:
						player3Wallet = Input::Numpad(5, "Please type in the Wallet Value.");
						SaveFile->players[2]->Wallet.value = static_cast<u32>(std::stoi(player3Wallet.c_str()));
						break;
				 	case 2: {
						player3Tan = Input::Numpad(2, "Please type in the Tan Value.");
						SaveFile->players[2]->PlayerTan = static_cast<u16>(std::stoi(player3Tan.c_str()));
						break;
					 }
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
		if(Selection < 2)	Selection++;
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						player4Name = Input::getLine(8, Lang::playerEditor[0]);
						SaveFile->players[3]->Name = StringUtils::UTF8toUTF16(player4Name.c_str());
						break;
				}   case 1:
						player4Wallet = Input::Numpad(5, "Please type in the Wallet Value.");
						SaveFile->players[3]->Wallet.value = static_cast<u32>(std::stoi(player4Wallet.c_str()));
						break;
				 	case 2: {
						player4Tan = Input::Numpad(2, "Please type in the Tan Value.");
						SaveFile->players[3]->PlayerTan = static_cast<u16>(std::stoi(player4Tan.c_str()));
						break;
					 }
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