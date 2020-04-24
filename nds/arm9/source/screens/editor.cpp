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
#include "fileBrowse.hpp"
#include "msg.hpp"
//#include "playerEditor.hpp"
#include "Sav.hpp"

std::shared_ptr<Sav> save;
// Bring that to other screens too.
SaveType savesType = SaveType::UNUSED;

const std::vector<std::string> Strings = {
	"Player",
	"Villager",
	"Misc",
};

// Japanese | PAL.
const std::vector<std::string> titleNames = {
	"おいでよ どうぶつの森",
	"Wild World",
	"とびだせ どうぶつの森",
	"New Leaf",
	"とびだせ どうぶつの森 amiibo+",
	"Welcome amiibo",
};

extern bool touching(touchPosition touch, Structs::ButtonPos button);

Editor::Editor() { Gui::hidePointer(); } // Hide Pointer when initializing.

bool Editor::loadSave() {
	save = nullptr;
	FILE* in = fopen(saveName.c_str(), "rb");
	if(in) {
		fseek(in, 0, SEEK_END);
		u32 size = ftell(in);
		fseek(in, 0, SEEK_SET);
		std::shared_ptr<u8[]> saveData = std::shared_ptr<u8[]>(new u8[size]);
		fread(saveData.get(), 1, size, in);
		fclose(in);
		save = Sav::getSave(saveData, size);
	} else {
		printf("Could not open SaveFile.\n");
		return false;
	}
	if(!save) {
		printf("SaveFile returned nullptr.\n");
		return false;
	}
	savesType = save->getType();
	
	return true;
}

void Editor::SaveInitialize() {
	saveName = browseForSave();
	// If User canceled, go screen back.
	if (saveName == "") {
		Gui::screenBack();
		return;
	}

	if (!loadSave()) {
		Msg::DisplayWarnMsg("Invalid SaveFile!");
	} else {
		loadState = SaveState::Loaded;
		// Clear Both Screens.
		Gui::clearScreen(false, true);
		Gui::clearScreen(true, true);
		Gui::DrawScreen();
		// Toggle Pointer.
		Gui::showPointer();
		selected = true;
	}
}

void Editor::Draw(void) const {
	if (loadState == SaveState::Loaded) {
		Gui::DrawTop(true);
		printTextCentered("LeafEdit - Editor", 0, 0, true, true);
		Gui::DrawBottom(true);

		for (int i = 0; i < 3; i++) {
			drawRectangle(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, DARK_GREEN, DARK_GREEN, false, true);
		}
		printTextCentered("Player", 0, 40, false, true);
		printTextCentered("Villager", 0, 90, false, true);
		printTextCentered("Misc", 0, 140, false, true);
	}
}

void Editor::Logic(u16 hDown, touchPosition touch) {
	// Only do Logic, if Save is loaded.
	if (loadState == SaveState::Loaded) {
		Gui::updatePointer(mainButtons[Selection].x+60, mainButtons[Selection].y+12);

		if (hDown & KEY_B) {
			Gui::screenBack();
			Gui::DrawScreen();
			selected = true;
			return;
		}

		if (hDown & KEY_DOWN) {
			if (Selection < 2)	Selection++;
			selected = true;
		}
		if (hDown & KEY_UP) {
			if (Selection > 0)	Selection--;
			selected = true;
		}

/*		if (hDown & KEY_A) {
			if (Selection == 0) {
				Gui::setScreen(std::make_unique<PlayerScreen>());
				Gui::DrawScreen();
				selected = true;
			}
		}

		if (hDown & KEY_TOUCH) {
			if (touching(touch, mainButtons[0])) {
				Gui::setScreen(std::make_unique<PlayerScreen>());
				Gui::DrawScreen();
				selected = true;
			}
		}*/
	} else {
		SaveInitialize();
	}
}