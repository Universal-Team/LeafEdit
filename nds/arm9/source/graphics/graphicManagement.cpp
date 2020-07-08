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

#include "graphicManagement.hpp"
#include "gui.hpp"
#include "Sav.hpp"

#include <nds.h>

extern std::shared_ptr<Sav> save;

void GraphicManagement::DrawAcre(u8 AcreID, int x, int y, float ScaleX, float ScaleY, bool top, bool Layer) {
	Image AcreData = loadImage("/graphics/acres/" + std::to_string(AcreID) + ".gfx"); // Load Image.
	drawImageScaled(x, y, ScaleX, ScaleY, AcreData, top, Layer, 0x20); // Draw Image.
}

void GraphicManagement::DrawVillager(u8 villagerID, int x, int y, bool top) {
	// 144 is the current exception, since we don't have the sprite for "Champ".
	if (villagerID > 149) {
		villagerID = 150; // 150 -> EMPTY.
	}

	Image villagerData = loadImage("/graphics/villagers/" + std::to_string(villagerID) + ".gfx"); // Load sprite.
	drawImage(x, y, villagerData, top, true, 0x20); // Draw sprite.
}

// 7 - 10 ; 13 - 16 ; 19 - 22; 25 - 28 -- 4x4 -> 16 Acres.
// This is for the Bottom Screen.
void GraphicManagement::DrawTownMap() {
	// First Line.
	DrawAcre(save->town()->acre(7)->id(), 10, 30, 1, 1, false);
	DrawAcre(save->town()->acre(8)->id(), 42, 30, 1, 1, false);
	DrawAcre(save->town()->acre(9)->id(), 74, 30, 1, 1, false);
	DrawAcre(save->town()->acre(10)->id(), 106, 30, 1, 1, false);
	// Second Line.
	DrawAcre(save->town()->acre(13)->id(), 10, 62, 1, 1, false);
	DrawAcre(save->town()->acre(14)->id(), 42, 62, 1, 1, false);
	DrawAcre(save->town()->acre(15)->id(), 74, 62, 1, 1, false);
	DrawAcre(save->town()->acre(16)->id(), 106, 62, 1, 1, false);
	// Third Line.
	DrawAcre(save->town()->acre(19)->id(), 10, 94, 1, 1, false);
	DrawAcre(save->town()->acre(20)->id(), 42, 94, 1, 1, false);
	DrawAcre(save->town()->acre(21)->id(), 74, 94, 1, 1, false);
	DrawAcre(save->town()->acre(22)->id(), 106, 94, 1, 1, false);
	// Fourth Line.
	DrawAcre(save->town()->acre(25)->id(), 10, 126, 1, 1, false);
	DrawAcre(save->town()->acre(26)->id(), 42, 126, 1, 1, false);
	DrawAcre(save->town()->acre(27)->id(), 74, 126, 1, 1, false);
	DrawAcre(save->town()->acre(28)->id(), 106, 126, 1, 1, false);
}

void GraphicManagement::DrawTownMapEditor() {
	// First Line.
	DrawAcre(save->town()->acre(7)->id(), 10, 30, 1, 1, true);
	DrawAcre(save->town()->acre(8)->id(), 42, 30, 1, 1, true);
	DrawAcre(save->town()->acre(9)->id(), 74, 30, 1, 1, true);
	DrawAcre(save->town()->acre(10)->id(), 106, 30, 1, 1, true);
	// Second Line.
	DrawAcre(save->town()->acre(13)->id(), 10, 62, 1, 1, true);
	DrawAcre(save->town()->acre(14)->id(), 42, 62, 1, 1, true);
	DrawAcre(save->town()->acre(15)->id(), 74, 62, 1, 1, true);
	DrawAcre(save->town()->acre(16)->id(), 106, 62, 1, 1, true);
	// Third Line.
	DrawAcre(save->town()->acre(19)->id(), 10, 94, 1, 1, true);
	DrawAcre(save->town()->acre(20)->id(), 42, 94, 1, 1, true);
	DrawAcre(save->town()->acre(21)->id(), 74, 94, 1, 1, true);
	DrawAcre(save->town()->acre(22)->id(), 106, 94, 1, 1, true);
	// Fourth Line.
	DrawAcre(save->town()->acre(25)->id(), 10, 126, 1, 1, true);
	DrawAcre(save->town()->acre(26)->id(), 42, 126, 1, 1, true);
	DrawAcre(save->town()->acre(27)->id(), 74, 126, 1, 1, true);
	DrawAcre(save->town()->acre(28)->id(), 106, 126, 1, 1, true);
}