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

#ifndef VILLAGERVIEWER_HPP
#define VILLAGERVIEWER_HPP

#include "screenCommon.hpp"

#include "structs.hpp"
#include <vector>

class VillagerViewer : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u16 hDown, touchPosition touch) override;
private:
	int villagerViewerSprite = 0;
	int villagerMode = 0;

	// Draw the current Villager Screen.
	void DrawVillager(void) const;
	void VillagerLogic(u16 hDown);

	// Villager Watching List.
	void DrawVillagerList(void) const;
	void VillagerListLogic(u16 hDown, touchPosition touch);

	// Current Villager and Title Draw.
	void DrawCurrentVillager(void) const;
	void DrawBox(void) const;
};

#endif