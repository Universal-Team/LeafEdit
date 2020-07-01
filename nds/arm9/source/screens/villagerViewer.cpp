/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
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

#include "graphicManagement.hpp"
#include "input.hpp"
#include "msg.hpp"
#include "villagerEditor.hpp"
#include "villagerViewer.hpp"
#include "Sav.hpp"


extern std::shared_ptr<Sav> save;
extern std::vector<std::string> g_villagerDatabase;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
std::unique_ptr<Villager> villager;
int currentVillager = 0;

VillagerViewer::VillagerViewer() {
	villager = save->villager(0);
}

void VillagerViewer::Draw(void) const {
	if (villagerMode == 0) {
		DrawVillager();
	} else if (villagerMode == 1) {
		DrawVillagerList();
	}
}

void VillagerViewer::DrawVillagerList(void) const {
	Gui::DrawTop(true);
	DrawBox();
	GraphicManagement::DrawVillager(villagerViewerSprite, 100, 60);
	printTextCentered("Villager ID: " + std::to_string(villagerViewerSprite), 0, 160, true, true);
	printTextCentered(g_villagerDatabase[villagerViewerSprite], 0, 130, true, true);
	Gui::DrawBottom(true);
}

void VillagerViewer::VillagerListLogic(u16 hDown, touchPosition touch) {
	u16 held = keysDownRepeat();

	// Switch current Villager.
	if (held & KEY_DOWN) {
		villagerViewerSprite++;
		if (villagerViewerSprite > 149)	villagerViewerSprite = 0;
		Gui::DrawScreen();
	} else if (held & KEY_UP) {
		villagerViewerSprite--;
		if (villagerViewerSprite < 0)	villagerViewerSprite = 149;
		Gui::DrawScreen();

	} else if (held & KEY_RIGHT) {
		villagerViewerSprite += 10;
		if (villagerViewerSprite > 139)	villagerViewerSprite = 0;
		Gui::DrawScreen();

	} else if (held & KEY_LEFT) {
		villagerViewerSprite -= 10;
		if (villagerViewerSprite < 0)	villagerViewerSprite = 149;
		Gui::DrawScreen();
	}

	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		villagerMode = 0;
		Gui::DrawScreen();
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
	u16 held = keysDownRepeat();

	// Switch to the Villager Editor Screen.
	if (hDown & KEY_A) {
		// Only allow Villager Editor if Villager exist.
		if (villager->exist()) {
			Gui::setScreen(std::make_unique<VillagerEditor>());
			Gui::DrawScreen();
			Gui::showPointer();
			selected = true;
		}
	}

	// Switch current Villager.
	if (held & KEY_R) {
		if(currentVillager < 7) {
			currentVillager++;
			villager = save->villager(currentVillager);
			Gui::DrawScreen();
		}
	} else if (held & KEY_L) {
		if(currentVillager > 0) {
			currentVillager--;
			villager = save->villager(currentVillager);
			Gui::DrawScreen();
		}
	}

	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		Gui::screenBack();
		villager = nullptr; // Reset to nullptr.
		// Display Save icon.
		setSpriteVisibility(Gui::saveID, false, true);
		setSpritePosition(Gui::saveID, false, 225, 172);
		updateOam();
		Gui::DrawScreen();
		Gui::showPointer();
		selected = true;
		return;
	}

	if (hDown & KEY_X) {
		villagerMode = 1;
		Gui::DrawScreen();
	}
}

void VillagerViewer::DrawVillager(void) const {
	Gui::DrawTop(true);
	DrawBox();
	DrawCurrentVillager();
	GraphicManagement::DrawVillager(villager->id(), 100, 60);
	printTextCentered(g_villagerDatabase[villager->id()], 0, 130, true, true);
	printTextCentered("Villager ID: " + std::to_string(villager->id()), 0, 160, true, true);
	Gui::DrawBottom(true);
}

// This will draw the current Villager and Title for the Villager Viewer Screen.
void VillagerViewer::DrawCurrentVillager(void) const {
	printTextCentered("LeafEdit - Villager Viewer", 0, 2, true, true);
	printTextCentered("Current Villager: " + std::to_string(currentVillager+1), 0, 180, true, true);
}

void VillagerViewer::DrawBox(void) const {
	drawRectangle(20, 132, 176, 50, GRAY, true, true);
	drawRectangle(20, 162, 176, 50, GRAY, true, true);
}