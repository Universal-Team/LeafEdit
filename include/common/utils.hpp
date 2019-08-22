#ifndef UTILS_HPP
#define UTILS_HPP

#include "common/archive.hpp"
#include "common/smdh.hpp"

#include <3ds.h>
#include <stdarg.h>
#include <string>

namespace StringUtils
{
	std::u16string UTF8toUTF16(const char* src);
    std::string UTF16toUTF8(const std::u16string& src);
	std::string format(std::string fmt_str, ...);
}

#endif
