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

#include "player.hpp"
#include "pattern.hpp"
#include "types.hpp"

#ifdef _3DS
#include "common/utils.hpp"
#include <citro2d.h>
#endif

#include <string>

static const u32 PaletteColors[] = {
    0xFFEEFFFF, 0xFF99AAFF, 0xEE5599FF, 0xFF66AAFF, 0xFF0066FF, 0xBB4477FF, 0xCC0055FF, 0x990033FF, 0x552233FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFBBCCFF, 0xFF7777FF, 0xDD3311FF, 0xFF5544FF, 0xFF0000FF, 0xCC6666FF, 0xBB4444FF, 0xBB0000FF, 0x882222FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xEEEEEEFF,
    0xDDCCBBFF, 0xFFCC66FF, 0xDD6622FF, 0xFFAA22FF, 0xFF6600FF, 0xBB8855FF, 0xDD4400FF, 0xBB4400FF, 0x663311FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xDDDDDDFF,
    0xFFEEDDFF, 0xFFDDCCFF, 0xFFCCAAFF, 0xFFBB88FF, 0xFFAA88FF, 0xDD8866FF, 0xBB6644FF, 0x995533FF, 0x884422FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xCCCCCCFF,
    0xFFCCFFFF, 0xEE88FFFF, 0xCC66DDFF, 0xBB88CCFF, 0xCC00FFFF, 0x996699FF, 0x8800AAFF, 0x550077FF, 0x330044FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xBBBBBBFF,
    0xFFBBFFFF, 0xFF99FFFF, 0xDD22BBFF, 0xFF55EEFF, 0xFF00CCFF, 0x885577FF, 0xBB0099FF, 0x880066FF, 0x550044FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xAAAAAAFF,
    0xDDBB99FF, 0xCCAA77FF, 0x774433FF, 0xAA7744FF, 0x993300FF, 0x773322FF, 0x552200FF, 0x331100FF, 0x221100FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x999999FF,
    0xFFFFCCFF, 0xFFFF77FF, 0xDDDD22FF, 0xFFFF00FF, 0xFFDD00FF, 0xCCAA00FF, 0x999900FF, 0x887700FF, 0x555500FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x888888FF,
    0xDDBBFFFF, 0xBB99EEFF, 0x6633CCFF, 0x9955FFFF, 0x6600FFFF, 0x554488FF, 0x440099FF, 0x220066FF, 0x221133FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x777777FF,
    0xBBBBFFFF, 0x8899FFFF, 0x3333AAFF, 0x3355EEFF, 0x0000FFFF, 0x333388FF, 0x0000AAFF, 0x111166FF, 0x000022FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x666666FF,
    0x99EEBBFF, 0x66CC77FF, 0x226611FF, 0x44AA33FF, 0x008833FF, 0x557755FF, 0x225500FF, 0x113322FF, 0x002211FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x555555FF,
    0xDDFFBBFF, 0xCCFF88FF, 0x88AA55FF, 0xAADD88FF, 0x88FF00FF, 0xAABB99FF, 0x66BB00FF, 0x559900FF, 0x336600FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x444444FF,
    0xBBDDFFFF, 0x77CCFFFF, 0x335599FF, 0x6699FFFF, 0x1177FFFF, 0x4477AAFF, 0x224477FF, 0x002277FF, 0x001144FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x333333FF,
    0xAAFFFFFF, 0x55FFFFFF, 0x0088BBFF, 0x55BBCCFF, 0x00CCFFFF, 0x4499AAFF, 0x006688FF, 0x004455FF, 0x002233FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x222222FF,
    0xCCFFEEFF, 0xAAEEDDFF, 0x33CCAAFF, 0x55EEBBFF, 0x00FFCCFF, 0x77AAAAFF, 0x00AA99FF, 0x008877FF, 0x004433FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x000000FF,
    0xAAFFAAFF, 0x77FF77FF, 0x66DD44FF, 0x00FF00FF, 0x22DD22FF, 0x55BB55FF, 0x00BB00FF, 0x008800FF, 0x224422FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

Pattern::Pattern(Player *player, u32 id) :
    Index(id), Offset(player->m_offset + 0x2C + id * 0x870)
{
    Name = Save::Instance()->ReadString(Offset, 20);
    CreatorId = Save::Instance()->ReadU16(Offset + 0x2A);
    CreatorName = Save::Instance()->ReadString(Offset + 0x2C, 8);
    CreatorGender = Save::Instance()->ReadU16(Offset + 0x3E);
    OriginatingTownId = Save::Instance()->ReadU16(Offset + 0x40);
    OriginatingTownName = Save::Instance()->ReadString(Offset + 0x42, 8);
    Save::Instance()->ReadArray(Palette.data(), Offset + 0x58, 16);
    Type = (DesignType)(Save::Instance()->ReadU8(Offset + 0x69) & 9); // TODO: Check the real max disgn type value and limit it to that.
    Save::Instance()->ReadArray(PatternData.data(), Offset + 0x6C, 0x800);
    #ifdef _3DS
    Decompress();
    #endif
}

Pattern::~Pattern() {
    #ifdef _3DS
    for (u32 *data : ImageData)
        linearFree(data);

    for (C2D_Image& image : Images)
        C2DUtils::C2D_ImageDelete(image);
    #endif
}

void Pattern::TakeOwnership(Player *player) {
    CreatorId = player->PlayerId;
    CreatorName = player->Name;
    CreatorGender = player->Gender;

    OriginatingTownId = player->TownId;
    OriginatingTownName = player->TownName;
}

void Pattern::Write() {
    #ifdef _3DS
    Compress();
    #endif
    Save::Instance()->Write(Offset, Name, 20);
    Save::Instance()->Write(Offset + 0x2A, CreatorId);
    Save::Instance()->Write(Offset + 0x2C, CreatorName, 8);
    Save::Instance()->Write(Offset + 0x3E, CreatorGender);
    Save::Instance()->Write(Offset + 0x40, OriginatingTownId);
    Save::Instance()->Write(Offset + 0x42, OriginatingTownName, 8);
    Save::Instance()->Write(Offset + 0x58, Palette.data(), 16);
    Save::Instance()->Write(Offset + 0x69, (u8)Type);
    Save::Instance()->Write(Offset + 0x6C, PatternData.data(), 0x800);
}

#ifdef _3DS

void Pattern::Decompress(void) {

    for (u32 *data : ImageData)
        linearFree(data);
    ImageData.clear();

    for (u32 i = 0; i < 4; ++i)
    {
        u32     idx = i * 0x200;
        u32     outputIdx = 0;
        u32   * output = (u32 *)linearAlloc(sizeof(u32) * 32 * 32);

        for (u32 x = 0; x < 0x200; ++x, ++idx)
        {
            u8  data = PatternData[idx];

            output[outputIdx++] = PaletteColors[Palette[(data & 0x0F)]]; // Left Pixel
            output[outputIdx++] = PaletteColors[Palette[((data >> 4) & 0x0F)]]; // Right Pixel
        }

        Images[i] = C2DUtils::ImageDataToC2DImage(output, 32, 32, GPU_RGBA8);
        ImageData.push_back(output);
    }
}

void Pattern::Compress(void) {

    for (int i = 0; i < 4; i++) {
        u32 readOffset = 0;
        u32 idx = i * 200;

        for (int x = 0; x < 0x200; x++) {
            u8 CompressedPixel = 0;

            for (int y = 0; y < 2; y++) {
                u32 Pixel = ImageData[i][readOffset++];

                for (int p = 0; p < 16; p++) {
                    if (PaletteColors[Palette[p]] == Pixel) {
                        CompressedPixel |= (p << (y * 4));
                    }
                }
            }

            PatternData[idx + x] = CompressedPixel;
        }
    }
}
#endif