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

#include "core/management/villagerManagement.hpp"

#include "core/save/offsets.h"
#include "core/save/villager.h"
#include "core/save/save.h"

#include "gui/keyboard.hpp"

#include "gui/screens/villagerEditor.hpp"
#include "gui/screens/screenCommon.hpp"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern Save* SaveFile;
extern std::vector<std::string> g_villagerDatabase;
extern std::vector<std::string> g_groups;
extern std::string villagerNameText;
extern u16 currentVillager;

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern int getSpecies(int id);

void VillagerEditor::Draw(void) const
{
	if (editorMode == 0) {
		DrawEditorSub();
	} else if (editorMode == 1) {
		DrawSubMenu();
	} else if (editorMode == 2) {
		DrawVillagerSelection();
	} else if (editorMode == 3) {
		DrawVillagerSetTest();
	}
}

void VillagerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (editorMode == 0) {
		EditorSubLogic(hDown);
	} else if (editorMode == 1) {
		SubMenuLogic(hDown, hHeld, touch);
	} else if (editorMode == 2) {
		VillagerSelectionLogic(hDown, hHeld, touch);
	} else if (editorMode == 3) {
		VillagerSetLogicTest(hDown, hHeld, touch);
	}
}

void VillagerEditor::DrawEditorSub(void) const {
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, "LeafEdit"))/2, 2, 0.8f, TXTCOLOR, "LeafEdit", 400);

	Gui::DrawBottom();

	if (Selection == 0) {
		Gui::Draw_ImageBlend(0, sprites_button_idx, Buttons[0].x, Buttons[0].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, Buttons[1].x, Buttons[1].y);

	} else if (Selection == 1) {
		Gui::sprite(0, sprites_button_idx, Buttons[0].x, Buttons[0].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, Buttons[1].x, Buttons[1].y, selectedColor);
	}

	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::get("SELECTING")))/2, Buttons[0].y+10, 0.6f, TXTCOLOR, Lang::get("SELECTING"), 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::get("MANUALLY")))/2, Buttons[1].y+10, 0.6f, TXTCOLOR, Lang::get("MANUALLY"), 140);
}

void VillagerEditor::EditorSubLogic(u32 hDown) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 1)	Selection++;
	}

	if (hDown & KEY_A) {
		switch(Selection) {
			case 0:
				editorMode = 1;
				break;
			case 1:
				manuallyVillager = Input::getu16(3, 398);
				SaveFile->villagers[currentVillager]->SetId(manuallyVillager);
				Gui::screenBack();
				return;
				break;
		}
	}
}


