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

#include <3ds.h>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

extern Save* SaveFile;
extern std::string villagerNameText;
std::vector<std::string> g_villagerDatabase;

extern C2D_SpriteSheet Villager;
extern C2D_SpriteSheet Villager2;


std::vector<Villagers> villagers = {
	{Anteater},
	{Anteater},

	{Bear},
	{Bear},

	{Bird},
	{Bird},

	{Bull},
	{Bull},

	{Cat},
	{Cat},

	{Cub},
	{Cub},

	{Chicken},
	{Chicken},

	{Cow},
	{Cow},

	{Alligator},
	{Alligator},

	{Deer},
	{Deer},

	{Dog},
	{Dog},

	{Duck},
	{Duck},

	{Elephant},
	{Elephant},

	{Frog},
	{Frog},

	{Goat},
	{Goat},

	{Gorilla},
	{Gorilla},

	{Hamster},
	{Hamster},

	{Hippo},
	{Hippo},

	{Horse},
	{Horse},

	{Kangaroo},
	{Kangaroo},

	{Koala},
	{Koala},

	{Lion},
	{Lion},

	{Monkey},
	{Monkey},

	{Mouse},
	{Mouse},

	{Octopus},
	{Octopus},

	{Ostrich},
	{Ostrich},

	{Eagle},
	{Eagle},

	{Penguin},
	{Penguin},

	{Pig},
	{Pig},

	{Rabbit},
	{Rabbit},

	{Rhino},
	{Rhino},

	{Sheep},
	{Sheep},

	{Squirrel},
	{Squirrel},

	{Tiger},
	{Tiger},

	{Wolf},
	{Wolf},
};

std::vector<Villagers> getSpeciesGroup(std::vector<Villagers> inVector, Species species) {
	std::vector<Villagers> outVector;

	for(uint i=0;i<inVector.size();i++) {
		if(inVector[i].species == species)    outVector.push_back(inVector[i]);
	}

	return outVector;
}

// example on, how to use it. (Will be deleted, when the Villager Editor is done.)
void anotherFunction(void) {
	std::vector<Villagers> speciesGroup = getSpeciesGroup(villagers, Bear);
}

int getSpecies(int id) {
	if(id <= 6) {
		return Anteater;
	} else if(id <= 20) {
		return Bear;
	} else if(id <= 34) {
		return Bird;
	} else if(id <= 40) {
		return Bull;
	} else if(id <= 63) {
		return Cat;
	} else if(id <= 79) {
		return Cub;
	} else if(id <= 88) {
		return Chicken;
	} else if(id <= 92) {
		return Cow;
	} else if(id <= 99) {
		return Alligator;
	} else if(id <= 110) {
		return Deer;
	} else if(id <= 126) {
		return Dog;
	} else if(id <= 143) {
		return Duck;
	} else if(id <= 154) {
		return Elephant;
	} else if(id <= 172) {
		return Frog;
	} else if(id <= 179) {
		return Goat;
	} else if(id <= 189) {
		return Gorilla;
	} else if(id <= 197) {
		return Hamster;
	} else if(id <= 205) {
		return Hippo;
	} else if(id <= 221) {
		return Horse;
	} else if(id <= 230) {
		return Koala;
	} else if(id <= 238) {
		return Kangaroo;
	} else if(id <= 245) {
		return Lion;
	} else if(id <= 253) {
		return Monkey;
	} else if(id <= 268) {
		return Mouse;
	} else if(id <= 272) {
		return Octopus;
	} else if(id <= 282) {
		return Ostrich;
	} else if(id <= 291) {
		return Eagle;
	} else if(id <= 304) {
		return Penguin;
	} else if(id <= 320) {
		return Pig;
	} else if(id <= 341) {
		return Rabbit;
	} else if(id <= 347) {
		return Rhino;
	} else if(id <= 360) {
		return Sheep;
	} else if(id <= 380) {
		return Squirrel;
	} else if(id <= 387) {
		return Tiger;
	} else if(id <= 398) {
		return Wolf;
	} else {
		return InvalidSpecies;
	}
}

