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

#include "common/config.hpp"
#include "common/utils.hpp"

#include "core/management/playerManagement.hpp"

#include "gui/keyboard.hpp"

#include "gui/screensnl/itemEditor.hpp"
#include "gui/screensnl/playerEditor.hpp"
#include "gui/screens/screenCommon.hpp"

#include "pattern.hpp"
#include "player.hpp"
#include "save.hpp"

#include <3ds.h>
#include <sys/stat.h>
#include <unistd.h>

extern Save* SaveFile;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

int selectedPassedPlayer;

void PlayerEditor::Draw(void) const {
	if (screen == 0) {
		DrawSubMenu();
	} else if (screen == 1) {
		DrawMainEditor();
	} else if (screen == 2) {
		DrawPlayerEditor();
	} else if (screen == 3) {
		DrawPlayerStyle(); // Style of the Player, like Face, Hair etc.
	} else if (screen == 4) {
		DrawPattern(); // Pattern Screen. Maybe Editing later on too?
	}
}

void PlayerEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (screen == 0) {
		SubMenuLogic(hDown, hHeld);
	} else if (screen == 1) {
		MainEditorLogic(hDown, hHeld, touch);
	} else if (screen == 2) {
		PlayerEditorLogic(hDown, hHeld, touch);
	} else if (screen == 3) {
		PlayerStyleLogic(hDown, hHeld, touch);
	} else if (screen == 4) {
		PatternLogic(hDown, hHeld, touch);
	}
}

void PlayerEditor::DrawTPCAndName(void) const {
	u32 player = 0;
	for (u32 x = 0; x < 4; x++, player++) {
		if (SaveFile->players[player]->Exists()) {
			C2D_DrawImageAt(Save::Instance()->players[player]->m_TPCPic, (float)(100 * player) + 18.f, 45.f, 0.5f, nullptr, 1.f, 1.f);
			Gui::DrawString(18.0 + (player * 100.f), 150, 0.5f, WHITE, StringUtils::UTF16toUTF8(SaveFile->players[player]->Name).c_str(), 55);
		}
	}
}

void PlayerEditor::DrawSubMenu(void) const {
	std::string activePlayer;
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("PLAYER_SELECTION"), 400);
	DrawTPCAndName();

	for (int i = 0; i < 4; i++) {
		if (i == selectedPlayer) {
			Gui::drawAnimatedSelector((float)(100 * i) + 18.f, 45.f, 64, 104, .030f, C2D_Color32(0, 0, 0, 0));
		}
	}
	activePlayer += Lang::get("CURRENT_PLAYER");
	activePlayer += ": ";
	for (int i = 0; i < 4; i++) {
		if (selectedPlayer == i) {
			activePlayer += std::to_string(i+1);
		}
	}
	Gui::DrawStringCentered(0, 212, 0.9f, WHITE, activePlayer, 320);
	Gui::DrawBottom();
}

