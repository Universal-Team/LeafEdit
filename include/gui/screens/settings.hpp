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
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "common/fileBrowse.h"
#include "common/structs.hpp"

#include "gui/screens/screen.hpp"

#include <vector>

class Settings : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int Selection = 0;
	void SelectionLogic(u32 hDown, u32 hHeld);
	
	void DrawSpriteSheetSelection(void) const;
	void SpriteSheetLogic(u32 hDown, u32 hHeld);

	void DrawLangScreen(void) const;
	void langScreenLogic(u32 hDown, touchPosition touch);

	std::vector<Structs::ButtonPos> settingsButtons = {
    	{90, 40, 140, 35, -1}, // Language
		{90, 100, 140, 35, -1}, // Selector
		{90, 160, 140, 35, -1}, // ?
	};

	std::vector<Structs::ButtonPos> langBlocks = {
		{37, 52, 20, 20, -1},
		{37, 92, 20, 20, -1},
		{37, 132, 20, 20, -1},
		{37, 172, 20, 20, -1},

		{177, 52, 20, 20, -1},
		{177, 92, 20, 20, -1},
		{177, 132, 20, 20, -1},
		{177, 172, 20, 20, -1}, 

		{293, 213, 27, 27, -1},
	};

	int screenMode = 0;
	uint selectedSpriteSheet = 0;
	int keyRepeatDelay = 3;
	mutable bool dirChanged = true;
	std::string selectedSheet = "";
	std::string finalSheet = "";
	std::vector<DirEntry> dirContents;
};

#endif