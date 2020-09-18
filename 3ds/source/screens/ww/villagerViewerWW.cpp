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
#include "villagerEditorWW.hpp"
#include "villagerViewerWW.hpp"

extern std::vector<std::tuple<u16, std::string, std::string>> villagerDB;
extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);
extern std::shared_ptr<Sav> save;

VillagerViewerWW::VillagerViewerWW() { this->update(); }

void VillagerViewerWW::update() {
	/* Get all Villager ID's for display. */
	for (int i = 0; i < 8; i++) {
		this->ID[i] = save->villager(i)->id();
	}
}

void VillagerViewerWW::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("VILLAGER_VIEWER"), 395, 0, font);
	SpriteManagement::DrawVillager(this->viewerIndex, 165, 100);
	Gui::DrawStringCentered(0, 150, 0.9f, BLACK, Lang::get("VILLAGER_NAME") + std::get<1>(villagerDB[this->viewerIndex]), 395, 0, font);
	Gui::DrawStringCentered(0, 180, 0.9f, BLACK, Lang::get("VILLAGER_ID") + std::to_string(this->viewerIndex), 395, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();
	
	for (int i = 0; i < 8; i++) {
		SpriteManagement::DrawVillager(this->ID[i], villagers[i].x, villagers[i].y);
	}
	
	GFX::DrawGUI(gui_pointer_idx, villagers[this->Selection].x + 18, villagers[this->Selection].y + 20);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void VillagerViewerWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_RIGHT) {
		if (this->Selection < save->maxVillager()) this->Selection++;
	}
	
	if (hRepeat & KEY_LEFT) {
		if (this->Selection > 0) this->Selection--;
	}

	if (hDown & KEY_SELECT) this->update();
	
	if (hRepeat & KEY_R) {
		if (this->viewerIndex < 150) this->viewerIndex++;
	}

	if (hRepeat & KEY_L) {
		if (this->viewerIndex > 0) this->viewerIndex--;
	}

	if (hDown & KEY_A) {
		if (save->villager(this->Selection)->exist()) {
			this->villager = save->villager(this->Selection);
			Gui::setScreen(std::make_unique<VillagerEditorWW>(this->villager), doFade, true);
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 8; i++) {
			if (iconTouch(touch, villagers[i])) {
				if (save->villager(i)->exist()) {
					this->villager = save->villager(i);
					Gui::setScreen(std::make_unique<VillagerEditorWW>(this->villager), doFade, true);
				}
			}
		}
	}
}