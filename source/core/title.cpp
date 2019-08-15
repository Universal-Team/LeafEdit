/*
 *   This file is part of PKSM
 *   Copyright (C) 2016-2019 Bernardo Giordano, Admiral Fish, piepie62
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

#include "title.hpp"


// Check, if a Save is accessible on the Card or on the installed Title.
bool Title::saveAccessible(FS_MediaType mediatype, u32 lowid, u32 highid)
{
    FS_Archive archive;
    Result res = save(&archive, mediatype, lowid, highid);
    if (R_SUCCEEDED(res))
    {
        FSUSER_CloseArchive(archive);
        return true;
    }
    return false;
}

// Save stuff.
Result Title::save(FS_Archive* archive, FS_MediaType mediatype, u32 lowid, u32 highid)
{
    const u32 path[3] = {mediatype, lowid, highid};
    return FSUSER_OpenArchive(archive, ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path});
}

// Load the Title. (Used for the Scan as well.)
bool Title::load(u64 id, FS_MediaType media, FS_CardType card)
{
    bool loadTitle = false;
    mId            = id;
    mMedia         = media;
    mCard          = card;

    if (mCard == CARD_CTR)
    {
        if (saveAccessible(mMedia, lowId(), highId()))
        {
            loadTitle = true;
        }
    }
    return loadTitle;
}


u32 Title::highId(void)
{
    return (u32)(mId >> 32);
}

u32 Title::lowId(void)
{
    return (u32)mId;
}

FS_MediaType Title::mediaType(void)
{
    return mMedia;
}

FS_CardType Title::cardType(void)
{
    return mCard;
}