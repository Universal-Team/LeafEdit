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

#include "core/acreManagement.hpp"

#include "core/save/offsets.h"
#include "core/save/save.h"

#include "gui/gui.hpp"

#include "gui/screens/acresEditor.hpp"

#include <3ds.h>

extern Save* SaveFile;

#define MAX_ACRE 218 // Define the Max Amount of Acres.

void AcresEditor::Draw(void) const {
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, "LeafEdit - Acre Editor"))/2, 2, 0.8f, Config::barText, "LeafEdit - Acre Editor", 400);
	Gui::DrawBottom();
	townMap();
}

void AcresEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}

// To-Do for the Acres Editor.
u8 AcresEditor::selectAcre(u8 selectedAcre) {
	return 0;
}

// Uhh.. Maps and such.

void AcresEditor::fullTown() const
{
    AcreManagement::InitAcres(42, 7, 20, 0, 0, MAP_ACRES); // Ends at x = 230px (resized to 30x30 images)
}

void AcresEditor::fullIsland() const
{
    AcreManagement::InitAcres(16, 4, 80, 40, 0, ISLAND_ACRES);
}

void AcresEditor::townMap() const
{
    AcreManagement::InitAcres(20, 5, 60, 40, 4, MAP_ACRES + 0x10);
}

void AcresEditor::islandMap() const
{
    AcreManagement::InitAcres(4, 2, 120, 80, 4, ISLAND_ACRES + 0xA);
}