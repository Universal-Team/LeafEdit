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

#ifndef _LEAFEDIT_CORE_PATTERN_IMAGE_HPP
#define _LEAFEDIT_CORE_PATTERN_IMAGE_HPP

#include "types.hpp"

#include <memory>
#include <vector>

class PatternImage {
public:
	virtual ~PatternImage() {}
	PatternImage() { }
	virtual void refresh() = 0; // Refresh buffer.
	virtual u32 getPixelColor(int x, int y) = 0;
	virtual u8 getRawPixel(int indx) = 0;
	virtual bool isValid() = 0;
	virtual std::unique_ptr<u8[]> &returnData() = 0;
	virtual u32 getRawPixelColor(int index) = 0;
	virtual int getPalette(int plt) = 0;
};

#endif