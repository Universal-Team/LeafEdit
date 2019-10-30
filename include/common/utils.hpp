#ifndef UTILS_HPP
#define UTILS_HPP

#include "common/archive.hpp"
#include "common/smdh.hpp"

#include "core/save/item.h"
#include "core/save/player.h"

#include <3ds.h>
#include <stdarg.h>
#include <string>
#include <vector>

namespace StringUtils
{
	std::u16string UTF8toUTF16(const char* src);
	std::string UTF16toUTF8(const std::u16string& src);
	std::string format(std::string fmt_str, ...);
}

namespace EditorUtils
{
	std::vector<u32> findPlayerReferences(Player *player);
	std::vector<std::pair<std::string, s32>> load_player_invitems(int selectedplayer);
	std::vector<std::pair<std::string, s32>> load_player_dresseritems(int selectedplayer, int dresser);
}

u16	strToU16(std::string str);

#endif