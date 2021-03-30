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

#include "badgeEditor.hpp"
#include "coreUtils.hpp"
#include "itemEditorNL.hpp"
#include "itemUtils.hpp"
#include "keyboard.hpp"
#include "patternEditor.hpp"
#include "playerEditorNL.hpp"
#include "playerManagement.hpp"
#include "Sav.hpp"
#include "spriteManagement.hpp"
#include "stringUtils.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

extern std::shared_ptr<Sav> save;
/* Bring that to other screens too. */
extern SaveType savesType;

const std::vector<std::string> g_TanValues = {
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"
};

extern std::vector<std::string> g_EyeColor;
extern std::vector<std::string> g_FaceType;
extern std::vector<std::string> g_HairColor;
extern std::vector<std::string> g_HairStyle;

PlayerEditorNL::PlayerEditorNL(std::unique_ptr<Player> refPlayer): player(std::move(refPlayer)) {
	C3D_FrameEnd(0);
	this->TPC = CoreUtils::LoadPlayerTPC(this->player, loaded);
}

PlayerEditorNL::~PlayerEditorNL() {
	C3D_FrameEnd(0);

	if (this->TPC.tex && this->loaded) {
		C2DUtils::C2D_ImageDelete(this->TPC);
		this->TPC.tex = nullptr;
		this->TPC.subtex = nullptr;
	}
}

void PlayerEditorNL::Draw(void) const {
	switch(this->Mode) {
		case 0:
			this->DrawSubMenu();
			break;

		case 1:
			this->DrawPlayer();
			break;

		case 2:
			this->DrawAppearance();
			break;

		case 3:
			this->DisplayPattern();
			break;
	}
}

void PlayerEditorNL::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	switch(this->Mode) {
		case 0:
			this->SubMenuLogic(hDown, hHeld, touch);
			break;

		case 1:
			this->PlayerLogic(hDown, hHeld, touch);
			break;

		case 2:
			this->AppearanceLogic(hDown, hHeld, touch);
			break;

		case 3:
			this->PatternLogic(hDown, hHeld, touch);
			break;
	}
}

/* Sub Menu. */
void PlayerEditorNL::DrawSubMenu(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("PLAYER_SUBMENU"), 395, 0, font);
	Gui::DrawStringCentered(0, 40, 0.7f, BLACK, Lang::get("PLAYER_NAME") + ": " + StringUtils::UTF16toUTF8(this->player->name()), 0, 0, font);
	Gui::DrawStringCentered(0, 60, 0.7f, BLACK, Lang::get("PLAYER_WALLET") + ": " + std::to_string(this->player->wallet()), 0, 0, font);
	Gui::DrawStringCentered(0, 90, 0.7f, BLACK, Lang::get("PLAYER_BANK") + ": " + std::to_string(this->player->bank()), 0, 0, font);
	Gui::DrawStringCentered(0, 120, 0.7f, BLACK, Lang::get("PLAYER_FACETYPE") + ": " + std::to_string(this->player->face()), 0, 0, font);

	if (this->player->hasTPC() && this->TPC.tex && this->loaded) {
		C2D_DrawImageAt(this->TPC, 60, 80, 0.5);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();

	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(this->mainButtons[i]);
		if (i == this->Selection) GFX::DrawGUI(gui_pointer_idx, this->mainButtons[i].x+100, this->mainButtons[i].y+30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void PlayerEditorNL::SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 5) this->Selection++;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->Selection < 3) this->Selection += 3;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->Selection < 6 && this->Selection > 2) this->Selection -= 3;
	}

	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				this->Selection = 0;
				this->Mode = 1;
				break;

			case 1:
				this->Selection = 0;
				this->Mode = 2;
				break;

			case 2:
				Gui::setScreen(std::make_unique<BadgeEditor>(this->player), doFade, true);
				break;

			case 3:
				Gui::setScreen(std::make_unique<ItemEditorNL>(this->player), doFade, true);
				break;

			case 4:
				/* Load Pattern. */
				C3D_FrameEnd(0);
				for (int i = 0; i < 10; i++) {
					this->pattern[i] = this->player->pattern(i);
					this->images[i] = this->pattern[i]->image(0);
					this->patternImage[i] = CoreUtils::patternImage(this->images[i], savesType);
				}

				this->Selection = 0;
				this->Mode = 3;
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, this->mainButtons[0])) {
			this->Selection = 0;
			this->Mode = 1;


		} else if (touching(touch, this->mainButtons[1])) {
			this->Selection = 0;
			this->Mode = 2;


		} else if (touching(touch, this->mainButtons[2])) {
			Gui::setScreen(std::make_unique<BadgeEditor>(this->player), doFade, true);


		} else if (touching(touch, this->mainButtons[3])) {
			Gui::setScreen(std::make_unique<ItemEditorNL>(this->player), doFade, true);


		} else if (touching(touch, this->mainButtons[4])) {
			/* Load Pattern. */
			C3D_FrameEnd(0);
			for (int i = 0; i < 10; i++) {
				this->pattern[i] = this->player->pattern(i);
				this->images[i] = this->pattern[i]->image(0);
				this->patternImage[i] = CoreUtils::patternImage(this->images[i], savesType);
			}

			this->Selection = 0;
			this->Mode = 3;
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}
}

