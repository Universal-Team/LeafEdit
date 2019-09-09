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

#include "logging.hpp"

#include "common/archive.hpp"
#include "common/config.hpp"
#include "common/structs.hpp"
#include "common/title.hpp"
#include "common/utils.hpp"

#include "core/gameLoader.hpp"

#include "gui/gui.hpp"

#include "gui/screens/screenCommon.hpp"
#include "gui/screens/titleSelection.hpp"

#include "lang/lang.h"

#include <3ds.h>
#include <dirent.h>

// The classic Fade Effect! ;P
int fadealpha = 255;
bool fadein = true;

// Set to 1, if testing some stuff. Leave to 0, if normal use.
int test = 0;

// If true -> Exit LeafEdit.
bool exiting = false;

bool WelcomeAmiibo;

// Touch Touch!
touchPosition touch;


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


// If an Error while startup appears, Return this!

static Result DisplayStartupError(const std::string& message, Result res)
{
    consoleInit(GFX_TOP, nullptr);
    printf("\x1b[2;16H\x1b[34mLeafEdit");
    printf("\x1b[5;1HError during startup: \x1b[31m0x%08lX\x1b[0m", res);
    printf("\x1b[8;1HDescription: \x1b[33m%s\x1b[0m", message.c_str());
    printf("\x1b[29;16HPress START to exit.");

	// For the Log.
	std::string error = message;
	error += ", ";
	error += std::to_string(res);
	Logging::writeToLog(error.c_str());

    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
    while (aptMainLoop() && !(hidKeysDown() & KEY_START))
    {
        hidScanInput();
    }
    return res;
}


int main()
{
	// Initialize Everything and check for errors.
	Result res;
    gfxInitDefault();

	if (R_FAILED(res = Archive::init())) {
		return DisplayStartupError("Archive::init failed.", res);
	}

	Config::loadConfig();

	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/LeafEdit", 0777); // main Path.
	mkdir("sdmc:/LeafEdit/Towns", 0777); // Town Management Path.
	mkdir("sdmc:/LeafEdit/Towns/Old", 0777); // Old Path.
	mkdir("sdmc:/LeafEdit/Towns/Welcome-Amiibo", 0777); // Welcome Amiibo Path.

	Logging::createLogFile(); // Create Log File, if it doesn't exists already.

	if (R_FAILED(res = romfsInit())) {
		return DisplayStartupError("romfsInit failed.", res);
	}

	if (R_FAILED(res = acInit())) {
		return DisplayStartupError("acInit failed.", res);
	}

	if (R_FAILED(res = amInit())) {
		return DisplayStartupError("amInit failed.", res);
	}

	if (R_FAILED(res = sdmcInit())) {
		return DisplayStartupError("sdmcInit failed.", res);
	}

	if (R_FAILED(res = cfguInit())) {
		return DisplayStartupError("cfguInit failed.", res);
	}

	if (R_FAILED(res = Gui::init())) {
		return DisplayStartupError("Gui::init failed.", res);
	}

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users

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
	LoadVillagerDatabase(Config::lang);

	TestStuff();

	GameLoader::checkUpdate();

	// Return the Welcome Amiibo State.
	if (Config::update == 0) {
		WelcomeAmiibo = false;
	} else if (Config::update == 1) {
		WelcomeAmiibo = true;
	}
	
	// Scan for available Titles to display.
	GameLoader::scanTitleID();
	
	// Set the Screen to the MainMenu.
	Gui::setScreen(std::make_unique<TitleSelection>());
	
	// We write a successfull Message, because it launched Successfully. Lol.
	Logging::writeToLog("LeafEdit launched successfully!");

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
	Archive::exit();

    return 0;
}