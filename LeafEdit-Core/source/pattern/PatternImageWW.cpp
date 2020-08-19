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

u8 PatternImageWW::getPaletteColor(u8 plt) {
	if (plt > 15) return 0;
	u8 paletteIndex = (u8)(((this->paletteData()[0]) & 0xF0) >> 4);
	return (u8)((paletteIndex * 15) + plt);
}

int PatternImageWW::getWWPaletteIndex() {
	return (u8)(((this->paletteData()[0]) & 0xF0) >> 4);
}

void PatternImageWW::setPaletteColor(int index, u8 color) {
	if (index > 14) return;
	this->paletteData()[0] = (u8)(((index) << 4) & 0xF0);
}

pixel PatternImageWW::getPixel(int index) {
	if (this->valid) {
		if (this->patternData() != nullptr) {
			return this->pixelPointer()[index];
		} 
	}

	return {0, 0};
}

void PatternImageWW::setPixel(int index, int color) {
	if (color > 15 || index > 0x3FF) return; // Out of scope.
	
	if (this->valid) {
		if (this->patternData() != nullptr) {
			if (index % 2 == 0) this->pixelPointer()[index / 2].left = color;
			else this->pixelPointer()[index / 2].right = color;
		}
	}
}

void PatternImageWW::setPixel(int x, int y, int color) {
	this->setPixel((x + (y * 32)), color);
}