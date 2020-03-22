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

#ifndef WWPATTERN_HPP
#define WWPATTERN_HPP

#include "wwsave.hpp"
#include "types.hpp"

#include <array>
#include <string>
#include <vector>

class WWPlayer;

class WWPattern {
public:
	WWPattern();
	WWPattern(WWPlayer *player, u32 id);
	WWPattern(u32 offset, u32 id);
	~WWPattern();

	void ReadPlayer();
	void ReadShop();

	std::u16string CreatorName;
	std::u16string Name;
	std::u16string TownName;
	u8 Palette;
	u8 Concept;
	u32 PaletteData;

	void Write();

	const u32 Index;
	const u32 Offset;
private:
};

#endif