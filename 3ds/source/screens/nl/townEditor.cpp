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

#include "acreManagement.hpp"
#include "gui.hpp"
#include "islandMapEditor.hpp"
#include "offsets.hpp"
#include "save.hpp"
#include "townEditor.hpp"
#include "townMapEditor.hpp"
#include "utils.hpp"

#include <3ds.h>

extern Save* SaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

void TownEditor::Draw(void) const {
	if (Mode == 0) {
		DrawSubMenu();
	}
}

void TownEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0) {
		SubMenuLogic(hDown, hHeld, touch);
	}
}

void TownEditor::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("TOWN_EDITOR"), 400);
	Gui::DrawStringCentered(0, 30, 0.8f, WHITE, "Seconds: " + std::to_string(SaveFile->town->TimePlayed%60), 200);
	Gui::DrawStringCentered(0, 60, 0.8f, WHITE, "Minutes: " + std::to_string((SaveFile->town->TimePlayed/60)%60), 200);
	Gui::DrawStringCentered(0, 90, 0.8f, WHITE, "Hours: " + std::to_string((SaveFile->town->TimePlayed/(60*60))%24), 200);
	Gui::DrawStringCentered(0, 120, 0.8f, WHITE, "Days: " + std::to_string(SaveFile->town->TimePlayed/(3600*24)), 200);

	GFX::DrawBottom();
	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (subSelection == i) {
			GFX::DrawSprite(sprites_pointer_idx, mainButtons[i].x+130, mainButtons[i].y+25);
		}
	}
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("TOWNMAP_EDITOR")))/2-80+17.5, 0.8, WHITE, Lang::get("TOWNMAP_EDITOR"), 130, 25);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("ISLANDMAP_EDITOR")))/2-20+17.5, 0.8, WHITE, Lang::get("ISLANDMAP_EDITOR"), 130, 25);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, "?"))/2+75-17.5, 0.8, WHITE, "?", 130, 25);
}

void TownEditor::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}

	if (hDown & KEY_UP) {
		if(subSelection > 0)	subSelection--;
	} else if (hDown & KEY_DOWN) {
		if(subSelection < 2)	subSelection++;
	}

	if (hDown & KEY_A) {
		if (subSelection == 0) {
			Gui::setScreen(std::make_unique<TownMapEditor>());
		} else if (subSelection == 1) {
			Gui::setScreen(std::make_unique<IslandMapEditor>());
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			Gui::setScreen(std::make_unique<TownMapEditor>());
		} else if (touching(touch, mainButtons[1])) {
			Gui::setScreen(std::make_unique<IslandMapEditor>());
		}
	}
}