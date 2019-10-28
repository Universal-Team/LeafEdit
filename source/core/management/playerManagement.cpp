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

#include "logging.hpp"

#include "common/config.hpp"
#include "common/utils.hpp"

#include "core/management/playerManagement.hpp"

#include "gui/keyboard.hpp"
#include "gui/msg.hpp"

#include "gui/screens/screenCommon.hpp"

#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"

#include <3ds.h>

extern Save* SaveFile;


// Player Names.
void PlayerManagement::PlayerName(int currentPlayer)
{
	C3D_FrameEnd(0);
	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NORMAL, 2, 8);
	swkbdSetHintText(&state, "Please type in the Player Name.");
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, SWKBD_FILTER_PROFANITY, 0);
	char input[9]	= {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[8]		= '\0';
	if (ret == SWKBD_BUTTON_CONFIRM)
	{
		// Set stuff.
		SaveFile->players[currentPlayer]->Name = StringUtils::UTF8toUTF16(input);

		// Log stuff.
		std::string log = "Current Player: ";
		log += std::to_string(currentPlayer + 1);
		log += " - ";
		log += "Got a name change to: ";
		log += input;
		Logging::writeToLog(log.c_str());
	}
}

// Player Wallet Amounts.
void PlayerManagement::PlayerWallet(int currentPlayer)
{
	C3D_FrameEnd(0);
	static SwkbdState state;
	static bool first = true;
	if (first)
	{
		swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 5);
		first = false;
	}
	swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
	char input[6]	= {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[5]		= '\0';
	if (ret == SWKBD_BUTTON_CONFIRM)
	{
		// Set stuff.
		u32 wallet = (u32)std::min(std::stoi(input), 99999);
		SaveFile->players[currentPlayer]->Wallet.value = wallet;

		// Log stuff.
		std::string log = "Current Player: ";
		log += std::to_string(currentPlayer + 1);
		log += " - ";
		log += "Got a Wallet Amount change to: ";
		log += std::to_string(wallet);
		Logging::writeToLog(log.c_str());
	}
}


// Tan! -> To:Do :: Implement a List to select a Tan instead of the keyboard.
void PlayerManagement::PlayerTan(int currentPlayer)
{
	C3D_FrameEnd(0);
	static SwkbdState state;
	static bool first = true;
	if (first)
	{
		swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 2);
		first = false;
	}
	swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
	char input[3]	= {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[2]		= '\0';
	if (ret == SWKBD_BUTTON_CONFIRM)
	{
		// Set stuff.
		u16 tan = (u16)std::min(std::stoi(input), 15);
		SaveFile->players[currentPlayer]->PlayerTan = tan;

		// Log stuff.
		std::string log = "Current Player: ";
		log += std::to_string(currentPlayer + 1);
		log += " - ";
		log += "Got a Tan Value change to: ";
		log += std::to_string(tan);
		Logging::writeToLog(log.c_str());
	}
}

// Bank Stuff.
void PlayerManagement::PlayerBank(int currentPlayer)
{
	C3D_FrameEnd(0);
	static SwkbdState state;
	static bool first = true;
	if (first)
	{
		swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 9);
		first = false;
	}
	swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
	char input[10]	= {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[9]		= '\0';
	if (ret == SWKBD_BUTTON_CONFIRM)
	{
		// Set stuff.
		u32 bank = (u32)std::min(std::stoi(input), 999999999);
		SaveFile->players[currentPlayer]->BankAmount.value = bank;

		// Log stuff.
		std::string log = "Current Player: ";
		log += std::to_string(currentPlayer + 1);
		log += " - ";
		log += "Got a Bank Amount change to: ";
		log += std::to_string(bank);
		Logging::writeToLog(log.c_str());
	}
}

void PlayerManagement::PlayerMaxBank(int currentPlayer, std::string bank)
{
	bank = "999999999";
	SaveFile->players[currentPlayer]->BankAmount.value = static_cast<u32>(std::stoi(bank.c_str()));

	std::string log = "Current Player: ";
	log += std::to_string(currentPlayer + 1);
	log += " - ";
	log += "Got the Bank Amount: maximized.";
	Logging::writeToLog(log.c_str());
}

