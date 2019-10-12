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

#include "common/title.hpp"
#include "common/utils.hpp"

// Delete the Texture of the Title Icons.

Title::~Title(void)
{
	if (mCard != CARD_TWL && mIcon.tex)
		C3D_TexDelete(mIcon.tex);
}


static C2D_Image loadTextureIcon(smdh_s* smdh)
{
	C3D_Tex* tex                          = new C3D_Tex;
	static const Tex3DS_SubTexture subt3x = {48, 48, 0.0f, 48 / 64.0f, 48 / 64.0f, 0.0f};
	C3D_TexInit(tex, 64, 64, GPU_RGB565);

	u16* dest = (u16*)tex->data + (64 - 48) * 64;
	u16* src  = (u16*)smdh->bigIconData;
	for (int j = 0; j < 48; j += 8)
	{
		std::copy(src, src + 48 * 8, dest);
		src += 48 * 8;
		dest += 64 * 8;
	}

	return C2D_Image{tex, &subt3x};
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
		smdh_s* smdh = loadSMDH(lowId(), highId(), mMedia);
		if (smdh == NULL)
		{
			return false;
		}

		char unique[12] = {0};
		sprintf(unique, "0x%05X ", (unsigned int)uniqueId());
		mName   = StringUtils::UTF16toUTF8((char16_t*)smdh->applicationTitles[1].shortDescription);

		if (Archive::saveAccessible(mMedia, lowId(), highId()))
		{
			loadTitle = true;
			mIcon     = loadTextureIcon(smdh);
		}
		delete smdh;
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

// Get the Icon from the SMDH.

C2D_Image Title::icon(void)
{
	return mIcon;
}

// get the name of the Title from the SMDH.

std::string Title::name(void)
{
	return mName;
}

u32 Title::uniqueId(void)
{
	return (lowId() >> 8);
}