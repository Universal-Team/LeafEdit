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

#include "PatternWA.hpp"
#include "saveUtils.hpp"
#include "stringUtils.hpp"

#include <cstring>
#include <unistd.h>

// Pattern Name.
std::u16string PatternWA::name() {
	return StringUtils::ReadUTF16String(patternPointer(), 0, 20);
}
void PatternWA::name(std::u16string v) {
	StringUtils::WriteUTF16String(patternPointer(), v, 0, 20);
}

// Creator ID.
u16 PatternWA::creatorid() {
	return SaveUtils::Read<u16>(patternPointer(), 0x2A);
}
void PatternWA::creatorid(u16 v) {
	return SaveUtils::Write<u16>(patternPointer(), 0x2A, v);
}

// Creator Name.
std::u16string PatternWA::creatorname() {
	return StringUtils::ReadUTF16String(patternPointer(), 0x2C, 8);
}
void PatternWA::creatorname(std::u16string v) {
	StringUtils::WriteUTF16String(patternPointer(), v, 0x2C, 8);
}

// Creator Gender.
u8 PatternWA::creatorGender() {
	return patternPointer()[0x3E];
}
void PatternWA::creatorGender(u8 v) {
	SaveUtils::Write<u8>(this->patternPointer(), 0x3E, v);
}

// Town ID.
u16 PatternWA::origtownid() {
	return SaveUtils::Read<u16>(patternPointer(), 0x40);
}
void PatternWA::origtownid(u16 v) {
	return SaveUtils::Write<u16>(patternPointer(), 0x40, v);
}

// Town Name.
std::u16string PatternWA::origtownname() {
	return StringUtils::ReadUTF16String(patternPointer(), 0x42, 8);
}
void PatternWA::origtownname(std::u16string v) {
	StringUtils::WriteUTF16String(patternPointer(), v, 0x42, 8);
}

// Own a Pattern.
void PatternWA::ownPattern(std::unique_ptr<Player> player) {
	// Only set if player is not nullptr!
	if (player != nullptr) {
		this->creatorid(player->playerid());
		this->creatorname(player->name());
		this->origtownid(player->townid());
		this->origtownname(player->townname());
		this->creatorGender(player->gender());
	}
}

// Design Type.
u8 PatternWA::designtype() {
	return (patternPointer()[0x69] & 9);
}
void PatternWA::designtype(u8 v) {
	SaveUtils::Write<u8>(this->patternPointer(), 0x69, (patternPointer()[0x69] & 0xF0) | (v & 0x9));
}

// Dump a Pattern to file.
void PatternWA::dumpPattern(const std::string fileName) {
	// Get Pattern size?
	u32 size = 0;
	if (this->patternPointer()[0x69] == 0x09){
		size = 620;
	} else {
		size = 2160;
	}

	// Open File.
	FILE* ptrn = fopen(fileName.c_str(), "wb");
	// Set Buffer.
	u8 *patternData = new u8[size];
	
	// Write Pattern data to Buffer.
	for(int i = 0; i < (int)size; i++) {
		SaveUtils::Write<u8>(patternData, i, this->patternPointer()[i], false);
	}

	// Write to file and close.
	fwrite(patternData, 1, size, ptrn);
	fclose(ptrn);
	// Free Buffer.
	delete(patternData);
}

// Inject a Pattern from a file.
void PatternWA::injectPattern(const std::string fileName) {
	if ((access(fileName.c_str(), F_OK) != 0))	return; // File not found. Do NOTHING.
	u32 size;
	// Open file and get size.
	FILE* ptrn = fopen(fileName.c_str(), "rb");
	fseek(ptrn, 0, SEEK_END);
	size = ftell(ptrn);
	fseek(ptrn, 0, SEEK_SET);
	// Create Buffer with the size and read the file.
	u8 *patternData = new u8[size];
	fread(patternData, 1, size, ptrn);

	// Set Buffer data to save.
	for(int i = 0; i < (int)size; i++) {
		SaveUtils::Write<u8>(this->patternPointer(), i, patternData[i]);
	}

	// Close File, cause we don't need it.
	fclose(ptrn);
	// Free Buffer.
	delete(patternData);
}

std::shared_ptr<PatternImage> PatternWA::image(const int pattern) {
	return std::make_shared<PatternImageNL>(this->data, (this->Offset + 0x6C + (pattern * 0x200)), this->Offset + 0x58);
}