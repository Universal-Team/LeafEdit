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

#include <citro3d.h>
#include <citro2d.h>
#include <3ds.h>
#include <algorithm>
#include <dirent.h>
#include <malloc.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "gui/gui.hpp"
#include "common/structs.hpp"
#include "gui/screens/screenCommon.hpp"
#include "gui/keyboard.hpp"
#include "lang/lang.h"
#include "common/settings.hpp"
#include "gui/screens/titleSelection.hpp"
#include "core/gameLoader.hpp"
#include "title.hpp"

// The classic Fade Effect! ;P
int fadealpha = 255;
bool fadein = true;

// Set to 1, if testing some stuff. Leave to 0, if normal use.
int test = 0;

// If true -> Exit LeafEdit.
bool exiting = false;

// Touch Touch!
touchPosition touch;

// Update Data bools.

extern bool updateEURFound;
extern bool updateUSAFound;
extern bool updateJPNFound;

// If button Position pressed -> Do something.
bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}

void TestStuff(void)
{
	if (test == 1) {
		// Currently nothing to test.
	} else if (test == 0) {
	}
}

int main()
{
	// Initialize Everything.
	romfsInit();
    gfxInitDefault();
	Gui::init();
	acInit();
	amInit();
	sdmcInit();
	cfguInit();
	Config::loadConfig();
	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users

	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/LeafEdit", 0777); // main Path.
	mkdir("sdmc:/LeafEdit/Towns", 0777); // Town Management Path.
	mkdir("sdmc:/LeafEdit/Towns/Old", 0777); // Old Path.
	mkdir("sdmc:/LeafEdit/Towns/Welcome-Amiibo", 0777); // Welcome Amiibo Path.

	// If Language isn't set -> Set the Language.
	if (Config::langSet == 0) {
		Lang::loadLangStrings(Config::lang);
		Config::setLanguage();
		Config::langSet = 1;
		Config::saveConfig();
	} else if (Config::langSet == 1) {
	}

	// Load The Strings from the Romfs.
	Lang::loadLangStrings(Config::lang);

	TestStuff();

	// Scan for available Titles to display.
	GameLoader::scanTitleID();

	// Set all the Titles currently to true.
	updateEURFound = true;
	updateUSAFound = true;
	updateJPNFound = true;
	
	// Set the Screen to the MainMenu.
	Gui::setScreen(std::make_unique<TitleSelection>());
	
	// Loop as long as the status is not exit
    while (aptMainLoop() && !exiting)
    {
        hidScanInput();
        u32 hHeld = hidKeysHeld();
        u32 hDown = hidKeysDown();
		hidTouchRead(&touch);
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, BLACK);
        C2D_TargetClear(bottom, BLACK);
		Gui::clearTextBufs();
		Gui::mainLoop(hDown, hHeld, touch);
		C3D_FrameEnd(0);

		if (fadein == true) {
			fadealpha -= 3;
			if (fadealpha < 0) {
				fadealpha = 0;
				fadein = false;
			}
		}
	}
	// Exit every process.
	cfguExit();
	sdmcExit();
	acExit();
	amExit();
	Gui::exit();
	gfxExit();
	romfsExit();

    return 0;
}