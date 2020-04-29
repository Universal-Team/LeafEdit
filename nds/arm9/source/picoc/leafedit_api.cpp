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

#include "msg.hpp"
#include "Sav.hpp"
#include "SavNL.hpp"
#include "SavWW.hpp"
#include "SavWA.hpp"
#include "saveUtils.hpp"

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
}