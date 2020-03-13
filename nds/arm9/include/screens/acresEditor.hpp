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

#include "screenCommon.hpp"

#include "structs.hpp"
#include <vector>

class AcresEditor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u16 hDown, touchPosition touch) override;
private:
	void DrawTop(void) const; // Draw Acre Image Selection.
	int Selection = 0;
	int AcrePosition = 7;
	u8 selectedAcre = 0;
	int AcreMode = 0;
	int SelectionToPos(); // Convert Selection to position.
	void updateTop();
	void updateMap(); // Update Town Map.

	std::vector<Structs::ButtonPos> MapPos = {
		// First Line.
		{10, 30, 32, 32},
		{42, 30, 32, 32},
		{74, 30, 32, 32},
		{106, 30, 32, 32},
		// Second Line.
		{10, 62, 32, 32},
		{42, 62, 32, 32},
		{74, 62, 32, 32},
		{106, 62, 32, 32},
		// Third Line.
		{10, 94, 32, 32},
		{42, 94, 32, 32},
		{74, 94, 32, 32},
		{106, 94, 32, 32},
		// Fourth Line.
		{10, 126, 32, 32},
		{42, 126, 32, 32},
		{74, 126, 32, 32},
		{106, 126, 32, 32}
	};
};

#endif