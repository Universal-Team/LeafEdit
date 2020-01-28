/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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

#include "wwChecksum.hpp"
#include "wwsave.hpp"

#include "types.hpp"

#include <string>

// Checksum offset is "0x15FDC" in AC:WW's case.
u16 WWChecksum::Calculate(const u16 *buffer, u64 size, uint checksumOffset)
{
    if ((checksumOffset & 1) == 1)
        return 0; // checksumOffset must be 16-bit aligned!");

    u16 checksum = 0;
    for (uint i = 0; i < size; i++)
    {
        if (i == checksumOffset) continue;
        checksum += buffer[i];
    }

    return (u16) -checksum;
}

bool WWChecksum::Verify(const u16 *buffer, u64 size, u16 currentChecksum, uint checksumOffset)
{
	if (Calculate(buffer, size, checksumOffset) == currentChecksum)	return true;
	else	return false;
}

// From ACSE Save.cs.
/*
				case SaveType.WildWorld:
					Write(SaveDataStartOffset + SaveInfo.SaveOffsets.Checksum,
						new UInt16LEChecksum().Calculate(
							SaveData.Skip(SaveDataStartOffset).Take(SaveInfo.SaveOffsets.SaveSize).ToArray(),
							(uint) SaveInfo.SaveOffsets.Checksum), IsBigEndian);

					SaveData.Skip(SaveDataStartOffset).Take(SaveInfo.SaveOffsets.SaveSize).ToArray().CopyTo(
						SaveData,
						SaveDataStartOffset + SaveInfo.SaveOffsets.SaveSize);
					break;


	 SaveData = _saveReader.ReadBytes((int)_saveFile.Length); -> 256000 iirc.
*/

void WWChecksum::UpdateChecksum(u16 *buffer, u64 size) {
	WWSave::Instance()->Write(0x15FDC, Calculate(buffer, size, 0xAFEE));
}
