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

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern Save* SaveFile;

// Player 1 / Mayor Stuff.
extern std::string player1Name; // Player Name [Mayor].
extern std::string player1Wallet; // Player Wallet Amount [Mayor].

void PlayerEditor::Draw(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += Lang::editor[1];
	Title += " ";
	Title += Lang::editor[6];

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

	Gui::Draw_Rect(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, WHITE);
	Gui::Draw_Rect(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, WHITE);

	if (Selection == 0) {
		Gui::drawAnimatedSelector(playerButtons[0].x, playerButtons[0].y, playerButtons[0].w, playerButtons[0].h, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (Selection == 1) {
		Gui::drawAnimatedSelector(playerButtons[1].x, playerButtons[1].y, playerButtons[1].w, playerButtons[1].h, .030f, C2D_Color32(0, 0, 0, 0));
	}


	// Display Player Name.
	Gui::DrawString(0, playerButtons[0].y+10, 0.6f, WHITE, Lang::editor[5]);
	Gui::DrawString(playerButtons[0].x+30, playerButtons[0].y+10, 0.72f, BLACK, player1Name.c_str());

	// Display Wallet Amount of the first Player. 
	Gui::DrawString(0, playerButtons[1].y+10, 0.6f, WHITE, Lang::editor[3]);
	Gui::DrawString(playerButtons[1].x+30, playerButtons[1].y+10, 0.72f, BLACK, player1Wallet.c_str());
}

void PlayerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 1)	Selection++;
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						player1Name = Input::getLine(8, Lang::playerEditor[0]);
						break;
				}   case 1:
						player1Wallet = Input::Numpad(5, "Please type in the Wallet Value.");
						break;
					 }
			}

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}