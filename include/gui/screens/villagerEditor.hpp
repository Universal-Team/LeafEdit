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

#ifndef VILLAGEREDITOR_HPP
#define VILLAGEREDITOR_HPP

#include "common/fileBrowse.h"
#include "common/structs.hpp"

#include "gui/screens/screen.hpp"

#include <vector>

class VillagerEditor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:

	// Basic Stuff.
	int editorMode = 0;
	int group = 1;
	int subMenuPage = 1;
	int Selection = 0;

	/*
		SUB MENU.
	*/
	void DrawSubMenu(void) const;
	void DrawSelection(void) const;
	void SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch); // SubMenu Logic.

	void DrawVillagerSelection(void) const;
	void VillagerSelectionLogic(u32 hDown, u32 hHeld, touchPosition touch);

	void EditorSubLogic(u32 hDown);
	void DrawEditorSub(void) const;

	void DrawVillagerSetTest(void) const;
	void VillagerSetLogicTest(u32 hDown, u32 hHeld, touchPosition touch);

	void DrawBox(void) const;

	// Villager Editor Stuff.
	bool isSelected = false; // If the Villager is Selected, this would be true.
	int editorPage = 1; // 30 Pages is the maximum because of ~ 299 Villagers.
	u16 selectedVillager; // This would be the new Selected Villager.
	int currentSelectedVillager;
	int currentRow = 1; // Return the current Selected Row.
	int currentSlot = 1; // Return the current Selected Slot.
	u16 manuallyVillager = 0;
	int startID = 0;
	int endID = 6;

	std::vector<Structs::ButtonPos> Buttons = {
		{90, 40, 140, 35, -1}, // Selection.
		{90, 100, 140, 35, -1}, // Manually.
	};

	// Touch Position of the search Icon, so you can search for a Villager ID.
	std::vector<Structs::ButtonPos> search = {
		{290, 3, 20, 20, -1},
	};
};

#endif