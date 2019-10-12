#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <string>

namespace Input {
	void DrawNumpad();

	std::string Numpad(std::string Text);
	std::string Numpad(uint maxLength, std::string Text);
	void drawKeyboard();

	// -1 if invaild text entered
	int getUint(int max, std::string Text);

	std::string getLine(std::string Text);
	std::string getLine(uint maxLength, std::string Text);

	// System Keyboard stuff.
	std::uint32_t getu32(uint maxLength, int maxNum); // u32.
	std::uint16_t getu16(uint maxLength, int maxNum); // u16.
	std::uint8_t getu8(uint maxLength, int maxNum); // u8.
	std::u16string getu16String(uint maxLength, const char *hint); // u16 String.
	std::string getString(uint maxLength, const char *hint); // normal string.
}

#endif