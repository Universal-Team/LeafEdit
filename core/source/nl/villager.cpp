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

#include "villager.hpp"

Villager::Villager(const u32 offset, const u32 index) {
	this->m_offset = offset;
	this->m_index = index;

	// Read specific stuff.
	this->ID = Save::Instance()->ReadU16(offset + 0x0);
	this->personality = Save::Instance()->ReadU8(offset + 0x2);
	this->Catchphrase = Save::Instance()->ReadString(offset + 0x24C6, 11);
	this->status = Save::Instance()->ReadU8(offset + 24E4);
	this->TownName = Save::Instance()->ReadString(offset + 0x24EE, 9);

	// Read Items.
	this->Shirt = std::make_shared<Item>(offset + 0x246E);
	this->Song = std::make_shared<Item>(offset + 0x2472);
	this->Wallpaper = std::make_shared<Item>(offset + 0x2476);
	this->Carpet = std::make_shared<Item>(offset + 0x247A);
	this->Umbrella = std::make_shared<Item>(offset + 0x247E);
	// Read Furniture's.
	for (int i = 0; i < 16; i++) {
		this->Furniture[i] = std::make_shared<Item>(offset + 0x2482 + i * ITEM_SIZE);
	}
}

Villager::~Villager() { }

void Villager::Write() {
	// Write specific stuff.
	Save::Instance()->Write(this->m_offset + 0x0, this->ID);
	Save::Instance()->Write(this->m_offset + 0x2, this->personality);
	Save::Instance()->Write(this->m_offset + 0x24C6, this->Catchphrase, 11);
	Save::Instance()->Write(this->m_offset + 24E4, this->status);
	Save::Instance()->Write(this->m_offset + 0x24C6, this->TownName, 9);

	// Write Items.
	this->Shirt->Write();
	this->Song->Write();
	this->Wallpaper->Write();
	this->Carpet->Write();
	this->Umbrella->Write();
	// Write Furniture's.
	for (int i = 0; i < 16; i++) {
		this->Furniture[i]->Write();
	}
}