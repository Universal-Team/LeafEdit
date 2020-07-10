	/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
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

#include <fat.h>

#include "config.hpp"
#include "fileBrowse.hpp"
#include "flashcard.hpp"
#include "itemUtils.hpp"
#include "lang.hpp"
#include "mainMenu.hpp"
#include "nitrofs.h"
#include "screenCommon.hpp"
#include "sound.hpp"
#include "structs.hpp"

touchPosition touch;
bool exiting = false;

// If button Position pressed -> Do something.
bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}

int main(int argc, char **argv) {
	initGraphics();
	keysSetRepeat(25,5);
	sysSetCardOwner(BUS_OWNER_ARM9); // Give ARM9 access to Slot-1 (for dumping/injecting saves)
	defaultExceptionHandler();
	//scanKeys(); // So it doesn't open the SD if A is held

	drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, true, false);
	drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, false, false);

	// Init filesystem
	if (!fatInitDefault()) {
		// Prints error if fatInitDefault() fails
		consoleDemoInit();
		printf("fatInitDefault() failed...");
		while(1) swiWaitForVBlank();
	}

	// Make directories
	mkdir(sdFound() ? "sd:/_nds" : "fat:/_nds", 0777);
	mkdir(sdFound() ? "sd:/_nds/LeafEdit" : "fat:/_nds/LeafEdit", 0777);
	mkdir(sdFound() ? "sd:/_nds/LeafEdit/backups" : "fat:/_nds/LeafEdit/backups", 0777);

	// Try to init NitroFS from argv provided to the game when it was launched
	if (!nitroFSInit(argv[0])) {
		// If that fails, try to init NitroFS on 'LeafEdit.nds'
		if (!nitroFSInit("LeafEdit.nds")) {
			if (!nitroFSInit("/_nds/LeafEdit/LeafEdit.nds")) {
				// Prints error if nitroFSInit() fails
				consoleDemoInit();
				printf("nitroFSInit() failed...\n\n");
				printf("Please copy LeafEdit.nds to:\n\n");
				printf("%s:/_nds/LeafEdit/\n", sdFound() ? (access("/Nintendo 3DS", F_OK) == 0 ? "sdmc" : "sd") : "fat");
				printf("                  LeafEdit.nds\n\n");
				printf("or launch LeafEdit using\n\n");
				printf("      TWiLight Menu++ or HBMenu\n\n\n\n\n");
				printf("(Note: TWiLight's Acekard\n");
				printf("        theme needs a copy in ^)\n\n");
				while(1) swiWaitForVBlank();
			}
		}
	}

	Config::load();
	Colors::load();
	ItemUtils::LoadDatabase(SaveType::WW);
	loadFont();
	Lang::load(1);
	printTextCentered(Lang::get("LOADING"), 0, 32, false, true);
	Log = std::make_unique<Logging>();

	Sound::init();
	Gui::initSprites();
	Gui::loadSprites();
	
	setSpriteVisibility(Gui::pointerID, false, true);
	updateOam();

	u16 hDown = 0;
	Gui::setScreen(std::make_unique<MainMenu>());
	Gui::clearScreen(false, true);
	// Draw Screen.
	Gui::DrawScreen();
	while(!exiting) {
		scanKeys();
		swiWaitForVBlank();
		hDown = keysDown();
		touchRead(&touch);
		Gui::mainLoop(hDown, touch);
	}
	
	return 0;
}