void PlayerEditor::DrawPlayerStyle(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("APPEARANCE"), 400);
	// Playername & TAN.
	Gui::Draw_Rect(40, 37, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 72, 320, 22, DARKER_COLOR);
	Gui::DrawStringCentered(0, 35, 0.9f, WHITE, Lang::get("PLAYER_NAME") + ": " + StringUtils::UTF16toUTF8(SaveFile->players[cp]->Name).c_str(), 380);
	Gui::DrawStringCentered(0, 70, 0.9f, WHITE, Lang::get("TAN_VALUE") + ": " + std::to_string((SaveFile->players[cp]->PlayerTan)), 380);

	// Player Hair & Face sprites.
	Gui::Draw_Rect(90, 105, 90, 40, DARKER_COLOR);
	PlayerManagement::DrawHair(SaveFile->players[cp]->hairStyle, 118, 106);
	Gui::Draw_Rect(90, 155, 90, 40, DARKER_COLOR);
	PlayerManagement::DrawFace(SaveFile->players[cp]->Gender, SaveFile->players[cp]->face, 115, 166);

	// Hair Color.
	Gui::Draw_Rect(200, 105, 90, 40, PlayerManagement::getHairColor(SaveFile->players[cp]->hairColor));
	// Eye Color.
	Gui::Draw_Rect(200, 155, 90, 40, PlayerManagement::getEyeColor(SaveFile->players[cp]->eyeColor));

	Gui::DrawBottom();
	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(playerButtons[i].x, playerButtons[i].y, playerButtons[i].w, playerButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			Gui::drawAnimatedSelector(playerButtons[i].x, playerButtons[i].y, playerButtons[i].w, playerButtons[i].h, .030f, SELECTED_COLOR);
		}
	}
	// Display Player name.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("PLAYER_NAME")))/2-80+17.5, 0.8, WHITE, Lang::get("PLAYER_NAME"), 130, 25);
	// Display Hair Style.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("HAIRSTYLE")))/2-20+17.5, 0.8, WHITE, Lang::get("HAIRSTYLE"), 130, 25);
	// Display Face.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("FACE")))/2+75-17.5, 0.8, WHITE, Lang::get("FACE"), 130, 25);
	// Display Tan.
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("TAN_VALUE")))/2-80+17.5, 0.8, WHITE, Lang::get("TAN_VALUE"), 130, 25);
	// Display Hair Color.
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("HAIR_COLOR")))/2-20+17.5, 0.8, WHITE, Lang::get("HAIR_COLOR"), 130, 25);
	// Display Eye Color.
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("EYE_COLOR")))/2+75-17.5, 0.8, WHITE, Lang::get("EYE_COLOR"), 130, 25);
}


void PlayerEditor::SubMenuLogic(u32 hDown, u32 hHeld) {
	for (int player = 0; player < 4; player++) {
		if (SaveFile->players[player]->Exists())	maxPlayer = player;
	}

	if (hDown & KEY_RIGHT) {
		selectedPlayer++;
		if(selectedPlayer > maxPlayer)	selectedPlayer = 0;
	} else if (hDown & KEY_LEFT) {
		selectedPlayer--;
		if(selectedPlayer < 0)	selectedPlayer = maxPlayer;
	} else if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		for (int i = 0; i < 4; i++) {
			if (i == selectedPlayer) {
				cp = i;
				Selection = 0;
				screen = 1;
			}
		}
	}
}

void PlayerEditor::DrawMainEditor(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("PLAYER_EDITOR"), 400);

	C2D_DrawImageAt(Save::Instance()->players[cp]->m_TPCPic, 170, 45.f, 0.5f, nullptr, 1, 1);
	Gui::DrawStringCentered(0, 150, 0.9f, WHITE, StringUtils::UTF16toUTF8(SaveFile->players[cp]->Name).c_str(), 55);
	Gui::DrawBottom();
	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(playerButtons[i].x, playerButtons[i].y, playerButtons[i].w, playerButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			Gui::drawAnimatedSelector(playerButtons[i].x, playerButtons[i].y, playerButtons[i].w, playerButtons[i].h, .030f, SELECTED_COLOR);
		}
	}
	Gui::sprite(0, sprites_back_idx, mainButtons[3].x, mainButtons[3].y);

		// Player Stuff like Wallet amount.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("PLAYER")))/2-80+17.5, 0.8, WHITE, Lang::get("PLAYER"), 130, 25);
		// Items.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("ITEMS")))/2-20+17.5, 0.8, WHITE, Lang::get("ITEMS"), 130, 25);
		// Appearance.
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("APPEARANCE")))/2+75-17.5, 0.8, WHITE, Lang::get("APPEARANCE"), 130, 25);
		// Pattern
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("PATTERN")))/2-80+17.5, 0.8, WHITE, Lang::get("PATTERN"), 130, 25);
}


