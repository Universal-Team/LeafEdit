#ifndef WW_STRING_UTILS_HPP
#define WW_STRING_UTILS_HPP

#include <string>

std::u16string wwToUnicode(const std::string &input, bool japanese);
std::string unicodeToWW(const std::u16string &input, bool japanese);

#endif
