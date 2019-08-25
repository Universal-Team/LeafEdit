#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <string>

namespace Input {
	void drawKeyboard();
	std::string getLine(std::string Text);
	std::string getLine(uint maxLength, std::string Text);
}

#endif