void PlayerEditor::MainEditorLogic(u32 hDown, u32 hHeld, touchPosition touch) {
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

	if (hDown & KEY_TOUCH && touching(touch, mainButtons[3])) {
		screen = 0;
	}

	if (hDown & KEY_B) {
		screen = 0;
	}

	if (hDown & KEY_A) {
		switch(Selection) {
				case 0:
					Selection = 0;
					screen = 2;
					break;
				case 1:
					selectedPassedPlayer = cp;
					Gui::setScreen(std::make_unique<ItemEditor>());
					break;
				case 2:
					Selection = 0;
					screen = 3;
					break;
				case 3:
					Selection = 0;
					screen = 4;
					break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, playerButtons[0])) {
			Selection = 0;
			screen = 2;
		} else if (touching(touch, playerButtons[1])) {
			selectedPassedPlayer = cp;
			Gui::setScreen(std::make_unique<ItemEditor>());
		} else if (touching(touch, playerButtons[2])) {
			Selection = 0;
			screen = 3;
		} else if (touching(touch, playerButtons[3])) {
			Selection = 0;
			screen = 4;
		}
	}
}

void PlayerEditor::DrawPlayerEditor(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("PLAYER_EDITOR"), 400);
	// Draw Rectangles first, then Player Info.
	Gui::Draw_Rect(40, 37, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 65, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 93, 320, 22, DARKER_COLOR);
	Gui::Draw_Rect(40, 121, 320, 22, DARKER_COLOR);
	Gui::DrawStringCentered(0, 35, 0.9f, WHITE, Lang::get("WALLET_AMOUNT") + ": " + std::to_string((SaveFile->players[cp]->Wallet.value)), 380);
	Gui::DrawStringCentered(0, 63, 0.9f, WHITE, Lang::get("BANK_AMOUNT") + ": " + std::to_string((SaveFile->players[cp]->BankAmount.value)), 380);
	Gui::DrawStringCentered(0, 91, 0.9f, WHITE, Lang::get("MEDAL_AMOUNT") + ": " + std::to_string((SaveFile->players[cp]->IslandMedals.value)), 380);
	Gui::DrawStringCentered(0, 119, 0.9f, WHITE, Lang::get("COUPON_AMOUNT") + ": " + std::to_string((SaveFile->players[cp]->MeowCoupons.value)), 380);
	// Player Bottom.
	Gui::DrawBottom();

	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(playerButtons[i].x, playerButtons[i].y, playerButtons[i].w, playerButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			Gui::drawAnimatedSelector(playerButtons[i].x, playerButtons[i].y, playerButtons[i].w, playerButtons[i].h, .030f, SELECTED_COLOR);
		}
	}

	if (currentPage == 1) {
		// Display Wallet Amount
		Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("WALLET_AMOUNT")))/2-80+17.5, 0.8, WHITE, Lang::get("WALLET_AMOUNT"), 130, 25);
		// Clear Wallet.
		Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("CLEAR_WALLET")))/2-20+17.5, 0.8, WHITE, Lang::get("CLEAR_WALLET"), 130, 25);
		// Max Wallet.
		Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("MAX_WALLET")))/2+75-17.5, 0.8, WHITE, Lang::get("MAX_WALLET"), 130, 25);
		// Display Bank Amount.
		Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("BANK_AMOUNT")))/2-80+17.5, 0.8, WHITE, Lang::get("BANK_AMOUNT"), 130, 25);
		// Display Medal Amount.
		Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("MEDAL_AMOUNT")))/2-20+17.5, 0.8, WHITE, Lang::get("MEDAL_AMOUNT"), 130, 25);
		// Display Coupon Amount.
		Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("COUPON_AMOUNT")))/2+75-17.5, 0.8, WHITE, Lang::get("COUPON_AMOUNT"), 130, 25);

	} else if (currentPage == 2) {

		// Max Bank.
		Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("MAX_BANK")))/2-80+17.5, 0.8, WHITE, Lang::get("MAX_BANK"), 130, 25);
		// Max Medals.
		Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("MAX_MEDALS")))/2-20+17.5, 0.8, WHITE, Lang::get("MAX_MEDALS"), 130, 25);
		// Max Coupons.
		Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, Lang::get("MAX_COUPONS")))/2+75-17.5, 0.8, WHITE, Lang::get("MAX_COUPONS"), 130, 25);
		// Clear Bank.
		Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("CLEAR_BANK")))/2-80+17.5, 0.8, WHITE, Lang::get("CLEAR_BANK"), 130, 25);
		// Clear Medals.
		Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("CLEAR_MEDALS")))/2-20+17.5, 0.8, WHITE, Lang::get("CLEAR_MEDALS"), 130, 25);
		// Clear Coupons.
		Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, Lang::get("CLEAR_COUPONS")))/2+75-17.5, 0.8, WHITE, Lang::get("CLEAR_COUPONS"), 130, 25);
	}
}

