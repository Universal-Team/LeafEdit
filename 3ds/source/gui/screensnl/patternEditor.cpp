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

#include "common/utils.hpp"

#include "core/management/playerManagement.hpp"

#include "gui/screensnl/patternEditor.hpp"
#include "gui/screens/screenCommon.hpp"

#include "pattern.hpp"
#include "player.hpp"
#include "save.hpp"

#include <3ds.h>

extern Save* SaveFile;
extern int selectedPassedPlayer;

void PatternEditor::Draw(void) const {
	if (patternMode == 0) {
		DrawPatternMenu();
	} else {
		//DrawPatternEditor(); // TODO.
	}
}

void PatternEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (patternMode == 0) {
		PatternMenuLogic(hDown, hHeld, touch);
	} else {
		//PatternEditorLogic(hDown, hHeld, touch); // TODO.
	}
}

// SUB Menu.
void PatternEditor::DrawPatternMenu(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("PATTERN"), 400);
	Gui::sprite(5, npc_tina_idx, 0, 45);
	Gui::sprite(5, npc_sina_idx, 100, 45);
	// Pattern informations.
	Gui::DrawString(230, 50, 0.7f, WHITE, Lang::get("PATTERN_NAME") + StringUtils::UTF16toUTF8(SaveFile->players[selectedPassedPlayer]->Patterns[Selection]->Name), 160);
	Gui::DrawString(230, 70, 0.7f, WHITE, Lang::get("CREATOR_NAME") + StringUtils::UTF16toUTF8(SaveFile->players[selectedPassedPlayer]->Patterns[Selection]->CreatorName), 160);
	Gui::DrawString(230, 90, 0.7f, WHITE, Lang::get("CREATOR_ID") + std::to_string(SaveFile->players[selectedPassedPlayer]->Patterns[Selection]->CreatorId), 160);
	Gui::DrawString(230, 110, 0.7f, WHITE, Lang::get("TOWN_NAME") + StringUtils::UTF16toUTF8(SaveFile->players[selectedPassedPlayer]->Patterns[Selection]->OriginatingTownName), 160);
	Gui::DrawString(230, 130, 0.7f, WHITE, Lang::get("TOWN_ID") + std::to_string(SaveFile->players[selectedPassedPlayer]->Patterns[Selection]->OriginatingTownId), 160);
	Gui::DrawBottom();
	// Display Pattern Images.
	for (int i = 0; i < 10; i++) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				C2D_DrawImageAt(Save::Instance()->players[selectedPassedPlayer]->Patterns[i]->Images[0], 17 + (x * 60), 60 + (y * 80), 0.5f, nullptr, 1.5f, 1.5f);
			}
		}
	}

	// Selection part.
	int selectY = 0, selectX = 0;
	if (Selection < 5)	selectY = 0;	else	selectY = 1;
	if (Selection > 4)	selectX = Selection - 5;	else	selectX = Selection;
	Gui::sprite(0, sprites_pointer_idx, 30 + (selectX * 60), 80 + (selectY * 80));
}

void PatternEditor::PatternMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
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

	/* TODO -> Switch to Pattern Editor with the passed Pattern.
	if (hDown & KEY_A) {
		SelectedPattern = Selection;
		patternMode = 1;
	}
	*/
}

/* Editor -> TODO.
void PatternEditor::DrawPatternEditor(void) const {

}

void PatternEditor::PatternEditorLogic(u32 hDown, u32 hHeld, touchPosition touch) {

}
*/