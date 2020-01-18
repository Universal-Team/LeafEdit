#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>

namespace StringUtils {
	std::u16string utf8to16(const std::string &text);
	std::string utf16to8(const std::u16string &text);
}


#endif