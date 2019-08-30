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

#include "common/config.hpp"
#include "common/utils.hpp"

#include "core/villagerManagement.hpp"

#include "gui/keyboard.hpp"

#include "gui/screens/screenCommon.hpp"
#include "villagerEditor.hpp"

#include "core/save/offsets.h"
#include "core/save/save.h"
#include "core/save/villager.h"

#include <3ds.h>
#include <map>

extern Save* SaveFile;

extern std::map<u16, std::string> g_villagerDatabase;
extern std::string villagerNameText;

// To-Do.
void VillagerManagement::editVillager(int currentVillager) {
	static int page = 1; // 30 Pages is the maximum because of ~ 299 Villagers.
	static u16 selectedVillager; // This would be the new selected Villager.
	bool isSelected = false; // If the Villager is selected, this would be true.

	C3D_FrameBegin(0);
	C2D_TargetClear(top, BLACK);
	C2D_SceneBegin(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);


	Gui::Draw_Rect(5, 30, 70, 70, GRAY);
	VillagerEditor2::DrawVillagerSprite(0, 5, 20);
	villagerNameText = g_villagerDatabase[0];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText.c_str()))/2-125-35, 80, 0.65f, WHITE, villagerNameText.c_str());


	Gui::Draw_Rect(85, 30, 70, 70, GRAY);
	VillagerEditor2::DrawVillagerSprite(1, 85, 20);
	std::string villagerNameText2 = g_villagerDatabase[1];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText2.c_str()))/2-45-35, 80, 0.65f, WHITE, villagerNameText2.c_str());

	Gui::Draw_Rect(165, 30, 70, 70, GRAY);
	VillagerEditor2::DrawVillagerSprite(2, 165, 20);
	std::string villagerNameText3 = g_villagerDatabase[2];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText3.c_str()))/2+35-35, 80, 0.65f, WHITE, villagerNameText3.c_str());

	Gui::Draw_Rect(245, 30, 70, 70, GRAY);
	VillagerEditor2::DrawVillagerSprite(3, 245, 20);
	std::string villagerNameText4 = g_villagerDatabase[3];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText4.c_str()))/2+115-35, 80, 0.65f, WHITE, villagerNameText4.c_str());

	Gui::Draw_Rect(325, 30, 70, 70, GRAY);
	VillagerEditor2::DrawVillagerSprite(4, 325, 20);
	std::string villagerNameText5 = g_villagerDatabase[4];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText5.c_str()))/2+195-35, 80, 0.65f, WHITE, villagerNameText5.c_str());

// Second Row!

	Gui::Draw_Rect(5, 140, 70, 70, GRAY);
	VillagerEditor2::DrawVillagerSprite(5, 5, 130);
	std::string villagerNameText6 = g_villagerDatabase[5];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText6.c_str()))/2-125-35, 190, 0.65f, WHITE, villagerNameText6.c_str());


	Gui::Draw_Rect(85, 140, 70, 70, GRAY);
	VillagerEditor2::DrawVillagerSprite(6, 85, 130);
	std::string villagerNameText7 = g_villagerDatabase[6];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText7.c_str()))/2-45-35, 190, 0.65f, WHITE, villagerNameText7.c_str());


	Gui::Draw_Rect(165, 140, 70, 70, GRAY);
	VillagerEditor2::DrawVillagerSprite(7, 165, 130);
	std::string villagerNameText8 = g_villagerDatabase[7];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText8.c_str()))/2+35-35, 190, 0.65f, WHITE, villagerNameText8.c_str());


	Gui::Draw_Rect(245, 140, 70, 70, GRAY);
	VillagerEditor2::DrawVillagerSprite(8, 245, 130);
	std::string villagerNameText9 = g_villagerDatabase[8];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText9.c_str()))/2+115-35, 190, 0.65f, WHITE, villagerNameText9.c_str());


	Gui::Draw_Rect(325, 140, 70, 70, GRAY);
	VillagerEditor2::DrawVillagerSprite(9, 325, 130);
	std::string villagerNameText10 = g_villagerDatabase[9];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText10.c_str()))/2+195-35, 190, 0.65f, WHITE, villagerNameText10.c_str());

	C3D_FrameEnd(0);
	while(1) {
		hidScanInput();
		if (keysDown() & KEY_A) {
			break;
		}
	}
}