/* Appearance. */
void PlayerEditorNL::DrawAppearance(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("APPEARANCE"), 400, 0, font);

	/* Playername & TAN. */
	Gui::Draw_Rect(40, 37, 320, 22, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
	Gui::Draw_Rect(40, 72, 320, 22, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
	Gui::DrawStringCentered(0, 35, 0.9f, WHITE, Lang::get("PLAYER_NAME") + ": " + StringUtils::UTF16toUTF8(this->player->name()), 380, 0, font);
	Gui::DrawStringCentered(0, 70, 0.9f, WHITE, Lang::get("PLAYER_TAN_VALUE") + ": " + std::to_string((this->player->tan())), 380, 0, font);

	/* Player Hair & Face sprites. */
	SpriteManagement::DrawHair(this->player->hairstyle(), 118, 106);
	SpriteManagement::DrawFace(this->player->gender(), this->player->face(), 115, 166);

	/* Hair Color. */
	Gui::Draw_Rect(200, 105, 90, 40, PlayerManagement::getHairColor(this->player->haircolor(), savesType));
	/* Eye Color. */
	Gui::Draw_Rect(200, 155, 90, 40, PlayerManagement::getEyeColor(this->player->eyecolor()));

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();

	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(this->appearanceBtn[i]);
		if (i == this->Selection) GFX::DrawGUI(gui_pointer_idx, this->appearanceBtn[i].x+100, this->appearanceBtn[i].y+30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void PlayerEditorNL::AppearanceLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 5) this->Selection++;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->Selection < 3) this->Selection += 3;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->Selection < 6 && this->Selection > 2) this->Selection -= 3;
	}

	if (hDown & KEY_B) {
		this->Selection = 0;
		this->Mode = 0;
	}

	if (hDown & KEY_A) {
		u8 temp = 0;
		switch(this->Selection) {
			case 0:
				this->player->name(StringUtils::UTF8toUTF16(Input::setString(8, StringUtils::UTF16toUTF8(this->player->name()), Lang::get("ENTER_PLAYERNAME"))));
				break;

			case 1:
				temp = (u8)GFX::ListSelection(this->player->hairstyle(), g_HairStyle, Lang::get("SELECT_HAIRSTYLE"));

				switch(this->player->gender()) {
					case 0:
						if (temp < 17) this->player->hairstyle(temp);
						else Msg::DisplayWaitMsg(Lang::get("NOT_VALID_HAIRSTYLE") + Lang::get("MALE") + "!");
						break;

					case 1:
						if (temp > 16) this->player->hairstyle(temp);

						else Msg::DisplayWaitMsg(Lang::get("NOT_VALID_HAIRSTYLE") + Lang::get("FEMALE") + "!");
						break;
				}
				break;

			case 2:
				temp = (u8)GFX::ListSelection(this->player->face(), g_FaceType, Lang::get("SELECT_FACETYPE"));

				switch(this->player->gender()) {
					case 0:
						if (temp < 12) this->player->face(temp);
						else Msg::DisplayWaitMsg(Lang::get("NOT_VALID_FACETYPE") + Lang::get("MALE") + "!");
						break;

					case 1:
						if (temp > 11) this->player->face(temp);
						else Msg::DisplayWaitMsg(Lang::get("NOT_VALID_FACETYPE") + Lang::get("FEMALE") + "!");
						break;
				}
				break;

			case 3:
				this->player->tan((u8)GFX::ListSelection(this->player->tan(), g_TanValues, Lang::get("SELECT_TAN")));
				break;

			case 4:
				this->player->haircolor((u8)GFX::ListSelection(this->player->haircolor(), g_HairColor, Lang::get("SELECT_HAIRCOLOR")));
				break;

			case 5:
				this->player->eyecolor((u8)GFX::ListSelection(this->player->eyecolor(), g_EyeColor, Lang::get("SELECT_EYECOLOR")));
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, this->appearanceBtn[0])) {
			this->player->name(StringUtils::UTF8toUTF16(Input::setString(8, StringUtils::UTF16toUTF8(this->player->name()), Lang::get("ENTER_PLAYERNAME"))));


		} else if (touching(touch, this->appearanceBtn[1])) {
			u8 temp = (u8)GFX::ListSelection(this->player->hairstyle(), g_HairStyle, Lang::get("SELECT_HAIRSTYLE"));

			switch(this->player->gender()) {
				case 0:
					if (temp < 17) this->player->hairstyle(temp);
					else Msg::DisplayWaitMsg(Lang::get("NOT_VALID_HAIRSTYLE") + Lang::get("MALE") + "!");
					break;

				case 1:
					if (temp > 16) this->player->hairstyle(temp);
					else Msg::DisplayWaitMsg(Lang::get("NOT_VALID_HAIRSTYLE") + Lang::get("FEMALE") + "!");
					break;
			}


		} else if (touching(touch, this->appearanceBtn[2])) {
			u8 temp = (u8)GFX::ListSelection(this->player->face(), g_FaceType, Lang::get("SELECT_FACETYPE"));

			switch(this->player->gender()) {
				case 0:
					if (temp < 12) this->player->face(temp);
					else Msg::DisplayWaitMsg(Lang::get("NOT_VALID_FACETYPE") + Lang::get("MALE") + "!");
					break;

				case 1:
					if (temp > 11) this->player->face(temp);
					else Msg::DisplayWaitMsg(Lang::get("NOT_VALID_FACETYPE") + Lang::get("FEMALE") + "!");
					break;
			}


		} else if (touching(touch, this->appearanceBtn[3])) {
			this->player->tan((u8)GFX::ListSelection(this->player->tan(), g_TanValues, Lang::get("SELECT_TAN")));


		} else if (touching(touch, this->appearanceBtn[4])) {
			this->player->haircolor((u8)GFX::ListSelection(this->player->haircolor(), g_HairColor, Lang::get("SELECT_HAIRCOLOR")));


		} else if (touching(touch, this->appearanceBtn[5])) {
			this->player->eyecolor((u8)GFX::ListSelection(this->player->eyecolor(), g_EyeColor, Lang::get("SELECT_EYECOLOR")));
		}
	}
}

