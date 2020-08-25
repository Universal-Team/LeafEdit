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

#include "config.hpp"
#include "download.hpp"
#include "init.hpp"
#include "gfx.hpp"
#include "gui.hpp"
#include "itemManager.hpp"
#include "lang.hpp"
#include "mainMenu.hpp"
#include "overlay.hpp"
#include "screenCommon.hpp"

#include <3ds.h>
#include <dirent.h>
#include <unistd.h>

int barOffset; // The additional offset for the text on the clean style.
std::unique_ptr<Config> config;
/* If true -> Exit LeafEdit. */
bool exiting = false;
bool doFade = true;

touchPosition touch;

u32 DARKER_COLOR, LIGHT_COLOR, LIGHTER_COLOR, SELECTED_COLOR, UNSELECTED_COLOR; // Color Types.

/* Include all spritesheet's. */
C2D_SpriteSheet Acres, GUI, Items, Players, Villager, Villager2;
C2D_Font font;

/* Is loaded state. */
bool sheetsLoaded		= false;
bool FontHasLoaded		= false;
bool changesMade		= false;

/* GodMode and whatnot. */
bool Debug = true;
bool GodMode = true;

bool is3dsx;
bool Is3dsxUpdated = false;
std::string path3dsx;

/* Return if current use is CIA or 3DSX. */
void getCurrentUsage(){
	u64 id;
	APT_GetProgramID(&id);

	if (id == 0x0004000004392100){
		is3dsx = false;
		return;
	}

	is3dsx = true;
}

/* If button Position pressed -> Do something. */
bool touching(touchPosition touch, ButtonType button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.xLength) && touch.py >= button.y && touch.py <= (button.y + button.yLength)) return true;
	else return false;
}

/* Icons are handled through Structs::ButtonPos. */
bool iconTouch(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h)) return true;
	else return false;
}

/* Check if Sheets are found. */
Result Init::CheckSheets() {
	if ((access("sdmc:/3ds/LeafEdit/assets/acres.t3x", F_OK) == 0 ) ||
	(access("sdmc:/3ds/LeafEdit/assets/items.t3x", F_OK) == 0 ) ||
	(access("sdmc:/3ds/LeafEdit/assets/players.t3x", F_OK) == 0 ) ||
	(access("sdmc:/3ds/LeafEdit/assets/villagers.t3x", F_OK) == 0 ) ||
	(access("sdmc:/3ds/LeafEdit/assets/villagers2.t3x", F_OK) == 0 )) {
		return 0;
	} else {
		return -1;
	}
}

/* Loading the Sheets. */
Result Init::loadSheets() {
	if (sheetsLoaded == false) {
		if (CheckSheets() != 0) {
			Msg::DisplayWarnMsg(Lang::get("SPRITESHEETS_NOT_FOUND"));
			return -1;
		} else {
			Acres			= C2D_SpriteSheetLoad("sdmc:/3ds/LeafEdit/assets/acres.t3x");
			Items			= C2D_SpriteSheetLoad("sdmc:/3ds/LeafEdit/assets/items.t3x");
			Players			= C2D_SpriteSheetLoad("sdmc:/3ds/LeafEdit/assets/players.t3x");
			Villager		= C2D_SpriteSheetLoad("sdmc:/3ds/LeafEdit/assets/villagers.t3x");
			Villager2		= C2D_SpriteSheetLoad("sdmc:/3ds/LeafEdit/assets/villagers2.t3x");
			sheetsLoaded	= true;
		}
	}

	return 0;
}

/* Unload all Sheets. */
Result Init::unloadSheets() {
	if (sheetsLoaded == true) {
		C2D_SpriteSheetFree(Acres);
		C2D_SpriteSheetFree(Items);
		C2D_SpriteSheetFree(Players);
		C2D_SpriteSheetFree(Villager);
		C2D_SpriteSheetFree(Villager2);
		sheetsLoaded = false;
	}

	return 0;
}

/* Load the font, if found. */
Result Init::loadFont() {
	if (FontHasLoaded == false) {
		if (access("sdmc:/3ds/LeafEdit/assets/font.bcfnt", F_OK) != 0 ) {
			Msg::DisplayWarnMsg(Lang::get("FONT_NOT_FOUND"));
			return -1;
		} else {
			Gui::loadFont(font, "sdmc:/3ds/LeafEdit/assets/font.bcfnt");
		}
	}

	FontHasLoaded = true;
	return 0;
}

/* Unload font, if loaded. */
Result Init::unloadFont() {
	if (FontHasLoaded) {
		Gui::unloadFont(font);
	}

	FontHasLoaded = false;
	return 0;
}

