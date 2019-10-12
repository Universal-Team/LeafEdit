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

#include "common/config.hpp"
#include "common/inifile.h"

#include "core/villagerManagement.hpp"

#include "gui/keyboard.hpp"

#include "gui/screens/leafEditEditor.hpp"
#include "gui/screens/settings.hpp"
#include "gui/screens/screenCommon.hpp"

#include "lang/lang.h"
#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern CIniFile sheetFileIni;

void Settings::Draw(void) const
{
	if(screenMode == 0) {
		std::string Title;
		Title += Lang::title;
		Title += " - ";
		Title += Lang::mainMenu[2];

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

		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::settings[1]))/2, settingsButtons[0].y+10, 0.6f, Config::buttonText, Lang::settings[0], 140);
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Spritesheet"))/2, settingsButtons[1].y+10, 0.6f, Config::buttonText, "Spritesheet", 140);
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Ini Editor"))/2, settingsButtons[2].y+10, 0.6f, Config::buttonText, "Ini Editor", 140);
	} else if(screenMode == 1) {
		DrawSpriteSheetSelection();
	} else if(screenMode == 2) {
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
					screenMode = 2;
					break;
				case 1:
					screenMode = 1;
					break;
				case 2:
					Gui::setScreen(std::make_unique<LeafEditEditor>());
					break;
			}
		}
	} else if(screenMode == 1) {
		SpriteSheetLogic(hDown, hHeld);
	} else if(screenMode == 2) {
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
		Msg::HelperBox("Select Language, to select the GUI Language.\nSelect Spritsheet, to select a custom Spritesheet.\nSelect Ini Editor, to edit the 'sheet.ini' of the spritesheet.\nPress B to exit from this Screen.");
	}
}

extern C2D_SpriteSheet sprites;

