#include "wwStringUtils.hpp"
#include <algorithm>
#include <array>

// ⓢ is actually s̊, but that's two characters
// ☔ would be better as 💧, but that's too big for a char16_t
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

// ☔ would be better as 💧, but that's too big for a char16_t
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

std::u16string wwToUnicode(const std::string &input, bool japanese) {
	std::u16string output;

	const std::array<char16_t, 256> &characters = japanese ? wwCharacterDictionaryJapanese : wwCharacterDictionary;

	for(char character : input) {
		if(character < characters.size()) {
			output += characters[character];
		}
	}

	return output;
}

std::string unicodeToWW(const std::u16string &input, bool japanese) {
	std::string output;

	const std::array<char16_t, 256> &characters = japanese ? wwCharacterDictionaryJapanese : wwCharacterDictionary;

	for(char character : input) {
		auto it = std::find(characters.begin(), characters.end(), character);
		if(it != characters.end()) {
			output += std::distance(characters.begin(), it);
		}
	}

	return output;
}