/* Player. */
void PlayerEditorNL::DrawPlayer(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("PLAYER"), 400, 0, font);

	Gui::Draw_Rect(40, 37, 320, 22, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
	Gui::Draw_Rect(40, 65, 320, 22, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
	Gui::Draw_Rect(40, 93, 320, 22, C2D_Color32(0xD5, 0xB0, 0x6E, 255));

	Gui::DrawStringCentered(0, 35, 0.9f, BLACK, Lang::get("PLAYER_WALLET") + ": " + std::to_string((this->player->wallet())), 380, 0, font);
	Gui::DrawStringCentered(0, 63, 0.9f, BLACK, Lang::get("PLAYER_BANK") + ": " + std::to_string((this->player->bank())), 380, 0, font);
	Gui::DrawStringCentered(0, 91, 0.9f, BLACK, Lang::get("PLAYER_MEDALS") + ": " + std::to_string((this->player->islandmedals())), 380, 0, font);

	if (savesType == SaveType::WA) {
		Gui::Draw_Rect(40, 121, 320, 22, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
		Gui::DrawStringCentered(0, 119, 0.9f, BLACK, Lang::get("PLAYER_COUPONS") + ": " + std::to_string((this->player->coupons())), 380, 0, font);
	}

	GFX::DrawBottom();

	for (int i = 0; i < 4; i++) {
		GFX::DrawButton(this->playerButtons[i]);
		if (i == this->Selection) GFX::DrawGUI(gui_pointer_idx, this->playerButtons[i].x+100, this->playerButtons[i].y+30);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void PlayerEditorNL::PlayerLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	/* Navigation. */
	if (hRepeat & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}

	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 5) this->Selection++;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->Selection < 3) this->Selection += 3;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->Selection < 6 && this->Selection > 2) this->Selection -= 3;
	}

	if (hDown & KEY_B) {
		this->Selection = 0;
		this->Mode = 0;
	}

	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				this->player->wallet((u32)Input::setInt(99999, Lang::get("ENTER_WALLET_AMOUNT"), 5, this->player->wallet()));
				break;

			case 1:
				this->player->bank((u32)Input::setInt(999999999, Lang::get("ENTER_BANK_AMOUNT"), 9, this->player->bank()));
				break;

			case 2:
				this->player->islandmedals((u32)Input::setInt(9999, Lang::get("ENTER_MEDAL_AMOUNT"), 4, this->player->islandmedals()));
				break;

			case 3:
				if (savesType == SaveType::WA) {
					this->player->coupons((u32)Input::setInt(9999, Lang::get("ENTER_COUPON_AMOUNT"), 4, this->player->coupons()));
				}

				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, this->playerButtons[0])) {
			this->player->wallet((u32)Input::setInt(99999, Lang::get("ENTER_WALLET_AMOUNT"), 5, this->player->wallet()));


		} else if (touching(touch, this->playerButtons[1])) {
			this->player->bank((u32)Input::setInt(999999999, Lang::get("ENTER_BANK_AMOUNT"), 9, this->player->bank()));


		} else if (touching(touch, this->playerButtons[2])) {
			this->player->islandmedals((u32)Input::setInt(9999, Lang::get("ENTER_MEDAL_AMOUNT"), 4, this->player->islandmedals()));


		} else if (touching(touch, this->playerButtons[3])) {
			if (savesType == SaveType::WA) {
				this->player->coupons((u32)Input::setInt(9999, Lang::get("ENTER_COUPON_AMOUNT"), 4, this->player->coupons()));
			}
		}
	}
}

