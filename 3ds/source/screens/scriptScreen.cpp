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

#include "Sav.hpp"
#include "scriptScreen.hpp"

#include "picoc.h"
#undef min // Get rid of picoc's min function
#include <unistd.h>

extern std::shared_ptr<Sav> save;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

// Return if Scripts are found.
bool ScriptScreen::returnIfExist() {
	dirContents.clear();
	chdir("sdmc:/LeafEdit/Scripts/");
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, {"c"});
	for(uint i=0;i<dirContentsTemp.size();i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}
	if (dirContents.size() == 0) {
		return false;
	}
	return true;
}

ScriptScreen::ScriptScreen() {
	refreshList();
}

// Refresh the Scriptlist and return scriptFound state.
void ScriptScreen::refreshList() {
	if (returnIfExist() == true) {
		Msg::DisplayMsg("Refreshing list...");
		dirContents.clear();
		chdir("sdmc:/LeafEdit/Scripts/");
		std::vector<DirEntry> dirContentsTemp;
		getDirectoryContents(dirContentsTemp, {"c"});
		for(uint i=0;i<dirContentsTemp.size();i++) {
			dirContents.push_back(dirContentsTemp[i]);
		}
		Selection = 0;
		ScriptsFound = true;
	} else {
		Selection = 0;
		ScriptsFound = false;
	}
}

// Script Selection list.
void ScriptScreen::Draw(void) const {
	if (ScriptsFound) {
		std::string scripts;
		GFX::DrawFileBrowseBG();
		Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "Select the script you like to use.", 395);
		Gui::DrawStringCentered(0, 217, 0.9f, WHITE, Lang::get("REFRESH"), 395);
		for (uint i=(Selection<8) ? 0 : (uint)Selection-8;i<dirContents.size()&&i<(((uint)Selection<8) ? 9 :(uint)Selection+1);i++) {
			scripts += dirContents[i].name + "\n";
		}
		for (uint i=0;i<((dirContents.size()<9) ? 9-dirContents.size() : 0);i++) {
			scripts += "\n";
		}

		if (Selection < 9)	GFX::DrawSelector(true, 24 + ((int)Selection * 21));
		else				GFX::DrawSelector(true, 24 + (8 * 21));
		Gui::DrawString(5, 25, 0.85f, BLACK, scripts, 360);
		GFX::DrawFileBrowseBG(false);
	} else {
		GFX::DrawTop();
		Gui::DrawStringCentered(0, -2, 0.8f, WHITE, "No Scripts found!", 395);
		GFX::DrawBottom();
	}
}

void ScriptScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (keyRepeatDelay)	keyRepeatDelay--;

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (Selection < (int)dirContents.size()-1) {
			Selection++;
		} else {
			Selection = 0;
		}
		if (fastMode == true) {
			keyRepeatDelay = 8;
		} else if (fastMode == false){
			keyRepeatDelay = 8;
		}
	}
	if (hHeld & KEY_UP && !keyRepeatDelay) {
		if (Selection > 0) {
			Selection--;
		} else {
			Selection = (int)dirContents.size()-1;
		}
		if (fastMode == true) {
			keyRepeatDelay = 8;
		} else if (fastMode == false){
			keyRepeatDelay = 8;
		}
	}

	if (hDown & KEY_A) {
		if (ScriptsFound) {
			// Make sure this is not a Directory.
			if (!dirContents[Selection].isDirectory) {
				applyScript(dirContents[Selection].name);
			}
		}
	}
	// Update Screen when needed.
	if(Selection < screenPos) {
		screenPos = Selection;
	} else if (Selection > screenPos + 3 - 1) {
		screenPos = Selection - 3 + 1;
	}
}

Picoc* picoC()
{
	static Picoc picoc;
	PicocInitialize(&picoc, PICOC_STACKSIZE);
	return &picoc;
}

// Read a script.
std::vector<u8> scriptRead(const std::string& path)
{
	std::vector<u8> ret;
	size_t size = 0;
	FILE* in	= fopen(path.c_str(), "rb");
	fseek(in, 0, SEEK_END);
	if (!ferror(in))
	{
		size = ftell(in);
		fseek(in, 0, SEEK_SET);
		ret = std::vector<u8>(size);
		fread(ret.data(), 1, size, in);
	}
	else
	{
		Msg::DisplayWarnMsg("Error while opening the script.");
	}
	fclose(in);
	return ret;
}

// Apply a script -> Run it.
void ScriptScreen::applyScript(std::string& file)
{
	parsePicoCScript(file);
	return;

	auto scriptData = scriptRead(file);

	if (scriptData.empty())
	{
		return;
	}
}

// Parse a PicoC Script and pass SaveType, Region, SaveBuffer, Length etc to the argv's.
void ScriptScreen::parsePicoCScript(std::string& file)
{
	// The loops used in PicoC make this basically a necessity
	aptSetHomeAllowed(false);
	// setup for printing errors
	static char error[4096];
	std::fill_n(error, sizeof(error), '\0');
	// Save stdout state
	int stdout_save = dup(STDOUT_FILENO);
	// Set stdout to buffer to error
	setvbuf(stdout, error, _IOFBF, 4096);

	Picoc* picoc = picoC();
	if (!PicocPlatformSetExitPoint(picoc))
	{
		PicocPlatformScanFile(picoc, file.c_str());
		char* args[4];
		args[0]				= (char*)save->rawData().get();
		std::string length  = std::to_string(save->getLength());
		args[1]				= length.data();
		char version		= save->version();
		args[2]				= &version;
		char region			= save->region();
		args[3]				= &region;
		PicocCallMain(picoc, 4, args);
	}

	// Restore stdout state
	dup2(stdout_save, STDOUT_FILENO);

	if (picoc->PicocExitValue != 0)
	{
		std::string show = error;
		if (!show.empty())
		{
			Msg::DisplayWaitMsg("Script Execution error!" + '\n' + file);
			show += "\nExit code: " + std::to_string(picoc->PicocExitValue);
			Msg::DisplayWaitMsg(show);
		}
	}

	PicocCleanup(picoc);
	// And here we'll clean up
	aptSetHomeAllowed(true);
}