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

#include "initial.hpp"
#include "mainMenu.hpp"
#include "screenCommon.hpp"
#include "utils.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern int fadealpha;

void Initial::Draw(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, Lang::get("WELCOME_TO_LEAFEDIT"), 400);
	Gui::DrawStringCentered(0, 30, 0.9f, WHITE, Lang::get("DEVELOPED_BY"), 390);
	Gui::DrawStringCentered(0, 70, 0.9f, WHITE, Lang::get("MAIN_DEV"), 390);
	GFX::DrawSprite(sprites_stackZ_idx, -20, 100);
	// Draw all supported Game sprites.
	GFX::DrawSprite(sprites_Icon_idx, 260, 100);
	GFX::DrawSprite(sprites_IconWA_idx, 260, 155);
	GFX::DrawSprite(sprites_IconWL_idx, 320, 100);
	GFX::DrawSprite(sprites_IconWW_idx, 320, 155);
	// Draw the Highlight -> The LeafEdit Icon! ;P
	GFX::DrawSprite(sprites_leafEdit_idx, 145, 100, 2, 2);
	
	Gui::DrawString(395-Gui::GetStringWidth(0.8, Lang::get("CURRENT_VERSION") + V_STRING), 215, 0.8, WHITE, Lang::get("CURRENT_VERSION") + V_STRING, 400);
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, fadealpha));
	GFX::DrawBottom();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, Lang::get("SELECT_LANG"), 310);
	for (int i = 0; i < 8; i++) {
		Gui::Draw_Rect(langBlocks[i].x, langBlocks[i].y, langBlocks[i].w, langBlocks[i].h, UNSELECTED_COLOR);
	}

	Gui::DrawString(langBlocks[0].x+25, langBlocks[0].y-2, 0.9f, WHITE, "Deutsch", 320);
	Gui::DrawString(langBlocks[1].x+25, langBlocks[1].y-2, 0.9f, WHITE, "English", 320);
	Gui::DrawString(langBlocks[2].x+25, langBlocks[2].y-2, 0.9f, WHITE, "Español", 320);
	Gui::DrawString(langBlocks[3].x+25, langBlocks[3].y-2, 0.9f, WHITE, "Français", 320);

	Gui::DrawString(langBlocks[4].x+25, langBlocks[4].y-2, 0.9f, WHITE, "Italiano", 320);
	Gui::DrawString(langBlocks[5].x+25, langBlocks[5].y-2, 0.9f, WHITE, "Lietuvių", 320);
	Gui::DrawString(langBlocks[6].x+25, langBlocks[6].y-2, 0.9f, WHITE, "Português", 320);
	Gui::DrawString(langBlocks[7].x+25, langBlocks[7].y-2, 0.9f, WHITE, "日本語", 320);

	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, fadealpha));
}

void Initial::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 8; i++) {
			if (touching(touch, langBlocks[i])) {
				Config::lang = i;
				Lang::load(Config::lang);
				Config::setBool("InitialSetup", true);
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		}
	}
}