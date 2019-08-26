#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <string>

namespace Input {
	void DrawNumpad();

	std::string Numpad(std::string Text);
	std::string Numpad(uint maxLength, std::string Text);
	void drawKeyboard();
	std::string getLine(std::string Text);
	std::string getLine(uint maxLength, std::string Text);
}

#endif
