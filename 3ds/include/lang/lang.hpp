#ifndef LANG_HPP
#define LANG_HPP

#include "json.hpp"

#include <string>

namespace Lang {
	std::string get(const std::string &key);
	// Load in game strings.
	void loadNL(int lang);
	void loadWW(int lang);
	void load(int lang);
}

#endif