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

class VillagerEditor : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:

	// Basic Stuff.
	int currentVillager = 1;
	int villagerMode = 1;

	// Logic.
	void EditorLogic(u32 hDown, u32 hHeld, touchPosition touch); // Editor Logic.
	void VillagerLogic(u32 hDown, u32 hHeld, touchPosition touch); // Villager Logic.
	void DrawEditorPage(void) const;

	// Villager Screens.
	void Villager1Draw(void) const;
	void Villager2Draw(void) const;
	void Villager3Draw(void) const;
	void Villager4Draw(void) const;
	void Villager5Draw(void) const;
	void Villager6Draw(void) const;
	void Villager7Draw(void) const;
	void Villager8Draw(void) const;
	void Villager9Draw(void) const;
	void Villager10Draw(void) const;

	// Villager Editor Stuff.
	bool isSelected = false; // If the Villager is Selected, this would be true.
	int editorPage = 1; // 30 Pages is the maximum because of ~ 299 Villagers.
	u16 selectedVillager; // This would be the new Selected Villager.
	int currentRow = 1; // Return the current Selected Row.
	int currentSlot = 1; // Return the current Selected Slot.


	void VillagerEditorDraw(int currentVillager) const;
	void DrawEditorSelection(void) const;

	// Current Villager.
	void DrawCurrentVillager(void) const;
};

#endif