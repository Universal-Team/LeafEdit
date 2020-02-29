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

#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "common.hpp"
#include "structs.hpp"

#include <vector>

class Editor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int EditorMode = 0; // 0 Main, 1 Editor.
	int Selection = 0;
	// Set Functions for save.
	void SetMode(int mode);
	void PrepareNL(const std::string savePath);
	void PrepareWW(const std::string savePath);
	void saveNL();
	void saveWW();

	// Draws & Logic.
	void DrawMain(void) const; // Main Screen.
	void MainLogic(u32 hDown, u32 hHeld, touchPosition touch);

	void DrawEditor(void) const;
	void EditorLogic(u32 hDown, u32 hHeld, touchPosition touch);
	
	std::vector<Structs::ButtonPos> editorButtons = {
		{90, 40, 140, 35}, // Player.
		{90, 100, 140, 35}, // Villager.
		{90, 160, 140, 35}, // WIP.
		{293, 213, 27, 27}, // Saving.
	};

	std::vector<Structs::ButtonPos> editorMainBtn = {
		{10, 100, 140, 35}, // RAW Saves.
		{170, 100, 140, 35}, // Title Loader.
	};
};

#endif