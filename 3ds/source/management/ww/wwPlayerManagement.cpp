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

#include "config.hpp"
#include "keyboard.hpp"
#include "msg.hpp"
#include "screenCommon.hpp"
#include "utils.hpp"
#include "wwPlayer.hpp"
#include "wwPlayerManagement.hpp"
#include "wwsave.hpp"

#include <3ds.h>

extern WWSave* WWSaveFile;

// Set Bells to Player.
void WWPlayerManagement::setBells(int currentPlayer) {
	WWSaveFile->players[currentPlayer]->Bells = Input::handleu32(5, "Enter the amount of Bells.", 99999, WWSaveFile->players[currentPlayer]->Bells);
}
// Set Name to Player.
void WWPlayerManagement::setName(int currentPlayer) {
	WWSaveFile->players[currentPlayer]->Name = Input::handleu16String(8, "Enter the Player name.", WWSaveFile->players[currentPlayer]->Name);
}
// Set Gender to Player.
void WWPlayerManagement::setGender(int currentPlayer) {
	std::string Gender;
	// Get right Gender.
	if (WWSaveFile->players[currentPlayer]->Gender == 0) {
		Gender += "Female";
	} else {
		Gender += "Male";
	}

	if (Msg::promptMsg("Change Gender to: " + Gender + "?")) {
		if (WWSaveFile->players[currentPlayer]->Gender == 0) {
			WWSaveFile->players[currentPlayer]->Gender = 1;
		} else if (WWSaveFile->players[currentPlayer]->Gender == 1) {
			WWSaveFile->players[currentPlayer]->Gender = 0;
		}
	}
}