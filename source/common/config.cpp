/*
*   This file is part of LeafEdit
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "common/inifile.h"
#include "common/config.hpp"
#include "common/structs.hpp"
#include "common/utils.hpp"

#include "core/villagerManagement.hpp"

#include "gui/gui.hpp"
#include "gui/keyboard.hpp"

#include "gui/screens/screenCommon.hpp"

#include "lang/lang.h"
#include "lang/langStrings.h"

#include <3ds.h>
#include <string>
#include <unistd.h>

using std::string;
using std::wstring;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

static CIniFile settingsini( "sdmc:/LeafEdit/Settings.ini" );
CIniFile sheetFileIni; // Sheet color informations.

int Config::check; // Update Checked.
int Config::update; // Update Available? ;P

int Config::Logging; // Enable / Disable writing the log.

int Config::LangLocation; // Language Location (Romfs/SD).
int Config::lang; // Current Language.
std::string Config::sheetIni; // Sheet Ini path.


// Config stuff for the Sprites.
int Config::barText;
int Config::buttonText;
int Config::bgText;
int Config::boxText;
int Config::fileBrowseText;
int Config::MessageText;
int Config::helpMsg;

int Config::Color1;
int Config::Color2;
int Config::Color3;


void Config::loadSheetIni() {
	Config::sheetIni = settingsini.GetString("MISC", "SheetIniPath", Config::sheetIni);
	std::string sheetIniFile = Config::sheetIni.c_str();
	std::string romfs = "romfs:/gfx/sheet.ini";

	if((access(sheetIniFile.c_str(), F_OK) == 0)) {
		sheetFileIni = sheetIniFile;
	} else {
		sheetFileIni = romfs;
		Config::sheetIni = romfs;
		settingsini.SaveIniFile("sdmc:/LeafEdit/Settings.ini");
	}
}

void Config::loadSheetIniStuff() {
	Config::barText = sheetFileIni.GetInt("COLORS", "BarText", Config::barText);
	Config::buttonText = sheetFileIni.GetInt("COLORS", "ButtonText", Config::buttonText);
	Config::bgText = sheetFileIni.GetInt("COLORS", "BGText", Config::bgText);
	Config::boxText = sheetFileIni.GetInt("COLORS", "BoxText", Config::boxText);
	Config::fileBrowseText = sheetFileIni.GetInt("COLORS", "FileBrowseText", Config::fileBrowseText);
	Config::MessageText = sheetFileIni.GetInt("COLORS", "MessageText", Config::MessageText);
	Config::helpMsg = sheetFileIni.GetInt("COLORS", "helpMsg", Config::helpMsg);
	// GUI Colors.
	Config::Color1 = sheetFileIni.GetInt("UI", "Color1", Config::Color1);
	Config::Color2 = sheetFileIni.GetInt("UI", "Color2", Config::Color2);
	Config::Color3 = sheetFileIni.GetInt("UI", "Color3", Config::Color3);
}

void Config::saveSheetIni(std::string sheetIniPath) {
	settingsini.SetString("MISC", "SheetIniPath", sheetIniPath.c_str());
	settingsini.SaveIniFile("sdmc:/LeafEdit/Settings.ini");
}


void Config::loadConfig() {
	// [CORE]
	Config::check = settingsini.GetInt("CORE", "CHECKED", 0);
	Config::update = settingsini.GetInt("CORE", "FOUND", 0);

	// [MISC]
	Config::Logging = settingsini.GetInt("MISC", "LOGGING", 0);
	Config::sheetIni = settingsini.GetString("MISC", "SheetIniPath", "romfs:/gfx/sheet.ini");

	// [UI]
	Config::LangLocation = settingsini.GetInt("UI", "LANGLOCATION", 0);
	Config::lang = settingsini.GetInt("UI", "LANGUAGE", 1);
}

void Config::saveConfig() {
	// [CORE]
	settingsini.SetInt("CORE", "CHECKED", Config::check);
	settingsini.SetInt("CORE", "FOUND", Config::update);

	// [MISC]
	settingsini.SetInt("MISC", "LOGGING", Config::Logging);

	// [UI]
	settingsini.SetInt("UI", "LANGLOCATION", Config::LangLocation);
	settingsini.SetInt("UI", "LANGUAGE", Config::lang);

	settingsini.SaveIniFile("sdmc:/LeafEdit/Settings.ini");
}