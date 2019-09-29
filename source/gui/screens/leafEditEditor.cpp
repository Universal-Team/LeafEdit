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
	int Sheet::SelectorColor;
	int Sheet::SelectorBG;
	int Sheet::SelectorText;
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
		EditorLogic(hDown, touch);
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
		(i == selectedSheetIniFile);

		if (selectedSheetIniFile == 0) {
			Gui::drawAnimatedSelector(0, 28, 400, 25, .005, Config::SelectorBG);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSheetIniFile == 1) {
			Gui::drawAnimatedSelector(0, 58, 400, 25, .005, Config::SelectorBG);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSheetIniFile == 2) {
			Gui::drawAnimatedSelector(0, 91, 400, 25, .005, Config::SelectorBG);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSheetIniFile == 3) {
			Gui::drawAnimatedSelector(0, 125, 400, 25, .005, Config::SelectorBG);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSheetIniFile == 4) {
			Gui::drawAnimatedSelector(0, 156, 400, 25, .005, Config::SelectorBG);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedSheetIniFile == 5) {
			Gui::drawAnimatedSelector(0, 188, 400, 25, .005, Config::SelectorBG);
			dirs +=  dirContents[i].name + "\n\n";
		} else {
			Gui::drawAnimatedSelector(0, 188, 400, 25, .005, Config::SelectorBG);
			dirs +=  dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}

	Gui::DrawString(26, 32, 0.53f, Config::SelectorText, dirs.c_str(), 400);

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
	Sheet::SelectorColor = sheetFile.GetInt("COLORS", "Selector", Sheet::SelectorColor);
	Sheet::SelectorBG = sheetFile.GetInt("COLORS", "SelectorBG", Sheet::SelectorBG);
	Sheet::SelectorText = sheetFile.GetInt("COLORS", "SelectorText", Sheet::SelectorText);
	Sheet::MessageText = sheetFile.GetInt("COLORS", "MessageText", Sheet::MessageText);
}

void LeafEditEditor::saveIniContents() {
	sheetFile.SetInt("COLORS", "BarText", Sheet::barText);
	sheetFile.SetInt("COLORS", "ButtonText", Sheet::buttonText);
	sheetFile.SetInt("COLORS", "BGText", Sheet::bgText);
	sheetFile.SetInt("COLORS", "BoxText", Sheet::boxText);
	sheetFile.SetInt("COLORS", "Selector", Sheet::SelectorColor);
	sheetFile.SetInt("COLORS", "SelectorBG", Sheet::SelectorBG);
	sheetFile.SetInt("COLORS", "SelectorText", Sheet::SelectorText);
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
	sheetFile.SetInt("COLORS", "Selector", BLACK);
	sheetFile.SetInt("COLORS", "SelectorBG", BLACK);
	sheetFile.SetInt("COLORS", "SelectorText", BLACK);
	sheetFile.SetInt("COLORS", "MessageText", BLACK);

	sheetFile.SaveIniFile(sheetIni);
	fclose(ini);
}

void LeafEditEditor::DrawIniEditor(void) const {
	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "LeafEdit Ini Editor"))/2, 0, 0.72f, Sheet::barText, "LeafEdit Ini Editor", 400);
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Press L/R to change the next Ini option."))/2, 215, 0.72f, Sheet::barText, "Press L/R to change the next Ini option.", 400);

	if (colorMode == 0) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the BarText Color."))/2, 90, 0.72f, Sheet::barText, "This is the BarText Color.", 400);
	} else if (colorMode == 1) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the BGText Color."))/2, 90, 0.72f, Sheet::bgText, "This is the BGText Color.", 400);
	} else if (colorMode == 2) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the ButtonText Color."))/2, 90, 0.72f, Sheet::buttonText, "This is the ButtonText Color.", 400);
	} else if (colorMode == 3) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the BoxText Color."))/2, 90, 0.72f, Sheet::boxText, "This is the BoxText Color.", 400);
	} else if (colorMode == 4) {
		drawTestSelector(0, 120);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the Selector Outline Color."))/2, 90, 0.72f, Sheet::SelectorColor, "This is the Selector Outline Color.", 400);
	} else if (colorMode == 5) {
		drawTestSelector(0, 120);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the Selector BG Color."))/2, 90, 0.72f, Sheet::SelectorBG, "This is the Selector BG Color.", 400);
	} else if (colorMode == 6) {
		drawTestSelector(0, 120);
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the Selector Text Color."))/2, 120, 0.72f, Sheet::SelectorText, "This is the Selector Text Color.", 400);
	} else if (colorMode == 7) {
		Gui::DrawString((400-Gui::GetStringWidth(0.72f, "This is the Message Text Color."))/2, 120, 0.72f, Sheet::MessageText, "This is the Message Text Color.", 400);
	}

	Gui::DrawBottom();
	
	for (int i = 0; i < 8; i++) {
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
	Gui::DrawString(29 + 6 * 25, 5, 0.5f, Sheet::barText, "7", 400);
	Gui::DrawString(29 + 7 * 25, 5, 0.5f, Sheet::barText, "8", 400);

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
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::SelectorColor, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::SelectorColor, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::SelectorColor, 0).c_str(), 400);
	} else if (colorMode == 5) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::SelectorBG, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::SelectorBG, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::SelectorBG, 0).c_str(), 400);
	} else if (colorMode == 6) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::SelectorText, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::SelectorText, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::SelectorText, 0).c_str(), 400);
	} else if (colorMode == 7) {
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::MessageText, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::MessageText, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(Sheet::MessageText, 0).c_str(), 400);
	}
}

