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

#ifndef SCRIPTS_HPP
#define SCRIPTS_HPP

#include "common/fileBrowse.h"

#include "gui/screens/screen.hpp"
#include "gui/screens/screenCommon.hpp"

#include <vector>

class Scripts : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	Scripts();
private:
	int mode = 0;
	void refresh(int location = 0);
	int scriptLocation = 1;

	int executeMode = 0; // 0 -> Player. 1 -> Villager? 2 -> Custom. 3 -> Invalid.

	// Screen Draws.
	void DrawSubMenu(void) const;
	void DrawSingleObject(void) const;
	// Logics.
	void subMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);
	void selectLogic(u32 hDown, u32 hHeld, touchPosition touch);

	int keyRepeatDelay = 0;
	int fastMode = false;
	
	std::vector<DirEntry> dirContents;
	mutable int screenPos = 0;
	mutable int screenPos2 = 0;
	int selection = 0;
	int selection2 = 0;

};

#endif