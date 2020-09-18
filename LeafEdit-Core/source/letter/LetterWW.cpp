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

#include "LetterWW.hpp"
#include "saveUtils.hpp"

u16 LetterWW::playerid() const {
	return 0;
}

std::u16string LetterWW::playername() const {
	return StringUtils::UTF8toUTF16("?");
}

u16 LetterWW::townid() const {
	return 0;
}

std::u16string LetterWW::townname() const {
	return StringUtils::UTF8toUTF16("?");
}

u8 LetterWW::playerindex() const {
	return 0;
}

std::u16string LetterWW::intro() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return StringUtils::ReadUTF8String(this->letterPointer(), 0x30, 0xF, this->region);

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return StringUtils::ReadUTF8String(this->letterPointer(), 0x2C, 0xA, this->region);

		case WWRegion::KOR_REV1:
			return StringUtils::ReadUTF16String(this->letterPointer(), 0x80, 0xF);

		case WWRegion::UNKNOWN:
			return StringUtils::UTF8toUTF16("?");
	}

	return StringUtils::UTF8toUTF16("?");
}

std::u16string LetterWW::body() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return StringUtils::ReadUTF8String(this->letterPointer(), 0x48, 84, this->region);

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return StringUtils::ReadUTF8String(this->letterPointer(), 0x36, 64, this->region);

		case WWRegion::KOR_REV1:
			return StringUtils::ReadUTF16String(this->letterPointer(), 0x93, 64);

		case WWRegion::UNKNOWN:
			return StringUtils::UTF8toUTF16("?");
	}

	return StringUtils::UTF8toUTF16("?");
}

std::u16string LetterWW::end() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return StringUtils::ReadUTF8String(this->letterPointer(), 0xC8, 0x1A, this->region);

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return StringUtils::ReadUTF8String(this->letterPointer(), 0x76, 0x10, this->region);

		case WWRegion::KOR_REV1:
			return StringUtils::ReadUTF16String(this->letterPointer(), 0x114, 0x1A);

		case WWRegion::UNKNOWN:
			return StringUtils::UTF8toUTF16("?");
	}

	return StringUtils::UTF8toUTF16("?");
}

u8 LetterWW::nameindex() const {
	return 0;
}

u8 LetterWW::paperid() const {
	return 0;
}

u8 LetterWW::flag() const {
	return 0;
}

u8 LetterWW::senderid() const {
	return 0;
}

u8 LetterWW::lettertype() const {
	return 0;
}

std::unique_ptr<Item> LetterWW::item() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return std::make_unique<ItemWW>(this->data, this->Offset + 0x1238);

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return std::make_unique<ItemWW>(this->data, this->Offset + 0x1192);

		case WWRegion::KOR_REV1:
			return std::make_unique<ItemWW>(this->data, this->Offset + 0x12A4);

		case WWRegion::UNKNOWN:
			return nullptr;
	}

	return nullptr;
}