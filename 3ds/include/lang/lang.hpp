#ifndef _LEAFEDIT_LANG_HPP
#define _LEAFEDIT_LANG_HPP

#include "json.hpp"
#include "types.hpp"
#include <string>

namespace Lang {
	std::string get(const std::string &key);
	/* Load in game strings. */
	void loadGameStrings(int lang, SaveType save);
	void load(int lang);
}

#endif