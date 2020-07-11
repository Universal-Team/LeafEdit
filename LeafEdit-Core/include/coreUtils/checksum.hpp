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

#ifndef _LEAFEDIT_CORE_CHECKSUM_HPP
#define _LEAFEDIT_CORE_CHECKSUM_HPP

#include "types.hpp"

typedef enum {
	CRC_REFLECTED = 0, //Most common in ACNL checksums.
	CRC_NORMAL = 1
} ChecksumType;

namespace Checksum {
	// New Leaf stuff.
	u32 CalculateCRC32Reflected(u8 *buf, u32 size);
	u32 CalculateCRC32Normal(u8 *buf, u32 size);
	bool VerifyCRC32(u32 crc, u8 *buf, u32 startOffset, u32 size, ChecksumType type = CRC_REFLECTED);
	u32 UpdateCRC32(u8 *data, u32 startOffset, u32 size, ChecksumType type = CRC_REFLECTED);
	void FixCRC32s(u8 *data);
	void FixNLCRC32s(u8 *data);
	
	// Wild World stuff.
	u16 CalculateWW(const u16 *buffer, u64 size, uint checksumOffset);
	bool VerifyWW(const u16 *buffer, u64 size, u16 currentChecksum, uint checksumOffset);
	// Checksum for EUR | USA ; JPN ; KOR.
	void UpdateWWChecksum(WWRegion region, u8 *saveBuffer, u16 *buffer, u64 size);
}

#endif