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

#ifndef TOWNEDITOR_HPP
#define TOWNEDITOR_HPP

#include "common/structs.hpp"

#include "gui/screens/screen.hpp"
#include "gui/screens/screenCommon.hpp"

#include <vector>

class TownEditor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int subSelection = 0;
	int Mode = 0;

	/* Sub Menu. */
	void DrawSubMenu(void) const;
	void SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);

	/* Mostly selection stuff. */
	int currentPosX = 0;
	int currentPosY = 0;
	int currentAcre = 0;
	u8 acreImage = 0x10;

	/* Town Map Editor. */
	void DrawTownEditor(void) const;
	void editorLogic(u32 hDown, u32 hHeld, touchPosition touch);

	/* Utilities */
	void DrawGrid(void) const;
	void DrawTownMap() const;
	void DrawCurrentPos(void) const;
	void updateAcre(void);


	std::vector<Structs::ButtonPos> mainButtons = {
		{90, 40, 140, 35, -1}, // Acres Editor.
		{90, 100, 140, 35, -1}, // Town Map Editor.
		{90, 160, 140, 35, -1}, // ?
	};
};

#endif