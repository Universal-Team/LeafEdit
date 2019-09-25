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

extern C2D_SpriteSheet sprites;

Structs::ButtonPos SelectorPos [] = {
		{90, 40, 140, 35, -1}, // Selector 1.
		{90, 100, 140, 35, -1}, // Selector 2.
		{90, 160, 140, 35, -1}, // Selector 3.
};

extern bool touching(touchPosition touch, Structs::ButtonPos button);

static CIniFile settingsini( "sdmc:/LeafEdit/Settings.ini" );

int Config::check; // Update Checked.
int Config::update; // Update Available? ;P

int Config::Logging; // Enable / Disable writing the log.

int Config::LangLocation; // Language Location (Romfs/SD).
int Config::lang; // Current Language.
int Config::selector; // Selector Design for the FileBrowse.
std::string Config::sheet; // Spritesheet Path.

void Config::loadSheet() {
	Config::sheet = settingsini.GetString("MISC", "SheetPath", Config::sheet);
	std::string sheetFile = Config::sheet;

	if((access(sheetFile.c_str(), F_OK) == 0)) {
		sprites    = C2D_SpriteSheetLoad(sheetFile.c_str());
	} else {
		sprites    = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
		Config::sheet = "romfs:/gfx/sprites.t3x";
		settingsini.SaveIniFile("sdmc:/LeafEdit/Settings.ini");
	}
}

void Config::saveSheet(std::string sheetPath) {
	settingsini.SetString("MISC", "SheetPath", sheetPath.c_str());
	settingsini.SaveIniFile("sdmc:/LeafEdit/Settings.ini");
}

void Config::loadConfig() {

	// [CORE]
	Config::check = settingsini.GetInt("CORE", "CHECKED", 0);
	Config::update = settingsini.GetInt("CORE", "FOUND", 0);

	// [MISC]
	Config::Logging = settingsini.GetInt("MISC", "LOGGING", 0);
	Config::sheet = settingsini.GetString("MISC", "SheetPath", "romfs:/gfx/sprites.t3x");

	// [UI]
	Config::LangLocation = settingsini.GetInt("UI", "LANGLOCATION", 0);
	Config::lang = settingsini.GetInt("UI", "LANGUAGE", 1);
	Config::selector = settingsini.GetInt("UI", "SELECTOR", 1);
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
	settingsini.SetInt("UI", "SELECTOR", Config::selector);

	settingsini.SaveIniFile("sdmc:/LeafEdit/Settings.ini");
}


void Config::setSelector() {
	touchPosition touch;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	C2D_TargetClear(bottom, BLACK);
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, "Please Select the Selector."))/2, 2, 0.8f, WHITE, "Please Select the Selector.", 400);
	Gui::DrawBottom();

	Gui::sprite(0, sprites_button_idx, SelectorPos[0].x, SelectorPos[0].y);
	Gui::sprite(0, sprites_button_idx, SelectorPos[1].x, SelectorPos[1].y);
	Gui::sprite(0, sprites_button_idx, SelectorPos[2].x, SelectorPos[2].y);

	Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Selector 1"))/2, SelectorPos[0].y+10, 0.6f, WHITE, "Selector 1", 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Selector 2"))/2, SelectorPos[1].y+10, 0.6f, WHITE, "Selector 2", 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Selector 3"))/2, SelectorPos[2].y+10, 0.6f, WHITE, "Selector 3", 140);

	C3D_FrameEnd(0);
	while(1)
	{
		hidScanInput();
		touchRead(&touch);
		if(hidKeysDown() & KEY_TOUCH) {
			if (touching(touch, SelectorPos[0])) {
				Config::selector = 1;
				break;
			} else if (touching(touch, SelectorPos[1])) {
				Config::selector = 2;
				break;
			} else if (touching(touch, SelectorPos[2])) {
				Config::selector = 3;
				break;
			}
		}
	}
	Config::saveConfig();
}