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

#include "gui/gui.hpp"

#include "gui/screens/credits.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Credits::Draw(void) const {
	if (DisplayMode == 1) {
		Gui::ScreenDraw(top);
		Gui::Draw_Rect(0, 0, 400, 30, green2);
		Gui::Draw_Rect(0, 30, 400, 180, green4);
		Gui::Draw_Rect(0, 210, 400, 30, green2);

		Gui::DrawString((400-Gui::GetStringWidth(0.8f, "LeafEdit - Credits"))/2, 2, 0.8f, greentext, "LeafEdit - Credits", 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, "Developed by: Universal-Team."))/2, 40, 0.8f, greentext, "Developed by: Universal-Team.", 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, "Main Developer: VoltZ"))/2, 70, 0.8f, greentext, "Main Developer: VoltZ", 400);
		Gui::sprite(0, sprites_voltZ_idx, 150, 110);
		std::string currentVersion = "Current Version: ";
		currentVersion += V_STRING;
		Gui::DrawString(395-Gui::GetStringWidth(FONT_SIZE_18, currentVersion), 215, FONT_SIZE_18, greentext, currentVersion, 400);

		Gui::ScreenDraw(bottom);
		Gui::Draw_Rect(0, 0, 320, 30, green2);
		Gui::Draw_Rect(0, 30, 320, 180, green3);
		Gui::Draw_Rect(0, 210, 320, 30, green2);

		Gui::DrawString((320-Gui::GetStringWidth(0.7f, "Many Thanks to:"))/2, 2, 0.8f, greentext, "Many Thanks to:", 320);
		Gui::DrawString((320-Gui::GetStringWidth(0.7f, "Cuyler, Slattz"))/2, 40, 0.7f, greentext, "Cuyler, Slattz", 320);
		Gui::DrawString((320-Gui::GetStringWidth(0.5f, "- For the core part of NLTK."))/2, 70, 0.5f, greentext, "- For the core part of NLTK.", 320);

		Gui::DrawString((320-Gui::GetStringWidth(0.7f, "All our Translators"))/2, 100, 0.7f, greentext, "All our Translators", 320);
		Gui::DrawString((320-Gui::GetStringWidth(0.5f, "- For helping translate LeafEdit's strings."))/2, 130, 0.5f, greentext, "- For helping translate LeafEdit's strings.", 320);

		Gui::DrawString((320-Gui::GetStringWidth(0.7f, "Pk11"))/2, 160, 0.7f, greentext, "Pk11", 320);
		Gui::DrawString((320-Gui::GetStringWidth(0.5f, "- For helping out with problems."))/2, 190, 0.5f, greentext, "- For helping out with problems.", 320);
		Gui::DrawString((320-Gui::GetStringWidth(0.55, discordText ? "Click here to show the QR Code." : "Join our Discord: https://discord.gg/KDJCfGF"))/2, 217, 0.55, greentext, discordText ? "Click here to show the QR Code." : "Join our Discord: https://discord.gg/KDJCfGF", 320);
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
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 30, green2);
	Gui::Draw_Rect(0, 30, 400, 180, green4);
	Gui::Draw_Rect(0, 210, 400, 30, green2);

	Gui::Draw_Rect(0, 0, 400, 240, DIM);
	Gui::sprite(0, sprites_discord_idx, 115, 35);

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, green2);
	Gui::Draw_Rect(0, 30, 320, 180, green3);
	Gui::Draw_Rect(0, 210, 320, 30, green2);

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