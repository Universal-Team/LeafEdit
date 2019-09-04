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
extern std::vector<std::string> g_villagerDatabase;
extern std::string villagerNameText;


void VillagerEditor::Draw(void) const
{
	if (editorMode == 1) {
		DrawSubMenu();
	}
}


void VillagerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (editorMode == 1) {
		SubMenuLogic(hDown, hHeld, touch);
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
		if(currentRow < 2)	currentRow++;
	}

	if (hDown & KEY_R) {
		if(subMenuPage < 4)	subMenuPage++;
	} else if (hDown & KEY_L) {
		if(subMenuPage > 1)	subMenuPage--;
	}


	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}

// Villager Editor Screen.
void VillagerEditor::DrawSubMenu(void) const
{
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);

	if (subMenuPage == 1) {
		Gui::Draw_Rect(5, 30, 70, 70, GRAY);
		Gui::villager(93, 10, 28);
		std::string GroupOne = "Alligator";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupOne.c_str()))/2-125-35, 83, 0.55f, WHITE, GroupOne.c_str());

		Gui::Draw_Rect(85, 30, 70, 70, GRAY);
		Gui::villager(1, 90, 28);
		std::string GroupTwo = "Anteater";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwo.c_str()))/2-45-35, 83, 0.55f, WHITE, GroupTwo.c_str());

		Gui::Draw_Rect(165, 30, 70, 70, GRAY);
		Gui::villager(9, 170, 28);
		std::string GroupThree = "Bear";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThree.c_str()))/2+35-35, 83, 0.55f, WHITE, GroupThree.c_str());

		Gui::Draw_Rect(245, 30, 70, 70, GRAY);
		Gui::villager(23, 250, 28);
		std::string GroupFour = "Bird";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupFour.c_str()))/2+115-35, 83, 0.55f, WHITE, GroupFour.c_str());

		Gui::Draw_Rect(325, 30, 70, 70, GRAY);
		Gui::villager(38, 330, 28);
		std::string GroupFive = "Bull";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupFive.c_str()))/2+195-35, 83, 0.55f, WHITE, GroupFive.c_str());

		// Second Row!

		Gui::Draw_Rect(5, 140, 70, 70, GRAY);
		Gui::villager(48, 10, 138);
		std::string GroupSix = "Cat";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupSix.c_str()))/2-125-35, 193, 0.55f, WHITE, GroupSix.c_str());

		Gui::Draw_Rect(85, 140, 70, 70, GRAY);
		Gui::villager(82, 90, 138);
		std::string GroupSeven = "Chicken";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupSeven.c_str()))/2-45-35, 193, 0.55f, WHITE, GroupSeven.c_str());

		Gui::Draw_Rect(165, 140, 70, 70, GRAY);
		Gui::villager(89, 170, 138);
		std::string GroupEight = "Cow";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupEight.c_str()))/2+35-35, 193, 0.55f, WHITE, GroupEight.c_str());

		Gui::Draw_Rect(245, 140, 70, 70, GRAY);
		Gui::villager(68, 250, 138);
		std::string GroupNine = "Cub";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupNine.c_str()))/2+115-35, 193, 0.55f, WHITE, GroupNine.c_str());

		Gui::Draw_Rect(325, 140, 70, 70, GRAY);
		Gui::villager(101, 330, 138);
		std::string GroupTen = "Deer";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTen.c_str()))/2+195-35, 193, 0.55f, WHITE, GroupTen.c_str());





	} else if (subMenuPage == 2) {


		Gui::Draw_Rect(5, 30, 70, 70, GRAY);
		Gui::villager(115, 10, 28);
		std::string GroupEleven = "Dog";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupEleven.c_str()))/2-125-35, 83, 0.55f, WHITE, GroupEleven.c_str());

		Gui::Draw_Rect(85, 30, 70, 70, GRAY);
		Gui::villager(142, 90, 28);
		std::string GroupTwelve = "Duck";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwelve.c_str()))/2-45-35, 83, 0.55f, WHITE, GroupTwelve.c_str());

		Gui::Draw_Rect(165, 30, 70, 70, GRAY);
		Gui::villager(283, 170, 28);
		std::string GroupThirteen = "Eagle";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirteen.c_str()))/2+35-35, 83, 0.55f, WHITE, GroupThirteen.c_str());

		Gui::Draw_Rect(245, 30, 70, 70, GRAY);
		Gui::villager(148, 250, 28);
		std::string GroupFourteen = "Elephant";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupFourteen.c_str()))/2+115-35, 83, 0.55f, WHITE, GroupFourteen.c_str());

		Gui::Draw_Rect(325, 30, 70, 70, GRAY);
		Gui::villager(159, 330, 28);
		std::string GroupFifteen = "Frog";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupFifteen.c_str()))/2+195-35, 83, 0.55f, WHITE, GroupFifteen.c_str());

		// Second Row!

		Gui::Draw_Rect(5, 140, 70, 70, GRAY);
		Gui::villager(176, 10, 138);
		std::string GroupSixteen = "Goat";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupSixteen.c_str()))/2-125-35, 193, 0.55f, WHITE, GroupSixteen.c_str());

		Gui::Draw_Rect(85, 140, 70, 70, GRAY);
		Gui::villager(181, 90, 138);
		std::string GroupSeventeen = "Gorilla";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupSeventeen.c_str()))/2-45-35, 193, 0.55f, WHITE, GroupSeventeen.c_str());

		Gui::Draw_Rect(165, 140, 70, 70, GRAY);
		Gui::villager(190, 170, 138);
		std::string GroupEightteen = "Hamster";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupEightteen.c_str()))/2+35-35, 193, 0.55f, WHITE, GroupEightteen.c_str());

		Gui::Draw_Rect(245, 140, 70, 70, GRAY);
		Gui::villager(199, 250, 138);
		std::string GroupNineteen = "Hippo";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupNineteen.c_str()))/2+115-35, 193, 0.55f, WHITE, GroupNineteen.c_str());

		Gui::Draw_Rect(325, 140, 70, 70, GRAY);
		Gui::villager(208, 330, 138);
		std::string GroupTwenty = "Horse";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwenty.c_str()))/2+195-35, 193, 0.55f, WHITE, GroupTwenty.c_str());


	} else if (subMenuPage == 3) {

		Gui::Draw_Rect(5, 30, 70, 70, GRAY);
		Gui::villager(232, 10, 28);
		std::string GroupTwentyone = "Kangaroo";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentyone.c_str()))/2-125-35, 83, 0.55f, WHITE, GroupTwentyone.c_str());

		Gui::Draw_Rect(85, 30, 70, 70, GRAY);
		Gui::villager(224, 90, 28);
		std::string GroupTwentytwo = "Koala";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentytwo.c_str()))/2-45-35, 83, 0.55f, WHITE, GroupTwentytwo.c_str());

		Gui::Draw_Rect(165, 30, 70, 70, GRAY);
		Gui::villager(239, 170, 28);
		std::string GroupTwentythree = "Lion";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentythree.c_str()))/2+35-35, 83, 0.55f, WHITE, GroupTwentythree.c_str());

		Gui::Draw_Rect(245, 30, 70, 70, GRAY);
		Gui::villager(247, 250, 28);
		std::string GroupTwentyfour = "Monkey";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentyfour.c_str()))/2+115-35, 83, 0.55f, WHITE, GroupTwentyfour.c_str());

		Gui::Draw_Rect(325, 30, 70, 70, GRAY);
		Gui::villager(258, 330, 28);
		std::string GroupTwentyfive = "Mouse";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentyfive.c_str()))/2+195-35, 83, 0.55f, WHITE, GroupTwentyfive.c_str());

		// Second Row!

		Gui::Draw_Rect(5, 140, 70, 70, GRAY);
		Gui::villager(272, 10, 138);
		std::string GroupTwentysix = "Octopus";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentysix.c_str()))/2-125-35, 193, 0.55f, WHITE, GroupTwentysix.c_str());

		Gui::Draw_Rect(85, 140, 70, 70, GRAY);
		Gui::villager(278, 90, 138);
		std::string GroupTwentyseven = "Ostrich";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentyseven.c_str()))/2-45-35, 193, 0.55f, WHITE, GroupTwentyseven.c_str());

		Gui::Draw_Rect(165, 140, 70, 70, GRAY);
		Gui::villager(300, 170, 138);
		std::string GroupTwentyeight = "Penguin";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentyeight.c_str()))/2+35-35, 193, 0.55f, WHITE, GroupTwentyeight.c_str());

		Gui::Draw_Rect(245, 140, 70, 70, GRAY);
		Gui::villager(305, 250, 138);
		std::string GroupTwentynine = "Pig";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupTwentynine.c_str()))/2+115-35, 193, 0.55f, WHITE, GroupTwentynine.c_str());

		Gui::Draw_Rect(325, 140, 70, 70, GRAY);
		Gui::villager(337, 330, 138);
		std::string GroupThirty = "Rabbit";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirty.c_str()))/2+195-35, 193, 0.55f, WHITE, GroupThirty.c_str());


	} else if (subMenuPage == 4) {

		Gui::Draw_Rect(5, 30, 70, 70, GRAY);
		Gui::villager(342, 10, 28);
		std::string GroupThirtyone = "Rhino";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirtyone.c_str()))/2-125-35, 83, 0.55f, WHITE, GroupThirtyone.c_str());

		Gui::Draw_Rect(85, 30, 70, 70, GRAY);
		Gui::villager(349, 90, 28);
		std::string GroupThirtytwo = "Sheep";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirtytwo.c_str()))/2-45-35, 83, 0.55f, WHITE, GroupThirtytwo.c_str());

		Gui::Draw_Rect(165, 30, 70, 70, GRAY);
		Gui::villager(362, 170, 28);
		std::string GroupThirtythree = "Squirrel";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirtythree.c_str()))/2+35-35, 83, 0.55f, WHITE, GroupThirtythree.c_str());

		Gui::Draw_Rect(245, 30, 70, 70, GRAY);
		Gui::villager(382, 250, 28);
		std::string GroupThirtyfour = "Tiger";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirtyfour.c_str()))/2+115-35, 83, 0.55f, WHITE, GroupThirtyfour.c_str());

		Gui::Draw_Rect(325, 30, 70, 70, GRAY);
		Gui::villager(388, 330, 28);
		std::string GroupThirtyfive = "Wolf";
		Gui::DrawString((400-Gui::GetStringWidth(0.55f, GroupThirtyfive.c_str()))/2+195-35, 83, 0.55f, WHITE, GroupThirtyfive.c_str());
	}



	// Draw Selection.
	DrawSelection();
	DrawSubMenuPage();

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);
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

void VillagerEditor::DrawSubMenuPage(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += "Villager";
	Title += " ";
	Title += "Editor";
	Title += " - ";
	Title += "SubMenu";

	// Initial String.
	std::string currentPage = Lang::misc[0];
	currentPage += " ";

	// currentPage
	for (int i = 1; i < 5; i++) {
		if (subMenuPage == i) {
			currentPage += std::to_string(i);
		}
	}

	currentPage += " / 4";
	Gui::DrawString((400-Gui::GetStringWidth(0.55f, currentPage.c_str()))/2, 215, 0.55f, WHITE, currentPage.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str());
}