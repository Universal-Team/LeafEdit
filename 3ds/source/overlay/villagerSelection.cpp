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

#include "common.hpp"
#include "keyboard.hpp"
#include "overlay.hpp"
#include "spriteManagement.hpp"
#include "stringDB.hpp"

extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);
extern std::vector<std::tuple<u16, std::string, std::string>> villagerDB;
extern touchPosition touch;

static std::vector<std::tuple<u16, std::string, std::string>> search(std::vector<std::string> searchCategory = {""}, std::string searchResult = "", bool compare = false) {
	return StringDB::searchTuple(searchResult, searchCategory, villagerDB, compare);
}

static Structs::ButtonPos searchBtn = {295, 3, 20, 20};

static void Draw(const int selection, const SaveType st, std::vector<std::tuple<u16, std::string, std::string>> vlList) {
	std::string villagerList;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawFileBrowseBG(true);
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, Lang::get("SELECT_VILLAGER"), 395, 0, font);

	for (int i = (selection < 8) ? 0 : selection - 8; i < (int)vlList.size() && i < ((selection < 8) ? 9 : selection + 1); i++) {
		villagerList += std::get<1>(vlList[i]) + " - " + std::get<2>(vlList[i]) + "\n";
	}

	for (uint i = 0; i < ((vlList.size() < 9) ? 9 - vlList.size() : 0); i++) {
		villagerList += "\n";
	}

	/* Selector Logic. */
	if (selection < 9) GFX::DrawSelector(true, 24 + (selection * 21));
	else GFX::DrawSelector(true, 24 + (8 * 21));

	Gui::DrawString(5, 25, 0.85f, BLACK, villagerList, 360, 0, font);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, std::to_string(selection) + " | " + std::to_string(vlList.size()-1), 395, 0, font);

	GFX::DrawBottom();
	GFX::DrawGUI(gui_search_idx, 295, 3);
	C3D_FrameEnd(0);
}


u16 Overlays::SelectVillager(u16 oldID, const SaveType st) {
	std::vector<std::tuple<u16, std::string, std::string>> villagerList = villagerDB;
	int selection = (int)oldID;

	while(1) {
		Draw(selection, st, villagerList);
		
		/* Logic, here. */
		u32 hRepeat = hidKeysDownRepeat();
		hidScanInput();
		hidTouchRead(&touch);

		if (hRepeat & KEY_DOWN) {
			if (selection < (int)villagerList.size()-1) selection++;
		}

		if (hRepeat & KEY_UP) {
			if (selection > 0) selection--;
		}

		if ((hRepeat & KEY_LEFT) || (hRepeat & KEY_L)) {
			if ((selection - 9) < 0) {
				selection = 0;
			} else {
				selection -= 9;
			}
		}

		if ((hRepeat & KEY_RIGHT) || (hRepeat & KEY_R)) {
			if ((selection + 9) > (int)villagerList.size()-1) {
				selection = (int)villagerList.size()-1;
			} else {
				selection += 9;
			}
		}

		if ((hidKeysDown() & KEY_X) || (hidKeysDown() & KEY_TOUCH && iconTouch(touch, searchBtn))) {
			selection = 0;
			villagerList.clear();

			const std::string searchResult = Input::setString(-1, Lang::get("ENTER_SEARCH"));
			villagerList = search({}, searchResult);
		}

		if (hidKeysDown() & KEY_Y) {
			villagerList = villagerDB;
		}

		if (hidKeysDown() & KEY_A) {
			return std::get<0>(villagerList[selection]);
		}

		if (hidKeysDown() & KEY_B) {
			return oldID;
		}
	}
}