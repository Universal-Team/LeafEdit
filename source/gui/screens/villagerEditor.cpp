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
#include <map>
#include <sys/stat.h>
#include <unistd.h>

extern Save* SaveFile;
extern std::map<u16, std::string> g_villagerDatabase;
std::string villagerNameText = "";


void VillagerEditor::Draw(void) const
{
	// Villager Viewer Part.
	if (currentVillager == 1 && villagerMode == 1) {
		Villager1Draw();
	} else if (currentVillager == 2 && villagerMode == 1) {
		Villager2Draw();
	} else if (currentVillager == 3 && villagerMode == 1) {
		Villager3Draw();
	} else if (currentVillager == 4 && villagerMode == 1) {
		Villager4Draw();
	} else if (currentVillager == 5 && villagerMode == 1) {
		Villager5Draw();
	} else if (currentVillager == 6 && villagerMode == 1) {
		Villager6Draw();
	} else if (currentVillager == 7 && villagerMode == 1) {
		Villager7Draw();
	} else if (currentVillager == 8 && villagerMode == 1) {
		Villager8Draw();
	} else if (currentVillager == 9 && villagerMode == 1) {
		Villager9Draw();
	} else if (currentVillager == 10 && villagerMode == 1) {
		Villager10Draw();

		// Editor Part.
	} else if (villagerMode == 2 && currentVillager == 1) {
		VillagerEditorDraw(0);
	} else if (villagerMode == 2 && currentVillager == 2) {
		VillagerEditorDraw(1);
	} else if (villagerMode == 2 && currentVillager == 3) {
		VillagerEditorDraw(2);
	} else if (villagerMode == 2 && currentVillager == 4) {
		VillagerEditorDraw(3);
	} else if (villagerMode == 2 && currentVillager == 5) {
		VillagerEditorDraw(4);
	} else if (villagerMode == 2 && currentVillager == 6) {
		VillagerEditorDraw(5);
	} else if (villagerMode == 2 && currentVillager == 7) {
		VillagerEditorDraw(6);
	} else if (villagerMode == 2 && currentVillager == 8) {
		VillagerEditorDraw(7);
	} else if (villagerMode == 2 && currentVillager == 9) {
		VillagerEditorDraw(8);
	} else if (villagerMode == 2 && currentVillager == 10) {
		VillagerEditorDraw(9);
	}
}


void VillagerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (villagerMode == 1) {
		VillagerLogic(hDown, hHeld, touch);
	} else if (villagerMode == 2) {
		EditorLogic(hDown, hHeld, touch);
	}
}

void VillagerEditor::EditorLogic(u32 hDown, u32 hHeld, touchPosition touch) {

	// Selection Logic.
	if (hDown & KEY_RIGHT) {
		if(currentSlot < 5)	currentSlot++;
	} else if (hDown & KEY_LEFT) {
		if(currentSlot > 1)	currentSlot--;
	} else if (hDown & KEY_UP) {
		if(currentRow > 1)	currentRow--;
	} else if (hDown & KEY_DOWN) {
		if(currentRow < 2)	currentRow++;
	}

	// Page Logic.
	if (hDown & KEY_R) {
		if(editorPage < 30)	editorPage++;
	} else if (hDown & KEY_L) {
		if(editorPage > 1)	editorPage--;
	}

	// Back to Villager Screen.
	if (hDown & KEY_B) {
		villagerMode = 1;
	}
}

void VillagerEditor::VillagerLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Switch to the Villager Editor Screen.
	if (hDown & KEY_A) {
		villagerMode = 2;
	}

	// Switch current Villager.
	if (hDown & KEY_R) {
		if(currentVillager < 10)	currentVillager++;
	} else if (hDown & KEY_L) {
		if(currentVillager > 1)	currentVillager--;
	}

	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}


