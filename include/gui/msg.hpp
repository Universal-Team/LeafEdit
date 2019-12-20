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
#ifndef MSG_HPP
#define MSG_HPP

#include <string>

namespace Msg
{
	void DisplayWarnMsg(std::string Text); // Display a Warn Message for about 2 seconds.
	void DisplayWarnMsg2(std::string Text); // Display a Warn Message for about 2 seconds. Used for more text.
	bool promptMsg(std::string msg); // This will be used for the prompt Messages, which needs confirmation with A or cancel with B.
	bool promptMsg2(std::string promptMsg); // This is part of promptMsg.
	void DisplayWaitMsg(std::string waitMsg, ...); // Will Display a Message, which needs a confirmation with A.
	void NotImplementedYet(void); // Display a not Implemented Message.
	void DisplayMsg(std::string Message); // Display just a Message.
	void SheetMsg(std::string Message); // Display a Message for the reloading Sheet.
	void HelperBox(std::string Msg); // Helper Box?
}

#endif