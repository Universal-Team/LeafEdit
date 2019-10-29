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

#include "common/inifile.h"

#include "core/management/itemManagement.hpp"
#include "core/management/villagerManagement.hpp"

#include "gui/keyboard.hpp"

#include "gui/screens/credits.hpp"
#include "gui/screens/leafEditEditor.hpp"
#include "gui/screens/settings.hpp"
#include "gui/screens/screenCommon.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern CIniFile sheetFileIni;

void Settings::Draw(void) const
{
	if(screenMode == 0) {
		std::string Title;
		Title += "LeafEdit";
		Title += " - ";
		Title += Lang::get("SETTINGS");

		Gui::DrawTop();
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, Config::barText, Title.c_str(), 400);

		Gui::DrawBottom();

		if (Selection == 0) {
			Gui::Draw_ImageBlend(0, sprites_button_idx, settingsButtons[0].x, settingsButtons[0].y, selectedColor);
			Gui::sprite(0, sprites_button_idx, settingsButtons[1].x, settingsButtons[1].y);
			Gui::sprite(0, sprites_button_idx, settingsButtons[2].x, settingsButtons[2].y);

		} else if (Selection == 1) {
			Gui::sprite(0, sprites_button_idx, settingsButtons[0].x, settingsButtons[0].y);
			Gui::Draw_ImageBlend(0, sprites_button_idx, settingsButtons[1].x, settingsButtons[1].y, selectedColor);
			Gui::sprite(0, sprites_button_idx, settingsButtons[2].x, settingsButtons[2].y);

		} else if (Selection == 2) {
			Gui::sprite(0, sprites_button_idx, settingsButtons[0].x, settingsButtons[0].y);
			Gui::sprite(0, sprites_button_idx, settingsButtons[1].x, settingsButtons[1].y);
			Gui::Draw_ImageBlend(0, sprites_button_idx, settingsButtons[2].x, settingsButtons[2].y, selectedColor);
		}

		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::get("LANGUAGE")))/2, settingsButtons[0].y+10, 0.6f, Config::buttonText, Lang::get("LANGUAGE"), 140);
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Ini Editor"))/2, settingsButtons[1].y+10, 0.6f, Config::buttonText, "Ini Editor", 140);
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Credits"))/2, settingsButtons[2].y+10, 0.6f, Config::buttonText, "Credits", 140);
	} else if(screenMode == 1) {
		DrawLangScreen();
	}
}

void Settings::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if(screenMode == 0) {
		SelectionLogic(hDown, hHeld);

		if (hDown & KEY_B) {
			Gui::screenBack();
			return;
		}

		if (hDown & KEY_A) {
			switch(Selection) {
				case 0:
					screenMode = 1;
					break;
				case 1:
					Gui::setScreen(std::make_unique<LeafEditEditor>());
					break;
				case 2:
					Gui::setScreen(std::make_unique<Credits>());
					break;
			}
		}
	} else if(screenMode == 1) {
		langScreenLogic(hDown, hHeld, touch);
	}
}

void Settings::SelectionLogic(u32 hDown, u32 hHeld)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	} else if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Select Language, to select the GUI Language.\nSelect Ini Editor, to edit the 'sheet.ini' of the spritesheet.\nPress B to exit from this Screen.");
	}
}


