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
#include "screenCommon.hpp"

// Include all spritesheet's maybe for their draw function here?
extern C2D_SpriteSheet Acres;
extern C2D_SpriteSheet Badges;
extern C2D_SpriteSheet Faces;
extern C2D_SpriteSheet GUI;
extern C2D_SpriteSheet Hairs;
extern C2D_SpriteSheet Items;
extern C2D_SpriteSheet NPCs;
extern C2D_SpriteSheet sprites;
extern C2D_SpriteSheet Villager;
extern C2D_SpriteSheet Villager2;
extern C2D_SpriteSheet WWAcres;
extern C2D_SpriteSheet WWFaces;
extern C2D_SpriteSheet WWVillagers;

void GFX::DrawTop(bool useBars, bool fullscreen) {
	Gui::ScreenDraw(Top);
	if (!fullscreen) {
		// Draw BG.
		Gui::DrawSprite(GUI, gui_top_ui_idx, 0, 0);
		if (useBars) {
			// Draw Bar. (We likely only need one.)
			Gui::DrawSprite(GUI, gui_top_bar_idx, 0, 0);
		}
	} else {
		// Just solid.
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(225,189,121, 255));
	}
}

void GFX::DrawBottom(bool fullscreen) {
	Gui::ScreenDraw(Bottom);
	if (!fullscreen) {
		Gui::DrawSprite(GUI, gui_bottom_ui_idx, 0, 0);
	} else {
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(225,189,121, 255));
	}
}

void GFX::DrawFileBrowseBG(bool isTop) {
	if (isTop) {
		Gui::ScreenDraw(Top);
		Gui::DrawSprite(GUI, gui_fb_bg_top_idx, 0, 0);
	} else {
		Gui::ScreenDraw(Bottom);
		Gui::DrawSprite(GUI, gui_fb_bg_bottom_idx, 0, 0);
	}
}

static void DrawList(int selection, const std::vector<std::string> &List, const std::string &Msg) {
	std::string lists;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawFileBrowseBG(true);
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, Msg, 400);
	for (int i = (selection<5) ? 0 : selection-5;i< (int)List.size() && i <((selection<5) ? 6 : selection+1);i++) {
		if (i == selection) {
			lists += "> " + List[i] + "\n\n";
		} else {
			lists += List[i] + "\n\n";
		}
	}
	Gui::DrawString(26, 32, 0.65f, BLACK, lists, 360);
	GFX::DrawFileBrowseBG(false);
	C3D_FrameEnd(0);
}

int GFX::ListSelection(int current, const std::vector<std::string> &list, const std::string &Msg) {
	s32 selection = current;
	int keyRepeatDelay = 0;
	while (1) {
		// Draw List.
		DrawList(selection, list, Msg);
		hidScanInput();
		if (keyRepeatDelay)	keyRepeatDelay--;
		if (hidKeysHeld() & KEY_DOWN && !keyRepeatDelay) {
			if (selection < (int)list.size()-1)	selection++;
			else	selection = 0;
			keyRepeatDelay = 6;
		}

		if (hidKeysHeld() & KEY_UP && !keyRepeatDelay) {
			if (selection > 0)	selection--;
			else if (selection == 0)	selection = (int)list.size()-1;
			keyRepeatDelay = 6;
		}

		if (hidKeysDown() & KEY_A) {
			return selection;
		}

		if (hidKeysDown() & KEY_B) {
			return current;
		}
	}
}

void GFX::DrawNPC(int img, int x, int y, float ScaleX, float ScaleY)
{
	Gui::DrawSprite(NPCs, img, x, y, ScaleX, ScaleY);
}

void GFX::DrawSprite(int img, int x, int y, float ScaleX, float ScaleY)
{
	Gui::DrawSprite(sprites, img, x, y, ScaleX, ScaleY);
}

void GFX::DrawGUI(int img, int x, int y, float ScaleX, float ScaleY)
{
	Gui::DrawSprite(GUI, img, x, y, ScaleX, ScaleY);
}

void GFX::DrawSpriteBlend(int img, int x, int y, u32 color, float ScaleX, float ScaleY)
{
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 0.5);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 0.5);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 0.5);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 0.5);
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, img), x, y, 0.5f, &tint, ScaleX, ScaleY);
}

// Draw a Button and draw Text on it.
void GFX::DrawButton(int x, int y, std::string ButtonText, bool selected) {
	if (selected)	Gui::DrawSprite(GUI, gui_button_selected_idx, x, y);
	else 			Gui::DrawSprite(GUI, gui_button_idx, x, y);
	// Draw String. TODO: Center.
	Gui::DrawStringCentered(- (158/2) + x, y + (61/2) - (Gui::GetStringHeight(0.9f, ButtonText) / 2), 0.9f, BLACK, ButtonText, 145, 30);
}