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

#include "core/management/villagerManagement.hpp"

#include "save.hpp"
#include "villager.hpp"

#include "gui/screens/screenCommon.hpp"

#include <3ds.h>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

extern Save* SaveFile;
extern std::string villagerNameText;
extern std::vector<std::string> g_villagerDatabase;

extern C2D_SpriteSheet Villager;
extern C2D_SpriteSheet Villager2;
extern C2D_SpriteSheet WWVillagers;

// Start & End ID.
std::vector<std::pair<u16, u16>> villager_IDs;

void VillagerManagement::getVillagerIDs(int group) {
	// Clear ID's first.
	villager_IDs.clear();
	if (group == 0) {
		villager_IDs.push_back(std::make_pair(93, 99));
	} else if (group == 1) {
		villager_IDs.push_back(std::make_pair(0, 6));
	} else if (group == 2) {
		villager_IDs.push_back(std::make_pair(7, 20));
	} else if (group == 3) {
		villager_IDs.push_back(std::make_pair(21, 34));
	} else if (group == 4) {
		villager_IDs.push_back(std::make_pair(35, 40));
	} else if (group == 5) {
		villager_IDs.push_back(std::make_pair(41, 63));
	} else if (group == 6) {
		villager_IDs.push_back(std::make_pair(80, 88));
	} else if (group == 7) {
		villager_IDs.push_back(std::make_pair(89, 92));
	} else if (group == 8) {
		villager_IDs.push_back(std::make_pair(64, 79));
	} else if (group == 9) {
		villager_IDs.push_back(std::make_pair(100, 110));
	} else if (group == 10) {
		villager_IDs.push_back(std::make_pair(111, 126));
	} else if (group == 11) {
		villager_IDs.push_back(std::make_pair(127, 143));
	} else if (group == 12) {
		villager_IDs.push_back(std::make_pair(283, 291));
	} else if (group == 13) {
		villager_IDs.push_back(std::make_pair(144, 154));
	} else if (group == 14) {
		villager_IDs.push_back(std::make_pair(155, 172));
	} else if (group == 15) {
		villager_IDs.push_back(std::make_pair(173, 179));
	} else if (group == 16) {
		villager_IDs.push_back(std::make_pair(180, 189));
	} else if (group == 17) {
		villager_IDs.push_back(std::make_pair(190, 198));
	} else if (group == 18) {
		villager_IDs.push_back(std::make_pair(199, 205));
	} else if (group == 19) {
		villager_IDs.push_back(std::make_pair(206, 221));
	} else if (group == 20) {
		villager_IDs.push_back(std::make_pair(231, 238));
	} else if (group == 21) {
		villager_IDs.push_back(std::make_pair(222, 230));
	} else if (group == 22) {
		villager_IDs.push_back(std::make_pair(239, 245));
	} else if (group == 23) {
		villager_IDs.push_back(std::make_pair(246, 253));
	} else if (group == 24) {
		villager_IDs.push_back(std::make_pair(254, 268));
	} else if (group == 25) {
		villager_IDs.push_back(std::make_pair(269, 272));
	} else if (group == 26) {
		villager_IDs.push_back(std::make_pair(273, 282));
	} else if (group == 27) {
		villager_IDs.push_back(std::make_pair(292, 304));
	} else if (group == 28) {
		villager_IDs.push_back(std::make_pair(305, 320));
	} else if (group == 29) {
		villager_IDs.push_back(std::make_pair(321, 341));
	} else if (group == 30) {
		villager_IDs.push_back(std::make_pair(342, 347));
	} else if (group == 31) {
		villager_IDs.push_back(std::make_pair(348, 360));
	} else if (group == 32) {
		villager_IDs.push_back(std::make_pair(361, 380));
	} else if (group == 33) {
		villager_IDs.push_back(std::make_pair(381, 387));
	} else if (group == 34) {
		villager_IDs.push_back(std::make_pair(388, 398));
	}
}

// Draw the Villager sprite.
void VillagerManagement::DrawVillager(u16 villagerId, int x, int y, float ScaleX, float ScaleY) {
	if (villagerId > 399) {
		villagerId = 399;
	}

	if (villagerId < 200) {
		DrawSprite(Villager, villagerId, x, y, ScaleX, ScaleY);
	}
	else {
		DrawSprite(Villager2, villagerId - 200, x, y, ScaleX, ScaleY);
	}
}

void VillagerManagement::DrawWWVillager(u8 villagerId, int x, int y, float ScaleX, float ScaleY) {
	if (villagerId > 150) {
		villagerId = 150; // 150 -> EMPTY.
	}
		DrawSprite(WWVillagers, villagerId, x, y, ScaleX, ScaleY);
}

void VillagerManagement::DrawVillagerSelection(int selection, int page) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	C2D_TargetClear(bottom, BLACK);
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, WHITE, "Select the new Villager.", 400);
	for (int i = 0; i < 10; i++) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				Gui::Draw_Rect(5 + x * 80, 30 + y * 110, 70, 70, UNSELECTED_COLOR);
				DrawVillager(i+page*10, 10 + x * 80, 28 + y * 110, 1, 1);
				Gui::DrawString(15+x*80, 83+y*110, 0.55f, WHITE, g_villagerDatabase[i+page*10], 60);
			}
		}
	}
	int selectY = 0, selectX = 0;

	if (selection < 5)	selectY = 0;	else	selectY = 1;
	if (selection > 4)	selectX = selection - 5;	else	selectX = selection;

	Gui::drawAnimatedSelector(5 + selectX * 80, 30 + selectY * 110, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, std::to_string(page+1) + " / 40", 400);
	Gui::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, DIM);
	C3D_FrameEnd(0);
}

u16 VillagerManagement::SelectVillager(u16 currentVillager) {
	s32 selection = 0;
	int page = 0;
	while(1)
	{
		u16 villagerImage;
		for (villagerImage = 0; villagerImage < 399; villagerImage++) {
			if (selection == villagerImage) {
				DrawVillagerSelection(selection, page);
			}
		}
		hidScanInput();

		if (hidKeysDown() & KEY_DOWN) {
			if (selection < 5)	selection += 5;
		}

		if (hidKeysDown() & KEY_UP) {
			if (selection > 4)	selection -= 5;
		}
		if (hidKeysDown() & KEY_RIGHT) {
			if (selection < 9)	selection++;
		}
		if (hidKeysDown() & KEY_LEFT) {
			if (selection > 0)	selection--;
		}
		if (hidKeysDown() & KEY_R) {
			if (page == 39)	page = 0;
			else if (page < 39)	page++;
		}
		if (hidKeysDown() & KEY_L) {
			if (page == 0)	page = 39;
			else if (page > 0)	page--;
		}
		if (hidKeysDown() & KEY_A) {
			return selection + page*10;
		}
		if (hidKeysDown() & KEY_B) {
			return currentVillager;
		}
	}
}