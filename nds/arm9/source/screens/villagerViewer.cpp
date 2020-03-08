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
#include "villagerEditor.hpp"
#include "villagerManagement.hpp"
#include "villagerViewer.hpp"
#include "wwsave.hpp"
#include "wwVillager.hpp"


extern WWSave* SaveFile;
extern std::vector<std::string> g_villagerDatabase;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
int currentVillager = 0;

void VillagerViewer::Draw(void) const
{
	if (villagerMode == 0) {
		DrawVillager();
	} else if (villagerMode == 1) {
		DrawVillagerList();
	}
}

void VillagerViewer::DrawVillagerList(void) const {
	Gui::DrawTop();
	DrawBox();
	VillagerManagement::DrawVillager(villagerViewerSprite, 100, 60);
	printTextCentered("Villager ID: " + std::to_string(villagerViewerSprite), 0, 160, true, true);
	printTextCentered(g_villagerDatabase[villagerViewerSprite], 0, 130, true, true);
	Gui::DrawBottom();
}

void VillagerViewer::VillagerListLogic(u16 hDown, touchPosition touch) {
	// Switch current Villager.
	if (hDown & KEY_DOWN) {
		Gui::clearScreen(true, true);
		villagerViewerSprite++;
		if (villagerViewerSprite > 149)	villagerViewerSprite = 0;
	} else if (hDown & KEY_UP) {
		Gui::clearScreen(true, true);
		villagerViewerSprite--;
		if (villagerViewerSprite < 0)	villagerViewerSprite = 149;

	} else if (hDown & KEY_RIGHT) {
		Gui::clearScreen(true, true);
		villagerViewerSprite += 10;
		if (villagerViewerSprite > 139)	villagerViewerSprite = 0;

	} else if (hDown & KEY_LEFT) {
		Gui::clearScreen(true, true);
		villagerViewerSprite -= 10;
		if (villagerViewerSprite < 0)	villagerViewerSprite = 149;
	}

	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		Gui::clearScreen(true, true);
		villagerMode = 0;
	}
}


void VillagerViewer::Logic(u16 hDown, touchPosition touch) {
	if (villagerMode == 0) {
		VillagerLogic(hDown);
	} else if (villagerMode == 1) {
		VillagerListLogic(hDown, touch);
	}
}

void VillagerViewer::VillagerLogic(u16 hDown) {
	// Switch to the Villager Editor Screen.
	if (hDown & KEY_A) {
		Gui::clearScreen(false, true);
		Gui::clearScreen(true, true);
		Gui::setScreen(std::make_unique<VillagerEditor>());
	}

	// Switch current Villager.
	if (hDown & KEY_R) {
		Gui::clearScreen(true, true);
		if(currentVillager < 7)	currentVillager++;
	} else if (hDown & KEY_L) {
		Gui::clearScreen(true, true);
		if(currentVillager > 0)	currentVillager--;
	}

	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		Gui::clearScreen(false, true);
		Gui::clearScreen(true, true);
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_X) {
		Gui::clearScreen(true, true);
		villagerMode = 1;
	}
}

void VillagerViewer::DrawVillager(void) const {
	Gui::DrawTop();
	DrawBox();
	DrawCurrentVillager();
	VillagerManagement::DrawVillager(SaveFile->villagers[currentVillager]->GetId(), 100, 60);
	printTextCentered(g_villagerDatabase[SaveFile->villagers[currentVillager]->GetId()], 0, 130, true, true);
	printTextCentered("Villager ID: " + std::to_string(SaveFile->villagers[currentVillager]->GetId()), 0, 160, true, true);
	Gui::DrawBottom();
}

// This will draw the current Villager and Title for the Villager Viewer Screen.
void VillagerViewer::DrawCurrentVillager(void) const
{
	printTextCentered("LeafEdit - Villager Viewer", 0, 2, true, true);
	printTextCentered("Current Villager: " + std::to_string(currentVillager+1), 0, 180, true, true);
}

void VillagerViewer::DrawBox(void) const {
	drawRectangle(20, 132, 176, 50, DARK_GREEN, true, true);
	drawRectangle(20, 162, 176, 50, DARK_GREEN, true, true);
}