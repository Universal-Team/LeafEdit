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

#ifndef _LEAFEDIT_CORE_PATTERN_IMAGE_WW_HPP
#define _LEAFEDIT_CORE_PATTERN_IMAGE_WW_HPP

#include "PatternImage.hpp"

class PatternImageWW : public PatternImage {
protected:
	std::shared_ptr<u8[]> data;
	u32 ptrnOffset;
	u32 pltOffset;
public:
	virtual ~PatternImageWW() {}
	PatternImageWW(std::shared_ptr<u8[]> dt, u32 patternOffset, u32 paletteOffset) :
		PatternImage(), data(dt), ptrnOffset(patternOffset), pltOffset(paletteOffset) { }

	bool isValid() const override { return this->valid; }
	u8 getPaletteColor(u8 plt) const override;
	int getWWPaletteIndex() const override;
	void setPaletteColor(int index, u8 color) override;
	pixel getPixel(int pixel) const override;
	void setPixel(int index, int color) override;
	void setPixel(int x, int y, int color) override;
private:
	bool valid = true;

	pixel *pixelPointer() const {
		return (pixel *)(data.get() + ptrnOffset);
	}

	u8* paletteData() const {
		return this->data.get() + pltOffset;
	}
};

#endif