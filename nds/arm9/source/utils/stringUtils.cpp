#include "stringUtils.hpp"

// Convert UTF-8 (up to 0xFFFF) to a single UTF-16 character
std::u16string StringUtils::utf8to16(const std::string &text) {
	std::u16string out;
	char16_t c;

	for(uint i=0;i<text.size();) {
		if(!(text[i] & 0x80)) {
			c = text[i++];
		} else if((text[i] & 0xE0) == 0xC0) {
			c  = (text[i++] & 0x1F) << 6;
			c |=  text[i++] & 0x3F;
		} else if((text[i] & 0xF0) == 0xE0) {
			c  = (text[i++] & 0x0F) << 12;
			c |= (text[i++] & 0x3F) << 6;
			c |=  text[i++] & 0x3F;
		} else {
			i++; // out of range or something (This only does up to 0xFFFF since it goes to a U16 anyways)
		}
		out += c;
	}
	return out;
}

// Convert UTF-16 (up to 0xFFFF) to UTF-8
std::string StringUtils::utf16to8(const std::u16string &text) {
	std::string out;

	for(uint i=0;i<text.size();i++) {
		if(text[i] <= 0x7F) {
			out += text[i];
		} else if(text[i] <= 0x7FF) {
			out += 0xC0 | (text[i] >> 6);
			out += 0x80 | (text[i] & 0x3F);
		} else if(text[i] <= 0xFFFF) {
			out += 0xE0 |  (text[i] >> 12);
			out += 0x80 | ((text[i] & 0xFC0) >> 6);
			out += 0x80 |  (text[i] & 0x03F);
		}
	}

	return out;
}