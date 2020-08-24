/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
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

#ifndef _LEAFEDIT_TOWNMAPEDITOR_WW_HPP
#define _LEAFEDIT_TOWNMAPEDITOR_WW_HPP

#include "Acre.hpp"
#include "common.hpp"
#include "Item.hpp"
#include "structs.hpp"

class TownMapEditorWW : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	TownMapEditorWW();
private:
	/* Specific stuff. */
	u32 maxAcres;
	std::unique_ptr<Item> MapItems[4096]; // Complete Town Map Items.
	std::unique_ptr<Acre> FullAcres[36]; // Complete Acres.

	/* Main Screen. */
	void DrawMapScreen(void) const;
	void MapScreenLogic(u32 hDown, u32 hHeld, touchPosition touch);

	/* Town Map Editor Selection & Positions. */
	int currentPosX = 0;
	int currentPosY = 0;
	int currentAcre = 0;
	int MapSelection = 0;
	int PositionX = 16;
	int PositionY = 16;
	/* Modes. Acre Editor & Town Map Editor. */
	int Mode = 1;
	int selectMode = 0; // No sub Selection.

	/* General Item stuff. */
	u16 itemID = 65521;
	int keyRepeatDelay = 0;

	/* Utilities Draw. */
	void DrawGrid(void) const;
	void DrawTownMap() const;
	void DrawInformation() const;
	void DrawCurrentPos(void) const;
	/* Converter's. */
	void convertToSelection();
	void convertToPosition();
	int SelectionToAcre(int selection) const;

	/* Operations. */
	void injectTo(int MapSlot);
	void remove(u16 ID);
	void setAll(u16 ID);
	void replace(u16 oldID, u16 newID);
	void removeWeeds();
	void waterFlowers();
	void updateStuff();

	void DrawTempItem(void) const;
	void TempItemLogic(u32 hDown, u32 hHeld, touchPosition touch);
	
	/* Acres Screen. */
	void DrawAcres(void) const;
	void AcresLogic(u32 hDown, u32 hHeld, touchPosition touch);
	void DrawMap(void) const;
	void DrawTopSelection(void) const;
	u8 selectedAcre = 0;
	bool FastMode = false; // Scrolling speed.
	int selection = 0;

	const std::vector<ButtonType> tempItemPos = {
		{20, 45, 280, 50, ""}, // ID.
		{20, 145, 280, 50, "SWITCH_ITEM_SELECTION"}
	};

	/* Display AC:WW Top Screen Map. */
	const std::vector<Structs::ButtonPos> WWMapPos = {
		{5, 40, 32, 32} ,{37, 40, 32, 32}, {69, 40, 32, 32}, {101, 40, 32, 32},
		{5, 72, 32, 32}, {37, 72, 32, 32}, {69, 72, 32, 32}, {101, 72, 32, 32},
		{5, 104, 32, 32}, {37, 104, 32, 32}, {69, 104, 32, 32}, {101, 104, 32, 32},
		{5, 136, 32, 32}, {37, 136, 32, 32}, {69, 136, 32, 32}, {101, 136, 32, 32}
	};

	/* Display AC:WW's full Town Map. */
	const std::vector<Structs::ButtonPos> wwPos = {
		/* First Line. */
		{64, 16, 32, 32},
		{96, 16, 32, 32},
		{128, 16, 32, 32},
		{160, 16, 32, 32},
		{192, 16, 32, 32},
		{224, 16, 32, 32},
		/* Second Line. */
		{64, 48, 32, 32},
		{96, 48, 32, 32},
		{128, 48, 32, 32},
		{160, 48, 32, 32},
		{192, 48, 32, 32},
		{224, 48, 32, 32},
		/* Third Line. */
		{64, 80, 32, 32},
		{96, 80, 32, 32},
		{128, 80, 32, 32},
		{160, 80, 32, 32},
		{192, 80, 32, 32},
		{224, 80, 32, 32},
		/* Fourth Line. */
		{64, 112, 32, 32},
		{96, 112, 32, 32},
		{128, 112, 32, 32},
		{160, 112, 32, 32},
		{192, 112, 32, 32},
		{224, 112, 32, 32},
		/* Fifth Line. */
		{64, 144, 32, 32},
		{96, 144, 32, 32},
		{128, 144, 32, 32},
		{160, 144, 32, 32},
		{192, 144, 32, 32},
		{224, 144, 32, 32},
		/* Sixth Line. */
		{64, 176, 32, 32},
		{96, 176, 32, 32},
		{128, 176, 32, 32},
		{160, 176, 32, 32},
		{192, 176, 32, 32},
		{224, 176, 32, 32}
	};

	/* Display Button. */
	const std::vector<ButtonType> mainButtons = {
		{220, 25, 90, 30, "ACRES"},
		{220, 55, 90, 30, "TEMP_ITEM"},
		{220, 85, 90, 30, "CLEAR_WEEDS"},
		{220, 115, 90, 30, "WATER_FLOWERS"},
		{220, 145, 90, 30, "ITEM_MISC"}
	};

	/* Display Operations. */
	const std::vector<ButtonType> operationBtn = {
		{95, 34, 130, 48, "REMOVE_ITEMS"},
		{95, 97, 130, 48, "FILL_TOWN"},
		{95, 159, 130, 48, "REPLACE_ITEMS"}
	};

	const std::vector<Structs::ButtonPos> icons = {
		{286, 213, 27, 27} // Back Icon.
	};
};

#endif