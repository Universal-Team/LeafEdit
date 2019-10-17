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
#include "common/sound.h"
#include "common/structs.hpp"
#include "common/title.hpp"
#include "common/utils.hpp"

#include "core/gameLoader.hpp"
#include "core/villagerManagement.hpp"

#include "gui/gui.hpp"
#include "gui/msg.hpp"

#include "gui/screens/screenCommon.hpp"
#include "gui/screens/titleSelection.hpp"

#include "lang/lang.h"

#include <3ds.h>
#include <dirent.h>

// The classic Fade Effect! ;P
int fadealpha = 255;
bool fadein = true;

bool dspfirmfound = false;

// Set to 1, if testing some stuff. Leave to 0, if normal use.
int test = 0;

// If true -> Exit LeafEdit.
bool exiting = false;

bool WelcomeAmiibo;

bool isCitra = false; // Because Citra is kinda weird with the Initialize Message display.

// Touch Touch!
touchPosition touch;

// sound effects.
sound *sfx_change = NULL;

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


void loadSounds() {
	// Load the sound effects if DSP is available.
	if (dspfirmfound) {
		sfx_change = new sound("romfs:/sfx/change.wav", 2, false);
	}
}

// If an Error while startup appears, Return this!
static Result DisplayStartupError(std::string message, Result res, bool isSheet)
{
	std::string errorMsg = std::to_string(res);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	C2D_TargetClear(bottom, BLACK);
	Gui::clearTextBufs();
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 27, SelectorBlue);
	Gui::Draw_Rect(0, 27, 400, 187, C2D_Color32(128, 128, 128, 255));
	Gui::Draw_Rect(5, 30, 390, 177, C2D_Color32(0, 0, 0, 190));
	Gui::Draw_Rect(0, 213, 400, 27, SelectorBlue);
	if (isSheet == false) {
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, "Oh no, an error occured!"))/2, 2, 0.7f, WHITE, "Oh no, an error occured!", 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, "Description: "+ message))/2, 100, 0.7f, WHITE, "Description: "+message, 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, "Press Start to exit."))/2, 140, 0.7f, WHITE, "Press Start to exit.", 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, "Error during Startup: "+errorMsg))/2, 50, 0.7f, WHITE, "Error during Startup: "+errorMsg, 400);
	} else {
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, "Oh no, an error occured!"))/2, 2, 0.7f, WHITE, "Oh no, an error occured!", 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, "Error: "+ message))/2, 40, 0.7f, WHITE, "Error: "+message, 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.45f, "It seems like, the Spritesheet is not in 'SD:/LeafEdit/'.\nYou can download the Spritesheets through Universal-Manager,\nOr manually under the extras Repo of Universal-Team inside\n'builds/LeafEdit/Spritesheets.zip/7z'."))/2, 70, 0.45f, WHITE, "It seems like, the Spritesheet is not in 'SD:/LeafEdit/'.\nYou can download the Spritesheets through Universal-Manager,\nOr manually under the extras Repo of Universal-Team inside\n'builds/LeafEdit/Spritesheets.zip/7z'.", 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.45f, "The easiest way is: To download Universal-Manager\nAnd then go to 'Updater', 'UNIV' and then click on\nThe version you want. It automatically checks, if the files are found.\nKeep in mind -> You need a Internet Connection to download this!"))/2, 140, 0.45f, WHITE, "The easiest way is: To download Universal-Manager\nAnd then go to 'Updater', 'UNIV' and then click on\nThe version you want. It automatically checks, if the files are found.\nKeep in mind -> You need a Internet Connection to download this!", 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, "Press Start to exit."))/2, 215, 0.7f, WHITE, "Press Start to exit.", 400);
	}
	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 27, SelectorBlue);
	Gui::Draw_Rect(0, 27, 320, 186, GREEN);
	Gui::Draw_Rect(0, 213, 320, 27, SelectorBlue);
	C3D_FrameEnd(0);

	// For the Log.
	std::string error = message;
	error += ", ";
	error += std::to_string(res);
	Logging::writeToLog(error.c_str());

	gspWaitForVBlank();
	while (aptMainLoop() && !(hidKeysDown() & KEY_START))
	{
		hidScanInput();
	}
	return res;
}