void LeafEditEditor::EditorLogic(u32 hDown, touchPosition touch) {
	int red;
	int green;
	int blue;

	if (hDown & KEY_L || hDown & KEY_LEFT) {
		if(colorMode > 0)	colorMode--;
	} 

	if (hDown & KEY_R || hDown & KEY_RIGHT) {
		if(colorMode < 7)	colorMode++;
	}

	if (hDown & KEY_B) {
		if(Msg::promptMsg("Would you like, to save the Ini?")) {
			saveIniContents();
			sheetsFile = "";
			mode = 0;
		} else {
			sheetsFile = "";
			mode = 0;
		}
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
					Sheet::SelectorColor = RGBA8(red, ColorHelper::getColorValue(Sheet::SelectorColor, 1), ColorHelper::getColorValue(Sheet::SelectorColor, 0), 255);
				} else if (colorMode == 5) {
					Sheet::SelectorBG = RGBA8(red, ColorHelper::getColorValue(Sheet::SelectorBG, 1), ColorHelper::getColorValue(Sheet::SelectorBG, 0), 255);
				} else if (colorMode == 6) {
					Sheet::SelectorText = RGBA8(red, ColorHelper::getColorValue(Sheet::SelectorText, 1), ColorHelper::getColorValue(Sheet::SelectorText, 0), 255);
				} else if (colorMode == 7) {
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
					Sheet::SelectorColor = RGBA8(ColorHelper::getColorValue(Sheet::SelectorColor, 2), green, ColorHelper::getColorValue(Sheet::SelectorColor, 0), 255);
				} else if (colorMode == 5) {
					Sheet::SelectorBG = RGBA8(ColorHelper::getColorValue(Sheet::SelectorBG, 2), green, ColorHelper::getColorValue(Sheet::SelectorBG, 0), 255);
				} else if (colorMode == 6) {
					Sheet::SelectorText = RGBA8(ColorHelper::getColorValue(Sheet::SelectorText, 2), green, ColorHelper::getColorValue(Sheet::SelectorText, 0), 255);
				} else if (colorMode == 7) {
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
					Sheet::SelectorColor = RGBA8(ColorHelper::getColorValue(Sheet::SelectorColor, 2), ColorHelper::getColorValue(Sheet::SelectorColor, 1), blue, 255);
				} else if (colorMode == 5) {
					Sheet::SelectorBG = RGBA8(ColorHelper::getColorValue(Sheet::SelectorBG, 2), ColorHelper::getColorValue(Sheet::SelectorBG, 1), blue, 255);
				} else if (colorMode == 6) {
					Sheet::SelectorText = RGBA8(ColorHelper::getColorValue(Sheet::SelectorText, 2), ColorHelper::getColorValue(Sheet::SelectorText, 1), blue, 255);
				} else if (colorMode == 7) {
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


void LeafEditEditor::drawTestSelector(float x, float y) const
{
	static constexpr int w     = 2;
	static float timer         = 0.0f;
	float highlight_multiplier = fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
	u8 r                       = Sheet::SelectorColor & 0xFF;
	u8 g                       = (Sheet::SelectorColor >> 8) & 0xFF;
	u8 b                       = (Sheet::SelectorColor >> 16) & 0xFF;
	u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);

	Gui::Draw_Rect(x, y, 400, 25, Sheet::SelectorBG);
	Gui::Draw_Rect(x, y, 400, w, color);                      // top
	Gui::Draw_Rect(x, y + w, w, 25 - 2 * w, color);          // left
	Gui::Draw_Rect(x + 400 - w, y + w, w, 25 - 2 * w, color); // right
	Gui::Draw_Rect(x, y + 25 - w, 400, w, color);             // bottom

	timer += .030f;
}