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

#ifndef _LEAFEDIT_TOWN_EDITOR_NL_HPP
#define _LEAFEDIT_TOWN_EDITOR_NL_HPP

#include "common.hpp"
#include "Pattern.hpp"
#include "Town.hpp"
#include "structs.hpp"

#include <vector>

class TownEditorNL : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	TownEditorNL(std::unique_ptr<Town> refTown);
private:
	int Selection = 0;
	int Mode = 0;
	std::unique_ptr<Town> town;
	std::unique_ptr<Pattern> pattern;

	const std::vector<ButtonType> mainButtons = {
		{15, 34, 102, 20, "MAP_EDITOR"},
		{15, 97, 102, 20, "ACRE_EDITOR"},
		{15, 159, 102, 20, "TOWNFLAG"},
		{175, 34, 102, 20, ""},
		{175, 97, 102, 20, ""},
		{175, 159, 102, 20, ""}
	};
};

#endif