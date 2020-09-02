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

#ifndef _LEAFEDIT_ITEM_EDITOR_WW_HPP
#define _LEAFEDIT_ITEM_EDITOR_WW_HPP

#include "common.hpp"
#include "coreUtils.hpp"
#include "Item.hpp"
#include "screenCommon.hpp"

#include <vector>

class ItemEditorWW : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	ItemEditorWW(std::unique_ptr<Player> &refPlayer);
	~ItemEditorWW(void);
private:
	/* Sub Menu. */
	void DrawSubMenu(void) const;
	void SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);

	/* Pocket. */
	void DrawPocket(void) const;
	void PocketLogic(u32 hDown, u32 hHeld, touchPosition touch);

	/* Dresser. */
	void DrawDresser(void) const;
	void DresserLogic(u32 hDown, u32 hHeld, touchPosition touch);


	std::unique_ptr<Player> &player;
	std::unique_ptr<Item> pockets[15];
	std::unique_ptr<Item> dresser[90];
	int Selection = 0, selectedItem = 0, Mode = 0, currentBox = 0;

	const std::vector<ButtonType> mainButtons = {
		{15, 34, 130, 48, "ITEM_POCKET"},
		{15, 97, 130, 48, "ITEM_DRESSER"}
	};

	const std::vector<Structs::ButtonPos> iconSlots = {
		{9, 59, 36, 36},
		{67, 59, 36, 36},
		{125, 59, 36, 36},
		{183, 59, 36, 36},
		{241, 59, 36, 36},

		{9, 117, 36, 36},
		{67, 117, 36, 36},
		{125, 117, 36, 36},
		{183, 117, 36, 36},
		{241, 117, 36, 36}
	};

	const std::vector<Structs::ButtonPos> pocketSlots = {
		{25, 40, 30, 30},
		{65, 40, 30, 30},
		{105, 40, 30, 30},
		{145, 40, 30, 30},
		{185, 40, 30, 30},
		{25, 80, 30, 30},
		{65, 80, 30, 30},
		{105, 80, 30, 30},
		{145, 80, 30, 30},
		{185, 80, 30, 30},
		{25, 140, 30, 30},
		{65, 140, 30, 30},
		{105, 140, 30, 30},
		{145, 140, 30, 30},
		{185, 140, 30, 30}
	};
};

#endif