void PlayerEditor::PlayerEditorLogic(u32 hDown, u32 hHeld, touchPosition touch) {

	// Selection / Page.
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

	// Pages.
	if (hDown & KEY_R) {
		if(currentPage < 2) currentPage++;
			Selection = 0;
	} else if (hDown & KEY_L) {
		if(currentPage > 1)	currentPage--;
			Selection = 0;
	}

	// Selection A / Touch.
	if (hDown & KEY_A) {
		if (currentPage == 1) {
			switch(Selection) {
				case 0:
					SaveFile->players[cp]->Wallet.value = Input::handleu32(5, Lang::get("ENTER_WALLET_AMOUNT"), 99999, SaveFile->players[cp]->Wallet.value);
					break;
				case 1:
					SaveFile->players[cp]->Wallet.value = 0;
					break;
				case 2:
					SaveFile->players[cp]->Wallet.value = 99999;
					break;
				case 3:
					SaveFile->players[cp]->BankAmount.value = Input::handleu32(9, Lang::get("ENTER_BANK_AMOUNT"), 999999999, SaveFile->players[cp]->BankAmount.value);
					break;
				case 4:
					SaveFile->players[cp]->IslandMedals.value = Input::handleu32(4, Lang::get("ENTER_MEDAL_AMOUNT"), 9999, SaveFile->players[cp]->IslandMedals.value);
					break;
				case 5:
					SaveFile->players[cp]->MeowCoupons.value = Input::handleu32(4, Lang::get("ENTER_COUPON_AMOUNT"), 9999, SaveFile->players[cp]->MeowCoupons.value);
					break;
			}

		} else if (currentPage == 2) {
			switch(Selection) {
				case 0:
					SaveFile->players[cp]->BankAmount.value = 999999999;
					break;
				case 1:
					SaveFile->players[cp]->IslandMedals.value = 9999;
					break;
				case 2:
					SaveFile->players[cp]->MeowCoupons.value = 9999;
					break;
				case 3:
					SaveFile->players[cp]->BankAmount.value = 0;
					break;
				case 4:
					SaveFile->players[cp]->IslandMedals.value = 0;
					break;
				case 5:
					SaveFile->players[cp]->MeowCoupons.value = 0;
					break;
			}
		}
	}

	if (hDown & KEY_TOUCH) {
		if (currentPage == 1) {
			if (touching(touch, playerButtons[0])) {
				SaveFile->players[cp]->Wallet.value = Input::handleu32(5, Lang::get("ENTER_WALLET_AMOUNT"), 99999, SaveFile->players[cp]->Wallet.value);
			} else if (touching(touch, playerButtons[1])) {
				SaveFile->players[cp]->Wallet.value = 0;
			} else if (touching(touch, playerButtons[2])) {
				SaveFile->players[cp]->Wallet.value = 99999;
			} else if (touching(touch, playerButtons[3])) {
				SaveFile->players[cp]->BankAmount.value = Input::handleu32(9, Lang::get("ENTER_BANK_AMOUNT"), 999999999, SaveFile->players[cp]->BankAmount.value);
			} else if (touching(touch, playerButtons[4])) {
				SaveFile->players[cp]->IslandMedals.value = Input::handleu32(4, Lang::get("ENTER_MEDAL_AMOUNT"), 9999, SaveFile->players[cp]->IslandMedals.value);
			} else if (touching(touch, playerButtons[5])) {
				SaveFile->players[cp]->MeowCoupons.value = Input::handleu32(4, Lang::get("ENTER_COUPON_AMOUNT"), 9999, SaveFile->players[cp]->MeowCoupons.value);
			}

		} else if (currentPage == 2) {
			if (touching(touch, playerButtons[0])) {
				SaveFile->players[cp]->BankAmount.value = 999999999;
			} else if (touching(touch, playerButtons[1])) {
				SaveFile->players[cp]->IslandMedals.value = 9999;
			} else if (touching(touch, playerButtons[2])) {
				SaveFile->players[cp]->MeowCoupons.value = 9999;
			} else if (touching(touch, playerButtons[3])) {
				SaveFile->players[cp]->BankAmount.value = 0;
			} else if (touching(touch, playerButtons[4])) {
				SaveFile->players[cp]->IslandMedals.value = 0;
			} else if (touching(touch, playerButtons[5])) {
				SaveFile->players[cp]->MeowCoupons.value = 0;
			}
		}
	}

	if (hDown & KEY_B) {
		Selection = 0;
		screen = 1;
	}
}

