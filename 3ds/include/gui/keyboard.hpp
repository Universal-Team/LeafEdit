/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
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

#ifndef _LEAFEDIT_KEYBOARD_HPP
#define _LEAFEDIT_KEYBOARD_HPP

#include <3ds.h>
#include <string>

namespace Input {
	void DrawHex();
	void DrawNumpad();
	void drawKeyboard();
	std::string Numpad(std::string Text);
	std::string Numpad(uint maxLength, std::string Text);

	/* -1 if invaild text entered. */
	int getUint(int max, std::string Text);

	std::string getHex(int max, std::string Text);

	std::string getString(uint maxLength, std::string Text, float inputTextSize = 0.6f);
	std::string getString(std::string Text);
	std::string getStringLong(std::string Text); // For longer text.

	std::uint32_t handleu32(uint maxLength, std::string Text, int maxNum, u32 oldValue);
	std::uint16_t handleu16(uint maxLength, std::string Text, int maxNum, u16 oldValue);
	std::uint8_t handleu8(uint maxLength, std::string Text, int maxNum, u8 oldValue);

	std::u16string handleu16String(uint maxLength, std::string Text, std::u16string oldString);
	std::string handleString(uint maxLength, std::string Text, std::string oldString);

	std::string setkbdString(uint maxLength, std::string Text);
}

#endif