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

#include "wwsave.hpp"
#include "wwVillager.hpp"
#include "wwVillagerManagement.hpp"

#include <3ds.h>

extern WWSave* WWSaveFile;
extern std::string villagerNameText;
extern std::vector<std::string> g_villagerDatabase;

extern C2D_SpriteSheet WWVillagers;

void WWVillagerManagement::DrawVillager(u8 villagerId, int x, int y, float ScaleX, float ScaleY) {
	if (villagerId > 150) {
		villagerId = 150; // 150 -> EMPTY.
	}
		Gui::DrawSprite(WWVillagers, villagerId, x, y, ScaleX, ScaleY);
}


void WWVillagerManagement::DrawVillagerSelection(int selection, int page) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawTop();
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

	Gui::drawAnimatedSelector(5 + selectX * 80, 30 + selectY * 110, 70, 70, .030f, SELECTED_COLOR, C2D_Color32(0, 0, 0, 0));
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, std::to_string(page+1) + " / 15", 400);
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, DIM);
	C3D_FrameEnd(0);
}

u8 WWVillagerManagement::SelectVillager(u8 currentVillager) {
	s32 selection = 0;
	int page = 0;
	while(1)
	{
		u8 villagerImage;
		for (villagerImage = 0; villagerImage < 149; villagerImage++) {
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
			if (page == 14)	page = 0;
			else if (page < 14)	page++;
		}
		if (hidKeysDown() & KEY_L) {
			if (page == 0)	page = 14;
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