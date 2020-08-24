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

/* Save class. */
#include "Sav.hpp"
#include "SavWW.hpp"
#include "SavNL.hpp"
#include "SavWA.hpp"

#include <cstring>

/* AC:NL Array for the Save Header. */
u8 ACNLArray[28] = {
	0x64, 0xED, 0xD1, 0x98, 0xF8, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* AC:WA Array for the Save Header. */
u8 ACWAArray[28] = {
	0x33, 0x3A, 0xD0, 0x46, 0x9E, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* Return the SaveType from a Buffer and load. */
std::unique_ptr<Sav> Sav::getSave(std::shared_ptr<u8[]> dt, u32 length) {
	switch (length) {
		case 0x40000:
		case 0x4007A:
		case 0x8007A:
			/* Check for AC:WW Saves and check their region. */
			if (memcmp(dt.get(), dt.get() + 0x12224, 0x12224) == 0) {
				Log->Write("Detected AC:WW Japanese save.");
				return std::make_unique<SavWW>(dt, WWRegion::JPN_REV0, length);
			} else if (memcmp(dt.get(), dt.get() + 0x15FE0, 0x15FE0) == 0) {
				Log->Write("Detected AC:WW Europe / USA save.");
				return std::make_unique<SavWW>(dt, WWRegion::EUR_REV1, length);
			} else if (memcmp(dt.get(), dt.get() + 0x173FC, 0x173FC) == 0) {
				Log->Write("Detected AC:WW Korean save.");
				return std::make_unique<SavWW>(dt, WWRegion::KOR_REV1, length);
			} else {
				return nullptr;
			}
		case 0x7FA00:
			/* Check if save header matches first! We don't want to edit a non-ac save. */
			if (memcmp(dt.get() + 0x80, ACNLArray, 0x1C) == 0) {
				Log->Write("Detected AC:NL save.");
				return std::make_unique<SavNL>(dt, length);
			} else {
				Log->Write("Detected Invalid save.");
				return nullptr;
			}
		case 0x80000:
			Log->Write("Detected a 512 KB save. Checking for 512 KB saves...");
			return check080000(dt, length);
		case 0x89B00:
			/* Check if save header matches first! We don't want to edit a non-ac save. */
			if (memcmp(dt.get() + 0x80, ACWAArray, 0x1C) == 0) {
				Log->Write("Detected AC:WA save.");
				return std::make_unique<SavWA>(dt, length);
			} else {
				Log->Write("Detected Invalid save.");
				return nullptr;
			}
	default:
		Log->Write("Detected Invalid save.");
		return nullptr;
	}
}

/* Because 0x80000 can be an AC:NL & AC:WW save, check it here! */
std::unique_ptr<Sav> Sav::check080000(std::shared_ptr<u8[]> dt, u32 length) {
	/* Check for AC:WW Japanese. */
	if (memcmp(dt.get(), dt.get() + 0x12224, 0x12224) == 0) {
		Log->Write("Detected AC:WW Japanese save.");
		return std::make_unique<SavWW>(dt, WWRegion::JPN_REV0, length);
		/* Check for AC:WW Europe | USA. */
	} else if (memcmp(dt.get(), dt.get() + 0x15FE0, 0x15FE0) == 0) {
		Log->Write("Detected AC:WW Europe / USA save.");
		return std::make_unique<SavWW>(dt, WWRegion::EUR_REV1, length);
		/* Check for AC:WW Korean. */
	} else if (memcmp(dt.get(), dt.get() + 0x173FC, 0x173FC) == 0) {
		Log->Write("Detected AC:WW Korean save.");
		return std::make_unique<SavWW>(dt, WWRegion::KOR_REV1, length);
		/* Check for AC:NL. */
	} else if (memcmp(dt.get() + 0x80, ACNLArray, 0x1C) == 0) {
		Log->Write("Detected AC:NL save.");
			return std::make_unique<SavNL>(dt, length);
	} else {
		/* No save checks matches, return nullptr. */
		Log->Write("Detected Invalid save.");
		return nullptr;
	}
}