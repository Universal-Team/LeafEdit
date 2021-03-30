/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2021 Universal-Team
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
/* Bring that to other screens too. */
extern SaveType savesType;

/* Init Player stuff. */
PlayerSelector::PlayerSelector() {
	C3D_FrameEnd(0);

	for (int i = 0; i < 4; i++) {
		if (save->player(i)->exist()) {
			this->playerNames[i] = StringUtils::UTF16toUTF8(save->player(i)->name());

			switch(savesType) {
				case SaveType::WW:
				case SaveType::HHD:
				case SaveType::UNUSED:
					break;

				case SaveType::NL:
				case SaveType::WA:
					std::unique_ptr<Player> tmpP = save->player(i);
					this->TPC[i] = CoreUtils::LoadPlayerTPC(tmpP, this->imageLoaded[i]);
					break;
			}
		}
	}
}

/* Make sure to destroy the TPC Image. */
PlayerSelector::~PlayerSelector() {
	C3D_FrameEnd(0);

	switch(savesType) {
		case SaveType::NL:
		case SaveType::WA:
			for (int i = 0; i < 4; i++) {
				if (this->imageLoaded[i]) {
					if (this->TPC[i].tex) {
						C2DUtils::C2D_ImageDelete(this->TPC[i]);
						this->TPC[i].tex = nullptr;
						this->TPC[i].subtex = nullptr;
					}
				}
			}
			break;

		case SaveType::WW:
		case SaveType::HHD:
		case SaveType::UNUSED:
			break;
	}
}

void PlayerSelector::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("PLAYER_SELECTION"), 395, 0, font);

	for (int i = 0; i < 4; i++) {
		if (save->player(i)->exist()) {

			switch(savesType) {
				case SaveType::WW:
					GFX::DrawGUI(gui_noTPC_idx, (float)(100 * i) + 18.f, 45.f, 1.f, 1.f); // So WW doesn't look too empty as well.
					break;

				case SaveType::NL:
				case SaveType::WA:
					if (this->TPC[i].tex && this->imageLoaded[i]) {
						C2D_DrawImageAt(this->TPC[i], (float)(100 * i) + 18.f, 45.f, 0.5f, nullptr, 1.f, 1.f);

					} else {
						GFX::DrawGUI(gui_noTPC_idx, (float)(100 * i) + 18.f, 45.f, 1.f, 1.f);
					}
					break;

				case SaveType::HHD:
				case SaveType::UNUSED:
					break;
			}
		}

		Gui::DrawString(18 + (i * 100), 150, 0.64f, BLACK, this->playerNames[i], 400, 0, font);
	}

	Gui::drawAnimatedSelector((100 * this->selectedPlayer) + 18, 45, 64, 104, .030, C2D_Color32(0, 0, 180, 255), C2D_Color32(0, 0, 0, 0));

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void PlayerSelector::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_RIGHT) {
		if (this->selectedPlayer < 3) this->selectedPlayer++;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->selectedPlayer > 0) this->selectedPlayer--;
	}

	if (hDown & KEY_A) {
		/* Check if player exist. */
		if (save->player(this->selectedPlayer)->exist()) {
			switch(savesType) {
				case SaveType::WW:
					Gui::setScreen(std::make_unique<PlayerEditorWW>(save->player(this->selectedPlayer)), doFade, true);
					break;

				case SaveType::NL:
				case SaveType::WA:
					Gui::setScreen(std::make_unique<PlayerEditorNL>(save->player(this->selectedPlayer)), doFade, true);
					break;

				case SaveType::HHD:
				case SaveType::UNUSED:
					break;
			}
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}
}