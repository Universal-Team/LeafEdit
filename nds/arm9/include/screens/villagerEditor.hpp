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

#ifndef _LEAFEDIT_VILLAGEREDITOR_HPP
#define _LEAFEDIT_VILLAGEREDITOR_HPP

#include "Item.hpp"
#include "screenCommon.hpp"

#include "structs.hpp"
#include <vector>

class VillagerEditor : public Screen {
public:
	void Draw(void) const override;
	void Logic(u16 hDown, touchPosition touch) override;
private:
	std::unique_ptr<Item> villagerItems[15];

	int villagerMode = 0;
	int Selection = 0;
	int itemSelection = 0;
	std::string returnPersonality() const;
	void DrawSubMenu(void) const;
	void DrawItems(void) const;
	void DrawBox(void) const;
	void updateInfo(void) const;
	void subLogic(u16 hDown, touchPosition touch);
	void ItemLogic(u16 hDown, touchPosition touch);

	std::vector<Structs::ButtonPos> villagerButtons = {
		{20, 30, 88, 32, -1}, // Replace.
		{20, 80, 88, 32, -1}, // Personality.
		{20, 130, 88, 32, -1}, // Items.
		{148, 30, 88, 32, -1}, // ?.
		{148, 80, 88, 32, -1}, // ?.
		{148, 130, 88, 32, -1} // ?.
	};

	std::vector<Structs::ButtonPos> items = {
		{25, 40, 30, 30, -1}, // Furniture 0.
		{65, 40, 30, 30, -1}, // Furniture 1.
		{105, 40, 30, 30, -1}, // Furniture 2.
		{145, 40, 30, 30, -1}, // Furniture 3.
		{185, 40, 30, 30, -1}, // Furniture 4.
		{25, 80, 30, 30, -1}, // Furniture 5.
		{65, 80, 30, 30, -1}, // Furniture 6.
		{105, 80, 30, 30, -1}, // Furniture 7.
		{145, 80, 30, 30, -1}, // Furniture 8.
		{185, 80, 30, 30, -1}, // Furniture 9.
		{25, 140, 30, 30, -1}, // Shirt.
		{65, 140, 30, 30, -1}, // Song.
		{105, 140, 30, 30, -1}, // Wallpaper.
		{145, 140, 30, 30, -1}, // Carpet.
		{185, 140, 30, 30, -1} // Umbrella.
	};
};

#endif