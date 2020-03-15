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

#ifndef ACRESEDITOR_HPP
#define ACRESEDITOR_HPP

#include "common.hpp"
#include "structs.hpp"

#include <vector>

class AcresEditor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int Selection = 0;
	int AcreMode = 0;
	u8 selectedAcre = 0;
	bool FastMode = false;

	void DrawTopSelection(void) const;
	void DrawMap(void) const;
	void DrawFullMap(void) const;
	void DrawIsland(void) const;
	int SelectionToAcre(int selection) const;

	std::vector<Structs::ButtonPos> islandPos = {
		{80, 40, 40, 40},
		{120, 40, 40, 40},
		{160, 40, 40, 40},
		{200, 40, 40, 40},

		{80, 80, 40, 40},
		{120, 80, 40, 40},
		{160, 80, 40, 40},
		{200, 80, 40, 40},

		{80, 120, 40, 40},
		{120, 120, 40, 40},
		{160, 120, 40, 40},
		{200, 120, 40, 40},

		{80, 160, 40, 40},
		{120, 160, 40, 40},
		{160, 160, 40, 40},
		{200, 160, 40, 40}
	};

	std::vector<Structs::ButtonPos> mapPos = {
		{20, 0, 40, 40},
		{60, 0, 40, 40},
		{100, 0, 40, 40},
		{140, 0, 40, 40},
		{180, 0, 40, 40},
		{220, 0, 40, 40},
		{260, 0, 40, 40},

		{20, 40, 40, 40},
		{60, 40, 40, 40},
		{100, 40, 40, 40},
		{140, 40, 40, 40},
		{180, 40, 40, 40},
		{220, 40, 40, 40},
		{260, 40, 40, 40},

		{20, 80, 40, 40},
		{60, 80, 40, 40},
		{100, 80, 40, 40},
		{140, 80, 40, 40},
		{180, 80, 40, 40},
		{220, 80, 40, 40},
		{260, 80, 40, 40},

		{20, 120, 40, 40},
		{60, 120, 40, 40},
		{100, 120, 40, 40},
		{140, 120, 40, 40},
		{180, 120, 40, 40},
		{220, 120, 40, 40},
		{260, 120, 40, 40},

		{20, 160, 40, 40},
		{60, 160, 40, 40},
		{100, 160, 40, 40},
		{140, 160, 40, 40},
		{180, 160, 40, 40},
		{220, 160, 40, 40},
		{260, 160, 40, 40},

		{20, 200, 40, 40},
		{60, 200, 40, 40},
		{100, 200, 40, 40},
		{140, 200, 40, 40},
		{180, 200, 40, 40},
		{220, 200, 40, 40},
		{260, 200, 40, 40}
	};

	std::vector<Structs::ButtonPos> townPos = {
		{60, 40, 40, 40},
		{100, 40, 40, 40},
		{140, 40, 40, 40},
		{180, 40, 40, 40},
		{220, 40, 40, 40},

		{60, 80, 40, 40},
		{100, 80, 40, 40},
		{140, 80, 40, 40},
		{180, 80, 40, 40},
		{220, 80, 40, 40},

		{60, 120, 40, 40},
		{100, 120, 40, 40},
		{140, 120, 40, 40},
		{180, 120, 40, 40},
		{220, 120, 40, 40},

		{60, 160, 40, 40},
		{100, 160, 40, 40},
		{140, 160, 40, 40},
		{180, 160, 40, 40},
		{220, 160, 40, 40}
	};
};

#endif