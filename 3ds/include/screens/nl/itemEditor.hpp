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

#ifndef ITEMEDITOR_HPP
#define ITEMEDITOR_HPP

#include "common.hpp"
#include "structs.hpp"

#include <vector>

class ItemEditor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	ItemEditor();
	~ItemEditor(void);

private:
	int currentBox = 0;
	int currentItem = 0;
	int itemMode = 0;
	int Selection = 0;

	int currentRow = 0;
	int selectedCategory = 0;

	void DrawSubMenu(void) const;
	void SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);

	void DisplayPocket(void) const;
	void PocketLogic(u32 hDown, u32 hHeld);

	void DisplayItems(void) const;

	void DisplayDresser(void) const;
	void DresserLogic(u32 hDown, u32 hHeld, touchPosition touch);

	void DisplayIslandBox(void) const;
	void islandBoxLogic(u32 hDown, u32 hHeld, touchPosition touch);

	void DisplayStorage(void) const;
	void storageLogic(u32 hDown, u32 hHeld, touchPosition touch);

	// Other.
	void DrawSlotSelection(void) const;

	// Sub Pos.
	std::vector<Structs::ButtonPos> mainButtons = {
		{10, 40, 140, 35}, // Pocket.
		{10, 100, 140, 35}, // Island box.
		{10, 160, 140, 35}, // Dresser.
		{170, 40, 140, 35}, // Storage.
		{170, 100, 140, 35}, // ?.
		{170, 160, 140, 35}, // ?.
	};

	std::vector<Structs::ButtonPos> catPos = {
		{5, 5, 25, 20}, // First Category.
		{55, 5, 25, 20}, // Second Category.
		{105, 5, 25, 20}, // Third Category.
		{155, 5, 25, 20}, // Fourth Category.
	};
	std::vector<Structs::ButtonPos> arrowPos = {
		{295, 0, 25, 25}, // Arrow Up.
		{295, 215, 25, 25}, // Arrow Down.
	};
};

#endif