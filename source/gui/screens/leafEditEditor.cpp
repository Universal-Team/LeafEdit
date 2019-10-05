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
	Gui::ScreenDraw(top);
	Gui::sprite(0, sprites_top_topbar_idx, 0, 0);
	Gui::sprite(0, sprites_fbBgTop_idx, 0, 27);
	Gui::sprite(0, sprites_top_bottombar_idx, 0, 213);
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "LeafEdit Ini Browse"))/2, 0, 0.72f, WHITE, "LeafEdit Ini Browse");

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

	Gui::ScreenDraw(bottom);
	Gui::sprite(0, sprites_bottom_topbar_idx, 0, 0);
	Gui::sprite(0, sprites_fbBgBottom_idx, 0, 27);
	Gui::sprite(0, sprites_bottom_bottombar_idx, 0, 213);
}

void LeafEditEditor::IniBrowseLogic(u32 hDown, u32 hHeld) {
	if (keyRepeatDelay)	keyRepeatDelay--;

			if (refresh) {
			dirContents.clear();
			char startPath[PATH_MAX];
			getcwd(startPath, PATH_MAX);
			chdir("sdmc:/LeafEdit/SpriteSheets");
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

		if(hDown & KEY_A) {
			if (dirContents.size() == 0) {
				Msg::DisplayWarnMsg("What are you trying to do? :P");
			} else {
				if(Msg::promptMsg("Do you want edit this ini? : \n\n "+dirContents[selectedSheetIniFile].name+"")) {
					sheetsFile = "sdmc:/LeafEdit/SpriteSheets/"+dirContents[selectedSheetIniFile].name;
					sheetsFile += "/sheet.ini";
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
			if (dirContents.size() == 0) {
				Msg::DisplayWarnMsg("What are you trying to do? :P");
			} else {
				if(Msg::promptMsg("Are you sure, you want to create 'sheet.ini' \ninto this folder?\nIt will reset the existing Ini File, if it already exist!")) {
					std::string newIni = "sdmc:/LeafEdit/SpriteSheets/"+dirContents[selectedSheetIniFile].name;
					newIni += "/sheet.ini";
					createNewSheet(newIni.c_str());
					refresh = true;
				}
			}

		} else if (hHeld & KEY_SELECT) {
			Msg::HelperBox("Press A to select the Folder with the ini file.\nPress Y to a Folder, to create a blank 'sheet.ini' File.\nPress Start to refresh the filelist.\nPress B to exit from this Screen.");
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
}

void LeafEditEditor::saveIniContents() {
	sheetFile.SetInt("COLORS", "BarText", Sheet::barText);
	sheetFile.SetInt("COLORS", "ButtonText", Sheet::buttonText);
	sheetFile.SetInt("COLORS", "BGText", Sheet::bgText);
	sheetFile.SetInt("COLORS", "BoxText", Sheet::boxText);
	sheetFile.SetInt("COLORS", "FileBrowseText", Sheet::fileBrowseText);
	sheetFile.SetInt("COLORS", "MessageText", Sheet::MessageText);
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

	sheetFile.SaveIniFile(sheetIni);
	fclose(ini);
}

void LeafEditEditor::DrawIniEditor(void) const {
	Gui::ScreenDraw(top);
	Gui::Draw_Rect(0, 0, 400, 27, GRAY);
	Gui::Draw_Rect(0, 27, 400, 186, DARKGRAY);
	Gui::Draw_Rect(0, 213, 400, 213, GRAY);

	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "LeafEdit Ini Editor"))/2, 0, 0.72f, Sheet::barText, "LeafEdit Ini Editor", 400);
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Press L/R to change the next Ini option."))/2, 215, 0.72f, Sheet::barText, "Press L/R to change the next Ini option.", 400);

	if (colorMode == 0) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the BarText Color."))/2, 90, 0.72f, Sheet::barText, "This is the BarText Color.", 400);
	} else if (colorMode == 1) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the BGText Color."))/2, 90, 0.72f, Sheet::bgText, "This is the BGText Color.", 400);
	} else if (colorMode == 2) {
		Gui::sprite(0, sprites_button_idx, 20, 80);
		Gui::DrawString(40, 90, 0.6f, Sheet::buttonText, "Unselected", 400);
		Gui::Draw_ImageBlend(0, sprites_button_idx, 220, 80, selectedColor);
		Gui::DrawString(240, 90, 0.6f, Sheet::buttonText, "Selected", 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the ButtonText Color."))/2, 150, 0.72f, Sheet::buttonText, "This is the ButtonText Color.", 400);
	} else if (colorMode == 3) {
		Gui::sprite(0, sprites_topbox_idx, 40, 80);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the BoxText Color."))/2, 80, 0.72f, Sheet::boxText, "This is the BoxText Color.", 400);
	} else if (colorMode == 4) {
		Gui::sprite(0, sprites_fbBgTop_idx, 0, 27);
		Gui::DrawString(26, 32, 0.53f, Sheet::fileBrowseText, "> This is the Selector Text Color.\n\nThis is the Selector Text Color.", 400);
	} else if (colorMode == 5) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the Message Text Color."))/2, 120, 0.72f, Sheet::MessageText, "This is the Message Text Color.", 400);
	}

	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 27, GRAY);
	Gui::Draw_Rect(0, 27, 320, 186, DARKGRAY);
	Gui::Draw_Rect(0, 213, 320, 213, GRAY);
	
	for (int i = 0; i < 6; i++) {
		if (colorMode == i) {
			Gui::Draw_Rect(25 + i * 25, 5, 16, 16, DARKGRAY);
		}
	}

	Gui::DrawString(29 + 0 * 25, 5, 0.5f, Sheet::barText, "1", 400);
	Gui::DrawString(29 + 1 * 25, 5, 0.5f, Sheet::barText, "2", 400);
	Gui::DrawString(29 + 2 * 25, 5, 0.5f, Sheet::barText, "3", 400);
	Gui::DrawString(29 + 3 * 25, 5, 0.5f, Sheet::barText, "4", 400);
	Gui::DrawString(29 + 4 * 25, 5, 0.5f, Sheet::barText, "5", 400);
	Gui::DrawString(29 + 5 * 25, 5, 0.5f, Sheet::barText, "6", 400);

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
		if(colorMode < 5)	colorMode++;
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
			int temp = Input::getUint(255, "Please Type in the Red RGB Value.");
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
				}
			}


		} else if (touching(touch, buttons[1])) {
			int temp = Input::getUint(255, "Please Type in the Green RGB Value.");
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
				}
			}


		} else if (touching(touch, buttons[2])) {
			int temp = Input::getUint(255, "Please Type in the Blue RGB Value.");
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
				}
			}
		} else if (touching(touch, buttons[3])) {
			if(Msg::promptMsg("Would you like, to save the Ini?")) {
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