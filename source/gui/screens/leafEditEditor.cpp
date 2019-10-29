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


#include "gui/colors.hpp"
#include "gui/keyboard.hpp"


#include "screens/leafEditEditor.hpp"
#include "screens/screenCommon.hpp"



extern bool touching(touchPosition touch, Structs::ButtonPos button);

	// Config stuff.
	int Sheet::barText;
	int Sheet::bgText;
	int Sheet::buttonText;
	int Sheet::boxText;
	int Sheet::fileBrowseText;
	int Sheet::MessageText;
	int Sheet::helpMsg;
	int Sheet::Color1;
	int Sheet::Color2;
	int Sheet::Color3;

extern CIniFile sheetFileIni;

void LeafEditEditor::Draw(void) const {
	if (mode == 0) {
		DrawIniBrowse();
	} else if (mode == 1) {
		DrawIniEditor();
	}
}

void LeafEditEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (mode == 0) {
		IniBrowseLogic(hDown, hHeld);
	} else if (mode == 1) {
		EditorLogic(hDown, hHeld, touch);
	}
}

void LeafEditEditor::DrawIniBrowse(void) const {
	std::string title = "LeafEdit - ";
	title += Lang::get("INI_BROWSE");

	Gui::DrawFileBrowseBG();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, title.c_str()))/2, 0, 0.72f, WHITE, title.c_str(), 400);
	std::string dirs;
	for (uint i=(selectedSheetIniFile<5) ? 0 : selectedSheetIniFile-5;i<dirContents.size()&&i<((selectedSheetIniFile<5) ? 6 : selectedSheetIniFile+1);i++) {
		if (i == selectedSheetIniFile) {
			dirs +=  ">" + dirContents[i].name + "\n\n";
		} else {
			dirs +=  dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}
	Gui::DrawString(26, 32, 0.51f, Config::fileBrowseText, dirs.c_str(), 400);
	Gui::DrawBottom();
}

void LeafEditEditor::IniBrowseLogic(u32 hDown, u32 hHeld) {
	if (keyRepeatDelay)	keyRepeatDelay--;

	if (refresh) {
		dirContents.clear();
		char startPath[PATH_MAX];
		getcwd(startPath, PATH_MAX);
		chdir("sdmc:/LeafEdit/Sheets");
		std::vector<DirEntry> dirContentsTemp;
		getDirectoryContents(dirContentsTemp);
		chdir(startPath);
		for(uint i=0;i<dirContentsTemp.size();i++) {
			dirContents.push_back(dirContentsTemp[i]);
		}
		refresh = false;
	}

	if (hDown & KEY_START) {
		refresh = true;
	}
	if (hDown & KEY_X) {
		if (dirContents.size() == 0) {
			Msg::DisplayWarnMsg(Lang::get("WHAT_YOU_DO"));
		} else {
			std::string prompt = Lang::get("WANT_USE_INI");
			prompt += "\n\n "+dirContents[selectedSheetIniFile].name+"";
			if(Msg::promptMsg(prompt.c_str())) {
				sheetFileIni = "sdmc:/LeafEdit/Sheets/"+dirContents[selectedSheetIniFile].name;
				Config::loadSheetIniStuff();
			}
		}
	} else if (hDown & KEY_A) {
		if (dirContents.size() == 0) {
			Msg::DisplayWarnMsg(Lang::get("WHAT_YOU_DO"));
		} else {
			std::string prompt = Lang::get("WANT_EDIT_INI");
			prompt += "\n\n "+dirContents[selectedSheetIniFile].name+"";
			if(Msg::promptMsg(prompt.c_str())) {
				sheetsFile = "sdmc:/LeafEdit/Sheets/"+dirContents[selectedSheetIniFile].name;
				sheetFile = sheetsFile;
				loadIniContents();
				mode = 1;
				savesPath = sheetsFile;
			}
		}
	} else if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if (hDown & KEY_Y) {
		if(Msg::promptMsg(Lang::get("CREATE_INI"))) {
			std::string newIni = "sdmc:/LeafEdit/Sheets/";
			newIni += Input::getLine(20, Lang::get("NEW_SHEET_NAME"));
			createNewSheet(newIni.c_str());
			refresh = true;
		}
	} else if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Press A to select the ini File.\nPress Y to to create a blank ini File.\nPress Start to refresh the filelist.\nPress B to exit from this Screen.");
	} else if (hHeld & KEY_UP) {
		if (selectedSheetIniFile > 0 && !keyRepeatDelay) {
			selectedSheetIniFile--;
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedSheetIniFile < dirContents.size()-1) {
			selectedSheetIniFile++;
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hDown & KEY_R) {
		fastMode = true;
	} else if (hDown & KEY_L) {
		fastMode = false;
	}
}