void VillagerEditor::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {

	// Selection Logic.
	if (hDown & KEY_RIGHT) {
		if(currentSlot < 5)	currentSlot++;
	} else if (hDown & KEY_LEFT) {
		if(currentSlot > 1)	currentSlot--;
	} else if (hDown & KEY_UP) {
		if(currentRow > 1)	currentRow--;
	} else if (hDown & KEY_DOWN) {
		if(subMenuPage == 4) {
		} else {
			if(currentRow < 2)	currentRow++;
		}
	}

	if (hDown & KEY_A) {
		if (currentSlot == 1 && currentRow == 1 && subMenuPage == 1) {
			startID = 93;
			endID = 99;
		} else if (currentSlot == 2 && currentRow == 1 && subMenuPage == 1) {
			startID = 0;
			endID = 6;
		} else if (currentSlot == 3 && currentRow == 1 && subMenuPage == 1) {
			startID = 7;
			endID = 20;
		} else if (currentSlot == 4 && currentRow == 1 && subMenuPage == 1) {
			startID = 21;
			endID = 34;
		} else if (currentSlot == 5 && currentRow == 1 && subMenuPage == 1) {
			startID = 35;
			endID = 40;
		} else if (currentSlot == 1 && currentRow == 2 && subMenuPage == 1) {
			startID = 41;
			endID = 63;
		} else if (currentSlot == 2 && currentRow == 2 && subMenuPage == 1) {
			startID = 80;
			endID = 88;
		} else if (currentSlot == 3 && currentRow == 2 && subMenuPage == 1) {
			startID = 89;
			endID = 92;
		} else if (currentSlot == 4 && currentRow == 2 && subMenuPage == 1) {
			startID = 64;
			endID = 79;
		} else if (currentSlot == 5 && currentRow == 2 && subMenuPage == 1) {
			startID = 100;
			endID = 110;
		} else if (currentSlot == 1 && currentRow == 1 && subMenuPage == 2) {
			startID = 111;
			endID = 126;
		} else if (currentSlot == 2 && currentRow == 1 && subMenuPage == 2) {
			startID = 127;
			endID = 143;
		} else if (currentSlot == 3 && currentRow == 1 && subMenuPage == 2) {
			startID = 283;
			endID = 291;
		} else if (currentSlot == 4 && currentRow == 1 && subMenuPage == 2) {
			startID = 144;
			endID = 154;
		} else if (currentSlot == 5 && currentRow == 1 && subMenuPage == 2) {
			startID = 155;
			endID = 172;
		} else if (currentSlot == 1 && currentRow == 2 && subMenuPage == 2) {
			startID = 173;
			endID = 179;
		} else if (currentSlot == 2 && currentRow == 2 && subMenuPage == 2) {
			startID = 180;
			endID = 189;
		} else if (currentSlot == 3 && currentRow == 2 && subMenuPage == 2) {
			startID = 190;
			endID = 198;
		} else if (currentSlot == 4 && currentRow == 2 && subMenuPage == 2) {
			startID = 199;
			endID = 205;
		} else if (currentSlot == 5 && currentRow == 2 && subMenuPage == 2) {
			startID = 206;
			endID = 221;
		} else if (currentSlot == 1 && currentRow == 1 && subMenuPage == 3) {
			startID = 231;
			endID = 238;
		} else if (currentSlot == 2 && currentRow == 1 && subMenuPage == 3) {
			startID = 222;
			endID = 230;
		} else if (currentSlot == 3 && currentRow == 1 && subMenuPage == 3) {
			startID = 239;
			endID = 245;
		} else if (currentSlot == 4 && currentRow == 1 && subMenuPage == 3) {
			startID = 246;
			endID = 253;
		} else if (currentSlot == 5 && currentRow == 1 && subMenuPage == 3) {
			startID = 254;
			endID = 268;
		} else if (currentSlot == 1 && currentRow == 2 && subMenuPage == 3) {
			startID = 269;
			endID = 272;
		} else if (currentSlot == 2 && currentRow == 2 && subMenuPage == 3) {
			startID = 273;
			endID = 282;
		} else if (currentSlot == 3 && currentRow == 2 && subMenuPage == 3) {
			startID = 292;
			endID = 304;
		} else if (currentSlot == 4 && currentRow == 2 && subMenuPage == 3) {
			startID = 305;
			endID = 320;
		} else if (currentSlot == 5 && currentRow == 2 && subMenuPage == 3) {
			startID = 321;
			endID = 341;
		} else if (currentSlot == 1 && currentRow == 1 && subMenuPage == 4) {
			startID = 342;
			endID = 347;
		} else if (currentSlot == 2 && currentRow == 1 && subMenuPage == 4) {
			startID = 348;
			endID = 360;
		} else if (currentSlot == 3 && currentRow == 1 && subMenuPage == 4) {
			startID = 361;
			endID = 380;
		} else if (currentSlot == 4 && currentRow == 1 && subMenuPage == 4) {
			startID = 381;
			endID = 387;
		} else if (currentSlot == 5 && currentRow == 1 && subMenuPage == 4) {
			startID = 388;
			endID = 398;
		}
		currentSelectedVillager = startID;
		editorMode = 3;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Select a Villager Group and press A. (Not implemented yet)\nPress L/R to switch the Pages.\nPress B to exit from this Screen.");
	}

	if (hDown & KEY_R) {
		if(subMenuPage == 3) {
			if(currentRow == 2) {
				if(currentSlot == 1 || currentSlot == 2 || currentSlot == 3 || currentSlot == 4 || currentSlot == 5) {
					subMenuPage = 4;
					currentRow = 1;
					currentSlot = 5;
				}
			} else {
				subMenuPage++;
			}
		} else {
			if(subMenuPage < 4)	subMenuPage++;
		}
	} else if (hDown & KEY_L) {
		if(subMenuPage > 1)	subMenuPage--;
	}


	if (hDown & KEY_B) {
		editorMode = 0;
	}
}

