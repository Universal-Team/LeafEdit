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
#include "core/villagerManagement.hpp"
#include "core/save/save.h"

#include <3ds.h>
#include <map>
#include <sys/stat.h>
#include <unistd.h>

extern C2D_SpriteSheet Villager;
extern C2D_SpriteSheet Villager2;
extern Save* SaveFile;
extern std::map<u16, std::string> g_villagerDatabase;
std::string villagerNameText = "";


void VillagerEditor::Draw(void) const
{
	if (currentVillager == 1) {
		Villager1Draw();
	} else if (currentVillager == 2) {
		Villager2Draw();
	} else if (currentVillager == 3) {
		Villager3Draw();
	} else if (currentVillager == 4) {
		Villager4Draw();
	} else if (currentVillager == 5) {
		Villager5Draw();
	} else if (currentVillager == 6) {
		Villager6Draw();
	} else if (currentVillager == 7) {
		Villager7Draw();
	} else if (currentVillager == 8) {
		Villager8Draw();
	} else if (currentVillager == 9) {
		Villager9Draw();
	} else if (currentVillager == 10) {
		Villager10Draw();
	}
}

void VillagerEditor::Villager1Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[0]->GetId();
	VillagerEditor2::DrawVillagerSprite(villagerId, 170, 60);
	DrawCurrentVillager();

	// Villager ID.
	std::string villagerID = "Villager ID: ";
	villagerID += std::to_string(Save::Instance()->villagers[0]->GetId());
	Gui::Draw_Rect(40, 162, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, WHITE, villagerID.c_str());

	// Villager Name.
	villagerNameText = g_villagerDatabase[villagerId];
	Gui::Draw_Rect(40, 132, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, WHITE, villagerNameText.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}


void VillagerEditor::Villager2Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[1]->GetId();
	VillagerEditor2::DrawVillagerSprite(villagerId, 170, 60);
	DrawCurrentVillager();

	// Villager ID.
	std::string villagerID = "Villager ID: ";
	villagerID += std::to_string(Save::Instance()->villagers[1]->GetId());
	Gui::Draw_Rect(40, 162, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, WHITE, villagerID.c_str());

	// Villager Name.
	villagerNameText = g_villagerDatabase[villagerId];
	Gui::Draw_Rect(40, 132, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, WHITE, villagerNameText.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}


void VillagerEditor::Villager3Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[2]->GetId();
	VillagerEditor2::DrawVillagerSprite(villagerId, 170, 60);
	DrawCurrentVillager();

	// Villager ID.
	std::string villagerID = "Villager ID: ";
	villagerID += std::to_string(Save::Instance()->villagers[2]->GetId());
	Gui::Draw_Rect(40, 162, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, WHITE, villagerID.c_str());

	// Villager Name.
	villagerNameText = g_villagerDatabase[villagerId];
	Gui::Draw_Rect(40, 132, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, WHITE, villagerNameText.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}


void VillagerEditor::Villager4Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[3]->GetId();
	VillagerEditor2::DrawVillagerSprite(villagerId, 170, 60);
	DrawCurrentVillager();

	// Villager ID.
	std::string villagerID = "Villager ID: ";
	villagerID += std::to_string(Save::Instance()->villagers[3]->GetId());
	Gui::Draw_Rect(40, 162, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, WHITE, villagerID.c_str());

	// Villager Name.
	villagerNameText = g_villagerDatabase[villagerId];
	Gui::Draw_Rect(40, 132, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, WHITE, villagerNameText.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}


void VillagerEditor::Villager5Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[4]->GetId();
	VillagerEditor2::DrawVillagerSprite(villagerId, 170, 60);
	DrawCurrentVillager();

	// Villager ID.
	std::string villagerID = "Villager ID: ";
	villagerID += std::to_string(Save::Instance()->villagers[4]->GetId());
	Gui::Draw_Rect(40, 162, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, WHITE, villagerID.c_str());

	// Villager Name.
	villagerNameText = g_villagerDatabase[villagerId];
	Gui::Draw_Rect(40, 132, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, WHITE, villagerNameText.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}


