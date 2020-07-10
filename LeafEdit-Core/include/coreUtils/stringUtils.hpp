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

#ifndef _LEAFEDIT_CORE_STRINGUTILS_HPP
#define _LEAFEDIT_CORE_STRINGUTILS_HPP

#include "types.hpp"

#include <string>

namespace StringUtils {
	// Converters.
	std::u16string UTF8toUTF16(const std::string& src);
	std::string UTF16toUTF8(const std::u16string& src);

	// Other utilities.
	std::u16string& toFullWidth(std::u16string& in);
	u16 strToU16(const std::string str);

	// New Leaf | Welcome Amiibo.
	std::u16string ReadNLString(const u8* data, int ofs, int len, char16_t term);
	void WriteNLString(u8 *data, const std::u16string &str, u32 offset, u32 maxSize);

	// Wild World.
	std::u16string wwToUnicode(const std::string &input, WWRegion region);
	std::string unicodeToWW(const std::u16string &input, WWRegion region);
	std::u16string ReadWWString(u8 *data, u32 offset, u32 maxSize, WWRegion region);
	void WriteWWString(u8 *data, const std::u16string &str, u32 offset, u32 maxSize, WWRegion region);
}

#endif