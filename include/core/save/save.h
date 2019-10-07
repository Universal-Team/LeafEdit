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

#ifndef SAVE_H
#define SAVE_H

#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/villager.h"


#include <string>

struct Region_Lock {
	u8 DerivedID;
	CFG_Region RegionID;
	u8 RawByte;
};

class Player;
class Villager;

class Save {
public:
	static Save* Initialize(const char *saveName, bool init);
	static Save* Instance();

	s8 ReadS8(u32 offset);
	u8 ReadU8(u32 offset);
	s16 ReadS16(u32 offset);
	u16 ReadU16(u32 offset);
	s32 ReadS32(u32 offset);
	u32 ReadU32(u32 offset);
	s64 ReadS64(u32 offset);
	u64 ReadU64(u32 offset);
	void ReadArray(u8 *dst, u32 offset, u32 count);
	void ReadArrayU16(u16 *dst, u32 offset, u32 count);
	std::u16string ReadString(u32 offset, u32 maxSize);

	u8* GetRawSaveData(void);
	u64 GetSaveSize(void);

	bool Write(u32 offset, u8 *buffer, u32 count);
	bool Write(u32 offset, s8 data);
	bool Write(u32 offset, u8 data);
	bool Write(u32 offset, s16 data);
	bool Write(u32 offset, u16 data);
	bool Write(u32 offset, s32 data);
	bool Write(u32 offset, u32 data);
	bool Write(u32 offset, s64 data);
	bool Write(u32 offset, u64 data);
	bool Write(u32 offset, std::u16string data, u32 maxSize);

	bool ChangesMade(void);
	void SetChangesMade(bool changesMade);

	bool Commit(bool close);
	void Close(void);

	Player *players[4];
	Villager* villagers[10];
	Region_Lock RegionLock;

private:
	u8 *m_saveBuffer = nullptr;
	u64 m_saveSize;
	Handle m_handle = 0;
	bool m_changesMade;

	Save(void);
	~Save(void);
	Save(Save const&) {};
	Save& operator=(Save const&) { return *Save::Instance(); }; // Is this right?
	static Save* m_pSave;
};

#endif