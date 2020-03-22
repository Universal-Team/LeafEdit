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

#ifndef ISLANDMAPEDITOR_HPP
#define ISLANDMAPEDITOR_HPP

#include "common.hpp"
#include "item.hpp"
#include "itemContainer.hpp"
#include "structs.hpp"

#include <vector>

class Item;
class ItemContainer;
class IslandMapEditor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	IslandMapEditor();
	~IslandMapEditor();
private:
	std::shared_ptr<ItemContainer> MapItems[1024];
	// TEMP.
	std::shared_ptr<Item> TempItem;
	std::shared_ptr<ItemContainer> TempContainer;
	// Injection.
	void injectTo(int MapSlot);

	/* Mostly selection stuff. */
	int Mode = 0;
	int selection = 0; // Selection for "Items" / "Acres".
	int selectionMode = 0;
	
	/* Main Screen. */
	void DrawMapScreen(void) const;
	void MapScreenLogic(u32 hDown, u32 hHeld, touchPosition touch);
	int currentPosX = 0;
	int currentPosY = 0;
	int currentAcre = 0;
	int MapSelection = 0;
	int PositionX = 16;
	int PositionY = 16;

	/* Acres Screen. */
	void DrawAcres(void) const;
	void AcresLogic(u32 hDown, u32 hHeld, touchPosition touch);
	void DrawIsland(void) const;
	void DrawTopSelection(void) const;
	u8 selectedAcre = 0;
	bool FastMode = false;

	/* Temp Item Screen. */
	void DrawTempItem(void) const;
	void TempItemLogic(u32 hDown, u32 hHeld, touchPosition touch);
	int TempSelection = 0;

	/* Utilities. */
	void DrawGrid(void) const;
	void DrawIslandMap() const;
	void DrawCurrentPos(void) const;
	void convertToSelection();
	void convertToPosition();
	void updateStuff();
	void ResetPositions();
	int SelectionToAcre(int selection) const;

	std::vector<Structs::ButtonPos> mapButtons = {
		{230, 75, 75, 30}, // Items.
		{230, 140, 75, 30}, // Acres.
	};

	std::vector<Structs::ButtonPos> buildingButtons = {
		{90, 40, 140, 35}, // ID.
		{90, 100, 140, 35}, // PosX.
	};

	std::vector<Structs::ButtonPos> tempItemPos = {
		{20, 40, 280, 30}, // ID.
		{20, 90, 280, 30}, // Flag.
		{20, 140, 280, 30} // Name.
	};

	std::vector<Structs::ButtonPos> islandPos = {
		{45, 80, 40, 40},
		{85, 80, 40, 40},
		{45, 120, 40, 40},
		{85, 120, 40, 40}
	};

	std::vector<Structs::ButtonPos> acreIslandPos = {
		{80, 40, 40, 40},
		{120, 40, 40, 40},
		{160, 40, 40, 40},
		{200, 40, 40, 40},

		{80, 80, 40, 40},
		{120, 80, 40, 40},
		{160, 80, 40, 40},
		{200, 80, 40, 40},

		{80, 120, 40, 40},
		{120, 120, 40, 40},
		{160, 120, 40, 40},
		{200, 120, 40, 40},

		{80, 160, 40, 40},
		{120, 160, 40, 40},
		{160, 160, 40, 40},
		{200, 160, 40, 40}
	};
};

#endif