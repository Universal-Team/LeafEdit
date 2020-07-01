	/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
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

#include "credits.hpp"
#include "gui.hpp"

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Credits::Draw(void) const {
	// 3 -> QR Code.
	if (DisplayMode != 3) {
		GFX::DrawTop();
		if (config->newStyle())	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209);
		Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - Credits", 400, 0, font);
		Gui::DrawStringCentered(0, 30, 0.9f, BLACK, "Developed by Universal-Team.", 390, 0, font);
		Gui::DrawStringCentered(0, 50, 0.9f, BLACK, "Main Developer: SuperSaiyajinStackie", 390, 0, font);
		GFX::DrawGUI(gui_stackie_idx, 5, 85);
		GFX::DrawGUI(gui_universal_core_idx, 200, 110);
		Gui::DrawString(395-Gui::GetStringWidth(0.8, std::string("Current Version: ") + V_STRING, font), 219, 0.8, WHITE, std::string("Current Version: ") + V_STRING, 400, 0, font);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		GFX::DrawBottom();
		Gui::DrawStringCentered(0, 217, 0.7f, BLACK, discordText ? Lang::get("SHOW_QR") : Lang::get("LINK"), 310, 0, font);
	}

	if (DisplayMode == 1) {
		Gui::DrawStringCentered(0, -2 + barOffset, 0.8f, BLACK, "General Credits", 310, 0, font);
		Gui::DrawStringCentered(0, 30, 0.8f, BLACK, "Cuyler, Slattz", 310, 0, font);
		Gui::DrawStringCentered(0, 50, 0.7f, BLACK, "For a lot of pre-research work with NLTK & ACSE.", 310, 0, font);
		Gui::DrawStringCentered(0, 80, 0.8f, BLACK, "piepie62, FlagBrew, PKSM-Core", 310, 0, font);
		Gui::DrawStringCentered(0, 100, 0.7f, BLACK, "For helping me and basically the idea of the Core-Structure.", 310, 0, font);
		Gui::DrawStringCentered(0, 130, 0.8f, BLACK, "Pk11", 310, 0, font);
		Gui::DrawStringCentered(0, 150, 0.7f, BLACK, "For helping me out by problems.", 310, 0, font);
		Gui::DrawStringCentered(0, 180, 0.8f, BLACK, "TotallyNotGuy", 310, 0, font);
		Gui::DrawStringCentered(0, 200, 0.7f, BLACK, "For the amazing Graphic work!", 310, 0, font);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	} else if (DisplayMode == 2) {
		Gui::DrawStringCentered(0, -2 + barOffset, 0.8f, BLACK, "Translators", 310, 0, font);
		Gui::DrawString(5, 45, 0.8f, BLACK, "Deutsch\nEnglish\nEspañol\nFrançais\nItaliano\nLietuvių\nPortuguês\n日本語", 310, 0, font);
		Gui::DrawString(150, 45, 0.8f, BLACK, "SuperSaiyajinStackie\nSuperSaiyajinStackie\nYoSoy\nantoine62\nedo9300\nlemonnade0\nChips, David Pires\nPk11", 310, 0, font);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	} else if (DisplayMode == 3) {
		qr_code();
	}
}

void Credits::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	Loop();

	if (hDown & KEY_RIGHT) {
		if (DisplayMode < 3)	DisplayMode++;
	}

	if (hDown & KEY_LEFT) {
		if (DisplayMode > 1)	DisplayMode--;
	}

	if (hDown & KEY_B) {
		Gui::screenBack(true);
	}
}

// Qr Code.
void Credits::qr_code() const {
	GFX::DrawTop();
	Gui::Draw_Rect(0, 0, 400, 240, DIM);
	GFX::DrawGUI(gui_discord_idx, 115, 35);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, DIM);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void Credits::Loop() {
	gspWaitForVBlank();
	if (delay > 0) {
		delay--;
	} else {
		delay = 120;
		discordText = !discordText;
	}
}