void VillagerEditor::Villager1Draw(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
    u16 villagerId = Save::Instance()->villagers[0]->GetId();
	Gui::villager(villagerId, 170, 60);
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
	Gui::villager(villagerId, 170, 60);
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
	Gui::villager(villagerId, 170, 60);
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
	Gui::villager(villagerId, 170, 60);
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
	Gui::villager(villagerId, 170, 60);
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
	Gui::villager(villagerId, 170, 60);
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
	Gui::villager(villagerId, 170, 60);
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
	Gui::villager(villagerId, 170, 60);
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
	Gui::villager(villagerId, 170, 60);
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
	Gui::villager(villagerId, 170, 60);
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




// Villager Editor Screen.
void VillagerEditor::VillagerEditorDraw(int currentVillager) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);

	Gui::Draw_Rect(5, 30, 70, 70, GRAY);
	Gui::villager(0, 5, 20);
	villagerNameText = g_villagerDatabase[0];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText.c_str()))/2-125-35, 80, 0.65f, WHITE, villagerNameText.c_str());


	Gui::Draw_Rect(85, 30, 70, 70, GRAY);
	Gui::villager(1, 85, 20);
	std::string villagerNameText2 = g_villagerDatabase[1];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText2.c_str()))/2-45-35, 80, 0.65f, WHITE, villagerNameText2.c_str());

	Gui::Draw_Rect(165, 30, 70, 70, GRAY);
	Gui::villager(2, 165, 20);
	std::string villagerNameText3 = g_villagerDatabase[2];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText3.c_str()))/2+35-35, 80, 0.65f, WHITE, villagerNameText3.c_str());

	Gui::Draw_Rect(245, 30, 70, 70, GRAY);
	Gui::villager(3, 245, 20);
	std::string villagerNameText4 = g_villagerDatabase[3];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText4.c_str()))/2+115-35, 80, 0.65f, WHITE, villagerNameText4.c_str());

	Gui::Draw_Rect(325, 30, 70, 70, GRAY);
	Gui::villager(4, 325, 20);
	std::string villagerNameText5 = g_villagerDatabase[4];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText5.c_str()))/2+195-35, 80, 0.65f, WHITE, villagerNameText5.c_str());

	// Second Row!

	Gui::Draw_Rect(5, 140, 70, 70, GRAY);
	Gui::villager(5, 5, 130);
	std::string villagerNameText6 = g_villagerDatabase[5];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText6.c_str()))/2-125-35, 190, 0.65f, WHITE, villagerNameText6.c_str());


	Gui::Draw_Rect(85, 140, 70, 70, GRAY);
	Gui::villager(6, 85, 130);
	std::string villagerNameText7 = g_villagerDatabase[6];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText7.c_str()))/2-45-35, 190, 0.65f, WHITE, villagerNameText7.c_str());


	Gui::Draw_Rect(165, 140, 70, 70, GRAY);
	Gui::villager(7, 165, 130);
	std::string villagerNameText8 = g_villagerDatabase[7];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText8.c_str()))/2+35-35, 190, 0.65f, WHITE, villagerNameText8.c_str());


	Gui::Draw_Rect(245, 140, 70, 70, GRAY);
	Gui::villager(8, 245, 130);
	std::string villagerNameText9 = g_villagerDatabase[8];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText9.c_str()))/2+115-35, 190, 0.65f, WHITE, villagerNameText9.c_str());


	Gui::Draw_Rect(325, 140, 70, 70, GRAY);
	Gui::villager(9, 325, 130);
	std::string villagerNameText10 = g_villagerDatabase[9];
	Gui::DrawString((400-Gui::GetStringWidth(0.65f, villagerNameText10.c_str()))/2+195-35, 190, 0.65f, WHITE, villagerNameText10.c_str());

	// Draw Selection.
	DrawEditorSelection();
	DrawEditorPage();

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);

	// Draw the current Villager on the bottom screen.
	u16 currentId = Save::Instance()->villagers[currentVillager]->GetId();
	Gui::villager(currentId, 140, 60);
}

void VillagerEditor::DrawEditorSelection(void) const
{
		// First Row.
	if (currentSlot == 1 && currentRow == 1) {
		Gui::drawAnimatedSelector(5, 30, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (currentSlot == 2 && currentRow == 1) {
		Gui::drawAnimatedSelector(85, 30, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (currentSlot == 3 && currentRow == 1) {
		Gui::drawAnimatedSelector(165, 30, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (currentSlot == 4 && currentRow == 1) {
		Gui::drawAnimatedSelector(245, 30, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (currentSlot == 5 && currentRow == 1) {
		Gui::drawAnimatedSelector(325, 30, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));

		// Second Row.
	} else if (currentSlot == 1 && currentRow == 2) {
		Gui::drawAnimatedSelector(5, 140, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (currentSlot == 2 && currentRow == 2) {
		Gui::drawAnimatedSelector(85, 140, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (currentSlot == 3 && currentRow == 2) {
		Gui::drawAnimatedSelector(165, 140, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (currentSlot == 4 && currentRow == 2) {
		Gui::drawAnimatedSelector(245, 140, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	} else if (currentSlot == 5 && currentRow == 2) {
		Gui::drawAnimatedSelector(325, 140, 70, 70, .030f, C2D_Color32(0, 0, 0, 0));
	}
}

void VillagerEditor::DrawEditorPage(void) const
{
	// Initial String.
	std::string currentPage = "Current Page: ";

	// currentPage
	for (int i = 1; i < 31; i++) {
		if (editorPage == i) {
			currentPage += std::to_string(i);
		}
	}

	currentPage += " / 30";
	Gui::DrawString(0, 0, 0.7f, WHITE, currentPage.c_str());
}


// This will draw the current Villager and Title for the Villagers Screen.
void VillagerEditor::DrawCurrentVillager(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += "Villager";
	Title += " ";
	Title += "Editor";

	std::string activeVillager = "Current Villager: ";

	for (int i = 1; i < 11; i++) {
		if (currentVillager == i) {
			activeVillager += std::to_string(i);
		}
	}

	Gui::DrawString(200, 215, 0.8f, WHITE, activeVillager.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str());
}