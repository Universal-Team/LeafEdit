/*
MIT License
This file is part of NLTK
Copyright (c) 2018-2019 Slattz, Cuyler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#ifndef PATTERN_H
#define PATTERN_H

#include "save.h"
#include "types.h"

#include <array>
#ifdef _3DS
#include <citro2d.h>
#endif
#include <string>
#include <vector>

class Player;

// TODO: Research the 1-7 design types
enum class DesignType : u8 {
    DressLongSleeve = 0,
    DressShortSleeve = 1,
    DressSleeveless = 2,
    ShirtLongSleeve = 3,
    ShirtShortSleeve = 4,
    ShirtSleeveless = 5,
    HornedHat = 6,
    KnitHat = 7,
    PhotoBoard = 8,
    Pattern = 9
};

class Pattern {
public:
    Pattern(Player *player, u32 id);
    ~Pattern();

    void TakeOwnership(Player *player);
    void Write();
    
    std::u16string              Name;
    u16							CreatorId;
    std::u16string              CreatorName;
    u16                         CreatorGender; // technically only a u8, but this allows us to ensure the following byte is always 0
    u16                         OriginatingTownId;
    std::u16string              OriginatingTownName;
    std::array<u8, 16>          Palette; // only the first 15 colors are valid
    DesignType                  Type;
    std::array<u8, 0x800>       PatternData;

    const u32                   Index;
    const u32                   Offset;
    #ifdef _3DS
    std::vector<u32 *>          ImageData;
    C2D_Image                   Images[4];
    #endif

private:
    #ifdef _3DS
    void Decompress(void);
    void Compress(void);
    #endif
};

#endif