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

#ifndef _LEAFEDIT_ITEM_EDITOR_NL_HPP
#define _LEAFEDIT_ITEM_EDITOR_NL_HPP

#include "common.hpp"
#include "coreUtils.hpp"
#include "Item.hpp"
#include "screenCommon.hpp"

#include <vector>

class ItemEditorNL : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	ItemEditorNL(std::shared_ptr<Player> player);
	~ItemEditorNL(void);
private:
	void DrawSlotSelection(void) const;

	// Sub Menu.
	void DrawSubMenu(void) const;
	void SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);

	// Pocket.
	void DrawPocket(void) const;
	void PocketLogic(u32 hDown, u32 hHeld, touchPosition touch);

	// Island box.
	void DrawIslandBox(void) const;
	void IslandBoxLogic(u32 hDown, u32 hHeld, touchPosition touch);

	// Dresser.
	void DrawDresser(void) const;
	void DresserLogic(u32 hDown, u32 hHeld, touchPosition touch);

	// Storage.
	void DrawStorage(void) const;
	void StorageLogic(u32 hDown, u32 hHeld, touchPosition touch);


	std::shared_ptr<Player> player;
	std::unique_ptr<Item> pockets[16];
	std::unique_ptr<Item> dresser[180];
	std::unique_ptr<Item> islandBox[40];
	std::unique_ptr<Item> storage[360];
	int Selection = 0, selectedItem = 0, Mode = 0, currentBox = 0;

	const std::vector<ButtonType> mainButtons = {
		{15, 34, 130, 48, "ITEM_POCKET"},
		{15, 97, 130, 48, "ITEM_ISLAND"},
		{15, 159, 130, 48, "ITEM_DRESSER"},
		{175, 34, 130, 48, "ITEM_STORAGE"},
		{175, 97, 130, 48, ""},
		{175, 159, 130, 48, ""}
	};

	const std::vector<Structs::ButtonPos> iconSlots = {
		{9, 59, 36, 34},
		{67, 59, 36, 34},
		{125, 59, 36, 34},
		{183, 59, 36, 34},
		{241, 59, 36, 34},

		{9, 117, 36, 34},
		{67, 117, 36, 34},
		{125, 117, 36, 34},
		{183, 117, 36, 34},
		{241, 117, 36, 34}
	};

	const std::vector<Structs::ButtonPos> pocketSlots = {
		{26, 47, 36, 34},
		{64, 47, 36, 34},
		{178, 47, 36, 34},
		{216, 47, 36, 34},

		{26, 85, 36, 34},
		{64, 85, 36, 34},
		{102, 85, 36, 34},
		{140, 85, 36, 34},
		{178, 85, 36, 34},
		{216, 85, 36, 34},

		{26, 123, 36, 34},
		{64, 123, 36, 34},
		{102, 123, 36, 34},
		{140, 123, 36, 34},
		{178, 123, 36, 34},
		{216, 123, 36, 34}
	};
};

#endif