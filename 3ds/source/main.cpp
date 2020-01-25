	/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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
#include "common/utils.hpp"

#include "core/gameLoader.hpp"

#include "gui/gui.hpp"
#include "gui/msg.hpp"

#include "gui/screens/screenCommon.hpp"
#include "gui/screens/titleSelection.hpp"

#include "lang/lang.hpp"

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

u32 DARKER_COLOR, LIGHT_COLOR, LIGHTER_COLOR, SELECTED_COLOR, UNSELECTED_COLOR; // C010r Typ3s.

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
	}
}

int main()
{
	gfxInitDefault();
	romfsInit();
	Gui::init();
	Archive::init();
	cfguInit();
	if(access("sdmc:/LeafEdit/Settings.json", F_OK) == -1 ) {
		Config::initializeNewConfig();
	}
	Config::load();
	Lang::load(Config::getLang("Lang"));
	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/LeafEdit", 0777); // main Path.
	mkdir("sdmc:/LeafEdit/Towns", 0777); // Town Management Path.
	mkdir("sdmc:/LeafEdit/Towns/Old", 0777); // Old Path.
	mkdir("sdmc:/LeafEdit/Towns/Welcome-Amiibo", 0777); // Welcome Amiibo Path.
	mkdir("sdmc:/LeafEdit/Towns/Welcome-Luxury", 0777); // Welcome Luxury Path.
	mkdir("sdmc:/LeafEdit/Towns/Wild-World", 0777); // Wild World Path.
	mkdir("sdmc:/LeafEdit/Backups", 0777); // Backup path.
	mkdir("sdmc:/LeafEdit/scripts", 0777); // Scripts path.
	Logging::createLogFile(); // Create Log File, if it doesn't exists already.
	amInit();
	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users

	if (Config::colorMode == 0) {
		DARKER_COLOR = DARKER_GREEN;
		LIGHT_COLOR = LIGHT_GREEN;
		LIGHTER_COLOR = LIGHTER_GREEN;
		SELECTED_COLOR = SELECTED_GREEN;
		UNSELECTED_COLOR = UNSELECTED_GREEN;
	} else if (Config::colorMode == 1) {
		DARKER_COLOR = DARKER_BLUE;
		LIGHT_COLOR = LIGHT_BLUE;
		LIGHTER_COLOR = LIGHTER_BLUE;
		SELECTED_COLOR = SELECTED_BLUE;
		UNSELECTED_COLOR = UNSELECTED_BLUE;
	} else if (Config::colorMode == 2) {
		DARKER_COLOR = DARKER_DEEPBLUE;
		LIGHT_COLOR = LIGHT_DEEPBLUE;
		LIGHTER_COLOR = LIGHTER_DEEPBLUE;
		SELECTED_COLOR = SELECTED_DEEPBLUE;
		UNSELECTED_COLOR = UNSELECTED_DEEPBLUE;
	} else {
		DARKER_COLOR = DARKER_DEEPRED;
		LIGHT_COLOR = LIGHT_DEEPRED;
		LIGHTER_COLOR = LIGHTER_DEEPRED;
		SELECTED_COLOR = SELECTED_DEEPRED;
		UNSELECTED_COLOR = UNSELECTED_DEEPRED;
	}


	TestStuff();

	GameLoader::checkUpdate();

	// Return the Welcome Amiibo State.
	if (Config::getBool("update") == false) {
		WelcomeAmiibo = false;
	} else if (Config::getBool("update") == true) {
		WelcomeAmiibo = true;
	}
	
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
	 // Unload all sheets, because you don't know, if people exit properly like they should.
	Gui::unloadSheets();
	Gui::unloadACWWSheets();
	Config::save();
	amExit();
	Archive::exit();
	Gui::exit();
	cfguExit();
	gfxExit();
	romfsExit();
	return 0;
}