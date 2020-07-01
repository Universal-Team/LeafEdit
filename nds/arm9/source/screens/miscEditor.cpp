/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
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

#include "acresEditor.hpp"
#include "miscEditor.hpp"
#include "pluginScreen.hpp"
#include "Sav.hpp"
#include "townMapEditor.hpp"

extern std::shared_ptr<Sav> save;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

void MiscEditor::Draw(void) const {
	Gui::DrawTop(true);
	printTextCentered("LeafEdit - Misc Editor", 0, 0, true, true);
	Gui::DrawBottom(true);

	for (int i = 0; i < 3; i++) {
		drawRectangle(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, GRAY, false, true);
	}
	printTextCentered("Acres Editor", 0, 40, false, true);
	printTextCentered("Town Editor", 0, 90, false, true);
	printTextCentered("Plugin", 0, 140, false, true);
}

void MiscEditor::Logic(u16 hDown, touchPosition touch) {
	Gui::updatePointer(mainButtons[selection].x+60, mainButtons[selection].y+12);

	if (hDown & KEY_B) {
		Gui::screenBack();
		// Display Save icon.
		setSpriteVisibility(Gui::saveID, false, true);
		setSpritePosition(Gui::saveID, false, 225, 172);
		updateOam();
		Gui::DrawScreen();
		selected = true;
		return;
	}
	if (hDown & KEY_DOWN) {
		if (selection < 2)	selection++;
		selected = true;
	}
	if (hDown & KEY_UP) {
		if (selection > 0)	selection--;
		selected = true;
	}

	if (hDown & KEY_A) {
		if (selection == 0) {
			// Check if acre isn't nullptr.
			if (save->town()->acre(0) != nullptr) {
				Gui::setScreen(std::make_unique<AcresEditor>());
				Gui::DrawScreen();
				selected = true;
			}
		} else if (selection == 1) {
			// Check if acre & items aren't nullptr.
			if (save->town()->acre(0) != nullptr && save->town()->item(0) != nullptr) {
				Gui::setScreen(std::make_unique<TownMapEditor>());
				// Hide Pointer for now.
				Gui::hidePointer();
				Gui::DrawScreen();
				selected = true;
			}
		} else if (selection == 2) {
			Gui::setScreen(std::make_unique<PluginScreen>());
			Gui::DrawScreen();
			selected = true;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			// Check if acre isn't nullptr.
			if (save->town()->acre(0) != nullptr) {
				Gui::setScreen(std::make_unique<AcresEditor>());
				Gui::DrawScreen();
				selected = true;
			}
		} else if (touching(touch, mainButtons[1])) {
			// Check if acre & items aren't nullptr.
			if (save->town()->acre(0) != nullptr && save->town()->item(0) != nullptr) {
				Gui::setScreen(std::make_unique<TownMapEditor>());
				// Hide Pointer for now.
				Gui::hidePointer();
				Gui::DrawScreen();
				selected = true;
			}
		} else if (touching(touch, mainButtons[2])) {
			Gui::setScreen(std::make_unique<PluginScreen>());
			Gui::DrawScreen();
			selected = true;
		}
	}
}