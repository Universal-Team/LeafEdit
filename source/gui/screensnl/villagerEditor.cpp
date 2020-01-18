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

#include "core/save/offsets.h"
#include "core/save/villager.h"
#include "core/save/save.h"

#include "gui/keyboard.hpp"

#include "gui/screensnl/villagerEditor.hpp"
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

void VillagerEditor::Draw(void) const
{
	if (editorMode == 0) {
		DrawSubMenu();
	} else if (editorMode == 1) {
		DrawReplaceSub();
	} else if (editorMode == 2) {
		DrawGroupSelection();
	} else if (editorMode == 3) {
		DrawVillagerSelection();
	} else if (editorMode == 4) {
		DrawVillagerSetTest();
	}
}

void VillagerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
		if (editorMode == 0) {
		SubMenuLogic(hDown, hHeld, touch);
	} else if (editorMode == 1) {
		ReplaceSubLogic(hDown, touch);
	} else if (editorMode == 2) {
		GroupSelectionLogic(hDown, hHeld, touch);
	} else if (editorMode == 3) {
		VillagerSelectionLogic(hDown, hHeld, touch);
	} else if (editorMode == 4) {
		VillagerSetLogicTest(hDown, hHeld, touch);
	}
}

void VillagerEditor::DrawSubMenu(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("VILLAGER_EDITOR_SUBMENU"), 390);

	Gui::Draw_Rect(40, 142, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 112, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 172, 320, 22, DARKER_COLOR);
	VillagerManagement::DrawVillager(Save::Instance()->villagers[currentVillager]->GetId(), 160, 40);
	Gui::DrawStringCentered(0, 110, 0.9f, WHITE, g_villagerDatabase[Save::Instance()->villagers[currentVillager]->GetId()], 310);
	Gui::DrawStringCentered(0, 140, 0.9f, WHITE, Lang::get("VILLAGER_ID") + std::to_string(Save::Instance()->villagers[currentVillager]->GetId()), 310);

	// Status.
	if (Save::Instance()->villagers[currentVillager]->getStatus() == 1) {
		Gui::DrawStringCentered(0, 170, 0.9f, WHITE, Lang::get("STATUS") + ": " + Lang::get("BOXED"), 310);
	} else {
		Gui::DrawStringCentered(0, 170, 0.9f, WHITE, Lang::get("STATUS") + ": " + Lang::get("UNBOXED"), 310);
	}

	Gui::DrawBottom();
	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(villagerButtons[i].x, villagerButtons[i].y, villagerButtons[i].w, villagerButtons[i].h, UNSELECTED_COLOR);
		if (SubSelection == i) {
			Gui::drawAnimatedSelector(villagerButtons[i].x, villagerButtons[i].y, villagerButtons[i].w, villagerButtons[i].h, .030f, SELECTED_COLOR);
		}
	}
	// Replace.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("REPLACE")))/2-80+17.5, 0.8, WHITE, Lang::get("REPLACE"), 130, 25);
	// Set Boxed.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("STATUS")))/2-20+17.5, 0.8, WHITE, Lang::get("STATUS"), 130, 25);
}

void VillagerEditor::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	// Selection.
	if (hDown & KEY_UP) {
		if(SubSelection > 0)	SubSelection--;
	}
	if (hDown & KEY_DOWN) {
			if(SubSelection < 5)	SubSelection++;
	}
	if (hDown & KEY_RIGHT) {
		if (SubSelection < 3) {
			SubSelection += 3;
		}
	}

	if (hDown & KEY_LEFT) {
		if (SubSelection < 6 && SubSelection > 2) {
			SubSelection -= 3;
		}
	}

	if (hDown & KEY_A) {
		switch (SubSelection) {
			case 0:
				editorMode = 1; // Sub Menu of Selection.
				break;
			case 1:
				if (Save::Instance()->villagers[currentVillager]->getStatus() == 1)	Save::Instance()->villagers[currentVillager]->setStatus(0);
				else	Save::Instance()->villagers[currentVillager]->setStatus(1);
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, villagerButtons[0])) {
			editorMode = 1;
		} else if (touching(touch, villagerButtons[1])) {
			if (Save::Instance()->villagers[currentVillager]->getStatus() == 1)	Save::Instance()->villagers[currentVillager]->setStatus(0);
			else	Save::Instance()->villagers[currentVillager]->setStatus(1);
		}
	}
}

// Replace Sub Screen.
void VillagerEditor::DrawReplaceSub(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit", 400);

	Gui::DrawBottom();

	for (int i = 0; i < 2; i++) {
		Gui::Draw_Rect(Buttons[i].x, Buttons[i].y, Buttons[i].w, Buttons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			Gui::drawAnimatedSelector(Buttons[i].x, Buttons[i].y, Buttons[i].w, Buttons[i].h, .030f, SELECTED_COLOR);
		}
	}
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("SELECTING")))/2-80+17.5, 0.8, WHITE, Lang::get("SELECTING"), 130, 25);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("MANUALLY")))/2-20+17.5, 0.8, WHITE, Lang::get("MANUALLY"), 130, 25);
}