void Settings::DrawLangScreen(void) const {
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Lang::get("SELECT_LANG")))/2, 2, 0.8f, Config::barText, Lang::get("SELECT_LANG"), 398);
	Gui::DrawBottom();

	if (Config::lang == 0) {
		Gui::Draw_Rect(37, 52, 20, 20, RED);
		Gui::Draw_Rect(37, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 172, 20, 20, Config::Color2);

		Gui::Draw_Rect(177, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 172, 20, 20, Config::Color2);

	} else if (Config::lang == 1) {
		Gui::Draw_Rect(37, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 92, 20, 20, RED);
		Gui::Draw_Rect(37, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 172, 20, 20, Config::Color2);

		Gui::Draw_Rect(177, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 172, 20, 20, Config::Color2);

	} else if (Config::lang == 2) {
		Gui::Draw_Rect(37, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 132, 20, 20, RED);
		Gui::Draw_Rect(37, 172, 20, 20, Config::Color2);

		Gui::Draw_Rect(177, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 172, 20, 20, Config::Color2);

	} else if (Config::lang == 3) {
		Gui::Draw_Rect(37, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 172, 20, 20, RED);

		Gui::Draw_Rect(177, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 172, 20, 20, Config::Color2);

	} else if (Config::lang == 4) {
		Gui::Draw_Rect(37, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 172, 20, 20, Config::Color2);

		Gui::Draw_Rect(177, 52, 20, 20, RED);
		Gui::Draw_Rect(177, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 172, 20, 20, Config::Color2);

	} else if (Config::lang == 5) {
		Gui::Draw_Rect(37, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 172, 20, 20, Config::Color2);

		Gui::Draw_Rect(177, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 172, 20, 20, RED);

	} else if (Config::lang == 6) {
		Gui::Draw_Rect(37, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 172, 20, 20, Config::Color2);

		Gui::Draw_Rect(177, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 92, 20, 20, RED);
		Gui::Draw_Rect(177, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 172, 20, 20, Config::Color2);

	} else if (Config::lang == 7) {
		Gui::Draw_Rect(37, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 132, 20, 20, Config::Color2);
		Gui::Draw_Rect(37, 172, 20, 20, Config::Color2);

		Gui::Draw_Rect(177, 52, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 92, 20, 20, Config::Color2);
		Gui::Draw_Rect(177, 132, 20, 20, RED);
		Gui::Draw_Rect(177, 172, 20, 20, Config::Color2);
	}

	Gui::DrawString(langBlocks[0].x+25, langBlocks[0].y-2, 0.7f, Config::bgText, "Deutsch", 320);
	Gui::DrawString(langBlocks[1].x+25, langBlocks[1].y-2, 0.7f, Config::bgText, "English", 320);
	Gui::DrawString(langBlocks[2].x+25, langBlocks[2].y-2, 0.7f, Config::bgText, "Español", 320);
	Gui::DrawString(langBlocks[3].x+25, langBlocks[3].y-2, 0.7f, Config::bgText, "Français", 320);

	Gui::DrawString(langBlocks[4].x+25, langBlocks[4].y-2, 0.7f, Config::bgText, "Italiano", 320);
	Gui::DrawString(langBlocks[5].x+25, langBlocks[5].y-2, 0.7f, Config::bgText, "Lietuvių", 320);
	Gui::DrawString(langBlocks[6].x+25, langBlocks[6].y-2, 0.7f, Config::bgText, "Português", 320);
	Gui::DrawString(langBlocks[7].x+25, langBlocks[7].y-2, 0.7f, Config::bgText, "日本語", 320);

	Gui::sprite(0, sprites_back_idx, langBlocks[8].x, langBlocks[8].y);
}

void Settings::langScreenLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = 0;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Press on the Box, to select your Language.\nPress B to exit from this Screen.");
	}

	if (hDown & KEY_TOUCH && touching(touch, langBlocks[8])) {
		screenMode = 0;
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, langBlocks[0])) {
			Config::lang = 0;
			ItemManagement::LoadDatabase(Config::lang);
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::load(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[1])) {
			Config::lang = 1;
			ItemManagement::LoadDatabase(Config::lang);
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::load(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[2])) {
			Config::lang = 2;
			ItemManagement::LoadDatabase(Config::lang);
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::load(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[3])) {
			Config::lang = 3;
			ItemManagement::LoadDatabase(Config::lang);
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::load(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[4])) {
			Config::lang = 4;
			ItemManagement::LoadDatabase(Config::lang);
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::load(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[5])) {
			Config::lang = 6;
			ItemManagement::LoadDatabase(Config::lang);
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::load(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[6])) {
			Config::lang = 7;
			ItemManagement::LoadDatabase(Config::lang);
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::load(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[7])) {
			Config::lang = 5;
			ItemManagement::LoadDatabase(Config::lang);
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::load(Config::lang);
			Config::saveConfig();
		}
	}
}