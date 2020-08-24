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

#ifndef _LEAFEDIT_COREUTILS_HPP
#define _LEAFEDIT_COREUTILS_HPP

#include "itemUtils.hpp"
#include "PatternImage.hpp"
#include "Player.hpp"
#include <3ds.h>
#include <citro2d.h>

/* AC:NL | AC:WA Region Lock. */
struct Region_Lock {
	u8 DerivedID;
	CFG_Region RegionID;
	u8 RawByte;
};

namespace CoreUtils {
	void FixInvalidBuildings(void); // Replace Invalid Buildings with Empty. (Fixes crashes in game.)
	u8 DeriveRegionLockID(u8 RegionID, u8 LanguageID);
	bool UpdateSaveRegion(Region_Lock &regionLock); // Update the save's region.
	void FixSaveRegion(Region_Lock &regionLock); // If save region does not match the console - fix it.
	/* C2D_Image LoadPlayerTPC(std::shared_ptr<Player> player); */
	void createBackup();

	/* Pattern stuff. */
	C2D_Image patternImage(std::shared_ptr<PatternImage> image, SaveType ST = SaveType::UNUSED);
	void generateEmptyPattern(SaveType ST, WWRegion region, std::shared_ptr<u8[]> &data);
	void dumpPatternInformation(SaveType ST, WWRegion region, std::shared_ptr<Pattern> &ptrn);
}

#endif