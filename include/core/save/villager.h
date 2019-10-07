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

#ifndef VILLAGER_H
#define VILLAGER_H

#include <3ds.h>
#include <string>
#include "encryptedInt32.h"
#include "save.h"

class Villager {
public:
	Villager(void);
	~Villager(void);
	Villager(const u32 offset, const u32 index);

	enum Personality : u8 {
		Lazy, Jock, Cranky, Smug, Normal, Peppy, Snooty, Uchi, NotSet
	};

	#pragma pack(push, 1)
	struct Villager_s {
		u16 Id; // 0 - 1 (2)
		Villager::Personality Personality; // 2 (1)
		u8 Unknown1[0x246B]; // 3 - 246D
		u32 Shirt; // 246E - 2471
		u32 Song; // 2472 - 2475
		u32 Wallpaper; // 2476 - 2479
		u32 Carpet; // 247A - 247D
		u32 Umbrella; // 247E - 2481
		u32 Furniture[16]; // 2482 - 24C1
		u32 Unknown2; // 24C2 - 24C5
		char16_t Catchphrase[11]; // Last character is null terminator | 24C6 - 24DB
		u8 Unknown3[8]; // 24DC - 24E3
		u8 Status; // Also general flags? | 24E4
		u8 Unknown4[9]; // 24E5 - 24ED
		char16_t TownName[9]; // Last character is null terminator | 24EE - 24FF
		u8 Unknown5[0x18]; // 2500 - 2517
	};
	#pragma pack(pop)

	u32 GetOffset(void) const;
	u32 GetIndex(void) const;

	u16 GetId(void) const;
	void SetId(const u16);

	void Write(void);

private:
	Villager_s* m_villagerData;

	u32 m_offset;
	u32 m_index;
};

#endif