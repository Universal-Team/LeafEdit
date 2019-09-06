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
#include "gui/msg.hpp"

#include "gui/screens/screenCommon.hpp"

#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"

#include <3ds.h>

extern Save* SaveFile;

// Player Stuff.
extern std::string player1Name; // Player Name [Mayor].
extern std::string player1Wallet; // Player Wallet Amount [Mayor].
extern std::string player1Tan; //Player Tan [Mayor].
extern std::string player1Bank;
extern std::string player1Medals;
extern std::string player1Coupons;

extern std::string player2Name; // Player Name [2].
extern std::string player2Wallet; // Player Wallet Amount [2].
extern std::string player2Tan; //Player Tan [2].
extern std::string player2Bank;
extern std::string player2Medals;
extern std::string player2Coupons;

extern std::string player3Name; // Player Name [3].
extern std::string player3Wallet; // Player Wallet Amount [3].
extern std::string player3Tan; //Player Tan [3].
extern std::string player3Bank;
extern std::string player3Medals;
extern std::string player3Coupons;

extern std::string player4Name; // Player Name [4].
extern std::string player4Wallet; // Player Wallet Amount [4].
extern std::string player4Tan; // Player Tan [4].
extern std::string player4Bank;
extern std::string player4Medals;
extern std::string player4Coupons;


// Player Names.
void PlayerManagement::PlayerName(int currentPlayer, std::string name)
{
	name = Input::getLine(8, Lang::playerEditor[0]);
	if (name != "") {
		SaveFile->players[currentPlayer]->Name = StringUtils::UTF8toUTF16(name.c_str());
	} else {
		Msg::DisplayWarnMsg("This is not a valid Playername!");
	}
}


// Player Wallet Amounts.
void PlayerManagement::PlayerWallet(int currentPlayer, std::string wallet)
{
	wallet = Input::Numpad(5, Lang::keyboard[0]);
	if (wallet != "") {
		SaveFile->players[currentPlayer]->Wallet.value = static_cast<u32>(std::stoi(wallet.c_str()));
	} else {
		Msg::DisplayWarnMsg("This is not a valid Wallet Amount!");
	}
}


// Tan! -> To:Do :: Implement a List to select a Tan instead of the keyboard.
void PlayerManagement::PlayerTan(int currentPlayer, std::string tan)
{
    tan = Input::Numpad(2, Lang::keyboard[1]);
	if (tan != "") {
		if (u16 tanValue = static_cast<u16>(std::stoi(tan.c_str())) =< 16) {
			SaveFile->players[currentPlayer]->PlayerTan = static_cast<u16>(std::stoi(tan.c_str()));
		} else {
			Msg::DisplayWarnMsg("This Tan Value is too high!");
		}
	} else {
		Msg::DisplayWarnMsg("This is not a valid Tan!");
	}
}

// Bank Stuff.
void PlayerManagement::PlayerBank(int currentPlayer, std::string bank)
{
	bank = Input::Numpad(9, Lang::keyboard[2]);
	if (bank != "") {
		SaveFile->players[currentPlayer]->BankAmount.value = static_cast<u32>(std::stoi(bank.c_str()));
	} else {
		Msg::DisplayWarnMsg("This is not a valid Bank Amount!");
	}
}

void PlayerManagement::PlayerMaxBank(int currentPlayer, std::string bank)
{
	bank = "999999999";
	SaveFile->players[currentPlayer]->BankAmount.value = static_cast<u32>(std::stoi(bank.c_str()));
}

void PlayerManagement::PlayerClearBank(int currentPlayer, std::string bank)
{
	bank = "0";
	SaveFile->players[currentPlayer]->BankAmount.value = static_cast<u32>(std::stoi(bank.c_str()));
}


// Medal Stuff.
void PlayerManagement::PlayerMedals(int currentPlayer, std::string medals)
{
	medals = Input::Numpad(4, Lang::keyboard[3]);
	if (medals != "") {
		SaveFile->players[currentPlayer]->IslandMedals.value = static_cast<u32>(std::stoi(medals.c_str()));
	} else {
		Msg::DisplayWarnMsg("This is not a valid Medals Amount!");
	}
}

void PlayerManagement::PlayerMaxMedals(int currentPlayer, std::string medals)
{
	medals = "9999";
	SaveFile->players[currentPlayer]->IslandMedals.value = static_cast<u32>(std::stoi(medals.c_str()));
}

void PlayerManagement::PlayerClearMedals(int currentPlayer, std::string medals)
{
	medals = "0";
	SaveFile->players[currentPlayer]->IslandMedals.value = static_cast<u32>(std::stoi(medals.c_str()));
}


// Coupon Stuff.
void PlayerManagement::PlayerCoupons(int currentPlayer, std::string coupons)
{
	coupons = Input::Numpad(4, Lang::keyboard[4]);
	if (coupons != "") {
		SaveFile->players[currentPlayer]->MeowCoupons.value = static_cast<u32>(std::stoi(coupons.c_str()));
	} else {
		Msg::DisplayWarnMsg("This is not a valid Coupon Amount!");
	}
}

void PlayerManagement::PlayerMaxCoupons(int currentPlayer, std::string coupons)
{
	coupons = "9999";
	SaveFile->players[currentPlayer]->MeowCoupons.value = static_cast<u32>(std::stoi(coupons.c_str()));
}

void PlayerManagement::PlayerClearCoupons(int currentPlayer, std::string coupons)
{
	coupons = "0";
	SaveFile->players[currentPlayer]->MeowCoupons.value = static_cast<u32>(std::stoi(coupons.c_str()));
}