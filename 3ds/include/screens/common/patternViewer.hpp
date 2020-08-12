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

#ifndef _LEAFEDIT_PATTERN_VIEWER_HPP
#define _LEAFEDIT_PATTERN_VIEWER_HPP

#include "common.hpp"
#include "Player.hpp"
#include "Pattern.hpp"
#include "PatternImage.hpp"
#include "structs.hpp"

#include <vector>

class PatternViewer : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	PatternViewer(std::shared_ptr<Player> plr, SaveType ST = SaveType::WW);
	~PatternViewer();

private:
	SaveType SType;
	void DrawPattern(void) const;
	void DisplayPatternInfo(void) const;
	int selectedPattern = 0;
	std::shared_ptr<Player> player;
	std::shared_ptr<Pattern> pattern[10];
	std::shared_ptr<PatternImage> images[10];
	C2D_Image patternImage[10];
};

#endif