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

#include <3ds.h>
#include <3ds/types.h>
#include <sys/stat.h>
#include "gui/gui.hpp"
#include "core/townManagement.hpp" 
#include "gui/keyboard.hpp" // For the Input Stuff.
#include "lang/langStrings.h" // For the Strings.
#include "common/settings.hpp"

extern bool Gamecard;

// Backup the Save from the AC:NL Cartridge or digital version to the Created Folder. (Will be probably merged to "void TownManagement::BackupTown()").
void TownManagement::BackupTownFiles() // To-Do.
{
}



// Create the Folder for the Backup with Keyboard input. It creates the typed in name to "sdmc:/LeafEdit/Towns/".
void TownManagement::BackupTown()
{
		std::string currentPath;
		std::string saveName = Input::getLine(Lang::typeName);
		currentPath += "sdmc:/LeafEdit/Towns/";
		currentPath += saveName.c_str();
		mkdir(currentPath.c_str(), 0777);
}



// Clear the current Save Data
void TownManagement::CreateNewTown()
{
}



// Restore the selected Town.
void TownManagement::RestoreTown() // To-Do.
{
}


// MEDIATYPE_GAME_CARD -> Gamecart.
// MEDIATYPE_SD -> Installed Title.
// MEDIATYPE_NAND -> NAND Title.


// Restore and Launch the selected Town. The current Commented out code would Launch Animal Crossing : New Leaf [EUR].
void TownManagement::LaunchTown()
{
//	u8 param[0x300];
//	u8 hmac[0x20];
//	memset(param, 0, sizeof(param));
//	memset(hmac, 0, sizeof(hmac));

//	if (Gamecard == false) {
//		APT_PrepareToDoApplicationJump(0, EU_GAMEID, MEDIATYPE_SD);
//	} else if (Gamecard == true) {
//		APT_PrepareToDoApplicationJump(0, EU_GAMEID, MEDIATYPE_GAME_CARD);
//	}
//	APT_DoApplicationJump(param, sizeof(param), hmac);
}



// Show the current Progress.
void TownManagement::ShowProgress() // To-Do.
{
}