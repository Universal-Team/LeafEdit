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

#include "gui/gui.hpp"

#include "gui/screens/credits.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Credits::Draw(void) const {
	if (DisplayMode == 1) {
		Gui::DrawTop();
		Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("CREDITS"), 400);
		Gui::DrawStringCentered(0, 30, 0.9f, WHITE, Lang::get("DEVELOPED_BY"), 390);
		Gui::DrawStringCentered(0, 70, 0.9f, WHITE, Lang::get("MAIN_DEV"), 390);
		Gui::sprite(0, sprites_stackZ_idx, 120, 100);
		Gui::DrawString(395-Gui::GetStringWidth(0.8, Lang::get("CURRENT_VERSION") + V_STRING), 215, 0.8, WHITE, Lang::get("CURRENT_VERSION") + V_STRING, 400);
		Gui::DrawBottom();
		Gui::DrawStringCentered(0, 2, 0.8f, WHITE, Lang::get("MANY_THANKS"), 310);
		Gui::DrawStringCentered(0, 40, 0.8f, WHITE, "Cuyler, Slattz", 310);
		Gui::DrawStringCentered(0, 70, 0.7f, WHITE, Lang::get("CORE_NLTK"), 310);
		Gui::DrawStringCentered(0, 100, 0.8f, WHITE, Lang::get("TRANSLATORS"), 310);
		Gui::DrawStringCentered(0, 130, 0.7f, WHITE, Lang::get("HELP_TRANSLATE"), 310);
		Gui::DrawStringCentered(0, 160, 0.8f, WHITE, "Pk11", 310);
		Gui::DrawStringCentered(0, 190, 0.7f, WHITE, Lang::get("HELP_PROBLEMS"), 310);
		Gui::DrawStringCentered(0, 217, 0.7f, WHITE, discordText ? Lang::get("SHOW_QR") : Lang::get("LINK"), 310);
		Gui::DrawString((320-Gui::GetStringWidth(0.8f, Lang::get("TRANSLATORS")))/2, 100, 0.8f, WHITE, Lang::get("TRANSLATORS"), 320);
		Gui::DrawString((320-Gui::GetStringWidth(0.7f, Lang::get("HELP_TRANSLATE")))/2, 130, 0.7f, WHITE, Lang::get("HELP_TRANSLATE"), 320);
	} else if (DisplayMode == 2) {
		qr_code();
	}
}

void Credits::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	Loop();
	if (DisplayMode == 1) {
		if (hDown & KEY_B) {
			Gui::screenBack();
			return;
		}
		if (hDown & KEY_TOUCH) {
			if (touching(touch, touchPos[1])) {
				DisplayMode = 2;
			}
		}

	} else if (DisplayMode == 2) {
		if (hDown & KEY_TOUCH) {
			if (touching(touch, touchPos[0])) {
				DisplayMode = 1;
			}
		}
		if (hDown & KEY_B) {
			DisplayMode = 1;
		}
	}
}

// Qr Code.

void Credits::qr_code() const
{
	Gui::DrawTop();
	Gui::Draw_Rect(0, 0, 400, 240, DIM);
	Gui::sprite(0, sprites_discord_idx, 115, 35);
	Gui::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, DIM);
	Gui::sprite(0, sprites_back_idx, touchPos[0].x, touchPos[0].y);
}

void Credits::Loop() {
	gspWaitForVBlank();
	if(delay > 0) {
		delay--;
	} else {
		delay = 120;
		discordText = !discordText;
	}
}