// Villager Editor Screen.
void VillagerEditor::DrawSubMenu(void) const
{
	std::string Title;
	Title += "LeafEdit - ";
	Title += Lang::get("VILLAGER_EDITOR_SUBMENU");

	// Initial String.
	std::string currentPage = Lang::get("CURRENT_PAGE");
	currentPage += " ";

	// currentPage
	for (int i = 1; i < 5; i++) {
		if (subMenuPage == i) {
			currentPage += std::to_string(i);
		}
	}

	currentPage += " / 4";

	Gui::DrawTop();

	Gui::DrawString((400-Gui::GetStringWidth(0.8f, currentPage.c_str()))/2, 212, 0.8f, TXTCOLOR, currentPage.c_str(), 400);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, TXTCOLOR, Title.c_str(), 400);

	if (subMenuPage == 1) {
		Gui::Draw_Rect(5, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(93, 10, 28);
		std::string GroupOne = g_groups[0];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupOne.c_str()))/2-125-35, 83, 0.55f, TXTCOLOR, GroupOne.c_str(), 70);

		Gui::Draw_Rect(85, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(1, 90, 28);
		std::string GroupTwo = g_groups[1]; 
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwo.c_str()))/2-45-35, 83, 0.55f, TXTCOLOR, GroupTwo.c_str(), 70);

		Gui::Draw_Rect(165, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(9, 170, 28);
		std::string GroupThree = g_groups[2];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThree.c_str()))/2+35-35, 83, 0.55f, TXTCOLOR, GroupThree.c_str(), 70);

		Gui::Draw_Rect(245, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(23, 250, 28);
		std::string GroupFour = g_groups[3];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupFour.c_str()))/2+115-35, 83, 0.55f, TXTCOLOR, GroupFour.c_str(), 70);

		Gui::Draw_Rect(325, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(38, 330, 28);
		std::string GroupFive = g_groups[4];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupFive.c_str()))/2+195-35, 83, 0.55f, TXTCOLOR, GroupFive.c_str(), 70);

		// Second Row!

		Gui::Draw_Rect(5, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(48, 10, 138);
		std::string GroupSix = g_groups[5];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupSix.c_str()))/2-125-35, 193, 0.55f, TXTCOLOR, GroupSix.c_str(), 70);

		Gui::Draw_Rect(85, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(82, 90, 138);
		std::string GroupSeven = g_groups[6];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupSeven.c_str()))/2-45-35, 193, 0.55f, TXTCOLOR, GroupSeven.c_str(), 70);

		Gui::Draw_Rect(165, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(89, 170, 138);
		std::string GroupEight = g_groups[7];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupEight.c_str()))/2+35-35, 193, 0.55f, TXTCOLOR, GroupEight.c_str(), 70);

		Gui::Draw_Rect(245, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(68, 250, 138);
		std::string GroupNine = g_groups[8];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupNine.c_str()))/2+115-35, 193, 0.55f, TXTCOLOR, GroupNine.c_str(), 70);

		Gui::Draw_Rect(325, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(101, 330, 138);
		std::string GroupTen = g_groups[9];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTen.c_str()))/2+195-35, 193, 0.55f, TXTCOLOR, GroupTen.c_str(), 70);


	} else if (subMenuPage == 2) {

		Gui::Draw_Rect(5, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(115, 10, 28);
		std::string GroupEleven = g_groups[10];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupEleven.c_str()))/2-125-35, 83, 0.55f, TXTCOLOR, GroupEleven.c_str(), 70);

		Gui::Draw_Rect(85, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(142, 90, 28);
		std::string GroupTwelve = g_groups[11];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwelve.c_str()))/2-45-35, 83, 0.55f, TXTCOLOR, GroupTwelve.c_str(), 70);

		Gui::Draw_Rect(165, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(283, 170, 28);
		std::string GroupThirteen = g_groups[12];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirteen.c_str()))/2+35-35, 83, 0.55f, TXTCOLOR, GroupThirteen.c_str(), 70);

		Gui::Draw_Rect(245, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(148, 250, 28);
		std::string GroupFourteen = g_groups[13];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupFourteen.c_str()))/2+115-35, 83, 0.55f, TXTCOLOR, GroupFourteen.c_str(), 70);

		Gui::Draw_Rect(325, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(159, 330, 28);
		std::string GroupFifteen = g_groups[14];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupFifteen.c_str()))/2+195-35, 83, 0.55f, TXTCOLOR, GroupFifteen.c_str(), 70);

		// Second Row!

		Gui::Draw_Rect(5, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(176, 10, 138);
		std::string GroupSixteen = g_groups[15];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupSixteen.c_str()))/2-125-35, 193, 0.55f, TXTCOLOR, GroupSixteen.c_str(), 70);

		Gui::Draw_Rect(85, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(181, 90, 138);
		std::string GroupSeventeen = g_groups[16];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupSeventeen.c_str()))/2-45-35, 193, 0.55f, TXTCOLOR, GroupSeventeen.c_str(), 70);

		Gui::Draw_Rect(165, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(190, 170, 138);
		std::string GroupEightteen = g_groups[17];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupEightteen.c_str()))/2+35-35, 193, 0.55f, TXTCOLOR, GroupEightteen.c_str(), 70);

		Gui::Draw_Rect(245, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(199, 250, 138);
		std::string GroupNineteen = g_groups[18];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupNineteen.c_str()))/2+115-35, 193, 0.55f, TXTCOLOR, GroupNineteen.c_str(), 70);

		Gui::Draw_Rect(325, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(208, 330, 138);
		std::string GroupTwenty = g_groups[19];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwenty.c_str()))/2+195-35, 193, 0.55f, TXTCOLOR, GroupTwenty.c_str(), 70);


	} else if (subMenuPage == 3) {

		Gui::Draw_Rect(5, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(232, 10, 28);
		std::string GroupTwentyone = g_groups[20];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentyone.c_str()))/2-125-35, 83, 0.55f, TXTCOLOR, GroupTwentyone.c_str(), 70);

		Gui::Draw_Rect(85, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(224, 90, 28);
		std::string GroupTwentytwo = g_groups[21];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentytwo.c_str()))/2-45-35, 83, 0.55f, TXTCOLOR, GroupTwentytwo.c_str(), 70);

		Gui::Draw_Rect(165, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(239, 170, 28);
		std::string GroupTwentythree = g_groups[22];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentythree.c_str()))/2+35-35, 83, 0.55f, TXTCOLOR, GroupTwentythree.c_str(), 70);

		Gui::Draw_Rect(245, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(247, 250, 28);
		std::string GroupTwentyfour = g_groups[23];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentyfour.c_str()))/2+115-35, 83, 0.55f, TXTCOLOR, GroupTwentyfour.c_str(), 70);

		Gui::Draw_Rect(325, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(258, 330, 28);
		std::string GroupTwentyfive = g_groups[24];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentyfive.c_str()))/2+195-35, 83, 0.55f, TXTCOLOR, GroupTwentyfive.c_str(), 70);

		// Second Row!

		Gui::Draw_Rect(5, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(272, 10, 138);
		std::string GroupTwentysix = g_groups[25];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentysix.c_str()))/2-125-35, 193, 0.55f, TXTCOLOR, GroupTwentysix.c_str(), 70);

		Gui::Draw_Rect(85, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(278, 90, 138);
		std::string GroupTwentyseven = g_groups[26];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentyseven.c_str()))/2-45-35, 193, 0.55f, TXTCOLOR, GroupTwentyseven.c_str(), 70);

		Gui::Draw_Rect(165, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(300, 170, 138);
		std::string GroupTwentyeight = g_groups[27];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentyeight.c_str()))/2+35-35, 193, 0.55f, TXTCOLOR, GroupTwentyeight.c_str(), 70);

		Gui::Draw_Rect(245, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(305, 250, 138);
		std::string GroupTwentynine = g_groups[28];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentynine.c_str()))/2+115-35, 193, 0.55f, TXTCOLOR, GroupTwentynine.c_str(), 70);

		Gui::Draw_Rect(325, 140, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(337, 330, 138);
		std::string GroupThirty = g_groups[29];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirty.c_str()))/2+195-35, 193, 0.55f, TXTCOLOR, GroupThirty.c_str(), 70);


	} else if (subMenuPage == 4) {

		Gui::Draw_Rect(5, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(342, 10, 28);
		std::string GroupThirtyone = g_groups[30];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirtyone.c_str()))/2-125-35, 83, 0.55f, TXTCOLOR, GroupThirtyone.c_str(), 70);

		Gui::Draw_Rect(85, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(349, 90, 28);
		std::string GroupThirtytwo = g_groups[31];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirtytwo.c_str()))/2-45-35, 83, 0.55f, TXTCOLOR, GroupThirtytwo.c_str(), 70);

		Gui::Draw_Rect(165, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(362, 170, 28);
		std::string GroupThirtythree = g_groups[32];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirtythree.c_str()))/2+35-35, 83, 0.55f, TXTCOLOR, GroupThirtythree.c_str(), 70);

		Gui::Draw_Rect(245, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(382, 250, 28);
		std::string GroupThirtyfour = g_groups[33];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirtyfour.c_str()))/2+115-35, 83, 0.55f, TXTCOLOR, GroupThirtyfour.c_str(), 70);

		Gui::Draw_Rect(325, 30, 70, 70, BGBOTCOLOR);
		VillagerManagement::DrawVillager(388, 330, 28);
		std::string GroupThirtyfive = g_groups[34];
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirtyfive.c_str()))/2+195-35, 83, 0.55f, TXTCOLOR, GroupThirtyfive.c_str(), 70);
	}

	// Draw Selection.
	DrawSelection();

	Gui::DrawBottom();
}

