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

#include "saveUtils.hpp"
#include "ShopWW.hpp"

/* Get able sister pattern. */
std::unique_ptr<Pattern> ShopWW::ableSisterPattern(int pattern) const {
	if (pattern > 7) return nullptr;

	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return std::make_unique<PatternWW>(this->data, this->Offset + 0xFAFC + pattern * 0x228, this->region);

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return std::make_unique<PatternWW>(this->data, this->Offset + 0xDAF8 + pattern * 0x220, this->region);

		case WWRegion::KOR_REV1:
			return std::make_unique<PatternWW>(this->data, this->Offset + 0x10AD0 + pattern * 0x234, this->region);
			
		case WWRegion::UNKNOWN:
			return nullptr;
	}

	return nullptr;
}