void loadMessage(std::string Message) {
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	C2D_TargetClear(bottom, BLACK);
	Gui::clearTextBufs();
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 27, SelectorBlue);
	Gui::Draw_Rect(0, 27, 400, 186, GREEN);
	Gui::Draw_Rect(0, 213, 400, 27, SelectorBlue);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Message))/2, 2, 0.8f, WHITE, Message, 400);
	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 27, SelectorBlue);
	Gui::Draw_Rect(0, 27, 320, 186, GREEN);
	Gui::Draw_Rect(0, 213, 320, 27, SelectorBlue);
	C3D_FrameEnd(0);
}

int main()
{
	// Initialize Everything and check for errors.
	Result res;
	gfxInitDefault();
	Gui::init();

	if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
	} else {
		Msg::DisplayWarnMsg("dspfirm.cdc not found!");
	}

	if (isCitra == false) {
		loadMessage("Initialize everything.. please wait.");
	}

	if (R_FAILED(res = Archive::init())) {
		return DisplayStartupError("Archive::init failed.", res, false);
	}

	if (R_FAILED(res = romfsInit())) {
		return DisplayStartupError("romfsInit failed.", res, false);
	}

	Config::loadConfig();

	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/LeafEdit", 0777); // main Path.
	mkdir("sdmc:/LeafEdit/Towns", 0777); // Town Management Path.
	mkdir("sdmc:/LeafEdit/Towns/Old", 0777); // Old Path.
	mkdir("sdmc:/LeafEdit/Towns/Welcome-Amiibo", 0777); // Welcome Amiibo Path.
	mkdir("sdmc:/LeafEdit/Towns/Welcome-Luxury", 0777); // Welcome Luxury Path.
	mkdir("sdmc:/LeafEdit/SpriteSheets", 0777); // Spritesheets path.

	Logging::createLogFile(); // Create Log File, if it doesn't exists already.

	if (R_FAILED(res = acInit())) {
		return DisplayStartupError("acInit failed.", res, false);
	}

	if (R_FAILED(res = amInit())) {
		return DisplayStartupError("amInit failed.", res, false);
	}

	if (R_FAILED(res = sdmcInit())) {
		return DisplayStartupError("sdmcInit failed.", res, false);
	}

	if (R_FAILED(res = cfguInit())) {
		return DisplayStartupError("cfguInit failed.", res, false);
	}

	if(access("sdmc:/LeafEdit/acres.t3x", F_OK) != -1 ) {
		Gui::loadSheets(1);
	} else {
		return DisplayStartupError("Acres Spritesheet not found.", res, true);
	}

	if(access("sdmc:/LeafEdit/items.t3x", F_OK) != -1 ) {
		Gui::loadSheets(2);
	} else {
		return DisplayStartupError("items Spritesheet not found.", res, true);
	}

	if(access("sdmc:/LeafEdit/villagers.t3x", F_OK) != -1 ) {
		Gui::loadSheets(3);
	} else {
		return DisplayStartupError("villagers Spritesheet not found.", res, true);
	}

	if(access("sdmc:/LeafEdit/villagers2.t3x", F_OK) != -1 ) {
		Gui::loadSheets(4);
	} else {
		return DisplayStartupError("villagers2 Spritesheet not found.", res, true);
	}


	Config::loadSheet();
	Config::loadSheetIni();
	Config::loadSheetIniStuff();
	loadSounds();

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users

	// Load The Strings from the Romfs.

	Lang::loadLangStrings(Config::lang);
	VillagerManagement::LoadVillagerDatabase(Config::lang);

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
	playChange();
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

		if (hDown & KEY_ZR) {
			playChange();
		}

		if (fadein == true) {
			fadealpha -= 3;
			if (fadealpha < 0) {
				fadealpha = 0;
				fadein = false;
			}
		}
	}
	// Exit every process.
	loadMessage("Exiting Services...");
	delete sfx_change;
	if (dspfirmfound) {
		ndspExit();
	}
	cfguExit();
	sdmcExit();
	acExit();
	amExit();
	Archive::exit();
	Gui::exit();
	gfxExit();
	romfsExit();
	return 0;
}