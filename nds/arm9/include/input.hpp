#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP
#include <string>

namespace Input {
	std::string getLine(std::string Text);
	std::string getLine(std::string Text, unsigned maxLength);

	int getInt(std::string Text);
	int getInt(std::string Text, unsigned max);

	bool getBool();
	bool getBool(std::string option1, std::string option2);

	void prompt(std::string message, const std::string &confirm);
}

#endif
