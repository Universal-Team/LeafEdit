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

#ifndef _LEAFEDIT_ACRES_EDITOR_WW_HPP
#define _LEAFEDIT_ACRES_EDITOR_WW_HPP

#include "Acre.hpp"
#include "common.hpp"
#include "structs.hpp"
#include "Town.hpp"

class AcresEditorWW : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	AcresEditorWW(std::unique_ptr<Town> &refTown);
private:
	std::unique_ptr<Town> &town;

	/* Specific stuff. */
	u32 maxAcres;
	std::unique_ptr<Acre> FullAcres[36]; // Complete Acres.

	void DrawMap(void) const;
	void DrawTopSelection(void) const;
	u8 selectedAcre = 0;
	bool FastMode = false; // Scrolling speed.
	int selection = 0;

	/* Display AC:WW's full Town Map. */
	const std::vector<Structs::ButtonPos> acrePos = {
		/* First Line. */
		{ 64, 16, 32, 32 },
		{ 96, 16, 32, 32 },
		{ 128, 16, 32, 32 },
		{ 160, 16, 32, 32 },
		{ 192, 16, 32, 32 },
		{ 224, 16, 32, 32 },
		/* Second Line. */
		{ 64, 48, 32, 32 },
		{ 96, 48, 32, 32 },
		{ 128, 48, 32, 32 },
		{ 160, 48, 32, 32 },
		{ 192, 48, 32, 32 },
		{ 224, 48, 32, 32 },
		/* Third Line. */
		{ 64, 80, 32, 32 },
		{ 96, 80, 32, 32 },
		{ 128, 80, 32, 32 },
		{ 160, 80, 32, 32 },
		{ 192, 80, 32, 32 },
		{ 224, 80, 32, 32 },
		/* Fourth Line. */
		{ 64, 112, 32, 32 },
		{ 96, 112, 32, 32 },
		{ 128, 112, 32, 32 },
		{ 160, 112, 32, 32 },
		{ 192, 112, 32, 32 },
		{ 224, 112, 32, 32 },
		/* Fifth Line. */
		{ 64, 144, 32, 32 },
		{ 96, 144, 32, 32 },
		{ 128, 144, 32, 32 },
		{ 160, 144, 32, 32 },
		{ 192, 144, 32, 32 },
		{ 224, 144, 32, 32 },
		/* Sixth Line. */
		{ 64, 176, 32, 32 },
		{ 96, 176, 32, 32 },
		{ 128, 176, 32, 32 },
		{ 160, 176, 32, 32 },
		{ 192, 176, 32, 32 },
		{ 224, 176, 32, 32 }
	};
};

#endif