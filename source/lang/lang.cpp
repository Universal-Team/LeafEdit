#include "common/config.hpp"
#include "common/inifile.h"

#include "lang/lang.h"
#include "lang/langStrings.h"

#include <fstream>



std::string langs[] = {"de", "en", "es", "fr", "it", "jp", "lt", "pt"};

void Lang::loadLangStrings(int lang) {
	// Declare variables
	char path[32];
	std::string line;

	// Load app strings
	if (Config::LangLocation == 0) {
		snprintf(path, sizeof(path), "romfs:/lang/%s/app.ini", langs[lang].c_str());
	} else if (Config::LangLocation == 1) {
		snprintf(path, sizeof(path), "sdmc:/LeafEdit/lang/%s/app.ini", langs[lang].c_str());
	}

	CIniFile ini(path);

	// [editor]
	Lang::editor[0] = ini.GetString("editor", "saveChanges", Lang::editor[0]);
	Lang::editor[1] = ini.GetString("editor", "player", Lang::editor[1]);
	Lang::editor[2] = ini.GetString("editor", "editor", Lang::editor[2]);
	Lang::editor[3] = ini.GetString("editor", "saveselect", Lang::editor[3]);
	Lang::editor[4] = ini.GetString("editor", "saveload", Lang::editor[4]);
	Lang::editor[5] = ini.GetString("editor", "returnmm", Lang::editor[5]);
	Lang::editor[6] = ini.GetString("editor", "villager", Lang::editor[6]);

	// [keyboard]
	Lang::typeName = ini.GetString("keyboard", "typeName", Lang::typeName);
	Lang::keyboard[0] = ini.GetString("keyboard", "walletAmount", Lang::keyboard[0]);
	Lang::keyboard[1] = ini.GetString("keyboard", "tan", Lang::keyboard[1]);
	Lang::keyboard[2]= ini.GetString("keyboard", "bankAmount", Lang::keyboard[2]);
	Lang::keyboard[3] = ini.GetString("keyboard", "MedalsAmount", Lang::keyboard[3]);
	Lang::keyboard[4] = ini.GetString("keyboard", "CouponsAmount", Lang::keyboard[4]);

	// [language]
	Lang::language = ini.GetString("language", "ccl", Lang::language);

	// [mainmenu]
	Lang::mainMenu[0] = ini.GetString("mainmenu", "townmanager", Lang::mainMenu[0]);
	Lang::mainMenu[1] = ini.GetString("mainmenu", "editor", Lang::mainMenu[1]);
	Lang::mainMenu[2] = ini.GetString("mainmenu", "settings", Lang::mainMenu[2]);

	// [messages]
	Lang::messages[0] = ini.GetString("messages", "1", Lang::messages[0]);
	Lang::messages[1] = ini.GetString("messages", "2", Lang::messages[1]);
	Lang::messages[2] = ini.GetString("messages", "3", Lang::messages[2]);
	Lang::messages[3] = ini.GetString("messages", "4", Lang::messages[3]);
	Lang::messages[4] = ini.GetString("messages", "5", Lang::messages[4]);
	Lang::messages[5] = ini.GetString("messages", "6", Lang::messages[5]);
	Lang::messages[6] = ini.GetString("messages", "7", Lang::messages[6]);
	Lang::messages[7] = ini.GetString("messages", "8", Lang::messages[7]);

	// [messages2]
	Lang::messages2[0] = ini.GetString("messages2", "continue", Lang::messages2[0]);
	Lang::messages2[1] = ini.GetString("messages2", "confirm", Lang::messages2[1]);
	Lang::messages2[2] = ini.GetString("messages2", "backup", Lang::messages2[2]);
	Lang::messages2[3] = ini.GetString("messages2", "restore", Lang::messages2[3]);
	Lang::messages2[4] = ini.GetString("messages2", "launch", Lang::messages2[4]);
	Lang::messages2[5] = ini.GetString("messages2", "normalLaunch", Lang::messages2[5]);
	Lang::messages2[6] = ini.GetString("messages2", "updateCheck", Lang::messages2[6]);
	Lang::messages2[7] = ini.GetString("messages2", "backupMsg", Lang::messages2[7]);
	Lang::messages2[8] = ini.GetString("messages2", "restoreMsg", Lang::messages2[8]);
	Lang::messages2[9] = ini.GetString("messages2", "refresh", Lang::messages2[9]);
	Lang::messages2[10] = ini.GetString("messages2", "backupLaunch", Lang::messages2[10]);
	Lang::messages2[11] = ini.GetString("messages2", "overwrite", Lang::messages2[11]);

	// [misc]
	Lang::notImplemented = ini.GetString("misc", "notImplemented", Lang::notImplemented);
	Lang::misc[0] = ini.GetString("misc", "currentPage", Lang::misc[0]);
	Lang::misc[1] = ini.GetString("misc", "currentPlayer", Lang::misc[1]);

	// [playerEditor]
	Lang::playerEditor[0] = ini.GetString("playerEditor", "playername", Lang::playerEditor[0]);
	Lang::playerEditor[1] = ini.GetString("playerEditor", "pName", Lang::playerEditor[1]);
	Lang::playerEditor[2] = ini.GetString("playerEditor", "pWallet", Lang::playerEditor[2]);
	Lang::playerEditor[3] = ini.GetString("playerEditor", "pTan", Lang::playerEditor[3]);
	Lang::playerEditor[4] = ini.GetString("playerEditor", "pBank", Lang::playerEditor[4]);
	Lang::playerEditor[5] = ini.GetString("playerEditor", "pMedals", Lang::playerEditor[5]);
	Lang::playerEditor[6] = ini.GetString("playerEditor", "pCoupons", Lang::playerEditor[6]);
	Lang::playerEditor[7] = ini.GetString("playerEditor", "maxBank", Lang::playerEditor[7]);
	Lang::playerEditor[8] = ini.GetString("playerEditor", "maxMedals", Lang::playerEditor[8]);
	Lang::playerEditor[9] = ini.GetString("playerEditor", "maxCoupons", Lang::playerEditor[9]);
	Lang::playerEditor[10] = ini.GetString("playerEditor", "clearBank", Lang::playerEditor[10]);
	Lang::playerEditor[11] = ini.GetString("playerEditor", "clearMedals", Lang::playerEditor[11]);
	Lang::playerEditor[12] = ini.GetString("playerEditor", "clearCoupons", Lang::playerEditor[12]);
	
	// [settings]
	Lang::settings[0] = ini.GetString("settings", "language", Lang::settings[0]);
	Lang::settings[1] = ini.GetString("settings", "selector", Lang::settings[1]);
	Lang::settings[2] = ini.GetString("settings", "?", Lang::settings[2]); // Unknown what to add.


	// [title]
	Lang::title = ini.GetString("title", "title", Lang::title);

	// [titleSelector]
	Lang::titleSelector[0] = ini.GetString("titleSelector", "title", Lang::titleSelector[0]);
	Lang::titleSelector[1] = ini.GetString("titleSelector", "card", Lang::titleSelector[1]);
	Lang::titleSelector[2] = ini.GetString("titleSelector", "installed", Lang::titleSelector[2]);

	// [townmanager]
	Lang::townmanager[0] = ini.GetString("townmanager", "launch", Lang::townmanager[0]);
	Lang::townmanager[1] = ini.GetString("townmanager", "backup", Lang::townmanager[1]);
	Lang::townmanager[2] = ini.GetString("townmanager", "restore", Lang::townmanager[2]);
	Lang::townmanager[3] = ini.GetString("townmanager", "select", Lang::townmanager[3]);
	Lang::townmanager[4] = ini.GetString("townmanager", "select2", Lang::townmanager[4]);

	// [update]
	Lang::update[0] = ini.GetString("update", "found", Lang::update[0]);
	Lang::update[1] = ini.GetString("update", "notFound", Lang::update[1]);
}