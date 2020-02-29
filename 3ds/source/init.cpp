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

#include "archive.hpp"
#include "config.hpp"
#include "init.hpp"
#include "initial.hpp"
#include "gameLoader.hpp"
#include "gfx.hpp"
#include "gui.hpp"
#include "lang.hpp"
#include "mainMenu.hpp"
#include "screenCommon.hpp"

#include <3ds.h>
#include <dirent.h>
#include <unistd.h>

// The classic Fade Effect! ;P
int fadealpha = 255;
bool fadein = true;

// If true -> Exit LeafEdit.
bool exiting = false;

bool WelcomeAmiibo;
touchPosition touch;

u32 DARKER_COLOR, LIGHT_COLOR, LIGHTER_COLOR, SELECTED_COLOR, UNSELECTED_COLOR; // Color Types.

// Include all spritesheet's.
C2D_SpriteSheet Acres;
C2D_SpriteSheet Badges;
C2D_SpriteSheet Faces;
C2D_SpriteSheet Hairs;
C2D_SpriteSheet Items;
C2D_SpriteSheet NPCs;
C2D_SpriteSheet sprites;
C2D_SpriteSheet Villager;
C2D_SpriteSheet Villager2;
C2D_SpriteSheet WWAcres;
C2D_SpriteSheet WWFaces;
C2D_SpriteSheet WWVillagers;

// Is loaded state.
bool NLSheetHasLoaded = false;
bool WWSheetHasLoaded = false;

// If button Position pressed -> Do something.
bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}

// Loading the Sheets.
Result Init::loadNLSheets() {
	if (NLSheetHasLoaded == false) {
		NLSheetHasLoaded = true;
		Acres	    = C2D_SpriteSheetLoad("romfs:/gfx/acres.t3x");
		Badges  	= C2D_SpriteSheetLoad("romfs:/gfx/badges.t3x");
		Faces	    = C2D_SpriteSheetLoad("romfs:/gfx/faces.t3x");
		Hairs	    = C2D_SpriteSheetLoad("romfs:/gfx/hairs.t3x");
		Items	    = C2D_SpriteSheetLoad("romfs:/gfx/items.t3x");
		NPCs	    = C2D_SpriteSheetLoad("romfs:/gfx/NPCs.t3x");
		Villager	= C2D_SpriteSheetLoad("romfs:/gfx/villagers.t3x");
		Villager2	= C2D_SpriteSheetLoad("romfs:/gfx/villagers2.t3x");
	}
	return 0;
}

Result Init::loadWWSheets() {
	if (WWSheetHasLoaded == false) {
		WWSheetHasLoaded = true;
		WWAcres     = C2D_SpriteSheetLoad("romfs:/gfx/wwacres.t3x");
		WWFaces     = C2D_SpriteSheetLoad("romfs:/gfx/wwfaces.t3x");
		WWVillagers	= C2D_SpriteSheetLoad("romfs:/gfx/wwvillagers.t3x");
	}
	return 0;
}

// Unload all Sheets.
Result Init::unloadNLSheets() {
	if (NLSheetHasLoaded == true) {
		NLSheetHasLoaded = false;
		C2D_SpriteSheetFree(Acres);
		C2D_SpriteSheetFree(Badges);
		C2D_SpriteSheetFree(Faces);
		C2D_SpriteSheetFree(Hairs);
		C2D_SpriteSheetFree(Items);
		C2D_SpriteSheetFree(NPCs);
		C2D_SpriteSheetFree(Villager);
		C2D_SpriteSheetFree(Villager2);
	}
	return 0;
}

Result Init::unloadWWSheets() {
	if (WWSheetHasLoaded == true) {
		WWSheetHasLoaded = false;
		C2D_SpriteSheetFree(WWAcres);
		C2D_SpriteSheetFree(WWFaces);
		C2D_SpriteSheetFree(WWVillagers);
	}
	return 0;
}

Result Init::Initialize() {
	gfxInitDefault();
	romfsInit();
	amInit();
	Archive::init();
	Gui::init();
	Gui::loadFont(false, "romfs:/font.bcfnt");
	Gui::loadSheet("romfs:/gfx/sprites.t3x", sprites);
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
	mkdir("sdmc:/LeafEdit/TPC", 0777); // TPC path.
	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users.

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

	GameLoader::checkUpdate();
	// Return the Welcome Amiibo State.
	if (Config::getBool("update") == false) {
		WelcomeAmiibo = false;
	} else if (Config::getBool("update") == true) {
		WelcomeAmiibo = true;
	}
	
	// Set the Screen to the MainMenu.
	if (Config::getBool("InitialSetup") != true) {
		Gui::setScreen(std::make_unique<Initial>());
	} else {
		Gui::setScreen(std::make_unique<MainMenu>());
	}

	return 0;
}

Result Init::MainLoop() {
	// Initialize everything.
	Initialize();

	// Loop as long as the status is not exiting.
	while (aptMainLoop() && !exiting)
	{
		hidScanInput();
		u32 hHeld = hidKeysHeld();
		u32 hDown = hidKeysDown();
		hidTouchRead(&touch);
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, BLACK);
		C2D_TargetClear(Bottom, BLACK);
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
	// Exit all services and exit the app.
	Exit();
	return 0;
}

Result Init::Exit() {
	// Exit every process.
	// Unload all sheets, because you don't know, if people exit properly like they should.
	unloadNLSheets();
	unloadWWSheets();
	Config::save();
	Archive::exit();
	Gui::exit();
	Gui::unloadFont();
	Gui::unloadSheet(sprites);
	cfguExit();
	gfxExit();
	romfsExit();
	amExit();
	return 0;
}