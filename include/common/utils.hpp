#ifndef UTILS_HPP
#define UTILS_HPP

#include <3ds.h>
#include <string>
#include <stdarg.h>
#include "smdh.hpp"
#include "archive.hpp"

namespace StringUtils
{
    std::string UTF16toUTF8(const std::u16string& src);
	std::string format(std::string fmt_str, ...);
}

#endif