void VillagerEditor::Villager6Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[5]->GetId();
	VillagerEditor2::DrawVillagerSprite(villagerId, 170, 60);
	DrawCurrentVillager();

	// Villager ID.
	std::string villagerID = "Villager ID: ";
	villagerID += std::to_string(Save::Instance()->villagers[5]->GetId());
	Gui::Draw_Rect(40, 162, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, WHITE, villagerID.c_str());

	// Villager Name.
	villagerNameText = g_villagerDatabase[villagerId];
	Gui::Draw_Rect(40, 132, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, WHITE, villagerNameText.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}


void VillagerEditor::Villager7Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[6]->GetId();
	VillagerEditor2::DrawVillagerSprite(villagerId, 170, 60);
	DrawCurrentVillager();

	// Villager ID.
	std::string villagerID = "Villager ID: ";
	villagerID += std::to_string(Save::Instance()->villagers[6]->GetId());
	Gui::Draw_Rect(40, 162, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, WHITE, villagerID.c_str());

	// Villager Name.
	villagerNameText = g_villagerDatabase[villagerId];
	Gui::Draw_Rect(40, 132, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, WHITE, villagerNameText.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}


void VillagerEditor::Villager8Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[7]->GetId();
	VillagerEditor2::DrawVillagerSprite(villagerId, 170, 60);
	DrawCurrentVillager();

	// Villager ID.
	std::string villagerID = "Villager ID: ";
	villagerID += std::to_string(Save::Instance()->villagers[7]->GetId());
	Gui::Draw_Rect(40, 162, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, WHITE, villagerID.c_str());

	// Villager Name.
	villagerNameText = g_villagerDatabase[villagerId];
	Gui::Draw_Rect(40, 132, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, WHITE, villagerNameText.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}


void VillagerEditor::Villager9Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[8]->GetId();
	VillagerEditor2::DrawVillagerSprite(villagerId, 170, 60);
	DrawCurrentVillager();

	// Villager ID.
	std::string villagerID = "Villager ID: ";
	villagerID += std::to_string(Save::Instance()->villagers[8]->GetId());
	Gui::Draw_Rect(40, 162, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, WHITE, villagerID.c_str());

	// Villager Name.
	villagerNameText = g_villagerDatabase[villagerId];
	Gui::Draw_Rect(40, 132, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, WHITE, villagerNameText.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}


void VillagerEditor::Villager10Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[9]->GetId();
	VillagerEditor2::DrawVillagerSprite(villagerId, 170, 60);
	DrawCurrentVillager();

	// Villager ID.
	std::string villagerID = "Villager ID: ";
	villagerID += std::to_string(Save::Instance()->villagers[9]->GetId());
	Gui::Draw_Rect(40, 162, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, WHITE, villagerID.c_str());

	// Villager Name.
	villagerNameText = g_villagerDatabase[villagerId];
	Gui::Draw_Rect(40, 132, 320, 22, GRAY);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, WHITE, villagerNameText.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
}



void VillagerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
//	if (hDown & KEY_X) {
//		VillagerManagement::editVillager(1);
//	}

	if (hDown & KEY_R) {
		if(currentVillager < 10)	currentVillager++;
	} else if (hDown & KEY_L) {
		if(currentVillager > 1)	currentVillager--;
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}

void VillagerEditor::DrawCurrentVillager(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += "Villager";
	Title += " ";
	Title += "Editor";

	std::string activeVillager = "Current Villager: ";
	if (currentVillager == 1) {
		activeVillager += "1";

	} else if (currentVillager == 2) {
		activeVillager += "2";

	} else if (currentVillager == 3) {
		activeVillager += "3";

	} else if (currentVillager == 4) {
		activeVillager += "4";

	} else if (currentVillager == 5) {
		activeVillager += "5";

	} else if (currentVillager == 6) {
		activeVillager += "6";

	} else if (currentVillager == 7) {
		activeVillager += "7";

	} else if (currentVillager == 8) {
		activeVillager += "8";

	} else if (currentVillager == 9) {
		activeVillager += "9";

	} else if (currentVillager == 10) {
		activeVillager += "10";
	}

	Gui::DrawString(200, 215, 0.8f, WHITE, activeVillager.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str());
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