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

#include "wwPattern.hpp"
#include "wwPlayer.hpp"
#include "types.hpp"

#include <string>

// Is that right?
static const u32 PaletteColors[] = {
	0xFFFF0000, 0xFFFF7331, 0xFFFFAD00, 0xFFFFFF00, 0xFFADFF00, 0xFF52FF00, 0xFF00FF00, 0xFF00AD52, 0xFF0052AD, 0xFF0000FF, 0xFF5200FF, 0xFFAD00FF, 0xFFFF00FF, 0xFF000000, 0xFFFFFFFF,
	0xFFFF7B7B, 0xFFFFB57B, 0xFFFFE77B, 0xFFFFFF7B, 0xFFDEFF7B, 0xFFADFF7B, 0xFF7BFF7B, 0xFF52AD84, 0xFF5284AD, 0xFF7B7BFF, 0xFFB57BFF, 0xFFE77BFF, 0xFFFF7BFF, 0xFF000000, 0xFFFFFFFF,
	0xFFA50000, 0xFFA53100, 0xFFA57300, 0xFFA5A500, 0xFF73A500, 0xFF31A500, 0xFF00A500, 0xFF005221, 0xFF002152, 0xFF0000A5, 0xFF3100A5, 0xFF7300A5, 0xFFA500A5, 0xFF000000, 0xFFFFFFFF,
	0xFF009C00, 0xFF5ACE6B, 0xFFB5FFDE, 0xFF009C6B, 0xFF52CEA5, 0xFFADFFD6, 0xFF0052AD, 0xFF2984D6, 0xFF5AADFF, 0xFF0000FF, 0xFF4A6BFF, 0xFF314ADE, 0xFF1821B5, 0xFF00008C, 0xFFFFFFFF,
	0xFFAD7300, 0xFFD6AD42, 0xFFFFDE8C, 0xFFFF0839, 0xFFFF4A6B, 0xFFFF949C, 0xFFAD00FF, 0xFFD663FF, 0xFFFFCEFF, 0xFFFFBD9C, 0xFFDE9473, 0xFFBD634A, 0xFF9C3921, 0xFF7B1000, 0xFFFFFFFF,
	0xFFFF0000, 0xFFFF5200, 0xFFFFB55A, 0xFFFFEFAD, 0xFF7B1000, 0xFFA54A31, 0xFFD6846B, 0xFFFFBD9C, 0xFF5AADFF, 0xFF84C6FF, 0xFFADE7FF, 0xFFD6FFFF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF00FF00, 0xFF42FF42, 0xFF8CFF8C, 0xFFD6FFD6, 0xFF0000FF, 0xFF4242FF, 0xFF8C8CFF, 0xFFD6D6FF, 0xFFFF0000, 0xFFFF4242, 0xFFFF8C8C, 0xFFFFD6D6, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF003100, 0xFF426342, 0xFF849C84, 0xFFC6D6C6, 0xFF7B1000, 0xFFA54A29, 0xFFD68C5A, 0xFFFFC68C, 0xFFD6B500, 0xFFE7CE39, 0xFFF7DE7B, 0xFFFFF7BD, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF0000FF, 0xFFFF0000, 0xFFFFFF00, 0xFF4242FF, 0xFFFF4242, 0xFFFFFF42, 0xFF8C8CFF, 0xFFFF8C8C, 0xFFFFFF8C, 0xFFD6D6FF, 0xFFFFD6D6, 0xFFFFFFD6, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF00FF00, 0xFF0000FF, 0xFFFF00FF, 0xFF42FF42, 0xFF4242FF, 0xFFFF42FF, 0xFF8CFF8C, 0xFF8C8CFF, 0xFFFF8CFF, 0xFFD6FFD6, 0xFFD6D6FF, 0xFFFFD6FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFFFF0000, 0xFFFF7B00, 0xFFFFFF00, 0xFF84FF00, 0xFF00FF00, 0xFF00847B, 0xFF0000FF, 0xFF7B00FF, 0xFFFF94FF, 0xFFD6B500, 0xFFBD1000, 0xFF5A1000, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF109463, 0xFF087B52, 0xFF108C39, 0xFF319C31, 0xFFCEA54A, 0xFFCE9439, 0xFFBD8C4A, 0xFFD68C31, 0xFFAD734A, 0xFF8C5A31, 0xFF6B4229, 0xFF84EFFF, 0xFF31CEEF, 0xFF00A5C6, 0xFFFFFFFF,
	0xFFD6DEE7, 0xFFB5CEDE, 0xFFE7EFEF, 0xFFF7F7F7, 0xFF84737B, 0xFF948C6B, 0xFF847B63, 0xFF9C845A, 0xFF739CB5, 0xFFFF2929, 0xFFFFFF00, 0xFF9421FF, 0xFF009CBD, 0xFF000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFF7EFEF, 0xFFE7DEDE, 0xFFD6CECE, 0xFFC6B5B5, 0xFFB5A5A5, 0xFFA59494, 0xFF9C8484, 0xFF8C6B6B, 0xFF7B5A5A, 0xFF6B4A4A, 0xFF5A3131, 0xFF4A2121, 0xFF421010, 0xFF310000,
	0xFFFFFFFF, 0xFFEFEFEF, 0xFFDEDEDE, 0xFFCECECE, 0xFFB5B5B5, 0xFFA5A5A5, 0xFF949494, 0xFF848484, 0xFF6B6B6B, 0xFF5A5A5A, 0xFF4A4A4A, 0xFF313131, 0xFF212121, 0xFF101010, 0xFF000000,
	0xFFFF8C7B, 0xFFFF0000, 0xFFFF7B00, 0xFFFFFF00, 0xFF008400, 0xFF00FF00, 0xFF0000FF, 0xFF009CFF, 0xFFD600FF, 0xFFFF6BFF, 0xFF9C0000, 0xFFFF9400, 0xFFFFBD94, 0xFF000000, 0xFFFFFFFF
};

