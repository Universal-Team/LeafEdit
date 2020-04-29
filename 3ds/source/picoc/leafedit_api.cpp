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
#include "encryptedInt32.hpp"
#include "fileBrowse.hpp"
#include "gfx.hpp"
#include "itemUtils.hpp"
#include "keyboard.hpp"
#include "Sav.hpp"
#include "SavNL.hpp"
#include "SavWW.hpp"
#include "SavWA.hpp"
#include "saveUtils.hpp"
#include "thread.hpp"
#include "utils.hpp"

#include <algorithm>
#include <errno.h>

#include "picoc.h"

extern std::shared_ptr<Sav> save;
extern bool changes;
extern char progressBarMsg[128];
extern bool showProgressBar;

[[noreturn]] void scriptFail(struct ParseState* Parser, const std::string& str) {
	ProgramFail(Parser, str.c_str());
	std::abort(); // Dummy call to suppress compiler warning: ProgramFail does not return
}

template <typename... Ts>
[[noreturn]] void scriptFail(struct ParseState* Parser, const std::string& str, Ts... args) {
	ProgramFail(Parser, str.c_str(), args...);
	std::abort(); // Dummy call to suppress compiler warning: ProgramFail does not return
}

extern "C" {
	#include "leafedit_api.h"

	// Display a warn Message.
	void msg_warn(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		Msg::DisplayWarnMsg((char*)Param[0]->Val->Pointer);
	}

	// Display a message which will stay until pressed A.
	void msg_waitMsg(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		Msg::DisplayWaitMsg((char*)Param[0]->Val->Pointer);
	}

	// Display a Message while a specific Action appears.
	void msg_splash(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		Msg::DisplayMsg((char*)Param[0]->Val->Pointer);
	}

	// Display a prompt which returns 1 by True, 0 by false.
	void msg_prompt(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		if (Msg::promptMsg((char*)Param[0]->Val->Pointer) == true) {
			ReturnValue->Val->Integer = 1;
		} else {
			ReturnValue->Val->Integer = 0;
		}
	}

	// Get the Item name from an ID.
	void getItem(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
		if (NumArgs != 1) {
			scriptFail(Parser, "Incorrect number of args (%i) for ItemUtils::getName(u16 ID).", NumArgs);
		}

		std::string itemName = ItemUtils::getName(Param[0]->Val->Integer);
		ReturnValue->Val->Pointer = const_cast<char*>(itemName.c_str());
	}

	// Open the Keyboard and return a char*.
	void keyboard_string(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
		if (NumArgs != 1) {
			scriptFail(Parser, "Incorrect number of args (%i) for Input::getString(std::string Text).", NumArgs);
		}

		std::string output = Input::getString((char*)Param[0]->Val->Pointer);
		ReturnValue->Val->Pointer = const_cast<char*>(output.c_str());
	}

	// Open the Keyboard and return an int.
	void keyboard_value(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
		if (NumArgs != 1) {
			scriptFail(Parser, "Incorrect number of args (%i) for Input::Numpad(std::string Text).", NumArgs);
		}

		std::string output = Input::Numpad((char*)Param[0]->Val->Pointer);
		ReturnValue->Val->Integer = std::stoi(output);
	}

	// Select a index from a list. Return the position as an int.
	void selectList(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		if (NumArgs != 3)
		{
			scriptFail(Parser, "Incorrect number of args (%i) for GFX::ListSelection.", NumArgs);
		}
		// Needed things for the function.
		std::string msg;
		std::vector<std::string> contents;
		// Script param stuff.
		int options = Param[2]->Val->Integer;
		char** cnt = (char**)Param[1]->Val->Pointer;
		char* message	= (char*)Param[0]->Val->Pointer;
		msg = message;

		// Set all to the Vector.
		for (int i = 0; i < options; i++)
		{
			contents.emplace_back(cnt[i]);
		}
		// Get the result from the function and return it!
		int result = (int)GFX::ListSelection(0, contents, message);
		ReturnValue->Val->Integer = result;
	}
	void setChangesMade(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs) {
		changes = true;
	}

	// Download a File.
	void download_file(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		if (NumArgs != 3)
		{
			scriptFail(Parser, "Incorrect number of args (%i) for download_file().", NumArgs);
		}

		snprintf(progressBarMsg, sizeof(progressBarMsg), (char*)Param[2]->Val->Pointer);
		showProgressBar = true;
		Threads::create((ThreadFunc)displayProgressBar);
		if (downloadToFile((char*)Param[0]->Val->Pointer, (char*)Param[1]->Val->Pointer) != 0) {
			showProgressBar = false;
			Msg::DisplayWarnMsg("Download Failed!");
			return;
		}
		showProgressBar = false;
	}

	// Select a File from the SD Card.
	void file_select(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		if (NumArgs != 2)
		{
			scriptFail(Parser, "Incorrect number of args (%i) for file_select().", NumArgs);
		}

		std::string output = searchForFile((char*)Param[0]->Val->Pointer, (char*)Param[1]->Val->Pointer);
		ReturnValue->Val->Pointer = const_cast<char*>(output.c_str());
		chdir("sdmc:/LeafEdit/Scripts/");
	}
}