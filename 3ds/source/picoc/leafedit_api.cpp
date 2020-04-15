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

#include "encryptedInt32.hpp"
#include "Sav.hpp"
#include "SavNL.hpp"
#include "SavWW.hpp"
#include "SavWA.hpp"
#include "saveUtils.hpp"
#include "utils.hpp"

#include <algorithm>
#include <errno.h>

#include "picoc.h"

extern std::shared_ptr<Sav> save;


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

	// Read a u8.
	void sav_read_u8(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		u32 offset = Param[0]->Val->UnsignedInteger;
		if (NumArgs != 1)
		{
			scriptFail(Parser, "Incorrect number of args (%i) for ReadU8.", NumArgs);
		}
		ReturnValue->Val->Integer = u8(save->savePointer()[offset]);
	}

	// Read a u16.
	void sav_read_u16(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		u32 offset = Param[0]->Val->UnsignedInteger;
		if (NumArgs != 1)
		{
			scriptFail(Parser, "Incorrect number of args (%i) for SaveUtils::Read<u16>.", NumArgs);
		}
		ReturnValue->Val->UnsignedInteger = SaveUtils::Read<u16>(save->savePointer(), offset);
	}

	// Read a u32.
	void sav_read_u32(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		u32 offset = Param[0]->Val->UnsignedInteger;
		if (NumArgs != 1)
		{
			scriptFail(Parser, "Incorrect number of args (%i) for SaveUtils::Read<u32>.", NumArgs);
		}
		ReturnValue->Val->UnsignedInteger = SaveUtils::Read<u32>(save->savePointer(), offset);
	}

	// Write a u8.
	void sav_write_u8(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		u32 offset = Param[0]->Val->UnsignedInteger;
		u8 value = Param[1]->Val->UnsignedInteger;
		if (NumArgs != 1)
		{
			scriptFail(Parser, "Incorrect number of args (%i) for Write U8.", NumArgs);
		}
		save->savePointer()[offset] = value;
	}

	// Write a u16.
	void sav_write_u16(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		u32 offset = Param[0]->Val->UnsignedInteger;
		u16 value = Param[1]->Val->UnsignedInteger;
		if (NumArgs != 2)
		{
			scriptFail(Parser, "Incorrect number of args (%i) for SaveUtils::Read<u16>.", NumArgs);
		}
		SaveUtils::Write<u16>(save->savePointer(), offset, value);
	}
	
	// Write a u32.
	void sav_write_u32(struct ParseState* Parser, struct Value* ReturnValue, struct Value** Param, int NumArgs)
	{
		u32 offset = Param[0]->Val->UnsignedInteger;
		u32 value = Param[1]->Val->UnsignedInteger;
		if (NumArgs != 2)
		{
			scriptFail(Parser, "Incorrect number of args (%i) for SaveUtils::Write<u32>.", NumArgs);
		}
		SaveUtils::Write<u32>(save->savePointer(), offset, value);
	}
}