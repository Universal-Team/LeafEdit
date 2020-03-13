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

#include "wwAcreManagement.hpp"
#include "wwsave.hpp"
#include "screenCommon.hpp"

extern WWSave* WWSaveFile;
extern C2D_SpriteSheet WWAcres;

// Draw the Acre sprite.
void WWAcreManagement::DrawAcre(u8 AcreID, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(WWAcres, AcreID, x, y, ScaleX, ScaleY);
}

// 7 - 10 ; 13 - 16 ; 19 - 22; 25 - 28 -- 4x4 -> 16 Acres.
// This is for the Bottom Screen.
void WWAcreManagement::DrawTownMapBottom() {
	// First Line.
	DrawAcre(WWSaveFile->town->FullAcres[7], 30, 33, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[8], 90, 33, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[9], 150, 33, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[10], 210, 33, 2, 2);
	// Second Line.
	DrawAcre(WWSaveFile->town->FullAcres[13], 30, 73, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[14], 90, 73, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[15], 150, 73, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[16], 210, 73, 2, 2);
	// Third Line.
	DrawAcre(WWSaveFile->town->FullAcres[19], 30, 113, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[20], 90, 113, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[21], 150, 113, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[22], 210, 113, 2, 2);
	// Fourth Line.
	DrawAcre(WWSaveFile->town->FullAcres[25], 30, 143, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[26], 90, 143, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[27], 150, 143, 2, 2);
	DrawAcre(WWSaveFile->town->FullAcres[28], 210, 143, 2, 2);
}