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

static const u32 PaletteColors[] = {
	0xFFEEFFFF, 0xFF99AAFF, 0xEE5599FF, 0xFF66AAFF, 0xFF0066FF, 0xBB4477FF, 0xCC0055FF, 0x990033FF, 0x552233FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFBBCCFF, 0xFF7777FF, 0xDD3311FF, 0xFF5544FF, 0xFF0000FF, 0xCC6666FF, 0xBB4444FF, 0xBB0000FF, 0x882222FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xEEEEEEFF,
	0xDDCCBBFF, 0xFFCC66FF, 0xDD6622FF, 0xFFAA22FF, 0xFF6600FF, 0xBB8855FF, 0xDD4400FF, 0xBB4400FF, 0x663311FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xDDDDDDFF,
	0xFFEEDDFF, 0xFFDDCCFF, 0xFFCCAAFF, 0xFFBB88FF, 0xFFAA88FF, 0xDD8866FF, 0xBB6644FF, 0x995533FF, 0x884422FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xCCCCCCFF,
	0xFFCCFFFF, 0xEE88FFFF, 0xCC66DDFF, 0xBB88CCFF, 0xCC00FFFF, 0x996699FF, 0x8800AAFF, 0x550077FF, 0x330044FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xBBBBBBFF,
	0xFFBBFFFF, 0xFF99FFFF, 0xDD22BBFF, 0xFF55EEFF, 0xFF00CCFF, 0x885577FF, 0xBB0099FF, 0x880066FF, 0x550044FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xAAAAAAFF,
	0xDDBB99FF, 0xCCAA77FF, 0x774433FF, 0xAA7744FF, 0x993300FF, 0x773322FF, 0x552200FF, 0x331100FF, 0x221100FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x999999FF,
	0xFFFFCCFF, 0xFFFF77FF, 0xDDDD22FF, 0xFFFF00FF, 0xFFDD00FF, 0xCCAA00FF, 0x999900FF, 0x887700FF, 0x555500FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x888888FF,
	0xDDBBFFFF, 0xBB99EEFF, 0x6633CCFF, 0x9955FFFF, 0x6600FFFF, 0x554488FF, 0x440099FF, 0x220066FF, 0x221133FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x777777FF,
	0xBBBBFFFF, 0x8899FFFF, 0x3333AAFF, 0x3355EEFF, 0x0000FFFF, 0x333388FF, 0x0000AAFF, 0x111166FF, 0x000022FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x666666FF,
	0x99EEBBFF, 0x66CC77FF, 0x226611FF, 0x44AA33FF, 0x008833FF, 0x557755FF, 0x225500FF, 0x113322FF, 0x002211FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x555555FF,
	0xDDFFBBFF, 0xCCFF88FF, 0x88AA55FF, 0xAADD88FF, 0x88FF00FF, 0xAABB99FF, 0x66BB00FF, 0x559900FF, 0x336600FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x444444FF,
	0xBBDDFFFF, 0x77CCFFFF, 0x335599FF, 0x6699FFFF, 0x1177FFFF, 0x4477AAFF, 0x224477FF, 0x002277FF, 0x001144FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x333333FF,
	0xAAFFFFFF, 0x55FFFFFF, 0x0088BBFF, 0x55BBCCFF, 0x00CCFFFF, 0x4499AAFF, 0x006688FF, 0x004455FF, 0x002233FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x222222FF,
	0xCCFFEEFF, 0xAAEEDDFF, 0x33CCAAFF, 0x55EEBBFF, 0x00FFCCFF, 0x77AAAAFF, 0x00AA99FF, 0x008877FF, 0x004433FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x000000FF,
	0xAAFFAAFF, 0x77FF77FF, 0x66DD44FF, 0x00FF00FF, 0x22DD22FF, 0x55BB55FF, 0x00BB00FF, 0x008800FF, 0x224422FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

// Pattern Name.
std::u16string PatternWA::name() {
	return StringUtils::ReadNLString(patternPointer(), 0, 20, u'\uFFFF');
}
void PatternWA::name(std::u16string v) {
	StringUtils::WriteNLString(patternPointer(), v, 0, 20);
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
	return StringUtils::ReadNLString(patternPointer(), 0x2C, 8, u'\uFFFF');
}
void PatternWA::creatorname(std::u16string v) {
	StringUtils::WriteNLString(patternPointer(), v, 0x2C, 8);
}

// Creator Gender.
u8 PatternWA::creatorGender() {
	return patternPointer()[0x3E];
}
void PatternWA::creatorGender(u8 v) {
	patternPointer()[0x3E] = v;
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
	return StringUtils::ReadNLString(patternPointer(), 0x42, 8, u'\uFFFF');
}
void PatternWA::origtownname(std::u16string v) {
	StringUtils::WriteNLString(patternPointer(), v, 0x42, 8);
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

// Palette Array. Offset: 0x58, count: 16

// Design Type.
u8 PatternWA::designtype() {
	return (patternPointer()[0x69] & 9);
}
void PatternWA::designtype(u8 v) { }

// Dump a Pattern to file.
void PatternWA::dumpPattern(const std::string fileName) {
	// Get Pattern size?
	u32 size = 0;
	if (patternPointer()[0x69] == 0x09){
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
		patternData[i] = patternPointer()[i];
	}
	// Write to file and close.
	fwrite(patternData, 1, size, ptrn);
	fclose(ptrn);
	// Free Buffer.
	delete(patternData);
}

// Inject a Pattern from a file.
void PatternWA::injectPattern(const std::string fileName) {
	if((access(fileName.c_str(), F_OK) != 0))	return; // File not found. Do NOTHING.
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
	for(int i = 0; i < (int)size; i++){
		patternPointer()[i] = patternData[i];
	}
	// Close File, cause we don't need it.
	fclose(ptrn);
	// Free Buffer.
	delete(patternData);
}

// TODO.
std::vector<u8> PatternWA::patternData() {
	std::vector<u8> patternData(0x400);
	return patternData;
}

// Palette Data Array. offset: 0x6C ; count: 0x800