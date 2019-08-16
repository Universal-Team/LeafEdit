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
#include "screenCommon.hpp"
#include "structs.hpp"
#include <unistd.h>
#include <string>
using std::string;
using std::wstring;

#include <3ds.h>

Structs::ButtonPos languagePos [] = {
	{20, 30, 120, 52, -1}, // Deutsch
	{180, 30, 120, 52, -1}, // English
	{20, 100, 120, 52, -1}, // Español

	{180, 100, 120, 52, -1}, // Français
	{20, 170, 120, 52, -1}, // Italiano
	{180, 170, 120, 52, -1}, // 日本語
};

extern bool touching(touchPosition touch, Structs::ButtonPos button);

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

std::vector<std::string> Language = {
	"Deutsch",
	"English",
	"Español",
	"Français",
	"Italiano",
	"日本語"
};


void Config::setLanguage() {
	touchPosition touch;
    Gui::clearTextBufs();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLACK);
    C2D_TargetClear(bottom, BLACK);
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
	Gui::DrawString((400-Gui::Draw_GetStringWidth(0.8f, Lang::language))/2, 2, 0.8f, WHITE, Lang::language);
	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 25, GREEN);
	Gui::Draw_Rect(0, 25, 320, 190, DARKGRAY);
	Gui::Draw_Rect(0, 215, 320, 25, GREEN);

	for(int i=0;i<3;i++) {
		Gui::Draw_Rect(20, 29+(i*65), languagePos[i].w, languagePos[i].h, WHITE);
		Gui::Draw_Rect(180, 29+(i*65), languagePos[i].w, languagePos[i].h, WHITE);

		Gui::DrawString(((320-Gui::Draw_GetStringWidth(0.65f, Language[(2*i)].c_str()))/2)-60-20, 46+(i*65), 0.65f, BLACK, Language[(2*i)].c_str());
		Gui::DrawString(((320-Gui::Draw_GetStringWidth(0.65f, Language[(2*i)+1].c_str()))/2)+60+20, 46+(i*65), 0.65f, BLACK, Language[(2*i)+1].c_str());
	}

	C3D_FrameEnd(0);
	while(1)
    {
		hidScanInput();
		touchRead(&touch);
		if(hidKeysDown() & KEY_TOUCH) {
			if (touching(touch, languagePos[0])) {
				Config::lang = 0;
				break;
			} else if (touching(touch, languagePos[1])) {
				Config::lang = 1;
				break;
			} else if (touching(touch, languagePos[2])) {
				Config::lang = 2;
				break;
			} else if (touching(touch, languagePos[3])) {
				Config::lang = 3;
				break;
			} else if (touching(touch, languagePos[4])) {
				Config::lang = 4;
				break;
			} else if (touching(touch, languagePos[5])) {
				Config::lang = 5;
				break;
			}
		}
    }
}