void PlayerEditorNL::DisplayPattern(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("PATTERN_VIEWER"), 395, 0, font);

	Gui::DrawStringCentered(0, 40, 0.7f, BLACK, Lang::get("PATTERN_NAME") + ": " + StringUtils::UTF16toUTF8(this->pattern[this->Selection]->name()), 395, 0, font);
	Gui::DrawStringCentered(0, 60, 0.7f, BLACK, Lang::get("PATTERN_CREATOR_NAME") + ": " +  StringUtils::UTF16toUTF8(this->pattern[this->Selection]->creatorname()), 395, 0, font);
	Gui::DrawStringCentered(0, 80, 0.7f, BLACK, Lang::get("PATTERN_CREATOR_ID") + ": " + std::to_string(pattern[this->Selection]->creatorid()), 395, 0, font);
	Gui::DrawStringCentered(0, 100, 0.7f, BLACK, Lang::get("PATTERN_ORIGIN_NAME") + ": " + StringUtils::UTF16toUTF8(this->pattern[this->Selection]->origtownname()), 395, 0, font);
	Gui::DrawStringCentered(0, 120, 0.7f, BLACK, Lang::get("PATTERN_ORIGIN_ID") + ": " + std::to_string(this->pattern[this->Selection]->origtownid()), 395, 0, font);

	if (this->pattern[this->Selection]->creatorGender()) {
		Gui::DrawStringCentered(0, 140, 0.7f, BLACK, Lang::get("GENDER") + ": " + Lang::get("FEMALE"), 395, 0, font);

	} else {
		Gui::DrawStringCentered(0, 140, 0.7f, BLACK, Lang::get("GENDER") + ": " + Lang::get("MALE"), 395, 0, font);
	}

	GFX::DrawBottom();

	for (int i = 0; i < 10; i++) {
		if (this->patternImage[i].tex) {
			C2D_DrawImageAt(this->patternImage[i], this->patternPos[i].x, this->patternPos[i].y, 0.5f, nullptr, 1.5f, 1.5f);
		}
	}

	GFX::DrawGUI(gui_pointer_idx, this->patternPos[this->Selection].x + 20, this->patternPos[this->Selection].y + 20);
}

