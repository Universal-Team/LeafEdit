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

#include "common.hpp"
#include "spriteManagement.hpp"
#include "villagerSelection.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern std::vector<std::string> g_villagerDatabase;
extern std::array<int, 333> nlVillagerIndex;
extern const std::string getVillagerName(int index);

void VillagerSelection::DrawOverlayTop(void) const {
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "Select your wanted Villager.", 395, 0, font);
	SpriteManagement::DrawVillager(this->selection, 165, 80);
	if (this->save != SaveType::NL) {
		Gui::DrawStringCentered(0, 140, 0.9f, WHITE, "Villager Name: " + g_villagerDatabase[this->selection], 395, 0, font);
	} else {
		Gui::DrawStringCentered(0, 140, 0.9f, WHITE, "Villager Name: " + getVillagerName(this->selection), 395, 0, font);
	}

	Gui::DrawStringCentered(0, 160, 0.9f, WHITE, "Villager ID: " + std::to_string(this->selection), 395, 0, font);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, std::to_string(this->selection+1) + " | " + std::to_string(this->maxSelection+1), 400, 0, font);
}


void VillagerSelection::DrawOverlayBottom(void) const { }

void VillagerSelection::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_RIGHT) {
		if (this->selection < this->maxSelection)	this->selection++;
	}

	if (hDown & KEY_LEFT) {
		if (this->selection > 0)	this->selection--;
	}

	if (hHeld & KEY_R) {
		if (this->selection < this->maxSelection)	this->selection++;
	}

	if (hHeld & KEY_L) {
		if (this->selection > 0)	this->selection--;
	}

	if (hDown & KEY_A) {
		this->villager->id(this->selection);
		this->isUsed = false;
	}

	if (hDown & KEY_B) {
		this->isUsed = false;
	}
}