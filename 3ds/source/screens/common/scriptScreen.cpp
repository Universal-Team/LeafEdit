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

#include "scriptScreen.hpp"
#include "screenCommon.hpp"

extern std::unique_ptr<Config> config;
extern SaveType savesType;

extern bool touching(touchPosition touch, ButtonType button);

ScriptScreen::ScriptScreen() { }

void ScriptScreen::load() {
	std::string tempScript = Overlays::SelectFile({"json"}, "sdmc:/3ds/LeafEdit/scripts/", Lang::get("SELECT_SCRIPT"));

	if (tempScript != "") {
		this->script = std::make_unique<Script>(tempScript);

		if (this->script->getValid()) this->loadMode = false;
		else Msg::DisplayWaitMsg(Lang::get("SCRIPT_INVALID"));

	} else {
		Gui::screenBack(doFade);
	}
}


void ScriptScreen::Draw(void) const {
	if (!this->loadMode) {
		if (this->script && this->script->getValid()) {
			std::string entries;
			for (int i = (this->selection < 8) ? 0 : this->selection - 8; i < this->script->getEntrySize() && i < ((this->selection < 8) ? 9 : this->selection + 1); i++) {
				entries += this->script->getEntryTitle(i) + "\n";
			}

			for (int i = 0; i < ((this->script->getEntrySize() < 9) ? 9 - this->script->getEntrySize() : 0); i++) {
				entries += "\n";
			}

			GFX::DrawTop();
			Gui::DrawStringCentered(0, -2, 0.9, WHITE, this->script->getScriptName(), 390, 0, font);
			Gui::DrawStringCentered(0, 40, 0.7, BLACK, Lang::get("SCRIPT_AUTHOR") + this->script->getAuthor(), 390, 0, font);
			Gui::DrawStringCentered(0, 60, 0.7, BLACK, Lang::get("SCRIPT_DESCRIPTION") + this->script->getDescription(), 390, 0, font);

			Gui::DrawStringCentered(0, 100, 0.7, BLACK, Lang::get("ENTRY_TITLE") + this->script->getEntryTitle(this->selection), 390, 0, font);
			Gui::DrawStringCentered(0, 120, 0.7, BLACK, Lang::get("ENTRY_DESCRIPTION") + this->script->getEntryDesc(this->selection), 390, 0, font);
			Gui::DrawStringCentered(0, 160, 0.7, BLACK, Lang::get("SCRIPT_ENTRY") + std::to_string(this->selection + 1) + " | " + std::to_string(this->script->getEntrySize()), 390, 0, font);

			if (this->script->gameSupported(this->selection)) {
				Gui::DrawStringCentered(0, 180, 0.7, BLACK, Lang::get("SUPPORTED") + Lang::get("YES") + ".", 390, 0, font);
				
			} else {
				Gui::DrawStringCentered(0, 180, 0.7, BLACK, Lang::get("SUPPORTED") + Lang::get("NO") + ".", 390, 0, font);
			}

			if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

			GFX::DrawFileBrowseBG(false);

			if (this->selection < 9) GFX::DrawSelector(false, 26 + ((int)this->selection * 21));
			else GFX::DrawSelector(false, 26 + (8 * 21));
			Gui::DrawString(5, 25, 0.85f, BLACK, entries, 360, 0, font);


			if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		}

		/* If not valid. */
	} else {
		GFX::DrawTop();
		Gui::DrawStringCentered(0, -2, 0.9, WHITE, Lang::get("SCRIPT_INVALID"), 390, 0, font);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		GFX::DrawBottom();
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	}
}


void ScriptScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (!this->loadMode) {
		if (hDown & KEY_B) {
			Gui::screenBack(doFade);
		}

		if (this->script) {
			if (hRepeat & KEY_DOWN) {
				if (this->selection < this->script->getEntrySize()-1) this->selection++;
			}

			if (hRepeat & KEY_UP) {
				if (this->selection > 0) this->selection--;
			}

			if (hDown & KEY_A) {
				if (this->script->entryExist(this->selection) && this->script->gameSupported(this->selection)) {
					if (Msg::promptMsg(Lang::get("EXECUTE_ENTRY") + "\n" + script->getEntryTitle(this->selection))) {
						this->script->execute(this->selection);
					}
				}
			}
		}

	} else {
		this->load();
	}
}