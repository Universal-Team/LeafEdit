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

// Is that right?
static const u32 PaletteColors[] = {
	0xFFFF0000, 0xFFFF7331, 0xFFFFAD00, 0xFFFFFF00, 0xFFADFF00, 0xFF52FF00, 0xFF00FF00, 0xFF00AD52, 0xFF0052AD, 0xFF0000FF, 0xFF5200FF, 0xFFAD00FF, 0xFFFF00FF, 0xFF000000, 0xFFFFFFFF,
	0xFFFF7B7B, 0xFFFFB57B, 0xFFFFE77B, 0xFFFFFF7B, 0xFFDEFF7B, 0xFFADFF7B, 0xFF7BFF7B, 0xFF52AD84, 0xFF5284AD, 0xFF7B7BFF, 0xFFB57BFF, 0xFFE77BFF, 0xFFFF7BFF, 0xFF000000, 0xFFFFFFFF,
	0xFFA50000, 0xFFA53100, 0xFFA57300, 0xFFA5A500, 0xFF73A500, 0xFF31A500, 0xFF00A500, 0xFF005221, 0xFF002152, 0xFF0000A5, 0xFF3100A5, 0xFF7300A5, 0xFFA500A5, 0xFF000000, 0xFFFFFFFF,
	0xFF009C00, 0xFF5ACE6B, 0xFFB5FFDE, 0xFF009C6B, 0xFF52CEA5, 0xFFADFFD6, 0xFF0052AD, 0xFF2984D6, 0xFF5AADFF, 0xFF0000FF, 0xFF4A6BFF, 0xFF314ADE, 0xFF1821B5, 0xFF00008C, 0xFFFFFFFF,
	0xFFAD7300, 0xFFD6AD42, 0xFFFFDE8C, 0xFFFF0839, 0xFFFF4A6B, 0xFFFF949C, 0xFFAD00FF, 0xFFD663FF, 0xFFFFCEFF, 0xFFFFBD9C, 0xFFDE9473, 0xFFBD634A, 0xFF9C3921, 0xFF7B1000, 0xFFFFFFFF,
	0xFFFF0000, 0xFFFF5200, 0xFFFFB55A, 0xFFFFEFAD, 0xFF7B1000, 0xFFA54A31, 0xFFD6846B, 0xFFFFBD9C, 0xFF5AADFF, 0xFF84C6FF, 0xFFADE7FF, 0xFFD6FFFF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF00FF00, 0xFF42FF42, 0xFF8CFF8C, 0xFFD6FFD6, 0xFF0000FF, 0xFF4242FF, 0xFF8C8CFF, 0xFFD6D6FF, 0xFFFF0000, 0xFFFF4242, 0xFFFF8C8C, 0xFFFFD6D6, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF003100, 0xFF426342, 0xFF849C84, 0xFFC6D6C6, 0xFF7B1000, 0xFFA54A29, 0xFFD68C5A, 0xFFFFC68C, 0xFFD6B500, 0xFFE7CE39, 0xFFF7DE7B, 0xFFFFF7BD, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF0000FF, 0xFFFF0000, 0xFFFFFF00, 0xFF4242FF, 0xFFFF4242, 0xFFFFFF42, 0xFF8C8CFF, 0xFFFF8C8C, 0xFFFFFF8C, 0xFFD6D6FF, 0xFFFFD6D6, 0xFFFFFFD6, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF00FF00, 0xFF0000FF, 0xFFFF00FF, 0xFF42FF42, 0xFF4242FF, 0xFFFF42FF, 0xFF8CFF8C, 0xFF8C8CFF, 0xFFFF8CFF, 0xFFD6FFD6, 0xFFD6D6FF, 0xFFFFD6FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFFFF0000, 0xFFFF7B00, 0xFFFFFF00, 0xFF84FF00, 0xFF00FF00, 0xFF00847B, 0xFF0000FF, 0xFF7B00FF, 0xFFFF94FF, 0xFFD6B500, 0xFFBD1000, 0xFF5A1000, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF109463, 0xFF087B52, 0xFF108C39, 0xFF319C31, 0xFFCEA54A, 0xFFCE9439, 0xFFBD8C4A, 0xFFD68C31, 0xFFAD734A, 0xFF8C5A31, 0xFF6B4229, 0xFF84EFFF, 0xFF31CEEF, 0xFF00A5C6, 0xFFFFFFFF,
	0xFFD6DEE7, 0xFFB5CEDE, 0xFFE7EFEF, 0xFFF7F7F7, 0xFF84737B, 0xFF948C6B, 0xFF847B63, 0xFF9C845A, 0xFF739CB5, 0xFFFF2929, 0xFFFFFF00, 0xFF9421FF, 0xFF009CBD, 0xFF000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFF7EFEF, 0xFFE7DEDE, 0xFFD6CECE, 0xFFC6B5B5, 0xFFB5A5A5, 0xFFA59494, 0xFF9C8484, 0xFF8C6B6B, 0xFF7B5A5A, 0xFF6B4A4A, 0xFF5A3131, 0xFF4A2121, 0xFF421010, 0xFF310000,
	0xFFFFFFFF, 0xFFEFEFEF, 0xFFDEDEDE, 0xFFCECECE, 0xFFB5B5B5, 0xFFA5A5A5, 0xFF949494, 0xFF848484, 0xFF6B6B6B, 0xFF5A5A5A, 0xFF4A4A4A, 0xFF313131, 0xFF212121, 0xFF101010, 0xFF000000,
	0xFFFF8C7B, 0xFFFF0000, 0xFFFF7B00, 0xFFFFFF00, 0xFF008400, 0xFF00FF00, 0xFF0000FF, 0xFF009CFF, 0xFFD600FF, 0xFFFF6BFF, 0xFF9C0000, 0xFFFF9400, 0xFFFFBD94, 0xFF000000, 0xFFFFFFFF
};

