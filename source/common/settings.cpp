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

#include "common/settings.hpp"
#include "common/inifile.h"
#include "gui/keyboard.hpp"
#include "gui/gui.hpp"
#include "lang/langStrings.h"
#include <unistd.h>
#include <string>
using std::string;
using std::wstring;

#include <3ds.h>

static CIniFile settingsini( "sdmc:/LeafEdit/Settings.ini" );

int Config::lang; // Current Language.
int Config::langSet; // Tell, if the current Language was already set.

void Config::loadConfig() {
	// [UI]
	Config::lang = settingsini.GetInt("UI", "LANGUAGE", 1);
	Config::langSet = settingsini.GetInt("UI", "LANGSET", 0);
}

void Config::saveConfig() {
	// [UI]
	settingsini.SetInt("UI", "LANGUAGE", Config::lang);
	settingsini.SetInt("UI", "LANGSET", Config::langSet);

	settingsini.SaveIniFile("sdmc:/LeafEdit/Settings.ini");
}

void Config::setLanguage() {
	std::string LangSetText;
	LangSetText += Lang::languages[6];;
	LangSetText += "\n\n00/0 : ";

	LangSetText += "Deutsch";
	LangSetText += "\n01/1 : ";

	LangSetText += "English";
	LangSetText += "\n02/2 : ";

	LangSetText += "Español";
	LangSetText += "\n03/3 : ";

	LangSetText += "Français";
	LangSetText += "\n04/4 : ";

	LangSetText += "Italiano";
	LangSetText += "\n05/5 : ";

	LangSetText += "日本の";

	std::string currentLanguage = Input::getLine(LangSetText.c_str());
	if (currentLanguage == "0" || currentLanguage == "00") {
		Config::lang = 0;
	} else if (currentLanguage == "1" || currentLanguage == "01") {
		Config::lang = 1;
	} else if (currentLanguage == "2" || currentLanguage == "02") {
		Config::lang = 2;
	} else if (currentLanguage == "3" || currentLanguage == "03") {
		Config::lang = 3;
	} else if (currentLanguage == "4" || currentLanguage == "04") {
		Config::lang = 4;
	} else if (currentLanguage == "5" || currentLanguage == "05") {
		Config::lang = 5;
	} else {
		Gui::DisplayWarnMsg(Lang::languages[7]);
		Config::setLanguage();
	}
}