void LeafEditEditor::loadIniContents() {
	Sheet::barText = sheetFile.GetInt("COLORS", "BarText", Sheet::barText);
	Sheet::buttonText = sheetFile.GetInt("COLORS", "ButtonText", Sheet::buttonText);
	Sheet::bgText = sheetFile.GetInt("COLORS", "BGText", Sheet::bgText);
	Sheet::boxText = sheetFile.GetInt("COLORS", "BoxText", Sheet::boxText);
	Sheet::fileBrowseText = sheetFile.GetInt("COLORS", "FileBrowseText", Sheet::fileBrowseText);
	Sheet::MessageText = sheetFile.GetInt("COLORS", "MessageText", Sheet::MessageText);
	Sheet::helpMsg = sheetFile.GetInt("COLORS", "HelpMsg", Sheet::helpMsg);

	Sheet::Color1 = sheetFile.GetInt("UI", "Color1", Sheet::Color1);
	Sheet::Color2 = sheetFile.GetInt("UI", "Color2", Sheet::Color2);
	Sheet::Color3 = sheetFile.GetInt("UI", "Color3", Sheet::Color3);
}

void LeafEditEditor::saveIniContents() {
	sheetFile.SetInt("COLORS", "BarText", Sheet::barText);
	sheetFile.SetInt("COLORS", "ButtonText", Sheet::buttonText);
	sheetFile.SetInt("COLORS", "BGText", Sheet::bgText);
	sheetFile.SetInt("COLORS", "BoxText", Sheet::boxText);
	sheetFile.SetInt("COLORS", "FileBrowseText", Sheet::fileBrowseText);
	sheetFile.SetInt("COLORS", "MessageText", Sheet::MessageText);
	sheetFile.SetInt("COLORS", "HelpMsg", Sheet::helpMsg);

	sheetFile.SetInt("UI", "Color1", Sheet::Color1);
	sheetFile.SetInt("UI", "Color2", Sheet::Color2);
	sheetFile.SetInt("UI", "Color3", Sheet::Color3);
	sheetFile.SaveIniFile(savesPath);
}

void LeafEditEditor::createNewSheet(std::string sheetIni) {
	// create the file.
	FILE* ini = fopen(sheetIni.c_str(), "w");
	sheetFile = sheetIni;

	sheetFile.SetInt("COLORS", "BarText", BLACK);
	sheetFile.SetInt("COLORS", "ButtonText", BLACK);
	sheetFile.SetInt("COLORS", "BGText", BLACK);
	sheetFile.SetInt("COLORS", "BoxText", BLACK);
	sheetFile.SetInt("COLORS", "FileBrowseText", BLACK);
	sheetFile.SetInt("COLORS", "MessageText", BLACK);
	sheetFile.SetInt("COLORS", "HelpMsg", WHITE);

	sheetFile.SetInt("UI", "Color1", BLACK);
	sheetFile.SetInt("UI", "Color2", BLACK);
	sheetFile.SetInt("UI", "Color3", BLACK);

	sheetFile.SaveIniFile(sheetIni);
	fclose(ini);
}

