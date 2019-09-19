	/*
*   This file is part of LeafEdit
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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

#include "core/villagerManagement.hpp"

#include "core/save/offsets.h"
#include "core/save/save.h"
#include "core/save/villager.h"

#include "gui/screens/screenCommon.hpp"

extern Save* SaveFile;
extern std::vector<std::string> g_villagerDatabase;
extern std::string villagerNameText;

extern C2D_SpriteSheet Villager;
extern C2D_SpriteSheet Villager2;

struct Villagers {
    int species; // or std::string?
    std::string name;
};

std::vector<Villagers> villagersData = {
    {12},  // vector index is the id, put the species and any other data you might need here, load the names from lang files later
    {13},
};

std::vector<Villagers> getSpeciesGroup(std::vector<Villagers> inVector, int species) {
    std::vector<Villagers> outVector;

    for(uint i=0;i<inVector.size();i++) {
        if(inVector[i].species == species)    outVector.push_back(inVector[i]);
    }

    return outVector;
}

// Draw the Villager sprite.
void VillagerManagement::DrawVillager(u16 villagerId, int x, int y) {
	if (villagerId > 399) {
		villagerId = 399;
	}

	if (villagerId < 200) {
		DrawSprite(Villager, villagerId, x, y);
	}
	else {
		DrawSprite(Villager2, villagerId - 200, x, y);
	}
}