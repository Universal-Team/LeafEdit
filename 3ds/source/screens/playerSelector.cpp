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

#include "coreUtils.hpp"
#include "playerEditorNL.hpp"
#include "playerEditorWW.hpp"
#include "playerSelector.hpp"
#include "Sav.hpp"
#include "stringUtils.hpp"
#include "utils.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern std::shared_ptr<Sav> save;
// Bring that to other screens too.
extern SaveType savesType;

// Init Player stuff.
PlayerSelector::PlayerSelector() {
	for (int i = 0; i < 4; i++) {
		if (save->player(i)->exist()) {
			// Only needed for New Leaf & Welcome Amiibo.
			if (savesType == SaveType::NL || savesType == SaveType::WA) {
				this->TPC[i] = CoreUtils::LoadPlayerTPC(save->player(i));
			}

			this->playerNames[i] = StringUtils::UTF16toUTF8(save->player(i)->name());
		}
	}
}

// Make sure to destroy the TPC Image.
PlayerSelector::~PlayerSelector() {
	// Only needed for New Leaf & Welcome Amiibo.
	if (savesType == SaveType::NL || savesType == SaveType::WA) {
		for (int i = 0; i < 4; i++) {
			// Delete TPC Image if isn't nullptr.
			if (save->player(i)->exist()) {
				C2DUtils::C2D_ImageDelete(this->TPC[i]);
				this->TPC[i].tex = nullptr;
				this->TPC[i].subtex = nullptr;
			}
		}
	}
}

void PlayerSelector::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - PlayerSelection", 395, 0, font);
	for (int i = 0; i < 4; i++) {
		if (save->player(i)->exist()) {
			// Only display TPC if New Leaf or Welcome Amiibo.
			if (savesType == SaveType::NL || savesType == SaveType::WA) {
				if (this->TPC[i].tex != nullptr) {
					C2D_DrawImageAt(this->TPC[i], (float)(100 * i) + 18.f, 45.f, 0.5f, nullptr, 1.f, 1.f);
				}
			} else {
				// Else draw NoTPC Image instead.
				GFX::DrawGUI(gui_noTPC_idx, (float)(100 * i) + 18.f, 45.f);
			}

			Gui::DrawString(18 + (i * 100), 150, 0.64f, BLACK, this->playerNames[i], 400, 0, font);
		}
	}

	if (save->player(selectedPlayer)->exist()) {
		Gui::drawAnimatedSelector((100 * selectedPlayer) + 18, 45, 64, 104, .030, C2D_Color32(0, 0, 180, 255), C2D_Color32(0, 0, 0, 0));
	}


	GFX::DrawBottom();
}

void PlayerSelector::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
	if (hDown & KEY_RIGHT) {
		if (selectedPlayer < 3)	selectedPlayer++;
	}

	if (hDown & KEY_LEFT) {
		if (selectedPlayer > 0)	selectedPlayer--;
	}

	if (hDown & KEY_A) {
		// Check if player exist.
		if (save->player(selectedPlayer)->exist()) {
			// New Leaf & Welcome Amiibo.
			if (savesType == SaveType::NL || savesType == SaveType::WA) {
				Gui::setScreen(std::make_unique<PlayerEditorNL>(save->player(selectedPlayer)), false, true);
				// Wild World.
			} else if (savesType == SaveType::WW) {
				Gui::setScreen(std::make_unique<PlayerEditorWW>(save->player(selectedPlayer)), false, true);
			}
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
	}
}