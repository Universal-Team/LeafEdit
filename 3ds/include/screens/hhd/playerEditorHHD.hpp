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

#ifndef _LEAFEDIT_PLAYER_EDITOR_HHD_HPP
#define _LEAFEDIT_PLAYER_EDITOR_HHD_HPP

#include "coreUtils.hpp"
#include "Pattern.hpp"
#include "PatternImage.hpp"
#include "PlayerHHD.hpp"
#include "structs.hpp"
#include <vector>

class PlayerEditorHHD : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch);
	PlayerEditorHHD(std::unique_ptr<PlayerHHD> refPlayer);

private:
	std::unique_ptr<PlayerHHD> player;
	int Selection = 0, PatternPage = 0, Mode = 0;

	void LoadPattern();

	std::unique_ptr<Pattern> pattern[15] = {nullptr};
	std::unique_ptr<PatternImage> images[15] = {nullptr};
	C2D_Image patternImage[15] = {nullptr};
	bool loaded[15] = {false};

	/* Draws. */
	void DrawSubMenu(void) const;
	void DrawPattern(void) const;
	void DrawUnlock(void) const;

	/* Logics. */
	void SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);
	void PatternLogic(u32 hDown, u32 hHeld, touchPosition touch);
	void UnlockLogic(u32 hDown, u32 hHeld, touchPosition touch);

	const std::vector<ButtonType> mainButtons = {
		{15, 34, 102, 20, "PATTERN"},
		{15, 97, 102, 20, "MISCS"},
		{15, 159, 102, 20, ""},
		{175, 34, 102, 20, ""},
		{175, 97, 102, 20, ""},
		{175, 159, 102, 20, ""}
	};

	const std::vector<ButtonType> unlockBtn = {
		{15, 34, 102, 20, "UNLOCK_HANDBOOKS"},
		{15, 97, 102, 20, "UNLOCK_EMOTIONS"},
		{15, 159, 102, 20, ""},
		{175, 34, 102, 20, ""},
		{175, 97, 102, 20, ""},
		{175, 159, 102, 20, ""}
	};

	/* 15 Pattern each page. */
	 const std::vector<Structs::ButtonPos> PatternPos = {
		{17, 40, 48, 48},
		{17, 100, 48, 48},
		{17, 160, 48, 48},

		{77, 40, 48, 48},
		{77, 100, 48, 48},
		{77, 160, 48, 48},

		{137, 40, 48, 48},
		{137, 100, 48, 48},
		{137, 160, 48, 48},

		{197, 40, 48, 48},
		{197, 100, 48, 48},
		{197, 160, 48, 48},

		{257, 40, 48, 48},
		{257, 100, 48, 48},
		{257, 160, 48, 48}
	};
};

#endif