void PlayerEditorNL::PatternLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_RIGHT) {
		if (this->Selection < 9) this->Selection++;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->Selection > 0) this->Selection--;
	}

	/* Refresh Pattern Images. */
	if (hDown & KEY_SELECT) {
		/* Free. */
		C3D_FrameEnd(0);
		for (int i = 0; i < 10; i++) {
			if (this->patternImage[i].tex) C2DUtils::C2D_ImageDelete(this->patternImage[i]);
		}

		for (int i = 0; i < 10; i++) {
			this->pattern[i] = this->player->pattern(i);
			this->images[i] = this->pattern[i]->image(0);
			this->patternImage[i] = CoreUtils::patternImage(this->images[i], savesType);
		}
	}

	if (hDown & KEY_B) {
		/* Free. */
		C3D_FrameEnd(0);
		for (int i = 0; i < 10; i++) {
			if (this->patternImage[i].tex) C2DUtils::C2D_ImageDelete(this->patternImage[i]);
		}

		this->Selection = 0;
		this->Mode = 0;
	}

	/* Open Pattern Editor. */
	if (hDown & KEY_A) {
		Gui::setScreen(std::make_unique<PatternEditor>(this->pattern[this->Selection]), doFade, true);
	}

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 10; i++) {
			if (iconTouch(touch, this->patternPos[i])) {
				Gui::setScreen(std::make_unique<PatternEditor>(this->pattern[i]), doFade, true);
			}
		}
	}
}

/* Letters. */
void PlayerEditorNL::DrawLetter(void) const {
	GFX::DrawTop();

	if (this->player->letter(this->Selection)->flag() != 0) { // Flag 0 -> No exist.
		Gui::DrawStringCentered(0, -2, 0.9f, WHITE, StringUtils::UTF16toUTF8(this->player->letter(this->Selection)->intro()), 400, 0, font);
		Gui::DrawStringCentered(0, 30, 0.9f, WHITE, StringUtils::UTF16toUTF8(this->player->letter(this->Selection)->body()), 400, 0, font);
		Gui::DrawStringCentered(0, 217, 0.9f, WHITE, StringUtils::UTF16toUTF8(this->player->letter(this->Selection)->end()), 400, 0, font);

	} else {
		Gui::DrawStringCentered(0, -2, 0.9f, WHITE, Lang::get("LETTER_NOT_AVAILABLE"), 395, 0, font);
	}

	GFX::DrawBottom();
}

void PlayerEditorNL::LetterLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hDown & KEY_B) {
		this->Selection = 0;
		this->Mode = 0;
	}

	if (hRepeat & KEY_RIGHT) {
		if (this->Selection < 9) this->Selection++;
	}

	if (hRepeat & KEY_LEFT) {
		if (this->Selection > 0) this->Selection--;
	}
}