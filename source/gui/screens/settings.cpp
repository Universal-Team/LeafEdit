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

#include "gui/keyboard.hpp"

#include "gui/screens/settings.hpp"
#include "gui/screens/screenCommon.hpp"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

void Settings::Draw(void) const
{
	if(screenMode == 0) {
		std::string Title;
		Title += Lang::title;
		Title += " - ";
		Title += Lang::mainMenu[2];
	
		Gui::DrawTop();
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, WHITE, Title.c_str(), 400);

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

		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::settings[0]))/2, settingsButtons[0].y+10, 0.6f, WHITE, Lang::settings[0], 140);
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::settings[1]))/2, settingsButtons[1].y+10, 0.6f, WHITE, Lang::settings[1], 140);
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Spritesheet"))/2, settingsButtons[2].y+10, 0.6f, WHITE, "Spritesheet", 140);
	} else if(screenMode == 1) {
		DrawSpriteSheetSelection();
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
					case 0: {
							Config::setLanguage();
							break;
					}	case 1:
							Config::setSelector();
							break;
						case 2: {
							screenMode = 1;
							break;
						 }
				}
		}
	} else if(screenMode == 1) {
		SpriteSheetLogic(hDown, hHeld);
	}
}

void Settings::SelectionLogic(u32 hDown, u32 hHeld)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	}
}

extern C2D_SpriteSheet sprites;

void Settings::DrawSpriteSheetSelection(void) const {
	Gui::ScreenDraw(top);
	Gui::sprite(0, sprites_topbar_idx, 0, 0);
	Gui::sprite(0, sprites_fbBgTop_idx, 0, 27);
	Gui::sprite(0, sprites_topbar_idx, 0, 213);

	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Select a Spritesheet to load."))/2, 2, 0.72f, WHITE, "Select a Spritesheet to load.", 400);

	Gui::DrawString((400-Gui::GetStringWidth(0.60f, Lang::messages2[9]))/2, 216, 0.60f, WHITE, Lang::messages2[9], 400);

	std::string sheets;
	for (uint i=(selectedSpriteSheet<5) ? 0 : selectedSpriteSheet-5;i<dirContents.size()&&i<((selectedSpriteSheet<5) ? 6 : selectedSpriteSheet+1);i++) {
		(i == selectedSpriteSheet);

		if (selectedSpriteSheet == 0) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 28, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 28, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 28, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			sheets +=  dirContents[i].name + "\n\n";

		} else if (selectedSpriteSheet == 1) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 58, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 58, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 58, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			sheets +=  dirContents[i].name + "\n\n";

		} else if (selectedSpriteSheet == 2) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 91, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 91, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 91, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			sheets +=  dirContents[i].name + "\n\n";

		} else if (selectedSpriteSheet == 3) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 125, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 125, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 125, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			sheets +=  dirContents[i].name + "\n\n";

		} else if (selectedSpriteSheet == 4) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 156, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 156, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 156, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			sheets +=  dirContents[i].name + "\n\n";

		} else if (selectedSpriteSheet == 5) {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			sheets +=  dirContents[i].name + "\n\n";
		} else {
			if (Config::selector == 1) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(0, 0, 0, 255));
			} else if (Config::selector == 2) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(255, 255, 255, 20));
			} else if (Config::selector == 3) {
				Gui::drawAnimatedSelector(0, 188, 400, 25, .005, C2D_Color32(255, 255, 255, 255));
			}
			sheets +=  dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		sheets += "\n\n";
	}

	if (Config::selector == 1) {
		Gui::DrawString(26, 32, 0.53f, WHITE, sheets.c_str(), 400);
	} else if (Config::selector == 2 || Config::selector == 3) {
		Gui::DrawString(26, 32, 0.53f, BLACK, sheets.c_str(), 400);
	}

	Gui::ScreenDraw(bottom);
	Gui::sprite(0, sprites_bottombar_idx, 0, 0);
	Gui::sprite(0, sprites_fbBgBottom_idx, 0, 27);
	Gui::sprite(0, sprites_bottombar_idx, 0, 213);
}


void Settings::SpriteSheetLogic(u32 hDown, u32 hHeld) {
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
			std::string prompt = "Would you like to load this SpriteSheet?";
			prompt += "\n\n";
			prompt += "'";
			prompt += dirContents[selectedSpriteSheet].name;
			prompt += "'";
			if(Msg::promptMsg(prompt.c_str())) {
				selectedSheet = dirContents[selectedSpriteSheet].name.c_str();
				finalSheet = "sdmc:/LeafEdit/SpriteSheets/";
				finalSheet += selectedSheet.c_str();
				Msg::SheetMsg("Now freeing the SpriteSheet...");
				C2D_SpriteSheetFree(sprites);
				Msg::SheetMsg("Now Loading the new SpriteSheet...");
				sprites	= C2D_SpriteSheetLoad(finalSheet.c_str());
			}
			finalSheet = "";
			selectedSheet = "";
			screenMode = 0;
		}

		if (hDown & KEY_Y) {
			std::string prompt = "Would you like to autoboot this SpriteSheet?";
			prompt += "\n\n";
			prompt += "'";
			prompt += dirContents[selectedSpriteSheet].name;
			prompt += "'";
			if(Msg::promptMsg(prompt.c_str())) {
				selectedSheet = dirContents[selectedSpriteSheet].name.c_str();
				finalSheet = "sdmc:/LeafEdit/SpriteSheets/";
				finalSheet += selectedSheet.c_str();
				Config::saveSheet(finalSheet.c_str());
			}
			finalSheet = "";
			selectedSheet = "";
			screenMode = 0;
		}

		if (hDown & KEY_SELECT) {
			std::string prompt = "Would you like to reset the SpriteSheet?";
			if(Msg::promptMsg(prompt.c_str())) {
				finalSheet = "romfs:/gfx/sprites.t3x";
				Config::saveSheet(finalSheet.c_str());
				Msg::SheetMsg("Now freeing the SpriteSheet...");
				C2D_SpriteSheetFree(sprites);
				Msg::SheetMsg("Now Loading the new SpriteSheet...");
				sprites	= C2D_SpriteSheetLoad(finalSheet.c_str());
			}
			finalSheet = "";
			screenMode = 0;
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
	} else if (hDown & KEY_SELECT) {
		dirChanged = true;
	}
}