void VillagerEditor::DrawSelection(void) const
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


void VillagerEditor::DrawVillagerSelection(void) const {
	Gui::DrawTop();
	u32 villager = 93;
	for (u32 y = 0; y < 2; y++) {
		for (u32 x = 0; x < 5; x++, villager++) {
			villagerNameText = g_villagerDatabase[villager];
			Gui::Draw_Rect(15 + x * 75, 40 + y * 90, 70, 70, BGBOTCOLOR);
			VillagerManagement::DrawVillager(villager, 15 + x * 75, 35 + y * 90);
			Gui::DrawString(15 + x * 75, 95 + y * 90, 0.45f, TXTCOLOR, villagerNameText, 400);
		}
	}
}


void VillagerEditor::DrawVillagerSetTest(void) const {
	Gui::DrawTop();
	DrawBox();
	std::string villagerID = Lang::get("VILLAGER_ID");
	std::string IDs;
	for (int villager = startID; villager < endID+1; villager++) {
		if (currentSelectedVillager == villager) {
			VillagerManagement::DrawVillager(villager, 160, 60);
			villagerNameText = g_villagerDatabase[villager];
			villagerID += std::to_string(currentSelectedVillager);
			IDs = std::to_string(villager);
			IDs += " / ";
			IDs += std::to_string(endID);
		}
	}
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, Lang::get("CHANGE_VILLAGER_LR")))/2, 2, 0.7f, TXTCOLOR, Lang::get("CHANGE_VILLAGER_LR"), 320);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerID.c_str()))/2, 160, 0.7f, TXTCOLOR, villagerID.c_str(), 320);
	Gui::DrawString((400-Gui::GetStringWidth(0.7f, villagerNameText.c_str()))/2, 130, 0.7f, TXTCOLOR, villagerNameText.c_str(), 320);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, IDs.c_str()))/2, 212, 0.8f, TXTCOLOR, IDs.c_str(), 400);
	Gui::DrawBottom();
	Gui::sprite(0, sprites_search_idx, 290, 3);
}

