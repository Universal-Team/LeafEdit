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
		DrawPatternEditor(); // TODO.
	}
}

void PatternEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (patternMode == 0) {
		PatternMenuLogic(hDown, hHeld, touch);
	} else {
		PatternEditorLogic(hDown, hHeld, touch); // TODO.
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

	if (hDown & KEY_A) {
		SelectedPattern = Selection;
		patternMode = 1;
	}
	
}

// Color Groups.
/*
std::array<u32, 9> OrangeArray {
	0xFFEEFFFF, 0xFF99AAFF, 0xEE5599FF,
	0xFF66AAFF, 0xFF0066FF, 0xBB4477FF,
	0xCC0055FF, 0x990033FF, 0x552233FF
};

std::array<u32, 9> RedArray {
	0xFFBBCCFF, 0xFF7777FF, 0xDD3311FF, // Looks more like "Orange" in game than darker type of red or so.
	0xFF5544FF, 0xFF0000FF, 0xCC6666FF,
	0xBB4444FF, 0xBB0000FF, 0x882222FF
};

std::array<u32, 9> SkinArray {
	0xDDCCBBFF, 0xFFCC66FF, 0xDD6622FF,
	0xFFAA22FF, 0xFF6600FF, 0xBB8855FF,
	0xDD4400FF, 0xBB4400FF, 0x663311FF
};

std::array<u32, 9> PinkArray {
	0xFFEEDDFF, 0xFFDDCCFF, 0xFFCCAAFF,
	0xFFBB88FF, 0xFFAA88FF, 0xDD8866FF,
	0xBB6644FF, 0x995533FF, 0x884422FF
};

std::array<u32, 9> LilaArray {
	0xFFCCFFFF, 0xEE88FFFF, 0xCC66DDFF,
	0xBB88CCFF, 0xCC00FFFF, 0x996699FF,
	0x8800AAFF, 0x550077FF, 0x330044FF
};

std::array<u32, 9> Pink2Array {
	0xFFBBFFFF, 0xFF99FFFF, 0xDD22BBFF,
	0xFF55EEFF, 0xFF00CCFF, 0x885577FF,
	0xBB0099FF, 0x880066FF, 0x550044FF
};

std::array<u32, 9> BrownArray {
	0xDDBB99FF, 0xCCAA77FF, 0x774433FF,
	0xAA7744FF, 0x993300FF, 0x773322FF,
	0x552200FF, 0x331100FF, 0x221100FF
};

std::array<u32, 9> YellowArray {
	0xFFFFCCFF, 0xFFFF77FF, 0xDDDD22FF,
	0xFFFF00FF, 0xFFDD00FF, 0xCCAA00FF,
	0x999900FF, 0x887700FF, 0x555500FF
};

std::array<u32, 9> Lila2Array {
	0xDDBBFFFF, 0xBB99EEFF, 0x6633CCFF,
	0x9955FFFF, 0x6600FFFF, 0x554488FF,
	0x440099FF, 0x220066FF, 0x221133FF
};

std::array<u32, 9> BlueArray {
	0xBBBBFFFF, 0x8899FFFF, 0x3333AAFF,
	0x3355EEFF, 0x0000FFFF, 0x333388FF,
	0x0000AAFF, 0x111166FF, 0x000022FF
};

std::array<u32, 9> GreenArray {
	0x99EEBBFF, 0x66CC77FF, 0x226611FF,
	0x44AA33FF, 0x008833FF, 0x557755FF,
	0x225500FF, 0x113322FF, 0x002211FF
};

std::array<u32, 9> LightGreenArray {
	0xDDFFBBFF, 0xCCFF88FF, 0x88AA55FF,
	0xAADD88FF, 0x88FF00FF, 0xAABB99FF,
	0x66BB00FF, 0x559900FF, 0x336600FF
};

std::array<u32, 9> LightBlueArray = {
	0xBBDDFFFF, 0x77CCFFFF, 0x335599FF,
	0x6699FFFF, 0x1177FFFF, 0x4477AAFF,
	0x224477FF, 0x002277FF, 0x001144FF
};

std::array<u32, 9> LighterBlueArray {
	0xAAFFFFFF, 0x55FFFFFF, 0x0088BBFF,
	0x55BBCCFF, 0x00CCFFFF, 0x4499AAFF,
	0x006688FF, 0x004455FF, 0x002233FF
};

std::array<u32, 9> LighterGreenArray {
	0xCCFFEEFF, 0xAAEEDDFF, 0x33CCAAFF,
	0x55EEBBFF, 0x00FFCCFF, 0x77AAAAFF,
	0x00AA99FF, 0x008877FF, 0x004433FF
};

std::array<u32, 9> brightGreenArray {
	0xAAFFAAFF, 0x77FF77FF, 0x66DD44FF,
	0x00FF00FF, 0x22DD22FF, 0x55BB55FF,
	0x00BB00FF, 0x008800FF, 0x224422FF
};
*/ // ^ Those might be wrong? No clue -> Needs recheck.

// TODO. 32x32 is the Grid, so 1024 pixels. It's scaled x7 so -> 224x224. The grid may be 7 pixel each grid index.
void PatternEditor::DrawGrid(void) const {
}

// TODO. 15 Colors palette draw.
void PatternEditor::DrawPalette(void) const {
}

void PatternEditor::DrawPatternEditor(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, WHITE, "LeafEdit - " + Lang::get("PATTERN_EDITOR"), 400);
	Gui::sprite(5, npc_tina_idx, 0, 45);
	Gui::sprite(5, npc_sina_idx, 100, 45);
	// Pattern informations.
	Gui::DrawString(230, 50, 0.7f, WHITE, Lang::get("PATTERN_NAME") + StringUtils::UTF16toUTF8(SaveFile->players[selectedPassedPlayer]->Patterns[SelectedPattern]->Name), 160);
	Gui::DrawString(230, 70, 0.7f, WHITE, Lang::get("CREATOR_NAME") + StringUtils::UTF16toUTF8(SaveFile->players[selectedPassedPlayer]->Patterns[SelectedPattern]->CreatorName), 160);
	Gui::DrawString(230, 90, 0.7f, WHITE, Lang::get("CREATOR_ID") + std::to_string(SaveFile->players[selectedPassedPlayer]->Patterns[SelectedPattern]->CreatorId), 160);
	Gui::DrawString(230, 110, 0.7f, WHITE, Lang::get("TOWN_NAME") + StringUtils::UTF16toUTF8(SaveFile->players[selectedPassedPlayer]->Patterns[SelectedPattern]->OriginatingTownName), 160);
	Gui::DrawString(230, 130, 0.7f, WHITE, Lang::get("TOWN_ID") + std::to_string(SaveFile->players[selectedPassedPlayer]->Patterns[SelectedPattern]->OriginatingTownId), 160);

	Gui::DrawBottom(false);
	C2D_DrawImageAt(SaveFile->players[selectedPassedPlayer]->Patterns[SelectedPattern]->Images[0], 8, 8, 0.5f, nullptr, 7, 7); // 224x224. 224/32 -> 7.
	Gui::Draw_Rect(240, 30, 70, 180, DARKER_COLOR);
	DrawGrid();
	DrawPalette();
}

void PatternEditor::PatternEditorLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		patternMode = 0;
	}
}