void PlayerManagement::PlayerClearBank(int currentPlayer, std::string bank)
{
	bank = "0";
	SaveFile->players[currentPlayer]->BankAmount.value = static_cast<u32>(std::stoi(bank.c_str()));

	std::string log = "Current Player: ";
	log += std::to_string(currentPlayer + 1);
	log += " - ";
	log += "Got the Bank Amount: cleared.";
	Logging::writeToLog(log.c_str());
}


// Medal Stuff.
void PlayerManagement::PlayerMedals(int currentPlayer)
{
	C3D_FrameEnd(0);
	static SwkbdState state;
	static bool first = true;
	if (first)
	{
		swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 4);
		first = false;
	}

	swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
	char input[5]	= {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[4]		= '\0';
	if (ret == SWKBD_BUTTON_CONFIRM)
	{
		// Set stuff.
		u32 medals = (u32)std::min(std::stoi(input), 9999);
		SaveFile->players[currentPlayer]->IslandMedals.value = medals;

		// Log stuff.
		std::string log = "Current Player: ";
		log += std::to_string(currentPlayer + 1);
		log += " - ";
		log += "Got a Medals Amount change to: ";
		log += std::to_string(medals);
		Logging::writeToLog(log.c_str());
	}
}

void PlayerManagement::PlayerMaxMedals(int currentPlayer, std::string medals)
{
	medals = "9999";
	SaveFile->players[currentPlayer]->IslandMedals.value = static_cast<u32>(std::stoi(medals.c_str()));

	std::string log = "Current Player: ";
	log += std::to_string(currentPlayer + 1);
	log += " - ";
	log += "Got the Medals Amount: maximized.";
	Logging::writeToLog(log.c_str());
}

void PlayerManagement::PlayerClearMedals(int currentPlayer, std::string medals)
{
	medals = "0";
	SaveFile->players[currentPlayer]->IslandMedals.value = static_cast<u32>(std::stoi(medals.c_str()));

	std::string log = "Current Player: ";
	log += std::to_string(currentPlayer + 1);
	log += " - ";
	log += "Got the Medals Amount: cleared.";
	Logging::writeToLog(log.c_str());
}


// Coupon Stuff.
void PlayerManagement::PlayerCoupons(int currentPlayer)
{
	C3D_FrameEnd(0);
	static SwkbdState state;
	static bool first = true;
	if (first)
	{
		swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 4);
		first = false;
	}

	swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
	char input[5]	= {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[4]		= '\0';
	if (ret == SWKBD_BUTTON_CONFIRM)
	{

		// Set stuff.
		u32 coupons = (u32)std::min(std::stoi(input), 9999);
		SaveFile->players[currentPlayer]->MeowCoupons.value = coupons;

		// Log stuff.
		std::string log = "Current Player: ";
		log += std::to_string(currentPlayer + 1);
		log += " - ";
		log += "Got a Coupons Amount change to: ";
		log += std::to_string(coupons);
		Logging::writeToLog(log.c_str());
	}
}

void PlayerManagement::PlayerMaxCoupons(int currentPlayer, std::string coupons)
{
	coupons = "9999";
	SaveFile->players[currentPlayer]->MeowCoupons.value = static_cast<u32>(std::stoi(coupons.c_str()));

	std::string log = "Current Player: ";
	log += std::to_string(currentPlayer + 1);
	log += " - ";
	log += "Got the Coupons Amount: maximized.";
	Logging::writeToLog(log.c_str());
}

void PlayerManagement::PlayerClearCoupons(int currentPlayer, std::string coupons)
{
	coupons = "0";
	SaveFile->players[currentPlayer]->MeowCoupons.value = static_cast<u32>(std::stoi(coupons.c_str()));

	std::string log = "Current Player: ";
	log += std::to_string(currentPlayer + 1);
	log += " - ";
	log += "Got the Coupons Amount: cleared.";
	Logging::writeToLog(log.c_str());
}