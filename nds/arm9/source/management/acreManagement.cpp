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

#include "acreManagement.hpp"
#include "gui.hpp"
#include "wwsave.hpp"
#include "wwTown.hpp"

#include <stdio.h>
#include <stdlib.h>

extern WWSave *SaveFile;

void AcreManagement::DrawAcre(u8 AcreID, int x, int y, float ScaleX, float ScaleY, bool top) {
	Image AcreData = loadImage("/graphics/acres/" + std::to_string(AcreID) + ".gfx"); // Load Image.
	drawImageScaled(x, y, ScaleX, ScaleY, AcreData, top, true); // Draw Image.
}

// 7 - 10 ; 13 - 16 ; 19 - 22; 25 - 28 -- 4x4 -> 16 Acres.
// This is for the Bottom Screen.
void AcreManagement::DrawTownMap() {
	// First Line.
	DrawAcre(SaveFile->town->FullAcres[7], 10, 30, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[8], 42, 30, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[9], 74, 30, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[10], 106, 30, 1, 1, false);
	// Second Line.
	DrawAcre(SaveFile->town->FullAcres[13], 10, 62, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[14], 42, 62, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[15], 74, 62, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[16], 106, 62, 1, 1, false);
	// Third Line.
	DrawAcre(SaveFile->town->FullAcres[19], 10, 94, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[20], 42, 94, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[21], 74, 94, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[22], 106, 94, 1, 1, false);
	// Fourth Line.
	DrawAcre(SaveFile->town->FullAcres[25], 10, 126, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[26], 42, 126, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[27], 74, 126, 1, 1, false);
	DrawAcre(SaveFile->town->FullAcres[28], 106, 126, 1, 1, false);
}