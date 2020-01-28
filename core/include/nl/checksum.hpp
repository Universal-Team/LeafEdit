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

#ifndef CHECKSUM_HPP
#define CHECKSUM_HPP

#include "types.hpp"

typedef enum
{
	CRC_REFLECTED = 0, //Most common in ACNL checksums
	CRC_NORMAL = 1
} ChecksumType;

u32 CalculateCRC32Reflected(u8 *buf, u32 size);
u32 CalculateCRC32Normal(u8 *buf, u32 size);
bool VerifyCRC32(u32 crc, u8 *buf, u32 startOffset, u32 size, ChecksumType type = CRC_REFLECTED);
u32 UpdateCRC32(u32 startOffset, u32 size, ChecksumType type = CRC_REFLECTED);
void FixCRC32s();

#endif