void PlayerEditor::PlayerStyleLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Selection A / Touch.
	if (hDown & KEY_A) {
		switch(Selection) {
			case 0:
				SaveFile->players[cp]->Name = Input::handleu16String(8, Lang::get("NEW_PLAYER_NAME"), SaveFile->players[cp]->Name);
				break;
			case 1:
				SaveFile->players[cp]->hairStyle = PlayerManagement::SelectHair(SaveFile->players[cp]->hairStyle);
				break;
			case 2:
				SaveFile->players[cp]->face = PlayerManagement::SelectFace(SaveFile->players[cp]->face, SaveFile->players[cp]->Gender);
				break;
			case 3:
				SaveFile->players[cp]->PlayerTan = PlayerManagement::SelectTan(SaveFile->players[cp]->PlayerTan);
				break;
			case 4:
				SaveFile->players[cp]->hairColor = PlayerManagement::SelectHairColor(SaveFile->players[cp]->hairColor);
				break;
			case 5:
				SaveFile->players[cp]->eyeColor = PlayerManagement::SelectEyeColor(SaveFile->players[cp]->eyeColor);
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, playerButtons[0])) {
			SaveFile->players[cp]->Name = Input::handleu16String(8, Lang::get("NEW_PLAYER_NAME"), SaveFile->players[cp]->Name);
		} else if (touching(touch, playerButtons[1])) {
			SaveFile->players[cp]->hairStyle = PlayerManagement::SelectHair(SaveFile->players[cp]->hairStyle);
		} else if (touching(touch, playerButtons[2])) {
			SaveFile->players[cp]->face = PlayerManagement::SelectFace(SaveFile->players[cp]->face, SaveFile->players[cp]->Gender);
		} else if (touching(touch, playerButtons[3])) {
			SaveFile->players[cp]->PlayerTan = PlayerManagement::SelectTan(SaveFile->players[cp]->PlayerTan);
		} else if (touching(touch, playerButtons[4])) {
			SaveFile->players[cp]->hairColor = PlayerManagement::SelectHairColor(SaveFile->players[cp]->hairColor);
		} else if (touching(touch, playerButtons[5])) {
			SaveFile->players[cp]->eyeColor = PlayerManagement::SelectEyeColor(SaveFile->players[cp]->eyeColor);
		}
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

	if (hDown & KEY_B) {
		Selection = 0;
		screen = 1;
	}
}

void PlayerEditor::DrawPattern(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("PATTERN"), 400);
	Gui::DrawBottom();
	for (int i = 0; i < 10; i++) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				C2D_DrawImageAt(Save::Instance()->players[cp]->Patterns[i]->Images[0], 17 + (x * 60), 60 + (y * 80), 0.5f, nullptr, 1.5f, 1.5f);
			}
		}
	}

	int selectY = 0, selectX = 0;
	if (Selection < 5)	selectY = 0;	else	selectY = 1;
	if (Selection > 4)	selectX = Selection - 5;	else	selectX = Selection;

	Gui::drawAnimatedSelector(17 + (selectX * 60), 60 + (selectY * 80), 48, 48, .030f, C2D_Color32(0, 0, 0, 0));
}

void PlayerEditor::PatternLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		screen = 1;
		Selection = 0;
	}

	if (hDown & KEY_DOWN) {
		if (Selection < 5)	Selection += 5;
	}
	if (hDown & KEY_UP) {
		if (Selection > 4)	Selection -= 5;
	}
	if (hDown & KEY_RIGHT) {
		if (Selection < 9)	Selection++;
	}
	if (hDown & KEY_LEFT) {
		if (Selection > 0)	Selection--;
	}
}