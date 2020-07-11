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

#include "Sav.hpp"
#include "screenCommon.hpp"
#include "spriteManagement.hpp"
#include "villagerEditorNL.hpp"
#include "villagerViewerNL.hpp"

extern std::vector<std::string> g_villagerDatabase;
extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);
extern std::shared_ptr<Sav> save;
// Bring that to other screens too.
extern SaveType savesType;

extern std::array<int, 333> nlVillagerIndex;

VillagerViewerNL::VillagerViewerNL() {
	this->update();
}

void VillagerViewerNL::update() {
	// Get all Villager ID's for display.
	for (int i = 0; i < 10; i++) {
		this->ID[i] = save->villager(i)->id();
	}
}

const std::string getVillagerName(int index) {
	if (index < 168) {
		return g_villagerDatabase[nlVillagerIndex[index]];
	} else {
		return g_villagerDatabase[200 + nlVillagerIndex[index]];
	}
}

void VillagerViewerNL::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - " + Lang::get("VILLAGER_VIEWER"), 395, 0, font);

	SpriteManagement::DrawVillager(this->viewerIndex, 165, 100);
	// Special handle for AC:NL & AC:WA.
	if (savesType == SaveType::WA) {
		Gui::DrawStringCentered(0, 150, 0.9f, BLACK, Lang::get("VILLAGER_NAME") + g_villagerDatabase[this->viewerIndex], 395, 0, font);
	} else {
		Gui::DrawStringCentered(0, 150, 0.9f, BLACK, Lang::get("VILLAGER_NAME") + getVillagerName(this->viewerIndex), 395, 0, font);
	}

	Gui::DrawStringCentered(0, 180, 0.9f, BLACK, Lang::get("VILLAGER_ID") + std::to_string(this->viewerIndex), 395, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	for (int i = 0; i < 10; i++) {
		SpriteManagement::DrawVillager(this->ID[i], villagers[i].x, villagers[i].y);
	}

	GFX::DrawGUI(gui_pointer_idx, villagers[Selection].x+18, villagers[Selection].y+20);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void VillagerViewerNL::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
	if (hDown & KEY_RIGHT) {
		if(Selection < save->maxVillager()) {
			Selection++;
		}
	} else if (hDown & KEY_LEFT) {
		if (Selection > 0) {
			Selection--;
		}
	}

	if (hDown & KEY_SELECT)	this->update();

	if (hDown & KEY_R) {
		if (savesType == SaveType::WA) {
			if (this->viewerIndex < 398)	this->viewerIndex++;
		} else {
			if (this->viewerIndex < 332)	this->viewerIndex++;
		}
	}

	if (hDown & KEY_L) {
		if (this->viewerIndex > 0)	this->viewerIndex--;
	}

	if (hDown & KEY_A) {
		if (save->villager(Selection)->exist()) {
			Gui::setScreen(std::make_unique<VillagerEditorNL>(save->villager(Selection)), doFade, true);
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 10; i++) {
			if (iconTouch(touch, villagers[i])) {
				if (save->villager(i)->exist()) {
					Gui::setScreen(std::make_unique<VillagerEditorNL>(save->villager(i)), doFade, true);
				}
			}
		}
	}
}