Result Init::Init() {
	gfxInitDefault();
	romfsInit();
	Gui::init();
	acInit();
	amInit();

	/* Create directories, if not existent. */
	mkdir("sdmc:/3ds", 0777); // For DSP dump
	mkdir("sdmc:/3ds/LeafEdit", 0777); // main Path.
	mkdir("sdmc:/3ds/LeafEdit/assets", 0777); // Assets path.
	/* Towns. */
	mkdir("sdmc:/3ds/LeafEdit/Towns", 0777); // Town Management Path.
	mkdir("sdmc:/3ds/LeafEdit/Towns/New-Leaf", 0777); // New Leaf Path.
	mkdir("sdmc:/3ds/LeafEdit/Towns/Welcome-Amiibo", 0777); // Welcome Amiibo Path.
	mkdir("sdmc:/3ds/LeafEdit/Towns/Welcome-Luxury", 0777); // Welcome Luxury Path.
	mkdir("sdmc:/3ds/LeafEdit/Towns/Wild-World", 0777); // Wild World Path.
	mkdir("sdmc:/3ds/LeafEdit/Backups", 0777); // Backup path.
	/* Pattern. */
	mkdir("sdmc:/3ds/LeafEdit/Pattern", 0777);
	/* Pattern Editor. */
	mkdir("sdmc:/3ds/LeafEdit/Pattern-Editor", 0777);

	mkdir("sdmc:/3ds/LeafEdit/scripts", 0777);
	Gui::loadSheet("romfs:/gfx/gui.t3x", GUI);
	cfguInit();

	/* Check for location changes. */
	if (access("sdmc:/LeafEdit/Settings.json", F_OK) == 0 && access("sdmc:/3ds/LeafEdit/Settings.json", F_OK) != 0) {
		Msg::DisplayWaitMsgInit(Lang::get("MAINPATH_CHANGE"));
	}


	config = std::make_unique<Config>();

	if (config->newStyle()) {
		barOffset = 0;
	} else {
		barOffset = 2;
	}
	
	ItemManager::loadColors();
	Lang::load(config->language());
	osSetSpeedupEnable(true); // Enable speed-up for New 3DS users.

	DARKER_COLOR = DARKER_GREEN;
	LIGHT_COLOR = LIGHT_GREEN;
	LIGHTER_COLOR = LIGHTER_GREEN;
	SELECTED_COLOR = SELECTED_GREEN;
	UNSELECTED_COLOR = UNSELECTED_GREEN;

	/* If sheets not found -> Download it. */
	if (CheckSheets() != 0) { Download::downloadAssets(); }

	/* Only Load Font if found, else load System font. */
	loadFont();

	getCurrentUsage();
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	if (is3dsx == true) {
			path3dsx = path;
			if (path3dsx == "sdmc:/") {
				path3dsx = path3dsx.substr(5, path3dsx.size());
			} else {
		}
	}

	return 0;
}

/* Screen set & Init part. */
Result Init::Initialize() {
	Init(); // Init base stuff.
	if (doFade) {
		fadein = true;
		fadealpha = 255;
	}

	Log = std::make_unique<Logging>();

	Overlays::SplashOverlay();
	
	/* Set the Screen to the MainMenu. */
	Gui::setScreen(std::make_unique<MainMenu>(), false, true);
	return 0;
}

Result Init::MainLoop() {
	/* Initialize everything. */
	Initialize();

	/* Loop as long as the status is not exiting. */
	while (aptMainLoop()) {
		hidScanInput();
		u32 hHeld = hidKeysHeld();
		u32 hDown = hidKeysDown();
		hidTouchRead(&touch);
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, BLACK);
		C2D_TargetClear(Bottom, BLACK);
		Gui::clearTextBufs();
		Gui::DrawScreen(true);
		Gui::ScreenLogic(hDown, hHeld, touch, doFade, true);
		C3D_FrameEnd(0);
		
		if (exiting || Is3dsxUpdated) {
			if (!fadeout)	break;
		}

		Gui::fadeEffects(16, 16, true);
	}

	/* Exit all services and exit the app. */
	Exit();
	return 0;
}

Result Init::Exit() {
	/*
		Exit every process.
		Unload all sheets, because you don't know, if people exit properly like they should.
	*/

	unloadSheets();
	acExit();
	amExit();
	Gui::exit();
	Init::unloadFont();
	config->save();
	Gui::unloadSheet(GUI);
	cfguExit();
	gfxExit();
	romfsExit();
	return 0;
}