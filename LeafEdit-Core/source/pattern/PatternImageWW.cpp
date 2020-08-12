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

#include "PatternImageWW.hpp"
#include "saveUtils.hpp"
#include "stringUtils.hpp"

#include <algorithm> // For std::max.
#include <cstring>

/* Color format seems to be: BRGA. */
static const u32 PaletteColors[] = {
	0xFF0000FF, 0xFF7331FF, 0xFFAD00FF, 0xFFFF00FF, 0xADFF00FF, 0x52FF00FF, 0x00FF00FF, 0x00AD52FF, 0x0052ADFF, 0x0000FFFF, 0x5200FFFF, 0xAD00FFFF, 0xFF00FFFF, 0x000000FF, 0xFFFFFFFF,
	0xFF7B7BFF, 0xFFB57BFF, 0xFFE77BFF, 0xFFFF7BFF, 0xDEFF7BFF, 0xADFF7BFF, 0x7BFF7BFF, 0x52AD84FF, 0x5284ADFF, 0x7B7BFFFF, 0xB57BFFFF, 0xE77BFFFF, 0xFF7BFFFF, 0x000000FF, 0xFFFFFFFF,
	0xA50000FF, 0xA53100FF, 0xA57300FF, 0xA5A500FF, 0x73A500FF, 0x31A500FF, 0x00A500FF, 0x005221FF, 0x002152FF, 0x0000A5FF, 0x3100A5FF, 0x7300A5FF, 0xA500A5FF, 0x000000FF, 0xFFFFFFFF,
	0x009C00FF, 0x5ACE6BFF, 0xB5FFDEFF, 0x009C6BFF, 0x52CEA5FF, 0xADFFD6FF, 0x0052ADFF, 0x2984D6FF, 0x5AADFFFF, 0x0000FFFF, 0x4A6BFFFF, 0x314ADEFF, 0x1821B5FF, 0x00008CFF, 0xFFFFFFFF,
	0xAD7300FF, 0xD6AD42FF, 0xFFDE8CFF, 0xFF0839FF, 0xFF4A6BFF, 0xFF949CFF, 0xAD00FFFF, 0xD663FFFF, 0xFFCEFFFF, 0xFFBD9CFF, 0xDE9473FF, 0xBD634AFF, 0x9C3921FF, 0x7B1000FF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF5200FF, 0xFFB55AFF, 0xFFEFADFF, 0x7B1000FF, 0xA54A31FF, 0xD6846BFF, 0xFFBD9CFF, 0x5AADFFFF, 0x84C6FFFF, 0xADE7FFFF, 0xD6FFFFFF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x00FF00FF, 0x42FF42FF, 0x8CFF8CFF, 0xD6FFD6FF, 0x0000FFFF, 0x4242FFFF, 0x8C8CFFFF, 0xD6D6FFFF, 0xFF0000FF, 0xFF4242FF, 0xFF8C8CFF, 0xFFD6D6FF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x003100FF, 0x426342FF, 0x849C84FF, 0xC6D6C6FF, 0x7B1000FF, 0xA54A29FF, 0xD68C5AFF, 0xFFC68CFF, 0xD6B500FF, 0xE7CE39FF, 0xF7DE7BFF, 0xFFF7BDFF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x0000FFFF, 0xFF0000FF, 0xFFFF00FF, 0x4242FFFF, 0xFF4242FF, 0xFFFF42FF, 0x8C8CFFFF, 0xFF8C8CFF, 0xFFFF8CFF, 0xD6D6FFFF, 0xFFD6D6FF, 0xFFFFD6FF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x00FF00FF, 0x0000FFFF, 0xFF00FFFF, 0x42FF42FF, 0x4242FFFF, 0xFF42FFFF, 0x8CFF8CFF, 0x8C8CFFFF, 0xFF8CFFFF, 0xD6FFD6FF, 0xD6D6FFFF, 0xFFD6FFFF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF7B00FF, 0xFFFF00FF, 0x84FF00FF, 0x00FF00FF, 0x00847BFF, 0x0000FFFF, 0x7B00FFFF, 0xFF94FFFF, 0xD6B500FF, 0xBD1000FF, 0x5A1000FF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x109463FF, 0x087B52FF, 0x108C39FF, 0x319C31FF, 0xCEA54AFF, 0xCE9439FF, 0xBD8C4AFF, 0xD68C31FF, 0xAD734AFF, 0x8C5A31FF, 0x6B4229FF, 0x84EFFFFF, 0x31CEEFFF, 0x00A5C6FF, 0xFFFFFFFF,
	0xD6DEE7FF, 0xB5CEDEFF, 0xE7EFEFFF, 0xF7F7F7FF, 0x84737BFF, 0x948C6BFF, 0x847B63FF, 0x9C845AFF, 0x739CB5FF, 0xFF2929FF, 0xFFFF00FF, 0x9421FFFF, 0x009CBDFF, 0x000000FF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xF7EFEFFF, 0xE7DEDEFF, 0xD6CECEFF, 0xC6B5B5FF, 0xB5A5A5FF, 0xA59494FF, 0x9C8484FF, 0x8C6B6BFF, 0x7B5A5AFF, 0x6B4A4AFF, 0x5A3131FF, 0x4A2121FF, 0x421010FF, 0x310000FF,
	0xFFFFFFFF, 0xEFEFEFFF, 0xDEDEDEFF, 0xCECECEFF, 0xB5B5B5FF, 0xA5A5A5FF, 0x949494FF, 0x848484FF, 0x6B6B6BFF, 0x5A5A5AFF, 0x4A4A4AFF, 0x313131FF, 0x212121FF, 0x101010FF, 0x000000FF,
	0xFF8C7BFF, 0xFF0000FF, 0xFF7B00FF, 0xFFFF00FF, 0x008400FF, 0x00FF00FF, 0x0000FFFF, 0x009CFFFF, 0xD600FFFF, 0xFF6BFFFF, 0x9C0000FF, 0xFF9400FF, 0xFFBD94FF, 0x000000FF, 0xFFFFFFFF
};

void PatternImageWW::refresh() {
	this->valid = false;

	// Palette Index.
	this->paletteIndex = (u8)(((this->paletteData()[0]) & 0xF0) >> 4);

	for (int i = 0; i < 15; i++) {
		this->colors[i] = {PaletteColors[(this->paletteIndex * 15) + i]};
	}
	
	this->valid = true; // TODO: Handle this differently?
}

u32 PatternImageWW::getPaletteColor(int plt) {
	if (plt > 14) return 0;
	return this->colors[plt];
}

/* Thanks to Slattz for this function! */
u32 PatternImageWW::getPixel(int index, bool right) {
	if (this->valid) {
		if (this->patternData() != nullptr) {
			u8 data = this->patternData()[index];
			if (right) data >>= 4;

			return this->colors[std::max<int>(0, (data & 0x0F) - 1)];
		} 
	}

	return 0;
}