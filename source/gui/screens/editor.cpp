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

#include "common/common.hpp" // For the TID's.
#include "common/config.hpp"
#include "common/utils.hpp"

#include "gui/keyboard.hpp"

#include "gui/screens/editor.hpp"
#include "gui/screens/mainMenu.hpp"
#include "gui/screens/miscEditor.hpp"
#include "gui/screens/playerEditor.hpp"
#include "gui/screens/screenCommon.hpp"
#include "gui/screens/villagerViewer.hpp"

#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

std::string selectedSaveFolderEditor = "";
Save* SaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Editor::Draw(void) const
{
	if (EditorMode == 1) {
		DrawBrowse();
	} else if (EditorMode == 2) {
		DrawSubMenu();
	}
}

void Editor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (EditorMode == 1) {
		BrowseLogic(hDown, hHeld);
	} else if (EditorMode == 2) {
		SubMenuLogic(hDown, hHeld, touch);
	}
}

void Editor::DrawSubMenu(void) const
{
	std::string Title;
	Title += Lang::title;
	Title += " - ";
	Title += Lang::editor[2];

	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, Title.c_str()))/2, 2, 0.8f, Config::barText, Title.c_str(), 400);

	Gui::DrawBottom();


	if (Selection == 0) {
		Gui::Draw_ImageBlend(0, sprites_button_idx, editorButtons[0].x, editorButtons[0].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, editorButtons[1].x, editorButtons[1].y);
		Gui::sprite(0, sprites_button_idx, editorButtons[2].x, editorButtons[2].y);

	} else if (Selection == 1) {
		Gui::sprite(0, sprites_button_idx, editorButtons[0].x, editorButtons[0].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, editorButtons[1].x, editorButtons[1].y, selectedColor);
		Gui::sprite(0, sprites_button_idx, editorButtons[2].x, editorButtons[2].y);

	} else if (Selection == 2) {
		Gui::sprite(0, sprites_button_idx, editorButtons[0].x, editorButtons[0].y);
		Gui::sprite(0, sprites_button_idx, editorButtons[1].x, editorButtons[1].y);
		Gui::Draw_ImageBlend(0, sprites_button_idx, editorButtons[2].x, editorButtons[2].y, selectedColor);
	}

	Gui::sprite(0, sprites_back_idx, editorButtons[3].x, editorButtons[3].y);

	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::editor[1]))/2, editorButtons[0].y+10, 0.6f, Config::buttonText, Lang::editor[1], 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, Lang::editor[6]))/2, editorButtons[1].y+10, 0.6f, Config::buttonText, Lang::editor[6], 140);
	Gui::DrawString((320-Gui::GetStringWidth(0.6f, "Misc Editor"))/2, editorButtons[2].y+10, 0.6f, Config::buttonText, "Misc Editor", 140);
}

void Editor::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	} else if (hDown & KEY_TOUCH && touching(touch, editorButtons[3])) {
			if (Msg::promptMsg(Lang::editor[0])) {
					SaveFile->Commit(false);
				} 
		EditorMode = 1;
		selectedSaveFolderEditor = "";
		SaveFile->Close();
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Press Player to access the Player Editor.\nPress Villager to access the Villager Editor.\nPress WIP to ? (Not implemented yet)\nPress the back Icon on the bottom Screen to exit with saving or not.");
	}

	if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
						Gui::setScreen(std::make_unique<PlayerEditor>());
						break;
				}   case 1:
						Gui::setScreen(std::make_unique<VillagerViewer>());
						break;
				 	case 2: {
						Gui::setScreen(std::make_unique<MiscEditor>());
						break;
					 }
			}
	}
}


void Editor::DrawBrowse(void) const
{
	Gui::ScreenDraw(top);
	Gui::sprite(0, sprites_top_topbar_idx, 0, 0);
	Gui::sprite(0, sprites_fbBgTop_idx, 0, 27);
	Gui::sprite(0, sprites_top_bottombar_idx, 0, 213);

	Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::editor[3]))/2, 2, 0.72f, Config::barText, Lang::editor[3], 400);

	std::string dirs;
	for (uint i=(selectedSave<5) ? 0 : selectedSave-5;i<dirContents.size()&&i<((selectedSave<5) ? 6 : selectedSave+1);i++) {
		if (i == selectedSave) {
			dirs += "> " + dirContents[i].name + "\n\n";
		} else {
			dirs += dirContents[i].name + "\n\n";
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


void Editor::BrowseLogic(u32 hDown, u32 hHeld) { 
	if (keyRepeatDelay)	keyRepeatDelay--;

			if (dirChanged) {
            dirContents.clear();
			std::string customPath = "sdmc:/LeafEdit/Towns/Welcome-Amiibo/";
			chdir(customPath.c_str());
            std::vector<DirEntry> dirContentsTemp;
            getDirectoryContents(dirContentsTemp);
            for(uint i=0;i<dirContentsTemp.size();i++) {
                  dirContents.push_back(dirContentsTemp[i]);
        }
		dirChanged = false;
	}

		if (hHeld & KEY_SELECT) {
			Msg::HelperBox("Select a Save, which you like to edit.\nPress Start to refresh the filelist.\nPress B to exit from this Screen.");
		}

		if(hDown & KEY_A) {
			if (dirContents.size() == 0) {
				Msg::DisplayWarnMsg("What are you trying to do? :P");
			} else {
				std::string prompt = Lang::editor[4];
				if(Msg::promptMsg(prompt.c_str())) {

				selectedSaveFolderEditor = "/LeafEdit/Towns/Welcome-Amiibo/";
					selectedSaveFolderEditor += dirContents[selectedSave].name.c_str();
					selectedSaveFolderEditor += "/garden_plus.dat";
					const char *save = selectedSaveFolderEditor.c_str();
					SaveFile = Save::Initialize(save, true);
					EditorMode = 2;
				}
			}
		}

		if (hHeld & KEY_UP) {
		if (selectedSave > 0 && !keyRepeatDelay) {
			selectedSave--;
			keyRepeatDelay = 6;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedSave < dirContents.size()-1) {
			selectedSave++;
			keyRepeatDelay = 6;
		}
	} else if (hDown & KEY_B) {
		if(Msg::promptMsg(Lang::editor[5])) {
			Gui::screenBack();
			return;
		}
	} else if (hDown & KEY_START) {
		dirChanged = true;
	}
}