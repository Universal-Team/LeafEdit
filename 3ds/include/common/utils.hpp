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

#ifndef UTILS_HPP
#define UTILS_HPP

#include "common/archive.hpp"

#include "core/save/item.h"
#include "core/save/player.h"

#include <3ds.h>
#include <citro2d.h>
#include <stdarg.h>
#include <string>
#include <vector>

namespace StringUtils
{
	std::u16string UTF8toUTF16(const std::string &text);
	std::string UTF16toUTF8(const std::u16string& src);
	std::string format(std::string fmt_str, ...);
}

namespace EditorUtils
{
	std::vector<u32> findPlayerReferences(Player *player);
	std::vector<std::pair<std::string, s32>> load_player_invitems(int selectedplayer);
	std::vector<std::pair<std::string, s32>> load_player_dresseritems(int selectedplayer, int dresser);
	std::vector<std::pair<std::string, s32>> load_player_islandbox(int selectedplayer, int islandBox);
	std::vector<std::pair<std::string, s32>> load_player_storageitems(int selectedplayer, int storage);
}

namespace Utils
{
	void colorLogic(int mode);
	void createBackup(void);
}

#define TEXTURE_TRANSFER_FLAGS \
    (GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | \
    GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
    GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))
	
namespace C2DUtils
{
	C2D_Image ImageDataToC2DImage(u32 *imageData, u32 width, u32 height, GPU_TEXCOLOR colorFormat);
	void C2D_ImageDelete(C2D_Image image);
}

u16	strToU16(std::string str);

#endif