void LeafEditEditor::DrawIniEditor(void) const {
	std::string title = "LeafEdit - ";
	title += Lang::get("INI_EDITOR");
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, title.c_str()))/2, 0, 0.72f, Sheet::barText, title.c_str(), 400);
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("CHANGE_INI_OPTION")))/2, 215, 0.72f, Sheet::barText, Lang::get("CHANGE_INI_OPTION"), 400);

	if (colorMode == 0) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("BARTEXT_COLOR")))/2, 90, 0.72f, Sheet::barText, Lang::get("BARTEXT_COLOR"), 400);
	} else if (colorMode == 1) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("BGTEXT_COLOR")))/2, 90, 0.72f, Sheet::bgText, Lang::get("BGTEXT_COLOR"), 400);
	} else if (colorMode == 2) {
		Gui::sprite(0, sprites_button_idx, 20, 80);
		Gui::DrawString(40, 90, 0.6f, Sheet::buttonText, Lang::get("UNSELECTED"), 400);
		Gui::Draw_ImageBlend(0, sprites_button_idx, 220, 80, selectedColor);
		Gui::DrawString(240, 90, 0.6f, Sheet::buttonText, Lang::get("SELECTED"), 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("BUTTONTEXT_COLOR")))/2, 150, 0.72f, Sheet::buttonText, Lang::get("BUTTONTEXT_COLOR"), 400);
	} else if (colorMode == 3) {
		Gui::Draw_Rect(40, 80, 320, 22, Config::Color3);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("BOXTEXT_COLOR")))/2, 80, 0.72f, Sheet::boxText, Lang::get("BOXTEXT_COLOR"), 400);
	} else if (colorMode == 4) {
		Gui::Draw_Rect(0, 27, 400, 31, Config::Color2);
		Gui::Draw_Rect(0, 58, 400, 31, Config::Color3);
		Gui::Draw_Rect(0, 89, 400, 31, Config::Color2);
		Gui::Draw_Rect(0, 120, 400, 31, Config::Color3);
		Gui::Draw_Rect(0, 151, 400, 31, Config::Color2);
		Gui::Draw_Rect(0, 182, 400, 31, Config::Color3);
		Gui::DrawString(26, 32, 0.53f, Sheet::fileBrowseText, Lang::get("SELTEXT_COLOR"), 400);
	} else if (colorMode == 5) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("MESTEXT_COLOR")))/2, 120, 0.72f, Sheet::MessageText, Lang::get("MESTEXT_COLOR"), 400);
	} else if (colorMode == 6) {
		Gui::sprite(0, sprites_helperBox_idx, 0, 27);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("INSTRUCTION_COLOR")))/2, 120, 0.72f, Sheet::helpMsg, Lang::get("INSTRUCTION_COLOR"), 400);
	} else if (colorMode == 7) {
		Gui::Draw_Rect(0, 0, 400, 30, Sheet::Color1);
		Gui::Draw_Rect(0, 210, 400, 30, Sheet::Color1);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("BAR_COLOR")))/2, 120, 0.72f, Sheet::Color1, Lang::get("BAR_COLOR"), 400);
	} else if (colorMode == 8) {
		Gui::sprite(0, sprites_helperBox_idx, 0, 27);
		Gui::Draw_Rect(0, 30, 400, 180, Sheet::Color2);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("BG_TOP_COLOR")))/2, 120, 0.72f, Sheet::Color2, Lang::get("BG_TOP_COLOR"), 400);
	} else if (colorMode == 9) {
		Gui::Draw_Rect(0, 30, 400, 180, Sheet::Color3);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("BG_BOTTOM_COLOR")))/2, 120, 0.72f, Sheet::Color3, Lang::get("BG_BOTTOM_COLOR"), 400);
	}

	Gui::DrawBottom();

	for (int i = 0; i < 10; i++) {
		if (colorMode == i) {
			Gui::Draw_Rect(25 + i * 25, 5, 16, 16, Config::Color2);
		}
	}

	Gui::DrawString(29 + 0 * 25, 5, 0.5f, Sheet::barText, "1", 400);
	Gui::DrawString(29 + 1 * 25, 5, 0.5f, Sheet::barText, "2", 400);
	Gui::DrawString(29 + 2 * 25, 5, 0.5f, Sheet::barText, "3", 400);
	Gui::DrawString(29 + 3 * 25, 5, 0.5f, Sheet::barText, "4", 400);
	Gui::DrawString(29 + 4 * 25, 5, 0.5f, Sheet::barText, "5", 400);
	Gui::DrawString(29 + 5 * 25, 5, 0.5f, Sheet::barText, "6", 400);
	Gui::DrawString(29 + 6 * 25, 5, 0.5f, Sheet::barText, "7", 400);
	Gui::DrawString(29 + 7 * 25, 5, 0.5f, Sheet::barText, "8", 400);
	Gui::DrawString(29 + 8 * 25, 5, 0.5f, Sheet::barText, "9", 400);
	Gui::DrawString(29 + 9 * 25, 5, 0.5f, Sheet::barText, "10", 400);

	Gui::Draw_Rect(buttons[0].x, buttons[0].y, 95, 41, RED);
	Gui::Draw_Rect(buttons[1].x, buttons[1].y, 95, 41, GREEN);
	Gui::Draw_Rect(buttons[2].x, buttons[2].y, 95, 41, SelectorBlue);

	if (colorMode == 0) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::barText, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::barText, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::barText, 0).c_str(), 400);
	} else if (colorMode == 1) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::bgText, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::bgText, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::bgText, 0).c_str(), 400);
	} else if (colorMode == 2) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::buttonText, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::buttonText, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::buttonText, 0).c_str(), 400);
	} else if (colorMode == 3) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::boxText, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::boxText, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::boxText, 0).c_str(), 400);
	} else if (colorMode == 4) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::fileBrowseText, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::fileBrowseText, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::fileBrowseText, 0).c_str(), 400);
	} else if (colorMode == 5) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::MessageText, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::MessageText, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::MessageText, 0).c_str(), 400);
	} else if (colorMode == 6) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::helpMsg, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::helpMsg, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::helpMsg, 0).c_str(), 400);
	} else if (colorMode == 7) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::Color1, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::Color1, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::Color1, 0).c_str(), 400);
	} else if (colorMode == 8) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::Color2, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::Color2, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::Color2, 0).c_str(), 400);
	} else if (colorMode == 9) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::Color3, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::Color3, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::Color3, 0).c_str(), 400);
	}

	Gui::sprite(0, sprites_back_idx, buttons[3].x, buttons[3].y);
}

