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

#include "Acre.hpp"
#include "screenCommon.hpp"
#include "structs.hpp"

#include <vector>

class AcresEditor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u16 hDown, touchPosition touch) override;
	AcresEditor();
private:
	std::unique_ptr<Acre> acres[36];

	// Draw stuff.
	void DrawTop(void) const; // Draw Acre Image Selection.
	void DrawAcres() const; // Draw the Acres from the bottom screen.

	// Selection stuff.
	int Selection = 0; // Selected Acre.
	u8 selectedAcre = 0; // Acre ID.

	// Update stuff.
	void updateTop(); // Update the Top Screen.
	void updateAcres(); // Update Acres.

	std::vector<Structs::ButtonPos> MapPos = {
		// First Line.
		{32, 0, 32, 32},
		{64, 0, 32, 32},
		{96, 0, 32, 32},
		{128, 0, 32, 32},
		{160, 0, 32, 32},
		{192, 0, 32, 32},
		// Second Line.
		{32, 32, 32, 32},
		{64, 32, 32, 32},
		{96, 32, 32, 32},
		{128, 32, 32, 32},
		{160, 32, 32, 32},
		{192, 32, 32, 32},
		// Third Line.
		{32, 64, 32, 32},
		{64, 64, 32, 32},
		{96, 64, 32, 32},
		{128, 64, 32, 32},
		{160, 64, 32, 32},
		{192, 64, 32, 32},
		// Fourth Line.
		{32, 96, 32, 32},
		{64, 96, 32, 32},
		{96, 96, 32, 32},
		{128, 96, 32, 32},
		{160, 96, 32, 32},
		{192, 96, 32, 32},
		// Fifth Line.
		{32, 128, 32, 32},
		{64, 128, 32, 32},
		{96, 128, 32, 32},
		{128, 128, 32, 32},
		{160, 128, 32, 32},
		{192, 128, 32, 32},
		// Sixth Line.
		{32, 160, 32, 32},
		{64, 160, 32, 32},
		{96, 160, 32, 32},
		{128, 160, 32, 32},
		{160, 160, 32, 32},
		{192, 160, 32, 32}
	};
};

#endif