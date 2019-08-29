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

#include "gui/screens/villagerEditor.hpp"
#include "gui/screens/screenCommon.hpp"

#include "core/save/offsets.h"
#include "core/save/villager.h"
#include "core/save/save.h"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern Save* SaveFile;
extern C2D_SpriteSheet Villager;
extern C2D_SpriteSheet Villager2;

void VillagerEditor::Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u32 idx = 0;
    for (u32 y = 0; y < 2; y++) {
        for (u32 x = 0; x < 5; x++, idx++) { 
            u16 villagerId = Save::Instance()->villagers[idx]->GetId();
            if (villagerId > 399) {
                villagerId = 399;
            }
			VillagerEditor2::DrawVillagerSprite(villagerId,  15 + x * 58, 67 + y * 58);
		}
	}


	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}

void VillagerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}


void VillagerEditor2::DrawVillagerSprite(u16 villagerId, int x, int y) {
    if (villagerId > 399) {
        villagerId = 399;
    }

    if (villagerId < 200) {
        DrawSprite(Villager, villagerId, x, y);
    }
    else {
        DrawSprite(Villager2, villagerId - 200, x, y);
    }
}