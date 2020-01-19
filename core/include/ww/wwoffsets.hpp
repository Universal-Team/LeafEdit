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

#ifndef WWOFFSETS_HPP
#define WWOFFSETS_HPP

typedef enum {
    SaveSize = 0x2BFBF, // Actual size is: 0x15FE0.
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

typedef enum {
    Patterns = 0,
    PatternSize = 0x228, //Extra 8 bytes for the Town Name over AC
    Messages = 0x1140,
    MessageSize = 0xF4,
    MessageCount = 10, //0x1AD4 = Dear Future Letter
    Pockets = 0x1B22,
    PocketsCount = 15,
    Bells = 0x1B40, //0x1B44
    Catalog = 0x1B48, //Includes Encyclopedia...
    CatalogSize = 0x123,
    FriendCode = 0x1E2C,
    Savings = 0x21E4,
    Emotions = 0x21EC,
    EmotionCount = 4,
    Reset = 0x21FC,
    ResetSize = 2,
    NookPoints = 0x2208,
    HeldItem = 0x220A,
    Shirt = 0x220C,
    Hat = 0x220E,
    FaceItem = 0x2210,
    Bed = 0x2212,
    InventoryBackground = 0x2214,
    LastPlayDate = 0x2216,
    LastPlayDateSize = 4,
    Birthday = 0x2218,
    BirthdaySize = 2,
    HairType = 0x223C, //Upper Nibble is Hair Type, Lower Nibble is Face Type
    HairColor = 0x223D, //Upper Nibble is Hair Color, Lower Nibble is Tan
    Gender = 0x228A,
    Identifier = 0x2280,
    IdentifierSize = 2,
    Name = 0x2282,
    NameSize = 8,
    TownIdentifier = 0x2276,
    TownIdentifierSize = 2,
    Dressers = 0,
    DressersCount = 90,
    DressersSize = 0
} AC_PLAYER_OFFSETS;

#endif