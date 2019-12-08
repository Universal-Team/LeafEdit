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

#include "core/management/villagerManagement.hpp"

#include "core/save/offsets.h"
#include "core/save/save.h"
#include "core/save/villager.h"

#include "gui/keyboard.hpp"

#include "gui/screens/screenCommon.hpp"
#include "gui/screens/villagerEditor.hpp"
#include "gui/screens/villagerViewer.hpp"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern Save* SaveFile;
extern std::vector<std::string> g_villagerDatabase;
std::string villagerNameText = "";
u16 currentVillager = 0;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

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
	std::string villagerID = Lang::get("VILLAGER_ID");
	for (int i = 0; i < 399; i++) {
		if (villagerViewerSprite == i) {
			VillagerManagement::DrawVillager(i, 160, 60);
			villagerNameText = g_villagerDatabase[i];
			villagerID += std::to_string(villagerViewerSprite);
		}
	}
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, TXTCOLOR, villagerID.c_str(), 320);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, TXTCOLOR, villagerNameText.c_str(), 320);
	Gui::DrawBottom();
	Gui::sprite(0, sprites_search_idx, 290, 3);
}

void VillagerViewer::VillagerListLogic(u32 hDown, u32 hHeld, touchPosition touch) {

	// Switch current Villager.
	if (hDown & KEY_DOWN) {
		villagerViewerSprite++;
		if (villagerViewerSprite > 398)	villagerViewerSprite = 0;
	} else if (hDown & KEY_UP) {
		villagerViewerSprite--;
		if (villagerViewerSprite < 0)	villagerViewerSprite = 398;

	} else if (hDown & KEY_RIGHT) {
		villagerViewerSprite += 10;
		if (villagerViewerSprite > 389)	villagerViewerSprite = 0;

	} else if (hDown & KEY_LEFT) {
		villagerViewerSprite -= 10;
		if (villagerViewerSprite < 0)	villagerViewerSprite = 398;
	}

	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		villagerMode = 0;
	}

	if (hDown & KEY_TOUCH && touching(touch, search[0])) {
		villagerViewerSprite = Input::getu16(3, 398);
	}
}



void VillagerViewer::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (villagerMode == 0) {
		VillagerLogic(hDown, hHeld);
	} else if (villagerMode == 1) {
		VillagerListLogic(hDown, hHeld, touch);
	}
}

void VillagerViewer::VillagerLogic(u32 hDown, u32 hHeld) {
	// Switch to the Villager Editor Screen.
	if (hDown & KEY_A) {
		Gui::setScreen(std::make_unique<VillagerEditor>());
		// Set the Screen to the Editor Class. -> To-Do.
	}

	// Switch current Villager.
	if (hDown & KEY_R) {
		if(currentVillager < 9)	currentVillager++;
	} else if (hDown & KEY_L) {
		if(currentVillager > 0)	currentVillager--;
	}

	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_X) {
		villagerMode = 1;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Press L/R to switch the Villager.\nPress A to open the Villager Editor. (Not implemented yet)\nPress B to exit from this Screen.\nPress X to open the Villager List.");
	}
}

void VillagerViewer::DrawVillager(void) const {
	Gui::DrawTop();
	DrawBox();
	std::string villagerID = Lang::get("VILLAGER_ID");
	DrawCurrentVillager();
	for (u16 i = 0; i < 10; i++) {
		if (currentVillager == i) {
			u16 villager = Save::Instance()->villagers[i]->GetId();
			VillagerManagement::DrawVillager(villager, 160, 60);
			villagerNameText = g_villagerDatabase[villager];
			villagerID += std::to_string(Save::Instance()->villagers[i]->GetId());
		}
	}
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, TXTCOLOR, villagerID.c_str(), 320);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, TXTCOLOR, villagerNameText.c_str(), 320);
	Gui::DrawBottom();
}

// This will draw the current Villager and Title for the Villager Viewer Screen.
void VillagerViewer::DrawCurrentVillager(void) const
{
	std::string Title;
	Title += "LeafEdit - ";
	Title += Lang::get("VILLAGER_VIEWER");

	std::string activeVillager = Lang::get("CURRENT_VILLAGER");

	for (int i = 0; i < 10; i++) {
		if (currentVillager == i) {
			activeVillager += std::to_string(i+1);
		}
	}

	Gui::DrawString((400-Gui::GetStringWidth(0.8f, activeVillager.c_str()))/2, 212, 0.8f, TXTCOLOR, activeVillager.c_str(), 400);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, TXTCOLOR, Title.c_str(), 400);
}

void VillagerViewer::DrawBox(void) const {
	Gui::Draw_Rect(40, 162, 320, 22, BGBOTCOLOR);
	Gui::Draw_Rect(40, 132, 320, 22, BGBOTCOLOR);
}