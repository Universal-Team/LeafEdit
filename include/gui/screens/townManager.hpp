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

#ifndef TOWNMANAGER_HPP
#define TOWNMANAGER_HPP

#include "common/fileBrowse.h"
#include "common/structs.hpp"

#include "gui/screens/screen.hpp"

#include <vector>

class TownManager : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	int Selection = 0;

	void DrawSubMenu(void) const;
	void SelectionLogic(u32 hDown, u32 hHeld);

	void DrawBrowse(void) const;
	void BrowseLogic(u32 hDown, u32 hHeld);

	std::vector<Structs::ButtonPos> townButtons = {
		{10, 40, 140, 35, -1}, // Launch.
		{10, 100, 140, 35, -1}, // Backup.
		{10, 160, 140, 35, -1}, // Restore.
		{170, 40, 140, 35, -1}, // Delete Gamecard / Installed Title save.
		{170, 100, 140, 35, -1}, // Delete Backup.
	};

	int screenMode = 0;
	uint selectedSave = 0;
	int keyRepeatDelay = 3;
	mutable bool dirChanged = true;
	std::vector<DirEntry> dirContents;
};

#endif