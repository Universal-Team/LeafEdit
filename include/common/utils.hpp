#ifndef UTILS_HPP
#define UTILS_HPP

#include "common/archive.hpp"
#include "common/smdh.hpp"
#include "player.h"

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
}

u16             strToU16(std::string str);
void            LoadVillagerDatabase(int lang);
#endif
