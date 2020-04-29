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

#include "Sav.hpp"
#include "screenCommon.hpp"
#include "spriteManagement.hpp"
#include "villagerViewerWW.hpp"

extern std::vector<std::string> g_villagerDatabase;
extern bool touching(touchPosition touch, ButtonType button);

std::unique_ptr<Villager> villagerWW;
extern std::shared_ptr<Sav> save;

VillagerViewerWW::VillagerViewerWW() {
	villagerWW = save->villager(Selection);
}

void VillagerViewerWW::Draw(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - VillagerViewer", 395);
	SpriteManagement::DrawVillager(villagerWW->id(), 160, 60);
	Gui::DrawStringCentered(0, 130, 0.9f, WHITE, "VillagerName: " + g_villagerDatabase[villagerWW->id()], 395);
	GFX::DrawBottom();
}

void VillagerViewerWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
	if ((hDown & KEY_RIGHT) || (hDown & KEY_R)) {
		if(Selection < save->maxVillager() -1) {
			Selection++;
			villagerWW = save->villager(Selection);
		}
	} else if ((hDown & KEY_LEFT) || (hDown & KEY_L)) {
		if (Selection > 0) {
			Selection--;
			villagerWW = save->villager(Selection);
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}