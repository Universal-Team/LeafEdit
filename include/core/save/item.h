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

#ifndef ITEM_H
#define ITEM_H

#include "common/file.hpp"

struct ItemBin_s {
	u16 ItemIcon;
	u16 ItemPrice;
	u16 Unknown0;
	u16 RefurbishID;
	u16 SoundID;
	u8  Category;
	s8  Unknown2;
	s8  Unknown3;
	s8  Unknown4;
	s8  PriceMultiplier;
	u8  Unknown5;
	s8  ClothingGroup;
	u8  CanRefurbish___;
	u8  DinosaurGroup;
	s8  ArtGroup;
	u8  Unknown11;
	u8  Unknown12;
	u8  Unknown13;
	u8  Unknown14;
	u8  Unknown15;
	u8  Unknown16;
	u8  Unknown17;
	u8  Unknown18;
	u8  Unknown19;
	u8  Unknown20;
};

struct ItemKind_s {
	u8 Unknown1;
	u8 Unknown2;
	u8 Unknown3;
};

class Item {
public:
	union {
    	struct {
        	u16 ID;
        	u16 Flags;
    	};
    	u32 Raw;
	};

	static void LoadItemBins(void);
	static void UnloadItemBins(void);
	static File* GetItemBin(void);
	static File* GetItemKindBin(void);
	static bool IsInvWhitelisted(u16 ItemID);

	Item(void);
	Item(const u32 offset);
	Item(const u16 id, const u16 flags);

	s32 IsNormalItem(void);
	ItemBin_s* GetItemBinSlot(void);
	ItemKind_s* GetItemKindSlot(void);
	u16 GetAxeDamageValue(void);
	u16 GetAxeDamageIcon(u16 ItemIcon);

	std::string GetName(void);
	bool IsInvWhitelisted(void);
	u8 GetCategory(void);
	u16 GetIconID(void);
	s32 GetSpritesheetID(void);

};

#endif