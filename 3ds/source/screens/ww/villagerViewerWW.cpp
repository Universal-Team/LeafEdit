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



#include "keyboard.hpp"
#include "screenCommon.hpp"
#include "villagerEditorWW.hpp"
#include "villagerViewerWW.hpp"
#include "wwsave.hpp"
#include "wwVillager.hpp"
#include "wwVillagerManagement.hpp"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern WWSave* WWSaveFile;
extern std::vector<std::string> g_villagerDatabase;
std::string wwVillagerNameText = "";
int wwCurrentVillager = 0;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void VillagerViewerWW::Draw(void) const
{
	if (villagerMode == 0) {
		DrawVillager();
	} else if (villagerMode == 1) {
		DrawVillagerList();
	}
}

void VillagerViewerWW::DrawVillagerList(void) const {
	GFX::DrawTop();
	DrawBox();
	std::string villagerID = Lang::get("VILLAGER_ID");
	for (int i = 0; i < 149; i++) {
		if (villagerViewerSprite == i) {
			WWVillagerManagement::DrawVillager(i, 160, 60);
			wwVillagerNameText = g_villagerDatabase[i];
			villagerID += std::to_string(villagerViewerSprite);
		}
	}

	Gui::DrawStringCentered(0, 160, 0.9f, WHITE, villagerID, 310);
	Gui::DrawStringCentered(0, 130, 0.9f, WHITE, wwVillagerNameText, 310);
	GFX::DrawBottom();
	GFX::DrawSprite(sprites_search_idx, 290, 3);
}

void VillagerViewerWW::VillagerListLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("B_BACK"));
	}

	// Switch current Villager.
	if (hDown & KEY_DOWN) {
		villagerViewerSprite++;
		if (villagerViewerSprite > 149)	villagerViewerSprite = 0;
	} else if (hDown & KEY_UP) {
		villagerViewerSprite--;
		if (villagerViewerSprite < 0)	villagerViewerSprite = 149;

	} else if (hDown & KEY_RIGHT) {
		villagerViewerSprite += 10;
		if (villagerViewerSprite > 139)	villagerViewerSprite = 0;

	} else if (hDown & KEY_LEFT) {
		villagerViewerSprite -= 10;
		if (villagerViewerSprite < 0)	villagerViewerSprite = 149;
	}

	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		villagerMode = 0;
	}

	if (hDown & KEY_TOUCH && touching(touch, search[0])) {
		villagerViewerSprite = Input::handleu8(3, Lang::get("ENTER_VILLAGER_ID"), 149, villagerViewerSprite);
	}
}



void VillagerViewerWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (villagerMode == 0) {
		VillagerLogic(hDown, hHeld);
	} else if (villagerMode == 1) {
		VillagerListLogic(hDown, hHeld, touch);
	}
}

void VillagerViewerWW::VillagerLogic(u32 hDown, u32 hHeld) {
	// Switch to the Villager Editor Screen.
	if (hDown & KEY_A) {
		Gui::setScreen(std::make_unique<VillagerEditorWW>());
		// Set the Screen to the Editor Class. -> To-Do.
	}
	
	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("B_BACK") + "\n" + Lang::get("LR_SWITCH_PAGE"));
	}

	// Switch current Villager.
	if (hDown & KEY_R) {
		if(wwCurrentVillager < 7)	wwCurrentVillager++;
	} else if (hDown & KEY_L) {
		if(wwCurrentVillager > 0)	wwCurrentVillager--;
	}

	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_X) {
		villagerMode = 1;
	}
}

void VillagerViewerWW::DrawVillager(void) const {
	GFX::DrawTop();
	DrawBox();
	std::string villagerID = Lang::get("VILLAGER_ID");
	DrawCurrentVillager();
	for (int i = 0; i < 8; i++) {
		if (wwCurrentVillager == i) {
			u8 villager = WWSaveFile->villagers[i]->GetId();
			WWVillagerManagement::DrawVillager(villager, 160, 60);
			if (WWSaveFile->villagers[i]->GetId() > 149) {
				wwVillagerNameText = g_villagerDatabase[150];
			} else {
				wwVillagerNameText = g_villagerDatabase[villager];
			}
			villagerID += std::to_string(WWSaveFile->villagers[i]->GetId());
		}
	}

	Gui::DrawStringCentered(0, 160, 0.9f, WHITE, villagerID, 310);
	Gui::DrawStringCentered(0, 130, 0.9f, WHITE, wwVillagerNameText, 310);
	GFX::DrawBottom();
}

// This will draw the current Villager and Title for the Villager Viewer Screen.
void VillagerViewerWW::DrawCurrentVillager(void) const
{
	std::string activeVillager = Lang::get("CURRENT_VILLAGER");
	for (int i = 0; i < 8; i++) {
		if (wwCurrentVillager == i) {
			activeVillager += std::to_string(i+1);
		}
	}
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("VILLAGER_VIEWER"), 390);
	Gui::DrawStringCentered(0, 212, 0.9f, WHITE, activeVillager, 390);
}

void VillagerViewerWW::DrawBox(void) const {
	Gui::Draw_Rect(40, 162, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 132, 320, 22, DARKER_COLOR);
}