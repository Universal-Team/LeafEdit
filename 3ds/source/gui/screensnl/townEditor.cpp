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

#include "core/management/acreManagement.hpp"

#include "offsets.hpp"
#include "save.hpp"

#include "gui/gui.hpp"

#include "gui/screensnl/acresEditor.hpp"
#include "gui/screensnl/townEditor.hpp"

#include <3ds.h>

extern Save* SaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

void TownEditor::Draw(void) const {
	if (Mode == 0) {
		DrawSubMenu();
	} else {
		DrawTownEditor();
	}
}

void TownEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (Mode == 0) {
		SubMenuLogic(hDown, hHeld, touch);
	} else if (Mode == 1) {
		editorLogic(hDown, hHeld, touch);
	}
}

void TownEditor::DrawSubMenu(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("TOWN_EDITOR"), 400);
	Gui::DrawBottom();
	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (subSelection == i) {
			Gui::drawAnimatedSelector(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, .030f, SELECTED_COLOR);
		}
	}
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("ACRES")))/2-80+17.5, 0.8, WHITE, Lang::get("ACRES"), 130, 25);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("TOWNMAP_EDITOR")))/2-20+17.5, 0.8, WHITE, Lang::get("TOWNMAP_EDITOR"), 130, 25);
}


// Display full Map on top screen for a better overview.
void TownEditor::DrawTownMap() const
{
	AcreManagement::InitAcres(20, 5, 90, 40, 4, MAP_ACRES + 0x10);
	int x;
	// Selection Logic.
	if (currentAcre < 5)	x = currentAcre;
	else if (currentAcre > 4 && currentAcre < 10)	x = currentAcre - 5;
	else if (currentAcre > 9 && currentAcre < 15)	x = currentAcre - 10;
	else	x = currentAcre - 15;

	Gui::drawGrid(90 + x * 40, 40 + currentAcre/5 * 40, 40, 40, WHITE);
}

void TownEditor::DrawTownEditor(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("TOWNMAP_EDITOR"), 400);
	Gui::DrawStringCentered(0, 217, 0.8f, WHITE, Lang::get("CURRENT_POSITION") + "32 | 16", 400);
	DrawTownMap();

	// Bottom Screen part. Grid & Acre.
	Gui::DrawBottom();
	AcreManagement::DrawAcre(SaveFile->ReadU8(MAP_ACRES + acreImage), 20, 40, 4, 4);
	DrawGrid();
	DrawCurrentPos();
}

void TownEditor::DrawCurrentPos(void) const {
	Gui::drawGrid(20 + (currentPosX*10.67), 40 + (currentPosY*10.67), 10.67, 10.67, WHITE);
}



/* NOTES:
	- The native acre resolution is 40x40, I scaled it x4, so it's around ~ 160x160.
	- A Grid is 10.66667~.
	- The Grid might be not like the actual ACNL ones, but it's the best I can do atm.
	- Notes end.
*/

void TownEditor::DrawGrid(void) const {
	for (int i = 0; i < 256; i++) {
		for (u32 y = 0; y < 15; y++) {
			for (u32 x = 0; x < 15; x++, i++) {
				Gui::drawGrid(20 + (x*10.67), 40 + (y*10.67), 10.67, 10.67);
			}
		}
	}
}

void TownEditor::updateAcre(void) {
	// First row.
	if (currentAcre == 0) {
		acreImage = 0x10;
	} else if (currentAcre == 1) {
		acreImage = 0x12;
	} else if (currentAcre == 2) {
		acreImage = 0x14;
	} else if (currentAcre == 3) {
		acreImage = 0x16;
	} else if (currentAcre == 4) {
		acreImage = 0x18;
	}
	// Second row.
	else if (currentAcre == 5) {
		acreImage = 0x1E;
	} else if (currentAcre == 6) {
		acreImage = 0x20;
	} else if (currentAcre == 7) {
		acreImage = 0x22;
	} else if (currentAcre == 8) {
		acreImage = 0x24;
	} else if (currentAcre == 9) {
		acreImage = 0x26;
	}
	// Third row.
	else if (currentAcre == 10) {
		acreImage = 0x2C;
	} else if (currentAcre == 11) {
		acreImage = 0x2E;
	} else if (currentAcre == 12) {
		acreImage = 0x30;
	} else if (currentAcre == 13) {
		acreImage = 0x32;
	} else if (currentAcre == 14) {
		acreImage = 0x34;
	}
	// Fourth row.
	else if (currentAcre == 15) {
		acreImage = 0x3A;
	} else if (currentAcre == 16) {
		acreImage = 0x3C;
	} else if (currentAcre == 17) {
		acreImage = 0x3E;
	} else if (currentAcre == 18) {
		acreImage = 0x40;
	} else if (currentAcre == 19) {
		acreImage = 0x42;
	}
}

void TownEditor::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_UP) {
		if(subSelection > 0)	subSelection--;
	} else if (hDown & KEY_DOWN) {
		if(subSelection < 2)	subSelection++;
	}

	if (hDown & KEY_A) {
		if (subSelection == 0) {
			if (Config::getBool("Debug") == true) {
				Gui::setScreen(std::make_unique<AcresEditor>());
			} else {
				Msg::DisplayWarnMsg2(Lang::get("NOT_SAVE_TO_USE"));
			}
		} else if (subSelection == 1) {
			Mode = 1;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			if (Config::getBool("Debug") == true) {
				Gui::setScreen(std::make_unique<AcresEditor>());
			} else {
				Msg::DisplayWarnMsg2(Lang::get("NOT_SAVE_TO_USE"));
			}
		} else if (touching(touch, mainButtons[1])) {
			Mode = 1;
		}
	}
}

void TownEditor::editorLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	updateAcre();

	if (hDown & KEY_B) {
		Mode = 0;
		currentPosX = 0;
		currentPosY = 0;
		currentAcre = 0;
	}

	if (hDown & KEY_RIGHT) {
		if (currentPosX == 14 && currentAcre < 19) {
			// Go one Acre next and reset X to 0.
			currentAcre++;
			currentPosX = 0;
		} else if (currentPosX < 14) {
			currentPosX++;
		}
	}

	if (hDown & KEY_LEFT) {
		if (currentPosX == 0 && currentAcre > 0) {
			// Go one Acre before.
			currentAcre--;
		} else if (currentPosX > 0) {
			currentPosX--;
		}
	}

	if (hDown & KEY_DOWN) {
		if (currentPosY == 14 && currentAcre < 15) {
			// Go one Acre down & reset Y to 0.
			currentAcre += 5;
			currentPosY = 0;
		} else if (currentPosY < 14) {
			currentPosY ++;
		}
	}

	if (hDown & KEY_UP) {
		if (currentPosY == 0 && currentAcre > 4) {
			// Go one Acre up.
			currentAcre -= 5;
		} else if (currentPosY > 0) {
			currentPosY--;
		}
	}
}