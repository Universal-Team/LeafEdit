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

#include "pluginScreen.hpp"
#include "screenCommon.hpp"

// Include all Plugin's.
#include "NLPlugin.hpp"
#include "UniversalPlugin.hpp"
#include "WAPlugin.hpp"
#include "WWPlugin.hpp"
// Test Purpose.
#include "TestPlugin.hpp"

extern SaveType savesType;

extern bool touching(touchPosition touch, ButtonType button);


void PluginScreen::Draw(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9, WHITE, "LeafEdit - Plugin", 390);
	GFX::DrawBottom();
	for (int i = 0; i < 2; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x+100, mainButtons[i].y+30);
	}
}


void PluginScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_RIGHT) {
		if (selection < 1)	selection++;
	}

	if (hDown & KEY_LEFT) {
		if (selection > 0)	selection--;
	}

	if (hDown & KEY_A) {
		if (selection == 0) {
			// Get correct Plugin.
			if (savesType == SaveType::WW) {
				plugin = std::make_unique<WWPlugin>();
			} else if (savesType == SaveType::NL) {
				plugin = std::make_unique<NLPlugin>();
			} else if (savesType == SaveType::WA) {
				plugin = std::make_unique<WAPlugin>();
			}
		} else if (selection == 1) {
			plugin = std::make_unique<UniversalPlugin>();
		}
		if (plugin != nullptr) {
			if (plugin->scriptMain() != 0) {
				Msg::DisplayWarnMsg("An error occured while executing script.");
			}
			Gui::screenBack();
			return;
		}
	}
}