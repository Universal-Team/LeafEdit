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

#ifndef _LEAFEDIT_PLAYEREDITOR_NL_HPP
#define _LEAFEDIT_PLAYEREDITOR_NL_HPP

#include "common.hpp"
#include "coreUtils.hpp"
#include "structs.hpp"

#include <vector>

class PlayerEditorNL : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	PlayerEditorNL(std::shared_ptr<Player> p);
	~PlayerEditorNL();
private:
	C2D_Image TPC;
	int Selection = 0;
	int Mode = 0;
	std::shared_ptr<Player> player;

	std::vector<ButtonType> mainButtons = {
		{15, 34, 130, 48, "APPEARANCE"},
		{15, 97, 130, 48, "BADGE_EDITOR"},
		{15, 159, 130, 48, ""},
		{175, 34, 130, 48, ""},
		{175, 97, 130, 48, ""},
		{175, 159, 130, 48, ""}
	};

	std::vector<ButtonType> appearanceBtn = {
		{15, 34, 130, 48, "PLAYER_NAME"},
		{15, 97, 130, 48, "PLAYER_HAIR_STYLE"},
		{15, 159, 130, 48, "PLAYER_FACETYPE"},
		{175, 34, 130, 48, "PLAYER_TAN_VALUE"},
		{175, 97, 130, 48, "PLAYER_HAIR_COLOR"},
		{175, 159, 130, 48, "PLAYER_EYE_COLOR"}
	};

	void DrawSubMenu(void) const;
	void SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);

	/*	Appearance.	*/
	void DrawAppearance(void) const;
	void AppearanceLogic(u32 hDown, u32 hHeld, touchPosition touch);
	/*	Player.	*/
};

#endif