void Settings::DrawSpriteSheetSelection(void) const {
	Gui::ScreenDraw(top);
	Gui::sprite(0, sprites_top_topbar_idx, 0, 0);
	Gui::sprite(0, sprites_fbBgTop_idx, 0, 27);
	Gui::sprite(0, sprites_top_bottombar_idx, 0, 213);

	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Select a Spritesheet to load."))/2, 2, 0.72f, Config::barText, "Select a Spritesheet to load.", 400);

	Gui::DrawString((400-Gui::GetStringWidth(0.60f, Lang::messages2[9]))/2, 216, 0.60f, Config::barText, Lang::messages2[9], 400);

	std::string sheets;
	for (uint i=(selectedSpriteSheet<5) ? 0 : selectedSpriteSheet-5;i<dirContents.size()&&i<((selectedSpriteSheet<5) ? 6 : selectedSpriteSheet+1);i++) {
		if (i == selectedSpriteSheet) {
			sheets += "> " + dirContents[i].name + "\n\n";
		} else {
			sheets += dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		sheets += "\n\n";
	}

	Gui::DrawString(26, 32, 0.51f, Config::fileBrowseText, sheets.c_str(), 400);

	Gui::ScreenDraw(bottom);
	Gui::sprite(0, sprites_bottom_topbar_idx, 0, 0);
	Gui::sprite(0, sprites_fbBgBottom_idx, 0, 27);
	Gui::sprite(0, sprites_bottom_bottombar_idx, 0, 213);
}


void Settings::SpriteSheetLogic(u32 hDown, u32 hHeld) {
	std::string defaultSheetIni = "romfs:/gfx/sheet.ini";
	if (keyRepeatDelay)	keyRepeatDelay--;

	if (dirChanged) {
		dirContents.clear();
		chdir("sdmc:/LeafEdit/SpriteSheets/");
		std::vector<DirEntry> dirContentsTemp;
		getDirectoryContents(dirContentsTemp);
		for(uint i=0;i<dirContentsTemp.size();i++) {
			dirContents.push_back(dirContentsTemp[i]);
		}
		dirChanged = false;
	}

	if(hDown & KEY_A) {
		if (dirContents.size() == 0) {
			Msg::DisplayWarnMsg("What are you trying to do? :P");
		} else {
			std::string prompt = "Would you like to load this SpriteSheet?";
			prompt += "\n\n";
			prompt += "'";
			prompt += dirContents[selectedSpriteSheet].name;
			prompt += "'";
			if(Msg::promptMsg(prompt.c_str())) {

				// Set the whole Path stuff.
				selectedSheet = dirContents[selectedSpriteSheet].name.c_str();
				finalSheet = "sdmc:/LeafEdit/SpriteSheets/";
				finalSheet += selectedSheet.c_str();
				sheetIni = finalSheet.c_str();
				finalSheet += "/sprites.t3x";
				sheetIni += "/sheet.ini";

				// Set the SpriteSheet.
				if((access(finalSheet.c_str(), F_OK) == 0)) {
					Msg::SheetMsg("Now freeing the SpriteSheet...");
					C2D_SpriteSheetFree(sprites);
					Msg::SheetMsg("Now Loading the new SpriteSheet...");
					sprites	= C2D_SpriteSheetLoad(finalSheet.c_str());
				} else {
					Msg::DisplayWarnMsg("'sprites.t3x' does not exist on this folder!");
				}

					// Set the Sheet Ini.
					if((access(sheetIni.c_str(), F_OK) == 0)) {
					sheetFileIni = sheetIni;
					Config::loadSheetIniStuff();
				} else {
					Msg::DisplayWarnMsg("'sheet.ini' does not exist on this folder!");
					sheetFileIni = defaultSheetIni;
					Config::loadSheetIniStuff();
				}
				Msg::SheetMsg("If something looks corrupted, then not all Graphics\nAre updated. Please inform the SpriteSheet Creator\nto update the SpriteSheet and the ini file.");
				for (int i = 0; i < 60*3; i++) {
					gspWaitForVBlank();
				}
			}

			// Clear String stuff.
			finalSheet = "";
			selectedSheet = "";
			sheetIni = "";
			screenMode = 0;
			playChange();
		}
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Select a Spritesheet and press A to load it.\nSelect a Spritesheet and press Y to autoload it at startup.\nPress X to reset the Spritesheet.\nPress Start to refresh the filelist.\nPress B to exit from this Screen.");
	}

	if (hDown & KEY_Y) {
		if (dirContents.size() == 0) {
			Msg::DisplayWarnMsg("What are you trying to do? :P");
		} else {
			std::string prompt = "Would you like to autoboot this SpriteSheet?";
			prompt += "\n\n";
			prompt += "'";
			prompt += dirContents[selectedSpriteSheet].name;
			prompt += "'";
			if(Msg::promptMsg(prompt.c_str())) {
				// Set the whole Path stuff.
				selectedSheet = dirContents[selectedSpriteSheet].name.c_str();
				finalSheet = "sdmc:/LeafEdit/SpriteSheets/";
				finalSheet += selectedSheet.c_str();
				sheetIni = finalSheet.c_str();
				finalSheet += "/sprites.t3x";
				sheetIni += "/sheet.ini";

				// Set the SpriteSheet.
				if((access(finalSheet.c_str(), F_OK) == 0)) {
					Config::saveSheet(finalSheet.c_str());
				} else {
					Msg::DisplayWarnMsg("'sprites.t3x' does not exist on this folder!");
				}

				// Set the Sheet Ini.
				if((access(sheetIni.c_str(), F_OK) == 0)) {
					Config::saveSheetIni(sheetIni.c_str());
				} else {
					Msg::DisplayWarnMsg("'sheet.ini' does not exist on this folder!");
				}
				Msg::SheetMsg("If something looks corrupted, then not all Graphics\nAre updated. Please inform the SpriteSheet Creator\nto update the SpriteSheet and the ini file.");
				for (int i = 0; i < 60*3; i++) {
					gspWaitForVBlank();
				}
			}

			// Clear String stuff.
			finalSheet = "";
			selectedSheet = "";
			sheetIni = "";
			screenMode = 0;
			playChange();
		}
	}

	if (hDown & KEY_X) {
		std::string prompt = "Would you like to reset the SpriteSheet?";
		if(Msg::promptMsg(prompt.c_str())) {
			finalSheet = "romfs:/gfx/sprites.t3x";
			sheetIni = "romfs:/gfx/sheet.ini";

			Config::saveSheet(finalSheet.c_str());
			Config::saveSheetIni(sheetIni.c_str());

			Msg::SheetMsg("Now freeing the SpriteSheet...");
			C2D_SpriteSheetFree(sprites);
			Msg::SheetMsg("Now Loading the new SpriteSheet...");
			sprites	= C2D_SpriteSheetLoad(finalSheet.c_str());
			sheetFileIni = sheetIni;
			Config::loadSheetIniStuff();
		}
		// Clear Strings.
		finalSheet = "";
		sheetIni = "";
		screenMode = 0;
		playChange();
	}

	if (hHeld & KEY_UP) {
		if (selectedSpriteSheet > 0 && !keyRepeatDelay) {
			selectedSpriteSheet--;
			keyRepeatDelay = 6;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedSpriteSheet < dirContents.size()-1) {
			selectedSpriteSheet++;
			keyRepeatDelay = 6;
		}
	} else if (hDown & KEY_B) {
		if(Msg::promptMsg(Lang::messages[7])) {
			screenMode = 0;
		}
	} else if (hDown & KEY_START) {
		dirChanged = true;
	}
}


void Settings::DrawLangScreen(void) const {
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Lang::language))/2, 2, 0.8f, Config::barText, Lang::language, 398);
	Gui::DrawBottom();

	if (Config::lang == 0) {
		Gui::sprite(0, sprites_selectedBox_idx, 37, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 172);

		Gui::sprite(0, sprites_unselectedBox_idx, 177, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 172);

	} else if (Config::lang == 1) {
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 52);
		Gui::sprite(0, sprites_selectedBox_idx, 37, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 172);

		Gui::sprite(0, sprites_unselectedBox_idx, 177, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 172);

	} else if (Config::lang == 2) {
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 92);
		Gui::sprite(0, sprites_selectedBox_idx, 37, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 172);

		Gui::sprite(0, sprites_unselectedBox_idx, 177, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 172);

	} else if (Config::lang == 3) {
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 132);
		Gui::sprite(0, sprites_selectedBox_idx, 37, 172);

		Gui::sprite(0, sprites_unselectedBox_idx, 177, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 172);

	} else if (Config::lang == 4) {
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 172);

		Gui::sprite(0, sprites_selectedBox_idx, 177, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 172);

	} else if (Config::lang == 5) {
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 172);

		Gui::sprite(0, sprites_unselectedBox_idx, 177, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 132);
		Gui::sprite(0, sprites_selectedBox_idx, 177, 172);

	} else if (Config::lang == 6) {
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 172);

		Gui::sprite(0, sprites_unselectedBox_idx, 177, 52);
		Gui::sprite(0, sprites_selectedBox_idx, 177, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 172);

	} else if (Config::lang == 7) {
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 92);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 37, 172);

		Gui::sprite(0, sprites_unselectedBox_idx, 177, 52);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 92);
		Gui::sprite(0, sprites_selectedBox_idx, 177, 132);
		Gui::sprite(0, sprites_unselectedBox_idx, 177, 172);
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
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::loadLangStrings(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[1])) {
			Config::lang = 1;
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::loadLangStrings(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[2])) {
			Config::lang = 2;
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::loadLangStrings(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[3])) {
			Config::lang = 3;
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::loadLangStrings(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[4])) {
			Config::lang = 4;
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::loadLangStrings(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[5])) {
			Config::lang = 6;
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::loadLangStrings(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[6])) {
			Config::lang = 7;
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::loadLangStrings(Config::lang);
			Config::saveConfig();

		} else if (touching(touch, langBlocks[7])) {
			Config::lang = 5;
			VillagerManagement::LoadVillagerDatabase(Config::lang);
			Lang::loadLangStrings(Config::lang);
			Config::saveConfig();
		}
	}
}