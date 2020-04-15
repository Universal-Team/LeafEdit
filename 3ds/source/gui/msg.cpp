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

#include "gfx.hpp"
#include "lang.hpp"
#include "msg.hpp"
#include "screenCommon.hpp"
#include "structs.hpp"

#include <vector>

const std::vector<ButtonType> promptBtn = {
	{0, 85, 149, 52, "Yes"}, // Yes.
	{162, 85, 149, 52, "No"}, // No.
	{80, 90, 149, 52, "Ok"} // OK.
};

const std::vector<std::string> prompt = {"YES", "NO"};

extern touchPosition touch;
extern bool touching(touchPosition touch, ButtonType button);

// Display a Message, which needs to be confirmed with A/B.
bool Msg::promptMsg2(std::string promptMsg)
{
	s32 selection = 1;
	while(1)
	{
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, BLACK);
		C2D_TargetClear(Bottom, BLACK);
		GFX::DrawTop(false);
		GFX::DrawGUI(gui_txt_box_idx, 0, 80);
		Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, promptMsg))/2, 0.8f, WHITE, promptMsg, 390, 70);
		GFX::DrawBottom();
		// Draw Bottom Screen part.
		for (int i = 0; i < (int)prompt.size(); i++) {
			GFX::DrawButton(promptBtn[i]);
			if (i == selection)	GFX::DrawGUI(gui_pointer_idx, promptBtn[i].x+100, promptBtn[i].y+30);
		}
		C3D_FrameEnd(0);

		// Selection part.
		gspWaitForVBlank();
		hidScanInput();
		hidTouchRead(&touch);

		if(hidKeysDown() & KEY_LEFT) {
			selection = 0;
		} else if(hidKeysDown() & KEY_RIGHT) {
			selection = 1;
		}
		if (hidKeysDown() & KEY_A) {
			if (selection == 0) {
				return true;
			} else {
				return false;
			}
		}
		if (hidKeysDown() & KEY_TOUCH && touching(touch, promptBtn[0])) {
			return true;
		}
		if (hidKeysDown() & KEY_TOUCH && touching(touch, promptBtn[1])) {
			return false;
		}
	}
}

bool Msg::promptMsg(std::string msg) {
	return Msg::promptMsg2(msg);
}

// Displays a Warn Message.
void Msg::DisplayWarnMsg(std::string Text)
{
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawTop(false);
	GFX::DrawGUI(gui_txt_box_idx, 0, 80);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, Text))/2, 0.8f, WHITE, Text, 395, 70);
	GFX::DrawBottom();
	C3D_FrameEnd(0);
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

// Displays a Warn Message. This is mostly be used for things with more text.
void Msg::DisplayWarnMsg2(std::string Text)
{
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawTop(false);
	GFX::DrawGUI(gui_txt_box_idx, 0, 80);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, Text))/2, 0.8f, WHITE, Text, 395, 70);
	GFX::DrawBottom();
	C3D_FrameEnd(0);
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

// Display a Message, which can be skipped with A.
void Msg::DisplayWaitMsg(std::string waitMsg, ...)
{
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawTop(false);
	GFX::DrawGUI(gui_txt_box_idx, 0, 80);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, waitMsg))/2, 0.8f, WHITE, waitMsg, 390, 70);
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 207);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, Lang::get("A_CONTINUE"), 395);
	GFX::DrawBottom();
	GFX::DrawButton(promptBtn[2]);
	GFX::DrawGUI(gui_pointer_idx, promptBtn[2].x+100, promptBtn[2].y+30);
	C3D_FrameEnd(0);

	while(1)
	{
		hidScanInput();
		hidTouchRead(&touch);
		if((hidKeysDown() & KEY_A) || (hidKeysDown() & KEY_TOUCH && touching(touch, promptBtn[2])))
			break;
	}
}

// TODO.
void Msg::HelperBox(std::string Msg) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	Gui::ScreenDraw(Top);
	Gui::Draw_Rect(0, 0, 400, 240, DIM);
	int textBoxHeight = Gui::GetStringHeight(0.7f, Msg) + 5;

	Gui::Draw_Rect(40, 211 - textBoxHeight, 320, textBoxHeight, DARKER_COLOR);
	Gui::Draw_Rect(44, 215 - textBoxHeight, 312, textBoxHeight - 8, LIGHT_COLOR);
	Gui::DrawStringCentered(0, 215 - textBoxHeight, 0.7, WHITE, Msg, 305, Gui::GetStringHeight(0.7f, Msg));
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, DIM);
	C3D_FrameEnd(0);
}

void Msg::DisplayMsg(std::string Message) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawTop(false);
	GFX::DrawGUI(gui_txt_box_idx, 0, 80);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, Message))/2, 0.8f, WHITE, Message, 390, 70);
	GFX::DrawBottom();
	C3D_FrameEnd(0);
}


void Msg::NotImplementedYet(void) {
	Msg::DisplayWarnMsg(Lang::get("NOT_IMPLEMENTED"));
}