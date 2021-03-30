/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2021 Universal-Team
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

#ifndef _LEAFEDIT_VILLAGER_VIEWER_WW_HPP
#define _LEAFEDIT_VILLAGER_VIEWER_WW_HPP

#include "common.hpp"
#include "structs.hpp"
#include "Villager.hpp"

#include <vector>

class VillagerViewerWW : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	VillagerViewerWW();
private:
	std::unique_ptr<Villager> villager;
	int Selection = 0, viewerIndex = 0;
	u16 ID[8];
	void update();

	const std::vector<Structs::ButtonPos> villagers = {
		{ 20, 50, 48, 48 },
		{ 90, 50, 48, 48 },
		{ 160, 50, 48, 48 },
		{ 230, 50, 48, 48 },

		{ 20, 120, 48, 48 },
		{ 90, 120, 48, 48 },
		{ 160, 120, 48, 48 },
		{ 230, 120, 48, 48 }
	};
};

#endif