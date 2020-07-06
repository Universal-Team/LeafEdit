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
#include "itemUtils.hpp"
#include "playerEditorNL.hpp"
#include "playerManagement.hpp"
#include "Sav.hpp"
#include "spriteManagement.hpp"
#include "stringUtils.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern std::shared_ptr<Sav> save;
// Bring that to other screens too.
extern SaveType savesType;

PlayerEditorNL::PlayerEditorNL(std::shared_ptr<Player> p): player(p) {
	this->TPC = CoreUtils::LoadPlayerTPC(this->player);
}

// Destroy TPC.
PlayerEditorNL::~PlayerEditorNL() {
	if (this->TPC.tex != nullptr && this->TPC.subtex != nullptr) {
		C2DUtils::C2D_ImageDelete(this->TPC);
		this->TPC.tex = nullptr;
		this->TPC.subtex = nullptr;
	}
}

void PlayerEditorNL::Draw(void) const {
	if (this->Mode == 0)	this->DrawSubMenu();
	else if (this->Mode == 1)	this->DrawAppearance();
}

void PlayerEditorNL::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->Mode == 0)	this->SubMenuLogic(hDown, hHeld, touch);
	else if (this->Mode == 1)	this->AppearanceLogic(hDown, hHeld, touch);
}

/*	Sub Menu.	*/
void PlayerEditorNL::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - Player SubMenu", 395, 0, font);
	Gui::DrawStringCentered(0, 40, 0.7f, BLACK, "Player Name: " + StringUtils::UTF16toUTF8(this->player->name()), 0, 0, font);
	Gui::DrawStringCentered(0, 60, 0.7f, BLACK, "Wallet: " + std::to_string(this->player->wallet()), 0, 0, font);
	Gui::DrawStringCentered(0, 90, 0.7f, BLACK, "Bank: " + std::to_string(this->player->bank()), 0, 0, font);
	Gui::DrawStringCentered(0, 120, 0.7f, BLACK, "FaceType: " + std::to_string(this->player->face()), 0, 0, font);

	// Only display TPC if Player has TPC support and is not nullptr.
	// NOTE: Citra don't seems to like to display TPC Images. I'm not sure why.
	if (this->player->tpcImage() != nullptr && this->player->hasTPC() && this->TPC.tex != nullptr) {
		C2D_DrawImageAt(this->TPC, 60, 80, 0.5);
	}

	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(this->mainButtons[i]);
		if (i == this->Selection)	GFX::DrawGUI(gui_pointer_idx, this->mainButtons[i].x+100, this->mainButtons[i].y+30);
	}
}

void PlayerEditorNL::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
	if (hDown & KEY_UP) {
		if (this->Selection > 0)	this->Selection--;
	}

	if (hDown & KEY_DOWN) {
		if (this->Selection < 5)	this->Selection++;
	}

	if (hDown & KEY_RIGHT) {
		if (this->Selection < 3) {
			this->Selection += 3;
		}
	}

	if (hDown & KEY_LEFT) {
		if (this->Selection < 6 && this->Selection > 2) {
			this->Selection -= 3;
		}
	}

	if (hDown & KEY_A) {
		if (this->Selection == 0) {
			this->Mode = 1;
		}
	}
	
	if (hDown & KEY_B) {
		Gui::screenBack();
	}
}

/*	Appearance.	*/
void PlayerEditorNL::DrawAppearance(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("APPEARANCE"), 400, 0, font);
	// Playername & TAN.
	Gui::Draw_Rect(40, 37, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 72, 320, 22, DARKER_COLOR);
	Gui::DrawStringCentered(0, 35, 0.9f, WHITE, "Player Name: " + StringUtils::UTF16toUTF8(this->player->name()), 380, 0, font);
	Gui::DrawStringCentered(0, 70, 0.9f, WHITE, "Tan Value: " + std::to_string((this->player->tan())), 380, 0, font);

	// Player Hair & Face sprites.
	SpriteManagement::DrawHair(this->player->hairstyle(), 118, 106);
	SpriteManagement::DrawFace(this->player->gender(), this->player->face(), 115, 166);

	// Hair Color.
	Gui::Draw_Rect(200, 105, 90, 40, PlayerManagement::getHairColor(this->player->haircolor(), savesType));
	// Eye Color.
	Gui::Draw_Rect(200, 155, 90, 40, PlayerManagement::getEyeColor(this->player->eyecolor()));

	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(this->appearanceBtn[i]);
		if (i == this->Selection)	GFX::DrawGUI(gui_pointer_idx, this->appearanceBtn[i].x+100, this->appearanceBtn[i].y+30);
	}
}

void PlayerEditorNL::AppearanceLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
	if (hDown & KEY_UP) {
		if (this->Selection > 0)	this->Selection--;
	}

	if (hDown & KEY_DOWN) {
		if (this->Selection < 5)	this->Selection++;
	}

	if (hDown & KEY_RIGHT) {
		if (this->Selection < 3) {
			this->Selection += 3;
		}
	}
	
	if (hDown & KEY_LEFT) {
		if (this->Selection < 6 && this->Selection > 2) {
			this->Selection -= 3;
		}
	}
	
	if (hDown & KEY_B) {
		this->Selection = 0;
		this->Mode = 0;
	}
}