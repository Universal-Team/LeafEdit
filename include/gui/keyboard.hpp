#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <string>

namespace Input {
	void DrawNumpad();
	void drawKeyboard();
	std::string Numpad(std::string Text);
	std::string Numpad(uint maxLength, std::string Text);

	// -1 if invaild text entered
	int getUint(int max, std::string Text);

	std::string getString(uint maxLength, std::string Text, float inputTextSize = 0.6f);
	std::string getString(std::string Text);
	std::string getStringLong(std::string Text); // For longer text.

	std::uint32_t handleu32(uint maxLength, std::string Text, int maxNum, u32 oldValue);
	std::uint16_t handleu16(uint maxLength, std::string Text, int maxNum, u16 oldValue);
	std::uint8_t handleu8(uint maxLength, std::string Text, int maxNum, u8 oldValue);

	std::u16string handleu16String(uint maxLength, std::string Text, std::u16string oldString);
	std::string handleString(uint maxLength, std::string Text, std::string oldString);
}

#endif