// Pattern Name.
std::u16string PatternWW::name() {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return StringUtils::ReadWWString(patternPointer(), 0x216, 15, this->region);
		case WWRegion::KOR_REV1:
			return StringUtils::ReadNLString(patternPointer(), 0x21F, 10, u'\uFFFF');
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
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			StringUtils::WriteWWString(patternPointer(), v, 0x216, 15, this->region);
			break;
		case WWRegion::KOR_REV1:
			StringUtils::WriteNLString(patternPointer(), v, 0x21F, 10); // No Region param etc cause uses NL's stuff.
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
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return 0; // TODO.
		case WWRegion::KOR_REV1:
			return SaveUtils::Read<u16>(patternPointer(), 0x20F);
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
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			break;
		case WWRegion::KOR_REV1:
			return SaveUtils::Write<u16>(patternPointer(), 0x20F, v);
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
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return StringUtils::ReadWWString(patternPointer(), 0x20C, 7, this->region);
		case WWRegion::KOR_REV1: // Could be changed cause -> UTF-16.
			return StringUtils::ReadNLString(patternPointer(), 0x211, 6, u'\uFFFF');
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
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			StringUtils::WriteWWString(patternPointer(), v, 0x20C, 7, this->region);
			break;
		case WWRegion::KOR_REV1:
			StringUtils::WriteNLString(patternPointer(), v, 0x211, 6); // No Region param etc cause uses NL's stuff.
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
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return 0; // TODO.
		case WWRegion::KOR_REV1:
			return SaveUtils::Read<u16>(patternPointer(), 0x201);
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
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			break;
		case WWRegion::KOR_REV1:
			return SaveUtils::Write<u16>(patternPointer(), 0x201, v);
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
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return StringUtils::ReadWWString(patternPointer(), 0x202, 7, this->region);
		case WWRegion::KOR_REV1:
			return StringUtils::ReadNLString(patternPointer(), 0x203, 6, u'\uFFFF');
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
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			StringUtils::WriteWWString(patternPointer(), v, 0x202, 7, this->region);
			break;
		case WWRegion::KOR_REV1:
			StringUtils::WriteNLString(patternPointer(), v, 0x203, 6); // No Region param etc cause uses NL's stuff.
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
			case WWRegion::JPN_REV0:
			case WWRegion::JPN_REV1:
				size = 0x228;
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
			patternData[i] = patternPointer()[i];
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
		if ((access(fileName.c_str(), F_OK) != 0))	return; // File not found. Do NOTHING.
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
			case WWRegion::JPN_REV0:
			case WWRegion::JPN_REV1:
				if (size == 0x228) {
					allowInject = true;
				}
				break;
			case WWRegion::KOR_REV1:
				if (size == 0x234) {
					allowInject = true;
				}
				break;
			case WWRegion::UNKNOWN:
				break;
		}

		if (allowInject) {
			u8 *patternData = new u8[size];
			fread(patternData, 1, size, ptrn);
			// Set Buffer data to save.
			for(int i = 0; i < (int)size; i++){
				patternPointer()[i] = patternData[i];
			}
			// Free Buffer.
			delete(patternData);
		}
		// Close File, cause we don't need it.
		fclose(ptrn);
	}
}

// TODO.
std::vector<u8> PatternWW::patternData() {
	std::vector<u8> patternData(0x400);
	return patternData;
}

// Palette array seems to be 0x200, it begins at 0x1 at Korean.
// Europe | USA | JPN - Research.