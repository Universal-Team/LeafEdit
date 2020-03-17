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

#ifndef MAPEDITORWW_HPP
#define MAPEDITORWW_HPP

#include "common.hpp"
#include "structs.hpp"
#include "wwItem.hpp"
#include "wwItemContainer.hpp"

#include <vector>

class WWItem;
class WWItemContainer;
class MapEditorWW : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	MapEditorWW();
private:
	// TEMP.
	std::shared_ptr<WWItem> TempItem;
	std::shared_ptr<WWItemContainer> TempContainer;

	/* Mostly selection stuff. */
	int Mode = 0;
	int selection = 0; // Selection for "Items" / "Buildings".
	int selectionMode = 0;
	int currentPosX = 0;
	int currentPosY = 0;
	int currentAcre = 0;
	int MapSelection = 0;
	int PositionX = 0;
	int PositionY = 0;

	void DrawPosition(void) const;
	void DrawTownMapEditor() const;
	void DrawInformation() const;
	void injectTo(int MapSlot);

	void convertToPosition();
	void convertToSelection();

	int SelectionToAcre(int i) const;
	void DrawGrid(void) const;

	void DrawMap(void) const;
	void MainLogic(u32 hDown, u32 hHeld, touchPosition touch);
	
	/* Temp Item Screen. */
	void DrawTempItem(void) const;
	void TempItemLogic(u32 hDown, u32 hHeld, touchPosition touch);
	int TempSelection = 0;

	std::vector<Structs::ButtonPos> MapPos = {
		{5, 40, 32, 32},
		{37, 40, 32, 32},
		{69, 40, 32, 32},
		{101, 40, 32, 32},

		{5, 72, 32, 32},
		{37, 72, 32, 32},
		{69, 72, 32, 32},
		{101, 72, 32, 32},

		{5, 104, 32, 32},
		{37, 104, 32, 32},
		{69, 104, 32, 32},
		{101, 104, 32, 32},

		{5, 136, 32, 32},
		{37, 136, 32, 32},
		{69, 136, 32, 32},
		{101, 136, 32, 32}
	};

	std::vector<Structs::ButtonPos> mapButtons = {
		{230, 75, 75, 30}, // Items.
		{230, 140, 75, 30}, // Acres(?).
	};

	std::vector<Structs::ButtonPos> tempItemPos = {
		{20, 40, 280, 30}, // ID.
		{20, 90, 280, 30}, // Name.
	};
};

#endif