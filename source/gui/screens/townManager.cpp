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
#include <citro3d.h>
#include <citro2d.h>
#include <3ds.h>
#include <sys/stat.h>

#include "gui/screens/screenCommon.hpp"
#include "gui/screens/townManager.hpp"
#include "core/townManagement.hpp"
#include "common.hpp" // For the TID's.

extern FS_MediaType currentMedia;
extern u64 currentID;
extern u32 currentLowID;
extern u32 currentHighID;
extern u32 currentUniqueID;

extern bool updateEURFound;
extern bool updateUSAFound;
extern bool updateJPNFound;

void TownManager::Draw(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += Lang::mainMenu[0];
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, GREEN);
	Gui::Draw_Rect(0, 30, 400, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 400, 30, GREEN);
	Gui::DrawString((400-Gui::Draw_GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str());

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, GREEN);
	Gui::Draw_Rect(0, 30, 320, 180, DARKGRAY);
	Gui::Draw_Rect(0, 210, 320, 30, GREEN);


	if (Selection == 0) {
		Gui::Draw_Rect(townButtons[0].x, townButtons[0].y, townButtons[0].w, townButtons[0].h, RED);
		Gui::Draw_Rect(townButtons[1].x, townButtons[1].y, townButtons[1].w, townButtons[1].h, GREEN);
		Gui::Draw_Rect(townButtons[2].x, townButtons[2].y, townButtons[2].w, townButtons[2].h, GREEN);

	} else if (Selection == 1) {
		Gui::Draw_Rect(townButtons[0].x, townButtons[0].y, townButtons[0].w, townButtons[0].h, GREEN);
		Gui::Draw_Rect(townButtons[1].x, townButtons[1].y, townButtons[1].w, townButtons[1].h, RED);
		Gui::Draw_Rect(townButtons[2].x, townButtons[2].y, townButtons[2].w, townButtons[2].h, GREEN);

	} else if (Selection == 2) {
		Gui::Draw_Rect(townButtons[0].x, townButtons[0].y, townButtons[0].w, townButtons[0].h, GREEN);
		Gui::Draw_Rect(townButtons[1].x, townButtons[1].y, townButtons[1].w, townButtons[1].h, GREEN);
		Gui::Draw_Rect(townButtons[2].x, townButtons[2].y, townButtons[2].w, townButtons[2].h, RED);
	}

	Gui::DrawString((320-Gui::Draw_GetStringWidth(0.6f, Lang::townmanager[0]))/2, townButtons[0].y+10, 0.6f, WHITE, Lang::townmanager[0]);
	Gui::DrawString((320-Gui::Draw_GetStringWidth(0.6f, Lang::townmanager[1]))/2, townButtons[1].y+10, 0.6f, WHITE, Lang::townmanager[1]);
	Gui::DrawString((320-Gui::Draw_GetStringWidth(0.6f, Lang::townmanager[2]))/2, townButtons[2].y+10, 0.6f, WHITE, Lang::townmanager[2]);
}

void TownManager::Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	SelectionLogic(hDown, hHeld);

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
					if (Gui::promptMsg(Lang::messages2[4])) {
						TownManagement::LaunchTown(currentMedia, currentID);
						}
						break;
				}	case 1:
						if (Gui::promptMsg(Lang::messages2[2])) {
						TownManagement::BackupTown(currentID, currentMedia, currentLowID, currentHighID);
						}
						break;
				 	case 2: {
						if (Gui::promptMsg(Lang::messages2[3])) {
							TownManagement::RestoreTown(currentID, currentMedia, currentLowID, currentHighID, currentUniqueID);
						}
						break;
					 }
			}
		}
}

void TownManager::SelectionLogic(u32 hDown, u32 hHeld)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	}
}