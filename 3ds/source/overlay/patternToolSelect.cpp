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

#include "common.hpp"
#include "overlay.hpp"

extern bool touching(touchPosition touch, ButtonType button);

static const std::vector<ButtonType> buttons = {
	{15, 34, 130, 48, "IMPORT_PATTERN"},
	{15, 97, 130, 48, "CHANGE_PALETTE"},
	{15, 159, 130, 48, "OWN_PATTERN"},
	{175, 34, 130, 48, "EXPORT_PATTERN"},
	{175, 97, 130, 48, "CLEAR_PATTERN"},
	{175, 159, 130, 48, "EXIT"}
};

static void Draw(int select) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, Lang::get("PATTERN_TOOL_MENU"), 395, 0, font);
	GFX::DrawBottom();

	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(buttons[i]);
	}

	GFX::DrawGUI(gui_pointer_idx, buttons[select].x + 100, buttons[select].y + 30);

	C3D_FrameEnd(0);
}

PatternMode Overlays::SelectPatternTool() {
	int selection = 0;

	while(1) {
		Draw(selection);
		u32 hDown = hidKeysDown();
		touchPosition touch;
		hidScanInput();
		hidTouchRead(&touch);

		if (hDown & KEY_TOUCH) {
			if (touching(touch, buttons[0])) {
				return PatternMode::Import;
			} else if (touching(touch, buttons[1])) {
				return PatternMode::Palette;
			} else if (touching(touch, buttons[2])) {
				return PatternMode::Own;
			} else if (touching(touch, buttons[3])) {
				return PatternMode::Export;
			} else if (touching(touch, buttons[4])) {
				return PatternMode::Clear;
			} else if (touching(touch, buttons[5])) {
				return PatternMode::Exit;
			}
		}

		if (hDown & KEY_UP) {
			if (selection > 0) selection--;
		}

		if (hDown & KEY_DOWN) {
			if (selection < 5) selection++;
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

		if (hDown & KEY_A) {
			switch(selection) {
				case 0:
					return PatternMode::Import;
				case 1:
					return PatternMode::Palette;
				case 2:
					return PatternMode::Own;
				case 3:
					return PatternMode::Export;
				case 4:
					return PatternMode::Clear;
				case 5:
					return PatternMode::Exit;
			}
		}

		if (hDown & KEY_B) {
			return PatternMode::Draw;
		}
	}
}