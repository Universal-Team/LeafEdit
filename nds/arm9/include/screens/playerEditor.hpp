/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
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

#ifndef _LEAFEDIT_PLAYEREDITOR_HPP
#define _LEAFEDIT_PLAYEREDITOR_HPP

#include "screenCommon.hpp"

#include "structs.hpp"
#include <vector>

class PlayerEditor : public Screen {
public:
	void Draw(void) const override;
	void Logic(u16 hDown, touchPosition touch) override;
private:
	void DrawPlayerSelection(void) const;
	void PlayerSelectionLogic(u16 hDown, touchPosition touch);

	void DrawSubMenu(void) const;
	void SubMenuLogic(u16 hDown, touchPosition touch);

	void DrawPlayerScreen(void) const;
	void PlayerLogic(u16 hDown, touchPosition touch);

	int selection = 0; // Current Player selection.
	int selectedPlayer = 0;
	int maxPlayer = 0;
	int cp = 0; // Current Player.
	int screen = 0; // Screen Mode.
	void DrawPlayerBoxes(void) const;

	std::vector<Structs::ButtonPos> mainButtons = {
		{80, 30, 88, 32, -1}, // Player.
		{80, 80, 88, 32, -1}, // Items.
		{80, 130, 88, 32, -1} // Appearance.
	};

	std::vector<Structs::ButtonPos> playerButtons = {
		{20, 30, 88, 32, -1}, // Bells.
		{20, 80, 88, 32, -1}, // ?.
		{20, 130, 88, 32, -1}, // ?.
		{148, 30, 88, 32, -1}, // ?.
		{148, 80, 88, 32, -1}, // ?.
		{148, 130, 88, 32, -1} // ?.
	};
};

#endif