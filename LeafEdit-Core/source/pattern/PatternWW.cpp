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

#include "PatternWW.hpp"
#include "saveUtils.hpp"
#include "stringUtils.hpp"

#include <cstring>
#include <unistd.h>

// Pattern Name.
std::u16string PatternWW::name() {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return StringUtils::ReadWWString(patternPointer(), 0x216, 15, this->region);
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return StringUtils::ReadWWString(patternPointer(), 0x212, 9, this->region);
		case WWRegion::KOR_REV1:
			return StringUtils::ReadNLString(patternPointer(), 0x21E, 10, u'\uFFFF');
		case WWRegion::UNKNOWN:
			return StringUtils::UTF8toUTF16("?");
	}

	return StringUtils::UTF8toUTF16("?");
}
void PatternWW::name(std::u16string v) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			StringUtils::WriteWWString(patternPointer(), v, 0x216, 15, this->region);
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			StringUtils::WriteWWString(patternPointer(), v, 0x212, 9, this->region);
			break;
		case WWRegion::KOR_REV1:
			StringUtils::WriteNLString(patternPointer(), v, 0x21E, 10); // No Region param etc cause uses NL's stuff.
			break;
		case WWRegion::UNKNOWN:
			break;
	}
}

// Creator ID.
u16 PatternWW::creatorid() {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			//return SaveUtils::Read<u16>(patternPointer(), 0x21E);
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return SaveUtils::Read<u16>(patternPointer(), 0x208);
		case WWRegion::KOR_REV1:
			return SaveUtils::Read<u16>(patternPointer(), 0x20E);
		case WWRegion::UNKNOWN:
			return 0;
	}

	return 0;
}
void PatternWW::creatorid(u16 v) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			//SaveUtils::Write<u16>(patternPointer(), 0x21E);
			break;
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			SaveUtils::Write<u16>(patternPointer(), 0x208, v);
			break;
		case WWRegion::KOR_REV1:
			SaveUtils::Write<u16>(patternPointer(), 0x20E, v);
			break;
		case WWRegion::UNKNOWN:
			break;
	}
}

// Creator Name.
std::u16string PatternWW::creatorname() {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return StringUtils::ReadWWString(patternPointer(), 0x20C, 7, this->region);
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return StringUtils::ReadWWString(patternPointer(), 0x20A, 6, this->region);
		case WWRegion::KOR_REV1: // Could be changed cause -> UTF-16.
			return StringUtils::ReadNLString(patternPointer(), 0x210, 6, u'\uFFFF');
		case WWRegion::UNKNOWN:
			return StringUtils::UTF8toUTF16("?");
	}

	return StringUtils::UTF8toUTF16("?");
}
void PatternWW::creatorname(std::u16string v) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			StringUtils::WriteWWString(patternPointer(), v, 0x20C, 7, this->region);
			break;
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			StringUtils::WriteWWString(patternPointer(), v, 0x20A, 6, this->region);
			break;
		case WWRegion::KOR_REV1:
			StringUtils::WriteNLString(patternPointer(), v, 0x210, 6); // No Region param etc cause uses NL's stuff.
			break;
		case WWRegion::UNKNOWN:
			break;
	}
}

// Creator Gender.
u8 PatternWW::creatorGender() {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return 0;
		case WWRegion::KOR_REV1:
			return 0;
		case WWRegion::UNKNOWN:
			return 0;
	}

	return 0;
}
void PatternWW::creatorGender(u8 v) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
		case WWRegion::KOR_REV1:
		case WWRegion::UNKNOWN:
			break;
	}
}

// Town ID.
u16 PatternWW::origtownid() {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return 0; // TODO.
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return SaveUtils::Read<u16>(patternPointer(), 0x200);
		case WWRegion::KOR_REV1:
			return SaveUtils::Read<u16>(patternPointer(), 0x200);
		case WWRegion::UNKNOWN:
			return 0;
	}

	return 0;
}
void PatternWW::origtownid(u16 v) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			break;
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			SaveUtils::Write<u16>(patternPointer(), 0x200, v);
			break;
		case WWRegion::KOR_REV1:
			SaveUtils::Write<u16>(patternPointer(), 0x200, v);
			break;
		case WWRegion::UNKNOWN:
			break;
	}
}

// Town Name.
std::u16string PatternWW::origtownname() {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return StringUtils::ReadWWString(patternPointer(), 0x202, 7, this->region);
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return StringUtils::ReadWWString(patternPointer(), 0x202, 6, this->region);
		case WWRegion::KOR_REV1:
			return StringUtils::ReadNLString(patternPointer(), 0x202, 6, u'\uFFFF');
		case WWRegion::UNKNOWN:
			return StringUtils::UTF8toUTF16("?");
	}

	return StringUtils::UTF8toUTF16("?");
}
void PatternWW::origtownname(std::u16string v) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			StringUtils::WriteWWString(patternPointer(), v, 0x202, 7, this->region);
			break;
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			StringUtils::WriteWWString(patternPointer(), v, 0x202, 6, this->region);
			break;
		case WWRegion::KOR_REV1:
			StringUtils::WriteNLString(patternPointer(), v, 0x202, 6); // No Region param etc cause uses NL's stuff.
			break;
		case WWRegion::UNKNOWN:
			break;
	}
}

