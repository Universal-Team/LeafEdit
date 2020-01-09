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
extern bool isROMHack; // For Welcome Luxury.
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
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("EDITOR"), 400);

	Gui::DrawBottom();

	for (int i = 0; i < 3; i++) {
		if (Selection == i) {
			Gui::Draw_Rect(editorButtons[i].x, editorButtons[i].y, editorButtons[i].w, editorButtons[i].h, selectedColor);
		} else {
			Gui::Draw_Rect(editorButtons[i].x, editorButtons[i].y, editorButtons[i].w, editorButtons[i].h, unselectedColor);
		}
	}
	Gui::sprite(0, sprites_back_idx, editorButtons[3].x, editorButtons[3].y);

	Gui::DrawStringCentered(0, editorButtons[0].y+10, 0.6f, WHITE, Lang::get("PLAYER"), 130);
	Gui::DrawStringCentered(0, editorButtons[1].y+10, 0.6f, WHITE, Lang::get("VILLAGER"), 130);
	Gui::DrawStringCentered(0, editorButtons[2].y+10, 0.6f, WHITE, Lang::get("MISC_EDITOR"), 130);
}

void Editor::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	} else if ((hDown & KEY_TOUCH && touching(touch, editorButtons[3])) || (hDown & KEY_START)) {
		if (Msg::promptMsg(Lang::get("SAVE_CHANGES"))) {
			SaveFile->Commit(false);
		}
		EditorMode = 1;
		selectedSaveFolderEditor = "";
		SaveFile->Close();
	}

	if (hDown & KEY_A) {
		switch(Selection) {
			case 0:
				Gui::setScreen(std::make_unique<PlayerEditor>());
				break;
			case 1:
				Gui::setScreen(std::make_unique<VillagerViewer>());
				break;
			case 2:
				Gui::setScreen(std::make_unique<MiscEditor>());
				break;
		}
	}
}


void Editor::DrawBrowse(void) const
{
	Gui::DrawFileBrowseBG();
	Gui::DrawStringCentered(0, 2, 0.6f, WHITE, Lang::get("SELECT_A_SAVE"), 395);
	Gui::DrawStringCentered(0, 218, 0.60f, WHITE, Lang::get("REFRESH"), 390);
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
	Gui::DrawString(26, 32, 0.51f, WHITE, dirs.c_str(), 360);
	Gui::DrawFileBrowseBG(false);
}


void Editor::BrowseLogic(u32 hDown, u32 hHeld) {
	if (keyRepeatDelay)	keyRepeatDelay--;

	if (dirChanged) {
		dirContents.clear();
		std::string customPath;
		if (isROMHack == true) {
			customPath += "sdmc:/LeafEdit/Towns/Welcome-Luxury/";
		} else if (isROMHack == false) {
			customPath += "sdmc:/LeafEdit/Towns/Welcome-Amiibo/";
		}
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
			Msg::DisplayWarnMsg(Lang::get("WHAT_YOU_DO"));
		} else {
			std::string prompt = Lang::get("LOAD_THIS_SAVE");
			if(Msg::promptMsg(prompt.c_str())) {
				if (isROMHack == true) {
					selectedSaveFolderEditor = "/LeafEdit/Towns/Welcome-Luxury/";
				} else if (isROMHack == false) {
					selectedSaveFolderEditor = "/LeafEdit/Towns/Welcome-Amiibo/";
				}
				selectedSaveFolderEditor += dirContents[selectedSave].name.c_str();
				selectedSaveFolderEditor += "/garden_plus.dat";
				if( access( selectedSaveFolderEditor.c_str(), F_OK ) != -1 ) {
					const char *save = selectedSaveFolderEditor.c_str();
					SaveFile = Save::Initialize(save, true);

					if (SaveFile->GetSaveSize() != SIZE_SAVE) {
						Msg::DisplayWarnMsg(Lang::get("SAVE_INCORRECT_SIZE"));
						SaveFile->Close();
						return;
					}
					EditorMode = 2;
				} else {
					Msg::DisplayWarnMsg(Lang::get("SAVE_NOT_FOUND"));
					return;
				}
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
		if(Msg::promptMsg(Lang::get("RETURN_MAINMENU"))) {
			Gui::unloadSheets();
			Gui::screenBack();
			return;
		}
	} else if (hDown & KEY_START) {
		dirChanged = true;
	}
}