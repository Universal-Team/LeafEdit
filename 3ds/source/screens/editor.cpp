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

#include "buildingManagement.hpp"
#include "editor.hpp"
#include "fileBrowse.hpp"
#include "init.hpp"
#include "itemManagement.hpp"
#include "miscEditor.hpp"
#include "miscEditorWW.hpp"
#include "offsets.hpp"
#include "playerEditor.hpp"
#include "playerEditorWW.hpp"
#include "save.hpp"
#include "utils.hpp"
#include "villagerManagement.hpp"
#include "villagerViewer.hpp"
#include "villagerViewerWW.hpp"
#include "wwItemManagement.hpp"
#include "wwsave.hpp"

#include <unistd.h>

extern std::vector<std::string> g_villagerDatabase;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
int saveType = 0; // Type of the save.. NL/WW.
Save *SaveFile;
WWSave *WWSaveFile;

void Editor::Draw(void) const {
	if (EditorMode == 0) {
		DrawMain();
	} else if (EditorMode == 1) {
		DrawEditor();
	}
}

void Editor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (EditorMode == 0) {
		MainLogic(hDown, hHeld, touch);
	} else if (EditorMode == 1) {
		EditorLogic(hDown, hHeld, touch);
	}
}

void Editor::DrawMain(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("EDITOR"), 400);
	GFX::DrawBottom();
	for (int i = 0; i < 2; i++) {
		Gui::Draw_Rect(editorMainBtn[i].x, editorMainBtn[i].y, editorMainBtn[i].w, editorMainBtn[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, editorMainBtn[i].x+130, editorMainBtn[i].y+25);
		}
	}
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("RAW_SAVES")))/2-20+17.5, 0.8, WHITE, Lang::get("RAW_SAVES"), 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("MEDIATYPE")))/2-20+17.5, 0.8, WHITE, Lang::get("MEDIATYPE"), 130, 25);
}

void Editor::MainLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_RIGHT) {
		if (Selection == 0)	Selection = 1;
	} else if (hDown & KEY_LEFT) {
		if (Selection == 1)	Selection = 0;
	}

	if (hDown & KEY_A) {
		if (Selection == 0) {
			std::string path = SaveBrowse::searchForSave({"sav", "dat"}, "sdmc:/LeafEdit/Towns/", Lang::get("SELECT_A_SAVE"));
			if (path != "") {
				if ((strcasecmp(path.substr(path.length()-3, 3).c_str(), "sav") == 0)) {
					PrepareWW(path);
				} else if ((strcasecmp(path.substr(path.length()-3, 3).c_str(), "dat") == 0)) {
					PrepareNL(path);
				}
			}
		} else if (Selection == 1) {
			Msg::NotImplementedYet();
		}
	}
	if (hDown & KEY_TOUCH) {
		if (touching(touch, editorMainBtn[0])) {
			std::string path = SaveBrowse::searchForSave({"sav", "dat"}, "sdmc:/LeafEdit/Towns/", Lang::get("SELECT_A_SAVE"));
			if (path != "") {
				if ((strcasecmp(path.substr(path.length()-3, 3).c_str(), "sav") == 0)) {
					PrepareWW(path);
				} else if ((strcasecmp(path.substr(path.length()-3, 3).c_str(), "dat") == 0)) {
					PrepareNL(path);
				}
			}
		} else if (touching(touch, editorMainBtn[1])) {
			Msg::NotImplementedYet();
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}

void Editor::DrawEditor(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("EDITOR"), 400);
	GFX::DrawBottom();

	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(editorButtons[i].x, editorButtons[i].y, editorButtons[i].w, editorButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, editorButtons[i].x+130, editorButtons[i].y+25);
		}
	}
	GFX::DrawSprite(sprites_back_idx, editorButtons[3].x, editorButtons[3].y);

	Gui::DrawStringCentered(0, editorButtons[0].y+10, 0.8f, WHITE, Lang::get("PLAYER"), 130);
	Gui::DrawStringCentered(0, editorButtons[1].y+10, 0.8f, WHITE, Lang::get("VILLAGER"), 130);
	Gui::DrawStringCentered(0, editorButtons[2].y+10, 0.8f, WHITE, Lang::get("MISC_EDITOR"), 130);
}

