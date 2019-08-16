#include "lang/lang.h"
#include <fstream>

#include "common/inifile.h"
#include "lang/langStrings.h"

std::string langs[] = { "de", "en", "es", "fr", "it", "jp"};

void Lang::loadLangStrings(int lang) {
	// Declare variables
	char path[32];
	std::string line;

	// Load app strings
	snprintf(path, sizeof(path), "romfs:/lang/%s/app.ini", langs[lang].c_str());
	CIniFile ini(path);

	// [keyboard]
	Lang::typeName = ini.GetString("keyboard", "typeName", Lang::typeName);

	// [language]
	Lang::language = ini.GetString("language", "ccl", Lang::language);

	// [scan]
	Lang::scan[0] = ini.GetString("scan", "TF", Lang::scan[0]);
	Lang::scan[1] = ini.GetString("scan", "TNF", Lang::scan[1]);
	Lang::scan[2] = ini.GetString("scan", "IDM", Lang::scan[2]);
	Lang::scan[3] = ini.GetString("scan", "IDDM", Lang::scan[3]);
	Lang::scan[4] = ini.GetString("scan", "NCI", Lang::scan[4]);

	// [title]
	Lang::title[0] = ini.GetString("title", "1", Lang::title[0]);
	Lang::title[1] = ini.GetString("title", "2", Lang::title[1]);
}