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

#include "editor.hpp"
#include "init.hpp"
#include "miscEditor.hpp"
#include "miscEditorWW.hpp"
#include "playerEditor.hpp"
#include "playerEditorWW.hpp"
#include "save.hpp"
#include "villagerViewer.hpp"
#include "villagerViewerWW.hpp"
#include "wwsave.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern int saveType; // Type of the save.. NL/WW.
extern Save *SaveFile;
extern WWSave *WWSaveFile;
extern std::string selectedSaveFolderEditor;

void Editor::Draw(void) const
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

void Editor::Logic(u32 hDown, u32 hHeld, touchPosition touch)
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
	selectedSaveFolderEditor = "";
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
	selectedSaveFolderEditor = "";
	Gui::screenBack();
	return;
}