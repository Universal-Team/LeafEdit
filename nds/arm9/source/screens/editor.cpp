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

#include "wwoffsets.h"
#include "wwsave.h"

WWSave* SaveFile;
std::string save = "";

void Editor::Draw(void) const {
	if (EditorMode != 0) {
		Gui::DrawTop();
		printTextCentered("LeafEdit - Editor", 0, 0, true, true);
		Gui::DrawBottom();
	}
}

void Editor::Logic(u16 hDown, touchPosition touch) {
	if (EditorMode == 0) {
		save = browseForSave();
		// Clear Both Screens.
		Gui::clearScreen(false, true);
		Gui::clearScreen(true, true);
		const char *saves = save.c_str();
		SaveFile = WWSave::Initialize(saves, true);
		EditorMode = 1;
	}

	if (EditorMode == 1) {
		if (hDown & KEY_START) {
			Msg::DisplayWaitMsg("Closing the File now!");
			SaveFile->Close();
			Gui::screenBack();
		}
	}
}