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

#ifndef VILLAGERMANAGEMENT_HPP
#define VILLAGERMANAGEMENT_HPP

#include <3ds.h>
#include <stdio.h>
#include <string>
#include <vector>

// Sorted with ID's!
enum Species {
	Anteater,
	Bear,
	Bird,
	Bull,
	Cat,
	Cub,
	Chicken,
	Cow,
	Alligator,
	Deer,
	Dog,
	Duck,
	Elephant,
	Frog,
	Goat,
	Gorilla,
	Hamster,
	Hippo,
	Horse,
	Kangaroo,
	Koala,
	Lion,
	Monkey,
	Mouse,
	Octopus,
	Ostrich,
	Eagle,
	Penguin,
	Pig,
	Rabbit,
	Rhino,
	Sheep,
	Squirrel,
	Tiger,
	Wolf,
	InvalidSpecies
};

struct Villagers {
	Species species;
	std::string name;
};

extern std::vector<Villagers> villagers;
extern std::vector<Villagers> getSpeciesGroup(std::vector<Villagers> inVector, Species species);

namespace VillagerManagement
{
	void DrawVillager(u16 villagerId, int x, int y, float ScaleX = 1, float ScaleY = 1); // Draw the Villager sprite.
	void LoadVillagerDatabase(int lang); // Load the Villager Names.
}

#endif