/*
*   This file is part of LeafEdit-Core
*   Copyright (C) 2020 Universal-Team
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

#include "saveUtils.hpp"
#include "stringUtils.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <sstream>

/*
	ⓢ is actually s̊, but that's two characters.
	☔ would be better as 💧, but that's too big for a char16_t.
*/

constexpr std::array<char16_t, 256> wwCharacterDictionary = {
	u'\0', u'A', u'B', u'C', u'D', u'E', u'F', u'G', u'H', u'I', u'J', u'K', u'L', u'M', u'N', u'O',
	u'P', u'Q', u'R', u'S', u'T', u'U', u'V', u'W', u'X', u'Y', u'Z', u'a', u'b', u'c', u'd', u'e',
	u'f', u'g', u'h', u'i', u'j', u'k', u'l', u'm', u'n', u'o', u'p', u'q', u'r', u's', u't', u'u',
	u'v', u'w', u'x', u'y', u'z', u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7', u'8', u'9', u'⨍',
	u'ⓢ', u'Œ', u'Ž', u'š', u'œ', u'ž', u'Ÿ', u'À', u'Á', u'Â', u'Ã', u'Ä', u'Å', u'Æ', u'Ç', u'È',
	u'É', u'Ê', u'Ë', u'Ì', u'Í', u'Î', u'Ï', u'Đ', u'Ñ', u'Ò', u'Ó', u'Ô', u'Õ', u'Ö', u'Ø', u'Ù',
	u'Ú', u'Û', u'Ü', u'Ý', u'Þ', u'β', u'à', u'á', u'â', u'ã', u'ä', u'å', u'æ', u'ç', u'è', u'é',
	u'ê', u'ë', u'ì', u'í', u'î', u'ï', u'ð', u'ñ', u'ò', u'ó', u'ô', u'õ', u'ö', u'ø', u'ù', u'ú',
	u'û', u'ü', u'ý', u'þ', u'ÿ', u' ', u'\n', u'!', u'"', u'#', u'$', u'%', u'&', u'´', u'(', u')',
	u'*', u'+', u',', u'-', u'.', u'/', u':', u';', u'<', u'=', u'>', u'?', u'@', u'[', u'\\', u']',
	u'^', u'_', u'´', u'{', u'|', u'}', u'~', u'€', u'‚', u'„', u'…', u'†', u'‡', u'＾', u'‰', u'⟨',
	u'‘', u'’', u'“', u'”', u'•', u'–', u'—', u'˜', u'™', u'⟩', u' ', u'¡', u'¢', u'£', u'¤', u'¥',
	u'╎', u'§', u'¨', u'©', u'ª', u'«', u'¬', u'-', u'®', u'‾', u'°', u'±', u'²', u'³', u'⁄', u'µ',
	u'¶', u'•', u'¸', u'¹', u'º', u'»', u'¼', u'½', u'¾', u'¿', u'×', u'÷', u'☔', u'★', u'❤', u'♪',
	u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0',
	u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0', u'\0',
};

/*
	☔ would be better as 💧, but that's too big for a char16_t.
*/