// Own a Pattern.
void PatternWW::ownPattern(std::unique_ptr<Player> player) {
	// Only set if player is not nullptr!
	if (player != nullptr) {
		this->creatorid(player->playerid());
		this->creatorname(player->name());
		this->origtownid(player->townid());
		this->origtownname(player->townname());
	}
}

// Design Type.
u8 PatternWW::designtype() {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return 0;
		case WWRegion::KOR_REV1:
			return patternPointer()[0x233]; // Todo: Research.
		case WWRegion::UNKNOWN:
			return 0;
	}

	return 0;
}
void PatternWW::designtype(u8 v) {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
		case WWRegion::KOR_REV1:
		case WWRegion::UNKNOWN:
			break;
	}
}

// Needs checking. Dump a Pattern to file.
void PatternWW::dumpPattern(const std::string fileName) {
	// If region == UNKNOWN -> Do NOTHING.
	if (this->region != WWRegion::UNKNOWN) {
		u32 size = 0;
		// Get size.
		switch(this->region) {
			case WWRegion::USA_REV0:
			case WWRegion::USA_REV1:
			case WWRegion::EUR_REV1:
				size = 0x228;
				break;
			case WWRegion::JPN_REV0:
			case WWRegion::JPN_REV1:
				size = 0x220;
				break;
			case WWRegion::KOR_REV1:
				size = 0x234;
				break;
			case WWRegion::UNKNOWN:
				break;
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
}

// Needs checking. Inject a Pattern from a file.
void PatternWW::injectPattern(const std::string fileName) {
	// If region == UNKNOWN -> Do NOTHING.
	if (this->region != WWRegion::UNKNOWN) {
		if ((access(fileName.c_str(), F_OK) != 0)) return; // File not found. Do NOTHING.
		bool allowInject = false;
		u32 size = 0;
		// Open file and get size.
		FILE* ptrn = fopen(fileName.c_str(), "rb");
		fseek(ptrn, 0, SEEK_END);
		size = ftell(ptrn);
		fseek(ptrn, 0, SEEK_SET);

		// Get size.
		switch(this->region) {
			case WWRegion::USA_REV0:
			case WWRegion::USA_REV1:
			case WWRegion::EUR_REV1:
				if (size == 0x228) allowInject = true;
				break;
			case WWRegion::JPN_REV0:
			case WWRegion::JPN_REV1:
				if (size == 0x220) allowInject = true;
				break;
			case WWRegion::KOR_REV1:
				if (size == 0x234) allowInject = true;
				break;
			case WWRegion::UNKNOWN:
				break;
		}

		if (allowInject) {
			u8 *patternData = new u8[size];
			fread(patternData, 1, size, ptrn);

			// Set Buffer data to save.
			for(int i = 0; i < (int)size; i++) {
				SaveUtils::Write<u8>(this->patternPointer(), i, patternData[i]);
			}

			// Free Buffer.
			delete(patternData);
		}

		// Close File, cause we don't need it.
		fclose(ptrn);
	}
}

// TODO.
u8* PatternWW::patternData(const int pattern) {
	u8* patternData = new u8[0x400];

	u8 *data = 0;

		// Get start offset.
		switch(this->region) {
			case WWRegion::USA_REV0:
			case WWRegion::USA_REV1:
			case WWRegion::EUR_REV1:
			case WWRegion::JPN_REV0:
			case WWRegion::JPN_REV1:
				data = this->patternPointer() + 0;
				break;
			case WWRegion::KOR_REV1:
				data = this->patternPointer() + 0;
				break;
			case WWRegion::UNKNOWN:
				return nullptr; // What else should be returned? :P
				break;
		}

	for(uint i = 0; i < 0x200; i++) {
		patternData[i * 2] = data[i] & 0xF;
		patternData[i * 2 + 1] = data[i] >> 4;
	}

	return patternData;
}

std::array<u8, 16> PatternWW::customPalette() {
	return {0};
}

std::shared_ptr<PatternImage> PatternWW::image(const int pattern) {
	u32 patternOffset = this->Offset + 0;
	u32 pltOffset = this->Offset + 0;

	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			patternOffset = this->Offset + 0;
			pltOffset = this->Offset + 0x226;
			break;
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			patternOffset = this->Offset + 0;
			pltOffset = this->Offset + 0x21C;
			break;
		case WWRegion::KOR_REV1:
			patternOffset = this->Offset + 0;
			pltOffset = this->Offset + 0x232;
			break;
		case WWRegion::UNKNOWN:
			return nullptr; // What else should be returned? :P
			break;
	}

	return std::make_shared<PatternImageWW>(this->data, patternOffset, pltOffset);
}