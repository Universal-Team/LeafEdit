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

#ifndef _LEAFEDIT_PLAYER_EDITOR_WW_HPP
#define _LEAFEDIT_PLAYER_EDITOR_WW_HPP

#include "common.hpp"
#include "Player.hpp"
#include "structs.hpp"

#include <vector>

class PlayerEditorWW : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	PlayerEditorWW(std::unique_ptr<Player> refPlayer);
private:
	int Selection = 0;
	int Mode = 0;
	std::unique_ptr<Player> player;

	const std::vector<ButtonType> mainButtons = {
		{ 15, 34, 102, 20, "PLAYER" },
		{ 15, 97, 102, 20, "APPEARANCE" },
		{ 15, 159, 102, 20, "ITEMS" },
		{ 175, 34, 102, 20, "PATTERN" },
		{ 175, 97, 102, 20, "" },
		{ 175, 159, 102, 20, "" }
	};

	const std::vector<ButtonType> appearanceBtn = {
		{ 15, 34, 102, 20, "PLAYER_NAME" },
		{ 15, 97, 102, 20, "PLAYER_HAIR_STYLE" },
		{ 15, 159, 102, 20, "PLAYER_FACETYPE" },
		{ 175, 34, 102, 20, "PLAYER_TAN_VALUE" },
		{ 175, 97, 102, 20, "PLAYER_HAIR_COLOR" }
	};

	const std::vector<ButtonType> playerButtons = {
		{ 15, 34, 102, 20, "PLAYER_WALLET" },
		{ 15, 97, 102, 20, "PLAYER_BANK" }
	};

	const std::vector<Structs::ButtonPos> patternPos = {
		{ 40 + (0 * 60), 60 + (0 * 80), 48, 48 },
		{ 40 + (1 * 60), 60 + (0 * 80), 48, 48 },
		{ 40 + (2 * 60), 60 + (0 * 80), 48, 48 },
		{ 40 + (3 * 60), 60 + (0 * 80), 48, 48 },

		{ 40 + (0 * 60), 60 + (1 * 80), 48, 48 },
		{ 40 + (1 * 60), 60 + (1 * 80), 48, 48 },
		{ 40 + (2 * 60), 60 + (1 * 80), 48, 48 },
		{ 40 + (3 * 60), 60 + (1 * 80), 48, 48 }
	};

	void DrawSubMenu(void) const;
	void SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);

	/* Appearance. */
	void DrawAppearance(void) const;
	void AppearanceLogic(u32 hDown, u32 hHeld, touchPosition touch);

	/* Pattern. */
	void DisplayPattern(void) const;
	void PatternLogic(u32 hDown, u32 hHeld, touchPosition touch);

	/* Player. */
	void DrawPlayer(void) const;
	void PlayerLogic(u32 hDown, u32 hHeld, touchPosition touch);

	std::unique_ptr<Pattern> pattern[8] = { nullptr };
	std::unique_ptr<PatternImage> images[8] = { nullptr };
	C2D_Image patternImage[8] = { nullptr };
};

#endif