constexpr std::array<char16_t, 256> wwCharacterDictionaryJapanese = {
	u'\0', u'あ', u'い', u'う', u'え', u'お', u'か', u'き', u'く', u'け', u'こ', u'さ', u'し', u'す', u'せ', u'そ',
	u'た', u'ち', u'つ', u'て', u'と', u'な', u'に', u'ぬ', u'ね', u'の', u'は', u'ひ', u'ふ', u'へ', u'ほ', u'ま',
	u'み', u'む', u'め', u'も', u'や', u'ゆ', u'よ', u'ら', u'り', u'る', u'れ', u'ろ', u'わ', u'を', u'ん', u'が',
	u'ぎ', u'ぐ', u'げ', u'ご', u'ざ', u'じ', u'ず', u'ぜ', u'ぞ', u'だ', u'ぢ', u'づ', u'で', u'ど', u'ば', u'び',
	u'ぶ', u'べ', u'ぼ', u'ぱ', u'ぴ', u'ぷ', u'ぺ', u'ぽ', u'ぁ', u'ぃ', u'ぅ', u'ぇ', u'ぉ', u'ゃ', u'ゅ', u'ょ',
	u'っ', u'ア', u'イ', u'ウ', u'エ', u'オ', u'カ', u'キ', u'ク', u'ケ', u'コ', u'サ', u'シ', u'ス', u'セ', u'ソ',
	u'タ', u'チ', u'ツ', u'テ', u'ト', u'ナ', u'二', u'ヌ', u'ネ', u'ノ', u'ハ', u'ヒ', u'フ', u'へ', u'ホ', u'マ',
	u'ミ', u'ム', u'メ', u'モ', u'ヤ', u'ユ', u'ヨ', u'ラ', u'リ', u'ル', u'レ', u'ロ', u'ワ', u'ヲ', u'ソ', u'ガ',
	u'ギ', u'グ', u'ゲ', u'ゴ', u'ザ', u'ジ', u'ズ', u'ゼ', u'ゾ', u'ダ', u'ヂ', u'ヅ', u'デ', u'ド', u'バ', u'ビ',
	u'ブ', u'ベ', u'ボ', u'パ', u'ピ', u'プ', u'ペ', u'ポ', u'ァ', u'ィ', u'ゥ', u'ェ', u'ォ', u'ャ', u'ュ', u'ョ',
	u'ッ', u'ヴ', u'A', u'B', u'C', u'D', u'E', u'F', u'G', u'H', u'I', u'J', u'K', u'L', u'M', u'N',
	u'O', u'P', u'Q', u'R', u'S', u'T', u'U', u'V', u'W', u'X', u'Y', u'Z', u'a', u'b', u'c', u'd',
	u'e', u'f', u'g', u'h', u'i', u'j', u'k', u'l', u'm', u'n', u'o', u'p', u'q', u'r', u's', u't',
	u'u', u'v', u'w', u'x', u'y', u'z', u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7', u'8', u'9',
	u' ', u'\n', u'ー', u'~', u'･', u'。', u'、', u'!', u'?', u'.', u',', u'｢', u'｣', u'(', u')', u'<',
	u'>', u'\'', u'\"', u'_', u'+', u'=', u'&', u'@', u':', u';', u'×', u'÷', u'☔', u'★', u'♥', u'♪',
};

/* Korean is different and uses the NL Strings. */
std::u16string StringUtils::wwToUnicode(const std::string &input, WWRegion region) {
	std::u16string output;
	const std::array<char16_t, 256> *characters;

	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			characters = &wwCharacterDictionary;
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			characters = &wwCharacterDictionaryJapanese;
			break;

		case WWRegion::KOR_REV1:
		case WWRegion::UNKNOWN:
			return output;

		default:
			return output;
	}

	for (char16_t character : input) {
		if ((*characters)[character] == '\0') break;
		if (character < characters->size()) {
			output += (*characters)[character];
		}
	}

	return output;
}

/* Korean is different and uses the NL Strings. */
std::string StringUtils::unicodeToWW(const std::u16string &input, WWRegion region) {
	std::string output;
	const std::array<char16_t, 256> *characters;

	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			characters = &wwCharacterDictionary;
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			characters = &wwCharacterDictionaryJapanese;
			break;

		case WWRegion::KOR_REV1:
		case WWRegion::UNKNOWN:
			return "";

		default:
			return "";
	}

	for(char16_t character : input) {
		auto it = std::find(characters->begin(), characters->end(), character);
		if (it != characters->end()) {
			output += std::distance(characters->begin(), it);
		}
	}

	return output;
}

