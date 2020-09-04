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

#include "editor.hpp"
#include "mainMenu.hpp"
#include "Sav.hpp"
#include "saveUtils.hpp"
#include "screenCommon.hpp"
#include "settings.hpp"
#include "updateCenter.hpp"

extern int fadealpha;
extern bool fadein;
extern bool touching(touchPosition touch, ButtonType button);
extern bool exiting;
extern std::shared_ptr<Sav> save;

#define TESTPATH "sdmc:/nogba/Battery/EUR.sav"

void doStuff() {
	/* Here we open the file and get the SaveType. */
	save = nullptr;
	FILE* in = fopen(TESTPATH, "rb");
	if (in) {
		fseek(in, 0, SEEK_END);
		u32 size = ftell(in);
		fseek(in, 0, SEEK_SET);
		std::shared_ptr<u8[]> saveData = std::shared_ptr<u8[]>(new u8[size]);
		fread(saveData.get(), 1, size, in);
		fclose(in);
		save = Sav::getSave(saveData, size);
	}

	/* Could not open file or savetype invalid, exit. */
	if (!save) {
		Msg::DisplayWarnMsg("Save is !save.");
		exiting = true;
		return;
	}

	/* And now we update the checksum at the end and write to file. */
	save->Finish();
	FILE* out = fopen(TESTPATH, "rb+");
	fwrite(save->rawData().get(), 1, save->getLength(), out);
	fclose(out);
	/* Exit. */
	exiting = true;
}

/* Whole Testing stuff. */
MainMenu::MainMenu() {
	//doStuff();
}


void MainMenu::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9, WHITE, "LeafEdit - " + Lang::get("MAINMENU"), 390, 0, font);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(mainButtons[i], 0.7);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x + 100, mainButtons[i].y + 30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void MainMenu::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_UP) {
		if (this->Selection > 1) this->Selection -= 2;
	}
	
	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 3 && this->Selection != 2 && this->Selection != 3) this->Selection += 2;
	}
	
	if (hRepeat & KEY_LEFT) {
		if (this->Selection%2) this->Selection--;
	}
	
	if (hRepeat & KEY_RIGHT) {
		if (!(this->Selection%2)) this->Selection++;
	}

	if (hDown & KEY_START) {
		exiting = true;
	}

	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				if (Msg::promptMsg(Lang::get("EXPERIMENTAL_EDITOR"))) {
					Gui::setScreen(std::make_unique<Editor>(), doFade, true);
				}
				break;
			case 1:
				Gui::setScreen(std::make_unique<Settings>(), doFade, true);
				break;
			case 2:
				Overlays::showCredits();
				break;
			case 3:
				Gui::setScreen(std::make_unique<UpdateCenter>(), doFade, true);
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			if (Msg::promptMsg(Lang::get("EXPERIMENTAL_EDITOR"))) {
				Gui::setScreen(std::make_unique<Editor>(), doFade, true);
			}
		} else if (touching(touch, mainButtons[1])) {
			Gui::setScreen(std::make_unique<Settings>(), doFade, true);
		} else if (touching(touch, mainButtons[2])) {
			Overlays::showCredits();
		} else if (touching(touch, mainButtons[3])) {
			Gui::setScreen(std::make_unique<UpdateCenter>(), doFade, true);
		}
	}
}