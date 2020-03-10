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

#include "gui.hpp"
#include "itemManagement.hpp"
#include "keyboard.hpp"
#include "player.hpp"
#include "save.hpp"
#include "shop.hpp"
#include "shopEditor.hpp"
#include "shopManagement.hpp"
#include "utils.hpp"

#include <3ds.h>
#include <map>

extern Save* SaveFile;
extern std::map<u16, std::string> g_itemDatabase;

static std::vector<std::pair<std::string, s32>> storeData; // Store the data.

extern bool touching(touchPosition touch, Structs::ButtonPos button);

// Only load item stuff, when accessing this screen and also unload by exit of that screen.
ShopEditor::ShopEditor() {
	ItemManagement::loadItems();
}

ShopEditor::~ShopEditor()
{
	ItemManagement::unloadItems();
}

void ShopEditor::Draw(void) const {
	if (shopMode == 0) {
		DrawSubMenu();
	} else if (shopMode == 1) {
		DrawTurnipScreen();
	}
}

void ShopEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (shopMode == 0) {
		SubMenuLogic(hDown, hHeld, touch);
	} else if (shopMode == 1) {
		TurnipLogic(hDown, hHeld, touch);
	}
}

void ShopEditor::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("SHOP_EDITOR"), 400);
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, mainButtons[i].x+130, mainButtons[i].y+25);
		}
	}
	// Turnip editor. [Prices]
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8f, Lang::get("TURNIP_EDITOR")))/2-80+17.5, 0.8, WHITE, Lang::get("TURNIP_EDITOR"), 130, 25);
}

void ShopEditor::DrawTurnipScreen(void) const {
	GFX::DrawTop();
	std::string title = "LeafEdit - " + Lang::get("TURNIP_EDITOR") + " - ";
	if (turnipMode == 0) {
		title += Lang::get("AM");
	} else {
		title += Lang::get("PM");
	}
	GFX::DrawNPC(npc_joan_idx, 20, 45);
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, title, 400);

	if (turnipMode == 0) {
		Gui::DrawString(180, 35, 0.8f, WHITE, Lang::get("MONDAY") + ": " + std::to_string(Save::Instance()->shop->AMPrice[0].value), 370);
		Gui::DrawString(180, 65, 0.8f, WHITE, Lang::get("TUESDAY") + ": " + std::to_string(Save::Instance()->shop->AMPrice[1].value), 370);
		Gui::DrawString(180, 95, 0.8f, WHITE, Lang::get("WEDNESDAY") + ": " + std::to_string(Save::Instance()->shop->AMPrice[2].value), 370);
		Gui::DrawString(180, 125, 0.8f, WHITE, Lang::get("THURSDAY") + ": " + std::to_string(Save::Instance()->shop->AMPrice[3].value), 370);
		Gui::DrawString(180, 155, 0.8f, WHITE, Lang::get("FRIDAY") + ": " + std::to_string(Save::Instance()->shop->AMPrice[4].value), 370);
		Gui::DrawString(180, 185, 0.8f, WHITE, Lang::get("SATURDAY") + ": " + std::to_string(Save::Instance()->shop->AMPrice[5].value), 370);
	} else {
		Gui::DrawString(180, 35, 0.8f, WHITE, Lang::get("MONDAY") + ": " + std::to_string(Save::Instance()->shop->PMPrice[0].value), 370);
		Gui::DrawString(180, 65, 0.8f, WHITE, Lang::get("TUESDAY") + ": " + std::to_string(Save::Instance()->shop->PMPrice[1].value), 370);
		Gui::DrawString(180, 95, 0.8f, WHITE, Lang::get("WEDNESDAY") + ": " + std::to_string(Save::Instance()->shop->PMPrice[2].value), 370);
		Gui::DrawString(180, 125, 0.8f, WHITE, Lang::get("THURSDAY") + ": " + std::to_string(Save::Instance()->shop->PMPrice[3].value), 370);
		Gui::DrawString(180, 155, 0.8f, WHITE, Lang::get("FRIDAY") + ": " + std::to_string(Save::Instance()->shop->PMPrice[4].value), 370);
		Gui::DrawString(180, 185, 0.8f, WHITE, Lang::get("SATURDAY") + ": " + std::to_string(Save::Instance()->shop->PMPrice[5].value), 370);
	}
	GFX::DrawBottom();

	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, mainButtons[i].x+130, mainButtons[i].y+25);
		}
	}

	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("MONDAY")))/2-80+17.5, 0.8, WHITE, Lang::get("MONDAY"), 130, 25);
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("TUESDAY")))/2-20+17.5, 0.8, WHITE, Lang::get("TUESDAY"), 130, 25);
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("WEDNESDAY")))/2+75-17.5, 0.8, WHITE, Lang::get("WEDNESDAY"), 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("THURDSDAY")))/2-80+17.5, 0.8, WHITE, Lang::get("THURSDAY"), 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("FRIDAY")))/2-20+17.5, 0.8, WHITE, Lang::get("FRIDAY"), 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("SATURDAY")))/2+75-17.5, 0.8, WHITE, Lang::get("SATURDAY"), 130, 25);
}


void ShopEditor::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		switch(Selection) {
			case 0:
				Selection = 0;
				shopMode = 1;
				break;
		}
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}

	// Selection.
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	}
	if (hDown & KEY_DOWN) {
			if(Selection < 5)	Selection++;
	}
	if (hDown & KEY_RIGHT) {
		if (Selection < 3) {
			Selection += 3;
		}
	}
	if (hDown & KEY_LEFT) {
		if (Selection < 6 && Selection > 2) {
			Selection -= 3;
		}
	}
	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			Selection = 0;
			shopMode = 1;
		}
	}
}

void ShopEditor::TurnipLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Selection = 0;
		shopMode = 0;
	}

	if (hDown & KEY_R) {
		if (turnipMode == 0)	turnipMode = 1;
	}
	
	if (hDown & KEY_L) {
		if (turnipMode == 1)	turnipMode = 0;
	}

	// Selection.
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	}

	if (hDown & KEY_DOWN) {
			if(Selection < 5)	Selection++;
	}

	if (hDown & KEY_RIGHT) {
		if (Selection < 3) {
			Selection += 3;
		}
	}
	if (hDown & KEY_LEFT) {
		if (Selection < 6 && Selection > 2) {
			Selection -= 3;
		}
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK") + "\n" + Lang::get("LR_MODESWITCH"));
	}

	if (hDown & KEY_A) {
		if (turnipMode == 0) {
			SaveFile->shop->AMPrice[Selection].value = Input::handleu32(4, Lang::get("ENTER_TURNIP_PRICE"), 9999, SaveFile->shop->AMPrice[Selection].value);
		} else {
			SaveFile->shop->PMPrice[Selection].value = Input::handleu32(4, Lang::get("ENTER_TURNIP_PRICE"), 9999, SaveFile->shop->PMPrice[Selection].value);
		}
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 6; i++) {
			if (touching(touch, mainButtons[i])) {
				if (turnipMode == 0) {
					SaveFile->shop->AMPrice[i].value = Input::handleu32(4, Lang::get("ENTER_TURNIP_PRICE"), 9999, SaveFile->shop->AMPrice[i].value);
				} else {
					SaveFile->shop->PMPrice[i].value = Input::handleu32(4, Lang::get("ENTER_TURNIP_PRICE"), 9999, SaveFile->shop->PMPrice[i].value);
				}
			}
		}
	}
}