void VillagerEditor::VillagerSetLogicTest(u32 hDown, u32 hHeld, touchPosition touch) {

	// Switch current Villager.
	if (hDown & KEY_RIGHT) {
		currentSelectedVillager++;
		if (currentSelectedVillager > endID)	currentSelectedVillager = startID;
	} else if (hDown & KEY_LEFT) {
		currentSelectedVillager--;
		if (currentSelectedVillager < startID)	currentSelectedVillager = endID;
	}

	if (hDown & KEY_A) {
		std::string prompt = Lang::get("SET_VILLAGER");
		prompt += "\n\n";
		prompt += villagerNameText;
		if(Msg::promptMsg(prompt.c_str())) {
			selectedVillager = currentSelectedVillager;
			SaveFile->villagers[currentVillager]->SetId(selectedVillager);
			editorMode = 0;
		}
	}

	if (hDown & KEY_TOUCH && touching(touch, search[0])) {
		int temp = Input::getu16(3, endID);
		if(temp < startID) {
			Msg::DisplayWarnMsg(Lang::get("VILLAGER_GROUP_DONT_START"));
			currentSelectedVillager = startID;
		} else {
			currentSelectedVillager = temp;
		}
	}
	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		editorMode = 1;
	}
}

void VillagerEditor::VillagerSelectionLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		editorMode = 1;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Select a Villager, which you like to replace with the selected one.\nPress B to exit from this Screen.");
	}
}

void VillagerEditor::DrawBox(void) const {
	Gui::Draw_Rect(40, 162, 320, 22, BGBOTCOLOR);
	Gui::Draw_Rect(40, 132, 320, 22, BGBOTCOLOR);
}