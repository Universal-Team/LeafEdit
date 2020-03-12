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

#include "input.hpp"
#include "msg.hpp"
#include "villagerManagement.hpp"
#include "villagerEditor.hpp"
#include "wwoffsets.hpp"
#include "wwsave.hpp"
#include "wwVillager.hpp"


extern WWSave* SaveFile;
extern std::vector<std::string> g_villagerDatabase;
extern std::vector<std::string> g_personality;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern int currentVillager;

void VillagerEditor::Draw(void) const {
	if (villagerMode == 0) {
		DrawSubMenu();
	}
}

void VillagerEditor::DrawSubMenu(void) const {
	Gui::DrawTop();
	DrawBox();

	VillagerManagement::DrawVillager(SaveFile->villagers[currentVillager]->ID, 100, 40);
	printTextCentered(g_villagerDatabase[SaveFile->villagers[currentVillager]->ID], 0, 100, true, true);
	printTextCentered("Villager ID: " + std::to_string(SaveFile->villagers[currentVillager]->ID), 0, 130, true, true);
	printTextCentered("Personality: " + VillagerManagement::returnPersonality(currentVillager), 0, 160, true, true);

	Gui::DrawBottom();
	for (int i = 0; i < 6; i++) {
		drawRectangle(villagerButtons[i].x, villagerButtons[i].y, villagerButtons[i].w, villagerButtons[i].h, DARK_GREEN, DARK_GREEN, false, true);
	}
	printTextCentered("Replace", -64, 40, false, true);
	printTextCentered("Personality", -64, 90, false, true);
}

void VillagerEditor::Logic(u16 hDown, touchPosition touch) {
	Gui::updatePointer(villagerButtons[Selection].x+60, villagerButtons[Selection].y+12);

	// Selection.
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
		selected = true;
	}
	
	if (hDown & KEY_DOWN) {
		if(Selection < 5)	Selection++;
		selected = true;
	}

	if (hDown & KEY_RIGHT) {
		if (Selection < 3) {
			Selection += 3;
			selected = true;
		}
	}

	if (hDown & KEY_LEFT) {
		if (Selection < 6 && Selection > 2) {
			Selection -= 3;
			selected = true;
		}
	}

	if (hDown & KEY_A) {
		switch (Selection) {
			case 0:
				SaveFile->villagers[currentVillager]->ID = (u8)Gui::selectList(SaveFile->villagers[currentVillager]->ID, g_villagerDatabase);
				Gui::DrawScreen();
				Gui::showPointer();
				selected = true;
				break;
			case 1:
				SaveFile->villagers[currentVillager]->personality = (u8)Gui::selectList(SaveFile->villagers[currentVillager]->personality, g_personality);
				Gui::DrawScreen();
				Gui::showPointer();
				selected = true;
				break;
		}
	}


	if (hDown & KEY_B) {
		Gui::screenBack();
		Gui::DrawScreen();
		Gui::hidePointer();
		selected = true;
		return;
	}
}

void VillagerEditor::DrawBox(void) const {
	drawRectangle(20, 132, 176, 50, DARK_GREEN, true, true);
	drawRectangle(20, 162, 176, 50, DARK_GREEN, true, true);
}