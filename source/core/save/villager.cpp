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

#include "villager.h"

Villager::Villager() { }

Villager::~Villager() {
	// Checking if a pointer is null is not necessary for delete. It will verify the memory is valid before attempting to delete it.
	if (this->m_villagerData != nullptr) {
		delete this->m_villagerData;
	}
}

Villager::Villager(const u32 offset, const u32 index) : m_villagerData(new Villager_s), m_offset(offset), m_index(index) {
	Save::Instance()->ReadArray(reinterpret_cast<u8 *>(this->m_villagerData), offset, sizeof(Villager_s));
}

u32 Villager::GetOffset() const {
	return this->m_offset;
}

u32 Villager::GetIndex() const {
	return this->m_index;
}

u16 Villager::GetId() const {
	return this->m_villagerData->Id;
}

void Villager::SetId(const u16 id) {
	this->m_villagerData->Id = id;
}

void Villager::Write() {
	Save::Instance()->Write(m_offset, reinterpret_cast<u8 *>(this->m_villagerData), sizeof(Villager_s));
}