void Editor::EditorLogic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	}

	if ((hDown & KEY_TOUCH && touching(touch, editorButtons[3])) || (hDown & KEY_START)) {
		if (saveType == 0) {
			saveNL();
		} else if (saveType == 1) {
			saveWW();
		}
	}

	if (hDown & KEY_A) {
		SetMode(Selection);
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("START_SAVE"));
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, editorButtons[0])) {
			SetMode(0);
		} else if (touching(touch, editorButtons[1])) {
			SetMode(1);
		} else if (touching(touch, editorButtons[2])) {
			SetMode(2);
		}
	}
}


// Prepare a New Leaf save.
void Editor::PrepareNL(const std::string savePath) {
	if(access(savePath.c_str(), F_OK) == 0) {
		std::string prompt = Lang::get("NEWLEAF_SAVE_DETECTED") + "\n" + Lang::get("LOAD_THIS_SAVE");
		if(Msg::promptMsg(prompt.c_str())) {
			const char *save = savePath.c_str();
			saveType = 0;
			SaveFile = Save::Initialize(save, true);
			// Check, if SaveFile has correct size.
			if (SaveFile->GetSaveSize() != SIZE_SAVE) {
				Msg::DisplayWarnMsg(Lang::get("SAVE_INCORRECT_SIZE"));
				SaveFile->Close();
				return;
			}
			Msg::DisplayMsg(Lang::get("PREPARING_EDITOR"));
			Init::loadNLSheets();
			BuildingManagement::loadDatabase();
			ItemManagement::LoadDatabase(1);
			Lang::loadNL(1);
			Utils::createBackup(false, savePath);
			EditorMode = 1;
		}
	}
}

// Prepare a Wild World save.
void Editor::PrepareWW(const std::string savePath) {
	if(access(savePath.c_str(), F_OK) == 0) {
		std::string prompt = Lang::get("WILDWORLD_SAVE_DETECTED") + "\n" + Lang::get("LOAD_THIS_SAVE");
		if(Msg::promptMsg(prompt.c_str())) {
			const char *save = savePath.c_str();
			saveType = 1; 
			WWSaveFile = WWSave::Initialize(save, true);
			Msg::DisplayMsg(Lang::get("PREPARING_EDITOR"));
			Init::loadWWSheets();
			WWItemManagement::LoadDatabase(1);
			Lang::loadWW(1);
			EditorMode = 1;
		}
	}
}

// Set Screen.
void Editor::SetMode(int mode) {
	if (saveType == 0) {
		switch (mode) {
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
	} else {
		switch (mode) {
			case 0:
				Gui::setScreen(std::make_unique<PlayerEditorWW>());
				break;
			case 1:
				Gui::setScreen(std::make_unique<VillagerViewerWW>());
				break;
			case 2:
				Gui::setScreen(std::make_unique<MiscEditorWW>());
				break;
		}
	}
}

void Editor::saveNL() {
	if (Msg::promptMsg(Lang::get("SAVE_CHANGES"))) {
		// Update Save Region.
		SaveFile->FixSaveRegion();
		SaveFile->Commit(false);
	}
	// Set Screen to Browse & Reset Save Folder.
	SaveFile->Close();
	Init::unloadNLSheets();
	Gui::screenBack();
	return;
}

void Editor::saveWW() {
	if (Msg::promptMsg(Lang::get("SAVE_CHANGES"))) {
		WWSaveFile->Commit(false);
	}
	// Set Screen to Browse & Reset Save Folder.
	WWSaveFile->Close();
	Init::unloadWWSheets();
	Gui::screenBack();
	return;
}