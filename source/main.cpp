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
#include "common/utils.hpp"

#include "core/gameLoader.hpp"
#include "core/management/itemManagement.hpp"
#include "core/titleLoader.hpp"
#include "core/management/villagerManagement.hpp"

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
	Gui::Draw_Rect(0, 0, 400, 30, green3);
	Gui::Draw_Rect(0, 30, 400, 180, green4);
	Gui::Draw_Rect(0, 210, 400, 30, green3);
	if (isSheet == false) {
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, Lang::get("ERROR_OCCURED")))/2, 2, 0.7f, WHITE, Lang::get("ERROR_OCCURED"), 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, Lang::get("DESCRIPTION")+ message))/2, 100, 0.7f, WHITE, Lang::get("DESCRIPTION")+message, 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, Lang::get("START_EXIT")))/2, 140, 0.7f, WHITE, Lang::get("START_EXIT"), 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, Lang::get("ERROR_STARTUP")+errorMsg))/2, 50, 0.7f, WHITE, Lang::get("ERROR_STARTUP")+errorMsg, 400);
	} else {
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, Lang::get("ERROR_OCCURED")))/2, 2, 0.7f, WHITE, Lang::get("ERROR_OCCURED"), 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, message))/2, 40, 0.7f, WHITE, message, 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.45f, Lang::get("ERRORMSG_1")))/2, 70, 0.45f, WHITE, Lang::get("ERRORMSG_1"), 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.45f, Lang::get("ERRORMSG_2")))/2, 140, 0.45f, WHITE, Lang::get("ERRORMSG_2"), 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.7f, Lang::get("START_EXIT")))/2, 215, 0.7f, WHITE, Lang::get("START_EXIT"), 400);
	}
	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, green3);
	Gui::Draw_Rect(0, 30, 320, 180, green2);
	Gui::Draw_Rect(0, 210, 320, 30, green3);
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
	Gui::Draw_Rect(0, 0, 400, 30, green3);
	Gui::Draw_Rect(0, 30, 400, 180, green4);
	Gui::Draw_Rect(0, 210, 400, 30, green3);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Message))/2, 2, 0.8f, WHITE, Message, 400);
	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, green3);
	Gui::Draw_Rect(0, 30, 320, 180, green2);
	Gui::Draw_Rect(0, 210, 320, 30, green3);
	C3D_FrameEnd(0);
}

int main()
{
	// Initialize Everything and check for errors.
	Result res;
	gfxInitDefault();
	Gui::init();

	romfsInit();
	Archive::init();

	if(access("sdmc:/LeafEdit/Settings.json", F_OK) == -1 ) {
		Config::initializeNewConfig();
	}
	
	Config::load();
	Lang::load(Config::getLang("Lang"));

	if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
	} else {
		Msg::DisplayWarnMsg(Lang::get("DSPFIRM_NOTFOUND"));
	}
	
	if (isCitra == false) {
		loadMessage(Lang::get("INITIALIZE_MSG"));
	}

	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/LeafEdit", 0777); // main Path.
	mkdir("sdmc:/LeafEdit/Towns", 0777); // Town Management Path.
	mkdir("sdmc:/LeafEdit/Towns/Old", 0777); // Old Path.
	mkdir("sdmc:/LeafEdit/Towns/Welcome-Amiibo", 0777); // Welcome Amiibo Path.
	mkdir("sdmc:/LeafEdit/Towns/Welcome-Luxury", 0777); // Welcome Luxury Path.
	mkdir("sdmc:/LeafEdit/Sheets", 0777); // Sheet Ini path.
	mkdir("sdmc:/LeafEdit/Backups", 0777); // Backup path.

	Logging::createLogFile(); // Create Log File, if it doesn't exists already.

	if (R_FAILED(res = amInit())) {
		return DisplayStartupError("amInit "+Lang::get("INIT_FAILED"), res, false);
	}

	if (R_FAILED(res = sdmcInit())) {
		return DisplayStartupError("sdmcInit "+Lang::get("INIT_FAILED"), res, false);
	}

	if (R_FAILED(res = cfguInit())) {
		return DisplayStartupError("cfguInit "+Lang::get("INIT_FAILED"), res, false);
	}

	Gui::loadSheets(0);
	if(access("sdmc:/LeafEdit/acres.t3x", F_OK) != -1 ) {
		Gui::loadSheets(1);
	} else {
		return DisplayStartupError("Acres "+Lang::get("NOT_FOUND_SPRSHT"), res, true);
	}

	if(access("sdmc:/LeafEdit/items.t3x", F_OK) != -1 ) {
		Gui::loadSheets(2);
	} else {
		return DisplayStartupError("items "+Lang::get("NOT_FOUND_SPRSHT"), res, true);
	}

	if(access("sdmc:/LeafEdit/villagers.t3x", F_OK) != -1 ) {
		Gui::loadSheets(3);
	} else {
		return DisplayStartupError("villagers "+Lang::get("NOT_FOUND_SPRSHT"), res, true);
	}

	if(access("sdmc:/LeafEdit/villagers2.t3x", F_OK) != -1 ) {
		Gui::loadSheets(4);
	} else {
		return DisplayStartupError("villagers2 "+Lang::get("NOT_FOUND_SPRSHT"), res, true);
	}

	loadSounds();

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users

	// Load The Strings from the Romfs.
	
	ItemManagement::LoadDatabase(Config::getLang("Lang"));

	TestStuff();

	GameLoader::checkUpdate();

	// Return the Welcome Amiibo State.
	if (Config::getBool("update") == false) {
		WelcomeAmiibo = false;
	} else if (Config::getBool("update") == true) {
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
	delete sfx_change;
	if (dspfirmfound) {
		ndspExit();
	}
	Config::save();
	cfguExit();
	sdmcExit();
	amExit();
	Archive::exit();
	Gui::exit();
	gfxExit();
	romfsExit();
	return 0;
}