std::string utf16DataToUtf8(const char16_t* data, size_t size, char16_t delim = 0) {
	std::string ret;
	ret.reserve(size);
	char addChar[4] = {0};

	for (size_t i = 0; i < size; i++) {
		if (data[i] == delim) {
			return ret;

		} else if (data[i] < 0x0080) {
			addChar[0] = data[i];
			addChar[1] = '\0';

		} else if (data[i] < 0x0800) {
			addChar[0] = 0xC0 | ((data[i] >> 6) & 0x1F);
			addChar[1] = 0x80 | (data[i] & 0x3F);
			addChar[2] = '\0';

		} else {
			addChar[0] = 0xE0 | ((data[i] >> 12) & 0x0F);
			addChar[1] = 0x80 | ((data[i] >> 6) & 0x3F);
			addChar[2] = 0x80 | (data[i] & 0x3F);
			addChar[3] = '\0';
		}

		ret.append(addChar);
	}

	return ret;
}

/* Might be useful for the Keyboard to convert to u16string. */
std::u16string StringUtils::UTF8toUTF16(const std::string& src) {
	std::u16string ret;
	ret.reserve(src.size());

	for (size_t i = 0; i < src.size(); i++) {
		u16 codepoint	= 0xFFFD;
		int iMod		= 0;
		if (src[i] & 0x80 && src[i] & 0x40 && src[i] & 0x20 && !(src[i] & 0x10) && i + 2 < src.size()) {
			codepoint	= src[i] & 0x0F;
			codepoint	= codepoint << 6 | (src[i + 1] & 0x3F);
			codepoint	= codepoint << 6 | (src[i + 2] & 0x3F);
			iMod		= 2;

		} else if (src[i] & 0x80 && src[i] & 0x40 && !(src[i] & 0x20) && i + 1 < src.size()) {
			codepoint	= src[i] & 0x1F;
			codepoint	= codepoint << 6 | (src[i + 1] & 0x3F);
			iMod		= 1;
			
		} else if (!(src[i] & 0x80)) {
			codepoint = src[i];
		}

		ret.push_back((char16_t)codepoint);
		i += iMod;
	}

	return ret;
}

/* Is used to display Text on 3DS. */
std::string StringUtils::UTF16toUTF8(const std::u16string& src) {
	return utf16DataToUtf8(src.data(), src.size());
}

/* Read a Wild World String. */
std::u16string StringUtils::ReadUTF8String(u8 *data, u32 offset, u32 maxSize, WWRegion region) {
	std::string str(reinterpret_cast<char *>(data + offset), maxSize + 1);
	return wwToUnicode(str, region);
}

void StringUtils::WriteUTF8String(u8 *data, const std::u16string &str, u32 offset, u32 maxSize, WWRegion region) {
	/* Do not allow a string longer as max. */
	if (str.length() > maxSize + 1) return;

	const std::string dataString(unicodeToWW(str, region));
	memcpy(data + offset, (u8 *)dataString.data(), maxSize);
}

/* Used to get the NL | WA Strings. */
std::u16string StringUtils::ReadUTF16String(u8* data, int ofs, int len) {
	return std::u16string(reinterpret_cast<char16_t *>(data + ofs), len + 1);
}

void StringUtils::WriteUTF16String(u8 *data, const std::u16string &str, u32 offset, u32 maxSize) {
	/* Do not allow a string longer as max. */
	if (str.length() > maxSize + 1) return;

	memcpy(data + offset, (u8 *)str.data(), maxSize * 2);
}

/* Converts a single latin character from half-width to full-width. */
char16_t tofullwidth(char16_t c) {
	if (c == ' ')	c = u'　';
	else if (c >= '!' && c <= '~')	c += 0xFEE0;
	return c;
}

std::u16string& StringUtils::toFullWidth(std::u16string& in) {
	std::transform(in.begin(), in.end(), in.begin(), tofullwidth);
	return in;
}

/* String to U16. Useful for the Item ID & Name at one. */
u16 StringUtils::strToU16(const std::string str) {
	u16 out;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> out;

	return out;
}