// Dunno if needed. :shrug:
int getStartID(int id) {
	if(id <= 6) {
		return 0;
	} else if(id <= 20) {
		return 7;
	} else if(id <= 34) {
		return 21;
	} else if(id <= 40) {
		return 35;
	} else if(id <= 63) {
		return 41;
	} else if(id <= 79) {
		return 64;
	} else if(id <= 88) {
		return 80;
	} else if(id <= 92) {
		return 89;
	} else if(id <= 99) {
		return 93;
	} else if(id <= 110) {
		return 100;
	} else if(id <= 126) {
		return 111;
	} else if(id <= 143) {
		return 127;
	} else if(id <= 154) {
		return 144;
	} else if(id <= 172) {
		return 155;
	} else if(id <= 179) {
		return 173;
	} else if(id <= 189) {
		return 180;
	} else if(id <= 197) {
		return 190;
	} else if(id <= 205) {
		return 198;
	} else if(id <= 221) {
		return 206;
	} else if(id <= 230) {
		return 222;
	} else if(id <= 238) {
		return 231;
	} else if(id <= 245) {
		return 239;
	} else if(id <= 253) {
		return 246;
	} else if(id <= 268) {
		return 254;
	} else if(id <= 272) {
		return 269;
	} else if(id <= 282) {
		return 273;
	} else if(id <= 291) {
		return 283;
	} else if(id <= 304) {
		return 292;
	} else if(id <= 320) {
		return 305;
	} else if(id <= 341) {
		return 321;
	} else if(id <= 347) {
		return 342;
	} else if(id <= 360) {
		return 348;
	} else if(id <= 380) {
		return 361;
	} else if(id <= 387) {
		return 381;
	} else if(id <= 398) {
		return 388;
	} else {
		return 399;
	}
}

// Dunno if needed. :shrug:
int getEndID(int id) {
	if(id <= 6) {
		return 6;
	} else if(id <= 20) {
		return 20;
	} else if(id <= 34) {
		return 34;
	} else if(id <= 40) {
		return 40;
	} else if(id <= 63) {
		return 63;
	} else if(id <= 79) {
		return 79;
	} else if(id <= 88) {
		return 88;
	} else if(id <= 92) {
		return 92;
	} else if(id <= 99) {
		return 99;
	} else if(id <= 110) {
		return 110;
	} else if(id <= 126) {
		return 126;
	} else if(id <= 143) {
		return 143;
	} else if(id <= 154) {
		return 154;
	} else if(id <= 172) {
		return 172;
	} else if(id <= 179) {
		return 179;
	} else if(id <= 189) {
		return 189;
	} else if(id <= 197) {
		return 197;
	} else if(id <= 205) {
		return 205;
	} else if(id <= 221) {
		return 221;
	} else if(id <= 230) {
		return 230;
	} else if(id <= 238) {
		return 238;
	} else if(id <= 245) {
		return 245;
	} else if(id <= 253) {
		return 253;
	} else if(id <= 268) {
		return 268;
	} else if(id <= 272) {
		return 272;
	} else if(id <= 282) {
		return 282;
	} else if(id <= 291) {
		return 291;
	} else if(id <= 304) {
		return 304;
	} else if(id <= 320) {
		return 320;
	} else if(id <= 341) {
		return 341;
	} else if(id <= 347) {
		return 347;
	} else if(id <= 360) {
		return 360;
	} else if(id <= 380) {
		return 380;
	} else if(id <= 387) {
		return 387;
	} else if(id <= 398) {
		return 398;
	} else {
		return 399;
	}
}

std::string villagerLanguages[] = {"de", "en", "es", "fr", "it", "jp", "lt", "pt"};

void VillagerManagement::LoadVillagerDatabase(int lang) {
	g_villagerDatabase.clear();

	std::string currentLine;
		std::ifstream villagerDatabase("romfs:/lang/" + villagerLanguages[lang] + "/villager.txt", std::ifstream::in);


			// This would be the real Function one time.
//	while (std::getline(villagerDatabase, currentLine)) {
//		villagers.name = currentLine;
//	}

	while (std::getline(villagerDatabase, currentLine)) {
		// TODO: Read default things like personality, catchphrases, & furniture.
		g_villagerDatabase.push_back(currentLine);
	}

	villagerDatabase.close();
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