/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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

#include "gui.hpp"
#include "input.hpp"
#include "playerManagement.hpp"
#include "screenCommon.hpp"
#include "stringUtils.hpp"
#include "wwPlayer.hpp"
#include "wwsave.hpp"

#include <nds.h>

extern WWSave* SaveFile;

// Set Bells to Player.
void PlayerManagement::setBells(int currentPlayer) {
	int num = Input::getInt("Enter the amount of Bells.", 99999);
	if(num != -1) {
		SaveFile->players[currentPlayer]->Bells = num;
	}
	// Screen clear.
	Gui::clearScreen(true, true);
	Gui::clearScreen(false, true);
}
// Set Name to Player.
void PlayerManagement::setName(int currentPlayer) {
	std::string test = Input::getLine("Enter the Playername.", 8);
	if (test != "") {
		SaveFile->players[currentPlayer]->Name = StringUtils::utf8to16(test);
	}
	// Screen clear.
	Gui::clearScreen(true, true);
	Gui::clearScreen(false, true);
}
// Set Gender to Player.
void PlayerManagement::setGender(int currentPlayer) {
	// Clear and draw GUI.
	std::string Gender;
	Gui::clearScreen(true, true);
	Gui::clearScreen(false, true);
	
	// Prepare right Gender for the Message prompt.
	if (SaveFile->players[currentPlayer]->Gender == 0) {
		Gender += "Female";
	} else {
		Gender += "Male";
	}

	printTextTinted("Change Gender to: " + Gender + "?", TextColor::gray, 5, 0, false, true);
	if (Input::getBool()) {
		if (SaveFile->players[currentPlayer]->Gender == 0) {
			SaveFile->players[currentPlayer]->Gender = 1;
		} else {
			SaveFile->players[currentPlayer]->Gender = 0;
		}
	}
	Gui::clearScreen(true, true);
	Gui::clearScreen(false, true);
}