void VillagerEditor::ReplaceSubLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		editorMode = 0;
	}

	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 1)	Selection++;
	}

	if (hDown & KEY_A) {
		switch(Selection) {
			case 0:
				editorMode = 2;
				break;
			case 1:
				manuallyVillager = Input::handleu16(3, Lang::get("ENTER_VILLAGER_ID"), 398, SaveFile->villagers[currentVillager]->GetId());
				SaveFile->villagers[currentVillager]->SetId(manuallyVillager);
				editorMode = 0;
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, Buttons[0])) {
			editorMode = 2;
		} else if (touching(touch, Buttons[1])) {
			manuallyVillager = Input::handleu16(3, Lang::get("ENTER_VILLAGER_ID"), 398, SaveFile->villagers[currentVillager]->GetId());
			SaveFile->villagers[currentVillager]->SetId(manuallyVillager);
			editorMode = 0;
		}
	}
}

// Group Selection Screen.

void VillagerEditor::DrawGroupSelection(void) const
{
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
	Gui::DrawStringCentered(0, 212, 0.9f, WHITE, currentPage, 390);
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("VILLAGER_EDITOR_SUBMENU"), 390);

	if (subMenuPage == 1) {
		Gui::Draw_Rect(5, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(93, 10, 28);
		Gui::DrawStringCentered(-160, 83, 0.7, WHITE, g_groups[0], 60);

		Gui::Draw_Rect(85, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(1, 90, 28);
		Gui::DrawStringCentered(-80, 83, 0.7, WHITE, g_groups[1], 60);

		Gui::Draw_Rect(165, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(9, 170, 28);
		Gui::DrawStringCentered(0, 83, 0.7, WHITE, g_groups[2], 60);

		Gui::Draw_Rect(245, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(23, 250, 28);
		Gui::DrawStringCentered(80, 83, 0.7, WHITE, g_groups[3], 60);

		Gui::Draw_Rect(325, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(38, 330, 28);
		Gui::DrawStringCentered(160, 83, 0.7, WHITE, g_groups[4], 60);

		// Second Row!
		Gui::Draw_Rect(5, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(48, 10, 138);
		Gui::DrawStringCentered(-160, 193, 0.7, WHITE, g_groups[5], 60);

		Gui::Draw_Rect(85, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(82, 90, 138);
		Gui::DrawStringCentered(-80, 193, 0.7, WHITE, g_groups[6], 60);

		Gui::Draw_Rect(165, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(89, 170, 138);
		Gui::DrawStringCentered(0, 193, 0.7, WHITE, g_groups[7], 60);

		Gui::Draw_Rect(245, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(68, 250, 138);
		Gui::DrawStringCentered(80, 193, 0.7, WHITE, g_groups[8], 60);

		Gui::Draw_Rect(325, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(101, 330, 138);
		Gui::DrawStringCentered(160, 193, 0.7, WHITE, g_groups[9], 60);


	} else if (subMenuPage == 2) {

		Gui::Draw_Rect(5, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(115, 10, 28);
		Gui::DrawStringCentered(-160, 83, 0.7, WHITE, g_groups[10], 60);

		Gui::Draw_Rect(85, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(142, 90, 28);
		Gui::DrawStringCentered(-80, 83, 0.7, WHITE, g_groups[11], 60);

		Gui::Draw_Rect(165, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(283, 170, 28);
		Gui::DrawStringCentered(0, 83, 0.7, WHITE, g_groups[12], 60);

		Gui::Draw_Rect(245, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(148, 250, 28);
		Gui::DrawStringCentered(80, 83, 0.7, WHITE, g_groups[13], 60);

		Gui::Draw_Rect(325, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(159, 330, 28);
		Gui::DrawStringCentered(160, 83, 0.7, WHITE, g_groups[14], 60);

		// Second Row!

		Gui::Draw_Rect(5, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(176, 10, 138);
		Gui::DrawStringCentered(-160, 193, 0.7, WHITE, g_groups[15], 60);

		Gui::Draw_Rect(85, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(181, 90, 138);
		Gui::DrawStringCentered(-80, 193, 0.7, WHITE, g_groups[16], 60);

		Gui::Draw_Rect(165, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(190, 170, 138);
		Gui::DrawStringCentered(0, 193, 0.7, WHITE, g_groups[17], 60);

		Gui::Draw_Rect(245, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(199, 250, 138);
		Gui::DrawStringCentered(80, 193, 0.7, WHITE, g_groups[18], 60);

		Gui::Draw_Rect(325, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(208, 330, 138);
		Gui::DrawStringCentered(160, 193, 0.7, WHITE, g_groups[19], 60);


	} else if (subMenuPage == 3) {

		Gui::Draw_Rect(5, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(232, 10, 28);
		std::string GroupTwentyone = g_groups[20];
		Gui::DrawStringCentered(-160, 83, 0.7, WHITE, g_groups[20], 60);

		Gui::Draw_Rect(85, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(224, 90, 28);
		Gui::DrawStringCentered(-80, 83, 0.7, WHITE, g_groups[21], 60);

		Gui::Draw_Rect(165, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(239, 170, 28);
		Gui::DrawStringCentered(0, 83, 0.7, WHITE, g_groups[22], 60);

		Gui::Draw_Rect(245, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(247, 250, 28);
		Gui::DrawStringCentered(80, 83, 0.7, WHITE, g_groups[23], 60);

		Gui::Draw_Rect(325, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(258, 330, 28);
		Gui::DrawStringCentered(160, 83, 0.7, WHITE, g_groups[24], 60);

		// Second Row!

		Gui::Draw_Rect(5, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(272, 10, 138);
		Gui::DrawStringCentered(-160, 193, 0.7, WHITE, g_groups[25], 60);

		Gui::Draw_Rect(85, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(278, 90, 138);
		Gui::DrawStringCentered(-80, 193, 0.7, WHITE, g_groups[26], 60);

		Gui::Draw_Rect(165, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(300, 170, 138);
		Gui::DrawStringCentered(0, 193, 0.7, WHITE, g_groups[27], 60);

		Gui::Draw_Rect(245, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(305, 250, 138);
		Gui::DrawStringCentered(80, 193, 0.7, WHITE, g_groups[28], 60);

		Gui::Draw_Rect(325, 140, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(337, 330, 138);
		Gui::DrawStringCentered(160, 193, 0.7, WHITE, g_groups[29], 60);


	} else if (subMenuPage == 4) {

		Gui::Draw_Rect(5, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(342, 10, 28);
		std::string GroupThirtyone = g_groups[30];
		Gui::DrawStringCentered(-160, 83, 0.7, WHITE, g_groups[30], 60);

		Gui::Draw_Rect(85, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(349, 90, 28);
		Gui::DrawStringCentered(-80, 83, 0.7, WHITE, g_groups[31], 60);

		Gui::Draw_Rect(165, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(362, 170, 28);
		Gui::DrawStringCentered(0, 83, 0.7, WHITE, g_groups[32], 60);

		Gui::Draw_Rect(245, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(382, 250, 28);
		Gui::DrawStringCentered(80, 83, 0.7, WHITE, g_groups[33], 60);

		Gui::Draw_Rect(325, 30, 70, 70, DARKER_COLOR);
		VillagerManagement::DrawVillager(388, 330, 28);
		Gui::DrawStringCentered(160, 83, 0.7, WHITE, g_groups[34], 60);
	}

	// Draw Selection.
	DrawSelection();

	Gui::DrawBottom();
}

void VillagerEditor::GroupSelectionLogic(u32 hDown, u32 hHeld, touchPosition touch) {

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
		editorMode = 4;
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
		editorMode = 1;
	}
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
			Gui::Draw_Rect(15 + x * 75, 40 + y * 90, 70, 70, DARKER_COLOR);
			VillagerManagement::DrawVillager(villager, 15 + x * 75, 35 + y * 90);
			Gui::DrawString(15 + x * 75, 95 + y * 90, 0.6f, WHITE, g_villagerDatabase[villager], 400);
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

	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, Lang::get("CHANGE_VILLAGER_LR"), 310);
	Gui::DrawStringCentered(0, 160, 0.9f, WHITE, villagerID, 310);
	Gui::DrawStringCentered(0, 130, 0.9f, WHITE, villagerNameText, 310);
	Gui::DrawStringCentered(0, 212, 0.9f, WHITE, IDs, 390);
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
		int temp = Input::handleu16(3, Lang::get("ENTER_VILLAGER_ID"), endID, 0);
		if(temp < startID) {
			Msg::DisplayWarnMsg(Lang::get("VILLAGER_GROUP_DONT_START"));
			currentSelectedVillager = startID;
		} else {
			currentSelectedVillager = temp;
		}
	}
	// Go back to the Editor Screen.
	if (hDown & KEY_B) {
		editorMode = 2;
	}
}

void VillagerEditor::VillagerSelectionLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		editorMode = 2;
	}
}

void VillagerEditor::DrawBox(void) const {
	Gui::Draw_Rect(40, 162, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 132, 320, 22, DARKER_COLOR);
}