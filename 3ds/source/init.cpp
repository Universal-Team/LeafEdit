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

#include "download.hpp"
#include "init.hpp"
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

touchPosition touch;

u32 DARKER_COLOR, LIGHT_COLOR, LIGHTER_COLOR, SELECTED_COLOR, UNSELECTED_COLOR; // Color Types.

// Include all spritesheet's.
C2D_SpriteSheet Acres;
C2D_SpriteSheet Badges;
C2D_SpriteSheet Faces;
C2D_SpriteSheet GUI;
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
bool NLSheetHasLoaded	= false;
bool WWSheetHasLoaded	= false;
bool FontHasLoaded		= false;
bool changesMade		= false;

// GodMode and whatnot.
bool Debug = true;
bool GodMode = true;

bool is3dsx;
bool Is3dsxUpdated = false;
std::string path3dsx;

// Return if current use is CIA or 3DSX.
void getCurrentUsage(){
	u64 id;
	APT_GetProgramID(&id);

	if(id == 0x0004000004392100){
		is3dsx = false;
		return;
	}

	is3dsx = true;
}

// If button Position pressed -> Do something.
bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}

// Check if Sheets are found.
Result Init::CheckSheets(int Mode) {
	// 0 -> NL.
	if (Mode == 0) {
		if((access("sdmc:/LeafEdit/assets/acres.t3x", F_OK) == 0 ) ||
		  (access("sdmc:/LeafEdit/assets/badges.t3x", F_OK) == 0 ) ||
		  (access("sdmc:/LeafEdit/assets/faces.t3x", F_OK) == 0 ) ||
		  (access("sdmc:/LeafEdit/assets/hairs.t3x", F_OK) == 0 ) ||
		  (access("sdmc:/LeafEdit/assets/items.t3x", F_OK) == 0 ) ||
		  (access("sdmc:/LeafEdit/assets/NPCs.t3x", F_OK) == 0 ) ||
		  (access("sdmc:/LeafEdit/assets/villagers.t3x", F_OK) == 0 ) ||
		  (access("sdmc:/LeafEdit/assets/villagers2.t3x", F_OK) == 0 )) {
				return 0;
			} else {
				return -1;
			}
	} else {
		if((access("sdmc:/LeafEdit/assets/wwacres.t3x", F_OK) == 0 ) ||
		  (access("sdmc:/LeafEdit/assets/wwfaces.t3x", F_OK) == 0 ) ||
		  (access("sdmc:/LeafEdit/assets/wwvillagers.t3x", F_OK) == 0 )) {
			  return 0;
		} else {
			  return -1;
		}
	}
}

// Loading the Sheets.
Result Init::loadNLSheets() {
	if (NLSheetHasLoaded == false) {
		if (CheckSheets(0) != 0) {
			Msg::DisplayWarnMsg(Lang::get("SPRITESHEETS_NOT_FOUND"));
			return -1;
		} else {
			Acres		= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/acres.t3x");
			Badges		= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/badges.t3x");
			Faces		= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/faces.t3x");
			Hairs		= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/hairs.t3x");
			Items		= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/items.t3x");
			NPCs		= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/NPCs.t3x");
			Villager	= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/villagers.t3x");
			Villager2	= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/villagers2.t3x");
		}
	}
	NLSheetHasLoaded = true;
	return 0;
}

Result Init::loadWWSheets() {
	if (WWSheetHasLoaded == false) {
		if (CheckSheets(1) != 0) {
			Msg::DisplayWarnMsg(Lang::get("SPRITESHEETS_NOT_FOUND"));
			return -1;
		} else {
			WWAcres	= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/wwacres.t3x");
			WWFaces		= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/wwfaces.t3x");
			WWVillagers	= C2D_SpriteSheetLoad("sdmc:/LeafEdit/assets/wwvillagers.t3x");
		}
	}
	WWSheetHasLoaded = true;
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

Result Init::loadFont() {
	if (FontHasLoaded == false) {
		if(access("sdmc:/LeafEdit/assets/font.bcfnt", F_OK) != 0 ) {
			Msg::DisplayWarnMsg(Lang::get("FONT_NOT_FOUND"));
			return -1;
		} else {
			Gui::loadFont(false, "sdmc:/LeafEdit/assets/font.bcfnt");
		}
	}
	FontHasLoaded = true;
	return 0;
}

Result Init::unloadFont() {
	if (FontHasLoaded == true) {
		Gui::unloadFont();
	}
	FontHasLoaded = false;
	return 0;
}

Result Init::Init() {
	gfxInitDefault();
	romfsInit();
	Gui::init();

	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/LeafEdit", 0777); // main Path.
	mkdir("sdmc:/LeafEdit/assets", 0777); // Assets path.
	mkdir("sdmc:/LeafEdit/Towns", 0777); // Town Management Path.
	mkdir("sdmc:/LeafEdit/Towns/Old", 0777); // Old Path.
	mkdir("sdmc:/LeafEdit/Towns/Welcome-Amiibo", 0777); // Welcome Amiibo Path.
	mkdir("sdmc:/LeafEdit/Towns/Welcome-Luxury", 0777); // Welcome Luxury Path.
	mkdir("sdmc:/LeafEdit/Towns/Wild-World", 0777); // Wild World Path.
	mkdir("sdmc:/LeafEdit/Backups", 0777); // Backup path.
	mkdir("sdmc:/LeafEdit/Scripts", 0777); // Scripts path.

	Gui::loadSheet("romfs:/gfx/sprites.t3x", sprites);
	Gui::loadSheet("romfs:/gfx/gui.t3x", GUI);
	cfguInit();
	Lang::load(1);
	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users.

	DARKER_COLOR = DARKER_GREEN;
	LIGHT_COLOR = LIGHT_GREEN;
	LIGHTER_COLOR = LIGHTER_GREEN;
	SELECTED_COLOR = SELECTED_GREEN;
	UNSELECTED_COLOR = UNSELECTED_GREEN;

	// If sheets not found -> Download it.
	if (CheckSheets(0) != 0 || CheckSheets(1) != 0) {
		Download::downloadAssets();
	}

	// Only Load Font if found, else load System font.
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

// Screen set & Init part.
Result Init::Initialize() {
	Init(); // Init base stuff.
	// Set the Screen to the MainMenu.
	Gui::setScreen(std::make_unique<MainMenu>());
	return 0;
}

Result Init::MainLoop() {
	// Initialize everything.
	Initialize();

	// Loop as long as the status is not exiting.
	while (aptMainLoop() && !exiting && !Is3dsxUpdated)
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
	Gui::exit();
	unloadFont();
	Gui::unloadSheet(sprites);
	Gui::unloadSheet(GUI);
	cfguExit();
	gfxExit();
	romfsExit();
	return 0;
}