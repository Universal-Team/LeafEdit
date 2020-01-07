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

#ifndef PLAYEREDITOR_HPP
#define PLAYEREDITOR_HPP

#include "common/structs.hpp"

#include "gui/screens/screen.hpp"

#include <vector>

class PlayerEditor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int Selection = 0; // The current option selection.
	int selectedPlayer; // current Selected Player.
	int currentPage = 1; // Page of the Player Editor.
	int cp = 0; // Current Player.
	int maxPlayer = 0; // Max available players.
	int screen = 0; // Sub menu.

	// Screen Draws.
	void DrawSubMenu(void) const;
	void DrawMainEditor(void) const;
	void DrawPlayerEditor(void) const;

	// Screen Logics.
	void SubMenuLogic(u32 hDOwn, u32 hHeld);
	void MainEditorLogic(u32 hDown, u32 hHeld, touchPosition touch);
	void PlayerEditorLogic(u32 hDown, u32 hHeld, touchPosition touch);

	// Other Draws.
	void DrawNameAndGender(void) const;
	
	// Button Struct.
	std::vector<Structs::ButtonPos> playerButtons = {
		{10, 40, 140, 35, -1}, // Player Name.
		{10, 100, 140, 35, -1}, // Wallet Amount.
		{10, 160, 140, 35, -1}, // Tan.

		{170, 40, 140, 35, -1}, // Bank.
		{170, 100, 140, 35, -1}, // Medals.
		{170, 160, 140, 35, -1}, // Coupons.
	};

	std::vector<Structs::ButtonPos> mainButtons = {
		{90, 40, 140, 35, -1}, // Player.
		{90, 100, 140, 35, -1}, // Items
		{90, 160, 140, 35, -1}, // WIP.
		{293, 213, 27, 27, -1}, // Back to Player Selection.
	};

	// Player Stuff. p -> Player.
	std::string pName;
	std::string pWallet;
	std::string pTan;
	std::string pBank;
	std::string pMedals;
	std::string pCoupons;
};

#endif