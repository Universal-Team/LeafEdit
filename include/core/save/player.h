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

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "core/save/encryptedInt32.h"
#include "core/save/item.h"
#include "core/save/save.h"

class Player {
public:
	Player(void);
	~Player(void);
	Player(u32 offset, u32 index);

	u16 PlayerId;
	u16 PlayerTan;
	std::u16string Name;
	u16 Gender; // might not be a u16, but the following byte is always? 0.
	u16 TownId;
	std::u16string TownName;
	Item *Pockets = nullptr;
	Item *Dresser = nullptr;
	u16 testItem;
	EncryptedInt32 Wallet;
	EncryptedInt32 BankAmount;
	EncryptedInt32 MeowCoupons;
	EncryptedInt32 IslandMedals;

	u8 hairStyle;
	u8 hairColor;
	u8 face;
	u8 eyeColor;

	void Write();
	bool Exists();

	u32 m_offset;
	u32 m_index;
};

#endif