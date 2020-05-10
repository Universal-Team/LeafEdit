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

#ifndef VILLAGERVIEWERNL_HPP
#define VILLAGERVIEWERNL_HPP

#include "common.hpp"
#include "structs.hpp"

#include <vector>

class VillagerViewerNL : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	VillagerViewerNL();
private:
	int Selection = 0;
	u16 ID[10];
	u16 viewerIndex = 0;

	std::vector<Structs::ButtonPos> villagers = {
		{15, 50, 48, 48},
		{73, 50, 48, 48},
		{131, 50, 48, 48},
		{189, 50, 48, 48},
		{247, 50, 48, 48},
		
		{15, 125, 48, 48},
		{73, 125, 48, 48},
		{131, 125, 48, 48},
		{189, 125, 48, 48},
		{247, 125, 48, 48}
	};
};

#endif