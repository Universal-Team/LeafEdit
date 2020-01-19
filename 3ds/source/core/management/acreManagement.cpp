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

#include "core/management/acreManagement.hpp"

#include "save.hpp"

#include "gui/screens/screenCommon.hpp"

extern Save* SaveFile;
extern C2D_SpriteSheet Acres;

// Draw the Villager sprite.
void AcreManagement::DrawAcre(u8 AcreID, int x, int y, float ScaleX, float ScaleY) {
	DrawSprite(Acres, AcreID, x, y, ScaleX, ScaleY);
}

void AcreManagement::InitAcres(const u8 LoopMax, const u8 GridXMax,
	const u8 GridXStartPos, const u8 GridYStartPos, const u8 ByteSkip, u32 Offset) {
	/*
	LoopMax: How many times to loop
	GridXMax: How many acres per row
	GridXStartPos: Start X Position for drawing
	GridYStartPos: Start Y Position for drawing
	ByteSkip: Num of bytes to skip when moving to new row
	Offset: offset of acre IDs in save
	*/

	float scale = 1; // The scale of the acre editor image buttons

	u8  acre = 0;           // acre ID
	u32 GridX = 0;          // X position of acre
	u32 GridY = 0;          // Y position of acre
	// u32 GridXOffset = (320 - (40 * scale) * GridXMax) / 2; // Offset to center the controls horizontally

	for (u32 i = 0; i < LoopMax; i++)
	{
		GridX = i % GridXMax; // Get remainder for the row
		if (GridX == 0 && i != 0)
		{
			GridY += 40 * scale; // 40 == height of acre img
			Offset += ByteSkip;
		}

		acre = SaveFile->ReadU8(Offset + i * 2);
		DrawAcre(acre, GridXStartPos + (40 * scale * GridX), GridYStartPos + GridY, scale, scale);
	}
}