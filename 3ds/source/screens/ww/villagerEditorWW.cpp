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
#include "wwsave.hpp"
#include "wwVillager.hpp"
#include "wwVillagerManagement.hpp"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern WWSave* WWSaveFile;
extern std::vector<std::string> g_villagerDatabase;
extern std::string villagerNameText;
extern int wwCurrentVillager;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void VillagerEditorWW::Draw(void) const
{
	if (editorMode == 0) {
		DrawSubMenu();
	}
}

void VillagerEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
		if (editorMode == 0) {
		SubMenuLogic(hDown, hHeld, touch);
	}
}

void VillagerEditorWW::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("VILLAGER_EDITOR_SUBMENU"), 390);

	Gui::Draw_Rect(40, 142, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 112, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 172, 320, 22, DARKER_COLOR);
	WWVillagerManagement::DrawVillager(WWSaveFile->villagers[wwCurrentVillager]->GetId(), 160, 40);
	Gui::DrawStringCentered(0, 110, 0.9f, WHITE, g_villagerDatabase[WWSaveFile->villagers[wwCurrentVillager]->GetId()], 310);
	Gui::DrawStringCentered(0, 140, 0.9f, WHITE, Lang::get("VILLAGER_ID") + std::to_string(WWSaveFile->villagers[wwCurrentVillager]->GetId()), 310);

	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(villagerButtons[i].x, villagerButtons[i].y, villagerButtons[i].w, villagerButtons[i].h, UNSELECTED_COLOR);
		if (SubSelection == i) {
			GFX::DrawSprite(sprites_pointer_idx, villagerButtons[i].x+130, villagerButtons[i].y+25);
		}
	}
	// Replace.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("REPLACE")))/2-80+17.5, 0.8, WHITE, Lang::get("REPLACE"), 130, 25);
}

void VillagerEditorWW::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}

	// Selection.
	if (hDown & KEY_UP) {
		if(SubSelection > 0)	SubSelection--;
	}
	if (hDown & KEY_DOWN) {
			if(SubSelection < 5)	SubSelection++;
	}
	if (hDown & KEY_RIGHT) {
		if (SubSelection < 3) {
			SubSelection += 3;
		}
	}

	if (hDown & KEY_LEFT) {
		if (SubSelection < 6 && SubSelection > 2) {
			SubSelection -= 3;
		}
	}

	if (hDown & KEY_A) {
		switch (SubSelection) {
			case 0:
				u8 oldID = WWSaveFile->villagers[wwCurrentVillager]->GetId();
				if (Msg::promptMsg("Would you like to replace the Villager?")) {
					u8 newID = WWVillagerManagement::SelectVillager(oldID);
					if (Msg::promptMsg("Set the Villager to:" + g_villagerDatabase[newID])) {
						WWSaveFile->villagers[wwCurrentVillager]->SetId(newID);
					}
				}
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, villagerButtons[0])) {
			u8 oldID = WWSaveFile->villagers[wwCurrentVillager]->GetId();
			if (Msg::promptMsg("Would you like to replace the Villager?")) {
				u8 newID = WWVillagerManagement::SelectVillager(oldID);
				if (Msg::promptMsg("Set the Villager to:" + g_villagerDatabase[newID])) {
					WWSaveFile->villagers[wwCurrentVillager]->SetId(newID);
				}
			}
		}
	}
}

void VillagerEditorWW::DrawBox(void) const {
	Gui::Draw_Rect(40, 162, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 132, 320, 22, DARKER_COLOR);
}