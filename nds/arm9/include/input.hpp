#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <string>

namespace Input {
	std::string getLine2(std::string Text, bool showPointer = true);
	std::string getLine(std::string Text, unsigned maxLength, bool showPointer = true);

	int getInt2(std::string Text, bool showPointer = true);
	int getInt(std::string Text, unsigned max, bool showPointer = true);

	bool getBool2(bool showPointer = true);
	bool getBool(std::string option1, std::string option2, bool showPointer = true);

	void prompt(std::string message, const std::string &confirm, bool showPointer = true);
}

#endif