// Needs checking.
void WWPattern::ReadPlayer() {
	this->TownName = WWSave::Instance()->ReadString(Offset + 0x202, 7, false);
	this->CreatorName = WWSave::Instance()->ReadString(Offset + 0x20C, 7, false);
	this->Name = WWSave::Instance()->ReadString(Offset + 0x216, 15, false);
	this->Palette = (u8)((WWSave::Instance()->ReadU8(Offset + 0x226) & 0xF0) >> 4);
	this->Concept = (u8)(WWSave::Instance()->ReadU8(Offset + 0x226) & 0x0F);
	this->PaletteData = PaletteColors[this->Palette];
}

// Needs checking.
void WWPattern::ReadShop() {
	this->TownName = WWSave::Instance()->ReadString(Offset + 0x202, 7, false);
	this->CreatorName = WWSave::Instance()->ReadString(Offset + 0x20C, 7, false);
	this->Name = WWSave::Instance()->ReadString(Offset + 0x216, 15, false);
	this->Palette = (u8)((WWSave::Instance()->ReadU8(Offset + 0x226) & 0xF0) >> 4);
	this->Concept = (u8)(WWSave::Instance()->ReadU8(Offset + 0x226) & 0x0F);
	this->PaletteData = PaletteColors[this->Palette];
}

WWPattern::WWPattern()  : Index(0), Offset(0) { }

// Needs checking.
WWPattern::WWPattern(u32 offset, u32 id) : Index(id), Offset(offset)
{
	ReadShop();
}

// Needs checking.
WWPattern::WWPattern(WWPlayer *player, u32 id) : Index(id), Offset(player->m_offset + 0 + id * 0x228)
{
	ReadPlayer();
}

WWPattern::~WWPattern() { }

void WWPattern::Write() { }