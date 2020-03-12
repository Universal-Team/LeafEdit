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

#include "config.hpp"
#include "keyboard.hpp"
#include "playerEditorWW.hpp"
#include "screenCommon.hpp"
#include "utils.hpp"
#include "wwPlayer.hpp"
#include "wwPlayerManagement.hpp"
#include "wwStringUtils.hpp"
#include "wwsave.hpp"

#include <3ds.h>

extern WWSave* WWSaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

int selectedPassedPlayerWW;
extern std::vector<std::string> g_wwFaceType;
extern std::vector<std::string> g_wwHairStyle;

const std::vector<std::string> &TanLevel = {
	"0",
	"1",
	"2",
	"3",
};

void PlayerEditorWW::Draw(void) const {
	if (screen == 0) {
		DrawPlayerSelection();
	} else if (screen == 1) {
		DrawSubMenu();
	} else if (screen == 2) {
		DrawPlayerScreen();
	}
}

void PlayerEditorWW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (screen == 0) {
		PlayerSelectionLogic(hDown, hHeld, touch);
	} else if (screen == 1) {
		SubMenuLogic(hDown, hHeld, touch);
	} else if (screen == 2) {
		PlayerLogic(hDown, hHeld, touch);
	}
}

void PlayerEditorWW::DrawPlayerBoxes(void) const {
	for (u32 y = 0; y < 2; y++) {
		for (u32 x = 0; x < 2; x++) {
			Gui::Draw_Rect(45 + x*160, 40 + y*100, 150, 60, DARKER_COLOR);
		}
	}
}

void PlayerEditorWW::DrawPlayerSelection(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - Player Selection", 400);
	DrawPlayerBoxes();
	Gui::DrawStringCentered(0, 214, 0.9f, WHITE, "Current Player: " + std::to_string(selectedPlayer+1), 400);
	GFX::DrawBottom();
}

void PlayerEditorWW::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - Player Sub Menu", 400);
	GFX::DrawBottom();
	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, mainButtons[i].x+130, mainButtons[i].y+25);
		}
	}
	Gui::DrawStringCentered(0, mainButtons[0].y+10, 0.8f, WHITE, Lang::get("PLAYER"), 130);
	Gui::DrawStringCentered(0, mainButtons[1].y+10, 0.8f, WHITE, Lang::get("ITEMS"), 130);
	Gui::DrawStringCentered(0, mainButtons[2].y+10, 0.8f, WHITE, Lang::get("APPEARANCE"), 130);
}

void PlayerEditorWW::DrawPlayerScreen(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - Player Editor", 400);
	Gui::Draw_Rect(40, 37, 320, 35, DARKER_COLOR);
	Gui::DrawStringCentered(0, 30, 0.9f, WHITE, "Item: " + WWSaveFile->players[cp]->Dresser[2]->GetName(), 380);
	Gui::DrawStringCentered(0, 45, 0.9f, WHITE, "Name: " + StringUtils::UTF16toUTF8(WWSaveFile->players[cp]->Name), 380);
	Gui::DrawStringCentered(0, 70, 0.9f, WHITE, "Bells: " + std::to_string(WWSaveFile->players[cp]->Bells), 380);
	Gui::DrawStringCentered(0, 95, 0.9f, WHITE, "HairType: " + std::to_string(WWSaveFile->players[cp]->HairType), 380);
	Gui::DrawStringCentered(0, 120, 0.9f, WHITE, "FaceType: " + std::to_string(WWSaveFile->players[cp]->FaceType), 380);
	Gui::DrawStringCentered(0, 145, 0.9f, WHITE, "HairColor: " + std::to_string(WWSaveFile->players[cp]->HairColor), 380);
	Gui::DrawStringCentered(0, 170, 0.9f, WHITE, "TAN: " + std::to_string(WWSaveFile->players[cp]->TAN), 380);
	Gui::DrawStringCentered(0, 195, 0.9f, WHITE, "Gender: " + std::to_string(WWSaveFile->players[cp]->Gender), 380);
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(playerButtons[i].x, playerButtons[i].y, playerButtons[i].w, playerButtons[i].h, UNSELECTED_COLOR);
		if (selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, playerButtons[i].x+130, playerButtons[i].y+25);
		}
	}
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, "Bells"))/2-80+17.5, 0.8, WHITE, "Bells", 130, 25);
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, "Name"))/2-20+17.5, 0.8, WHITE, "Name", 130, 25);
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, "Gender"))/2+75-17.5, 0.8, WHITE, "Gender", 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, "TAN"))/2-80+17.5, 0.8, WHITE, "TAN", 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, "FaceType"))/2-20+17.5, 0.8, WHITE, "FaceType", 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, "HairStyle"))/2+75-17.5, 0.8, WHITE, "HairStyle", 130, 25);

}

void PlayerEditorWW::PlayerLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_A) {
		switch (selection) {
			case 0:
				WWPlayerManagement::setBells(cp);
				break;
			case 1:
				WWPlayerManagement::setName(cp);
				break;
			case 2:
				WWPlayerManagement::setGender(cp);
				break;
			case 3:
				WWSaveFile->players[cp]->TAN = (u8)GFX::ListSelection((int)WWSaveFile->players[cp]->TAN, TanLevel, "Select the skin tone.");
				break;
			case 4:
				WWSaveFile->players[cp]->FaceType = (u8)GFX::ListSelection((int)WWSaveFile->players[cp]->FaceType, g_wwFaceType, "Select the Face Type.");
				break;
			case 5:
				WWSaveFile->players[cp]->HairType = (u8)GFX::ListSelection((int)WWSaveFile->players[cp]->HairType, g_wwHairStyle, "Select the HairStyle.");
				break;
			default:
				break;
		}
	}

	if (hDown & KEY_X) {
		WWSaveFile->players[0]->Gender = 1;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}

	// Selection.
	if (hDown & KEY_UP) {
		if(selection > 0)	selection--;
	}
	
	if (hDown & KEY_DOWN) {
			if(selection < 5)	selection++;
	}

	if (hDown & KEY_RIGHT) {
		if (selection < 3) {
			selection += 3;
		}
	}

	if (hDown & KEY_LEFT) {
		if (selection < 6 && selection > 2) {
			selection -= 3;
		}
	}
	
	if (hDown & KEY_B) {
		selection = 0;
		screen = 1;
	}
}

void PlayerEditorWW::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		selection = 0;
		screen = 0;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}

	if (hDown & KEY_DOWN) {
		if (selection < 2)	selection++;
	}

	if (hDown & KEY_UP) {
		if (selection > 0)	selection--;
	}

	if (hDown & KEY_A) {
			// Player.
		if (selection == 0) {
			selection = 0;
			screen = 2;
		}
	}
}

void PlayerEditorWW::PlayerSelectionLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	for (int player = 0; player < 4; player++) {
		if (WWSaveFile->players[player]->Exists())	maxPlayer = player;
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}
	
	if (hDown & KEY_RIGHT) {
		selectedPlayer++;
		if(selectedPlayer > maxPlayer)	selectedPlayer = 0;
	} else if (hDown & KEY_LEFT) {
		selectedPlayer--;
		if(selectedPlayer < 0)	selectedPlayer = maxPlayer;
	}

	if (hDown & KEY_A) {
		cp = selectedPlayer;
		selection = 0;
		screen = 1;
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}