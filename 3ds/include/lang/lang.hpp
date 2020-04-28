#ifndef LANG_HPP
#define LANG_HPP

#include "json.hpp"
#include "types.hpp"
#include <string>

namespace Lang {
	std::string get(const std::string &key);
	// Load in game strings.
	void loadLang(int lang, SaveType save);
	void load(int lang);
}

#endif