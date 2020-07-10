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

/*
*   This file is part of LeafEdit-Core
*   Copyright (C) 2020 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "encryptedInt32.hpp"
#include "types.hpp"

#include <random>

EncryptedInt32::EncryptedInt32() { }

EncryptedInt32::EncryptedInt32(u32 unencryptedValue) {
	value = unencryptedValue;
}

EncryptedInt32::EncryptedInt32(u32 intLow, u32 intHigh) {
	decrypt(intLow, intHigh);
}

EncryptedInt32::EncryptedInt32(u64 encryptedInt)
	: EncryptedInt32(static_cast<u32>(encryptedInt & 0xFFFFFFFF), static_cast<u32>((encryptedInt >> 32) & 0xFFFFFFFF)) { }

u8 EncryptedInt32::calculateChecksum(u32 intLow) {
	return static_cast<u8>(((intLow & 0xFF) + ((intLow >> 8) & 0xFF) + ((intLow >> 16) & 0xFF) + ((intLow >> 24) & 0xFF) + 0xBA) & 0xFF);
}

u32 EncryptedInt32::decrypt(u64 encryptedInt) {
	return EncryptedInt32::decrypt(static_cast<u32>(encryptedInt & 0xFFFFFFFF), static_cast<u32>((encryptedInt >> 32) & 0xFFFFFFFF));
}

u32 EncryptedInt32::decrypt(u32 intLow, u32 intHigh) {
	u16 adjustValue = static_cast<u16>(intHigh & 0xFFFF);
	u8 shiftValue = static_cast<u8>((intHigh >> 16) & 0xFF);
	u8 checksum = static_cast<u8>((intHigh >> 24) & 0xFF);

	u8 calculatedChecksum = calculateChecksum(intLow);

	if (calculatedChecksum != checksum) {
		// TODO: Log
		printf("\x1b[29;4HInvalid encrypted int checksum! Expected: %02X | Got: %02X", calculatedChecksum, checksum);
		return 0;
	}

	u8 leftShiftCount = static_cast<u8>(0x1C - shiftValue);
	u8 rightShiftCount = static_cast<u8>(0x20 - leftShiftCount);

	value = (((intLow << leftShiftCount) + (intLow >> rightShiftCount)) - (adjustValue + 0x8F187432));
	return value;
}

void EncryptedInt32::encrypt(u32 &encryptedIntOut, u32 &encryptionDataOut) {
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 0x10000 - 1);
	std::uniform_int_distribution<std::mt19937::result_type> dist2(0, 0x1A - 1);

	u16 adjustValue = static_cast<u16>(dist(rng) & 0xFFFF);
	u8 shiftValue = static_cast<u8>(dist(rng) & 0x19);

	u32 encryptedInt = (value + adjustValue + 0x8F187432);
	encryptedInt = (encryptedInt >> (0x1C - shiftValue)) + (encryptedInt << (shiftValue + 4));
	u32 encryptionData = (calculateChecksum(encryptedInt) << 24) | (shiftValue << 16) | adjustValue;

	printf("\x1b[27;4HEncrypted Int value: %08X", static_cast<unsigned int>(encryptedInt));
	printf("\x1b[28;4HEncrypted Data value: %08X", static_cast<unsigned int>(encryptionData));

	encryptedIntOut = encryptedInt;
	encryptionDataOut = encryptionData;
}