/*
*   This file is part of WildEdit
*   Copyright (C) 2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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

#pragma once

#ifndef WWOFFSETS_H
#define WWOFFSETS_H

typedef enum {
    SaveSize = 0x15FE0,
    TownId = 0x0002,
    TownName = 0x0004,
    TownNameSize = 8,
    PlayerStart = 0x000C,
    PlayerSize = 0x228C,
    VillagerData = 0x8A3C,
    VillagerSize = 0x700,
    AcreData = 0xC330,
    AcreDataSize = 0x24,
    TownData = 0xC354,
    TownDataSize = 0x2000,
    BuriedData = 0xE354,
    BuriedDataSize = 0x200,
    GrassType = 0xE554,
    HouseData = 0xE558,
    HouseDataSize = 0x15A0,
    Debt = 0xFAE8,
    Checksum = 0x15FDC
} AC_Readonly_Offsets;

typedef enum {
    PatternCount = 8,
    AcreCount = 36,
    XAcreCount = 6,
    TownAcreCount = 16,
    TownYAcreStart = 1,
    TownYAcreCount = 4,
    VillagerCount = 8
} AC_Readonly_SaveOffsets;