void LeafEditEditor::EditorLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	int red;
	int green;
	int blue;

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Press L/R to switch the Ini Options.\nPress Start to save the options.\nPress B to revert the changes without saving.");
	}
	if (hDown & KEY_L || hDown & KEY_LEFT) {
		if(colorMode > 0)	colorMode--;
	}

	if (hDown & KEY_R || hDown & KEY_RIGHT) {
		if(colorMode < 9)	colorMode++;
	}

	if (hDown & KEY_START) {
		saveIniContents();
		sheetsFile = "";
		mode = 0;
	}

	if (hDown & KEY_B) {
		sheetsFile = "";
		mode = 0;
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, buttons[0])) {
			int temp = Input::getUint(255, Lang::get("ENTER_RED_RGB"));
			if(temp != -1) {
				red = temp;
				if (colorMode == 0) {
					Sheet::barText = RGBA8(red, ColorHelper::getColorValue(Sheet::barText, 1), ColorHelper::getColorValue(Sheet::barText, 0), 255);
				} else if (colorMode == 1) {
					Sheet::bgText = RGBA8(red, ColorHelper::getColorValue(Sheet::bgText, 1), ColorHelper::getColorValue(Sheet::bgText, 0), 255);
				} else if (colorMode == 2) {
					Sheet::buttonText = RGBA8(red, ColorHelper::getColorValue(Sheet::buttonText, 1), ColorHelper::getColorValue(Sheet::buttonText, 0), 255);
				} else if (colorMode == 3) {
					Sheet::boxText = RGBA8(red, ColorHelper::getColorValue(Sheet::boxText, 1), ColorHelper::getColorValue(Sheet::boxText, 0), 255);
				} else if (colorMode == 4) {
					Sheet::fileBrowseText = RGBA8(red, ColorHelper::getColorValue(Sheet::fileBrowseText, 1), ColorHelper::getColorValue(Sheet::fileBrowseText, 0), 255);
				} else if (colorMode == 5) {
					Sheet::MessageText = RGBA8(red, ColorHelper::getColorValue(Sheet::MessageText, 1), ColorHelper::getColorValue(Sheet::MessageText, 0), 255);
				} else if (colorMode == 6) {
					Sheet::helpMsg = RGBA8(red, ColorHelper::getColorValue(Sheet::helpMsg, 1), ColorHelper::getColorValue(Sheet::helpMsg, 0), 255);
				} else if (colorMode == 7) {
					Sheet::Color1 = RGBA8(red, ColorHelper::getColorValue(Sheet::Color1, 1), ColorHelper::getColorValue(Sheet::Color1, 0), 255);
				} else if (colorMode == 8) {
					Sheet::Color2 = RGBA8(red, ColorHelper::getColorValue(Sheet::Color2, 1), ColorHelper::getColorValue(Sheet::Color2, 0), 255);
				} else if (colorMode == 9) {
					Sheet::Color3 = RGBA8(red, ColorHelper::getColorValue(Sheet::Color3, 1), ColorHelper::getColorValue(Sheet::Color3, 0), 255);
				}
			}


		} else if (touching(touch, buttons[1])) {
			int temp = Input::getUint(255, Lang::get("ENTER_GREEN_RGB"));
			if(temp != -1) {
				green = temp;
				if (colorMode == 0) {
					Sheet::barText = RGBA8(ColorHelper::getColorValue(Sheet::barText, 2), green, ColorHelper::getColorValue(Sheet::barText, 0), 255);
				} else if (colorMode == 1) {
					Sheet::bgText = RGBA8(ColorHelper::getColorValue(Sheet::bgText, 2), green, ColorHelper::getColorValue(Sheet::bgText, 0), 255);
				} else if (colorMode == 2) {
					Sheet::buttonText = RGBA8(ColorHelper::getColorValue(Sheet::buttonText, 2), green, ColorHelper::getColorValue(Sheet::buttonText, 0), 255);
				} else if (colorMode == 3) {
					Sheet::boxText = RGBA8(ColorHelper::getColorValue(Sheet::boxText, 2), green, ColorHelper::getColorValue(Sheet::boxText, 0), 255);
				} else if (colorMode == 4) {
					Sheet::fileBrowseText = RGBA8(ColorHelper::getColorValue(Sheet::fileBrowseText, 2), green, ColorHelper::getColorValue(Sheet::fileBrowseText, 0), 255);
				} else if (colorMode == 5) {
					Sheet::MessageText = RGBA8(ColorHelper::getColorValue(Sheet::MessageText, 2), green, ColorHelper::getColorValue(Sheet::MessageText, 0), 255);
				} else if (colorMode == 6) {
					Sheet::helpMsg = RGBA8(ColorHelper::getColorValue(Sheet::helpMsg, 2), green, ColorHelper::getColorValue(Sheet::helpMsg, 0), 255);
				} else if (colorMode == 7) {
					Sheet::Color1 = RGBA8(ColorHelper::getColorValue(Sheet::Color1, 2), green, ColorHelper::getColorValue(Sheet::Color1, 0), 255);
				} else if (colorMode == 8) {
					Sheet::Color2 = RGBA8(ColorHelper::getColorValue(Sheet::Color2, 2), green, ColorHelper::getColorValue(Sheet::Color2, 0), 255);
				} else if (colorMode == 9) {
					Sheet::Color3 = RGBA8(ColorHelper::getColorValue(Sheet::Color3, 2), green, ColorHelper::getColorValue(Sheet::Color3, 0), 255);
				}
			}


		} else if (touching(touch, buttons[2])) {
			int temp = Input::getUint(255, Lang::get("ENTER_BLUE_RGB"));
			if(temp != -1) {
				blue = temp;
				if (colorMode == 0) {
					Sheet::barText = RGBA8(ColorHelper::getColorValue(Sheet::barText, 2), ColorHelper::getColorValue(Sheet::barText, 1), blue, 255);
				} else if (colorMode == 1) {
					Sheet::bgText = RGBA8(ColorHelper::getColorValue(Sheet::bgText, 2), ColorHelper::getColorValue(Sheet::bgText, 1), blue, 255);
				} else if (colorMode == 2) {
					Sheet::buttonText = RGBA8(ColorHelper::getColorValue(Sheet::buttonText, 2), ColorHelper::getColorValue(Sheet::buttonText, 1), blue, 255);
				} else if (colorMode == 3) {
					Sheet::boxText = RGBA8(ColorHelper::getColorValue(Sheet::boxText, 2), ColorHelper::getColorValue(Sheet::boxText, 1), blue, 255);
				} else if (colorMode == 4) {
					Sheet::fileBrowseText = RGBA8(ColorHelper::getColorValue(Sheet::fileBrowseText, 2), ColorHelper::getColorValue(Sheet::fileBrowseText, 1), blue, 255);
				} else if (colorMode == 5) {
					Sheet::MessageText = RGBA8(ColorHelper::getColorValue(Sheet::MessageText, 2), ColorHelper::getColorValue(Sheet::MessageText, 1), blue, 255);
				} else if (colorMode == 6) {
					Sheet::helpMsg = RGBA8(ColorHelper::getColorValue(Sheet::helpMsg, 2), ColorHelper::getColorValue(Sheet::helpMsg, 1), blue, 255);
				} else if (colorMode == 7) {
					Sheet::Color1 = RGBA8(ColorHelper::getColorValue(Sheet::Color1, 2), ColorHelper::getColorValue(Sheet::Color1, 1), blue, 255);
				} else if (colorMode == 8) {
					Sheet::Color2 = RGBA8(ColorHelper::getColorValue(Sheet::Color2, 2), ColorHelper::getColorValue(Sheet::Color2, 1), blue, 255);
				} else if (colorMode == 9) {
					Sheet::Color3 = RGBA8(ColorHelper::getColorValue(Sheet::Color3, 2), ColorHelper::getColorValue(Sheet::Color3, 1), blue, 255);
				}
			}
		} else if (touching(touch, buttons[3])) {
			if(Msg::promptMsg(Lang::get("SAVE_INI"))) {
				saveIniContents();
				sheetsFile = "";
				mode = 0;
			} else {
				sheetsFile = "";
				mode = 0;
			}
		}
	}
}