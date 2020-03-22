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

#ifndef ENCRYPTEDINT32_HPP
#define ENCRYPTEDINT32_HPP

#include "types.hpp"

class EncryptedInt32 {
public:
	EncryptedInt32();
	EncryptedInt32(u32 value);
	EncryptedInt32(u32 intLow, u32 intHigh);
	EncryptedInt32(u64 encryptedInt);

	u32 value;
	void encrypt(u32 &encryptedIntOut, u32 &encryptionDataOut);
	u32 decrypt(u32 intLow, u32 intHigh);
	u32 decrypt(u64 encryptedInt);

private:
	u8 calculateChecksum(u32 intHigh);
};

#endif