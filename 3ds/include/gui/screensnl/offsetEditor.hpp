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

#ifndef OFFSETEDITOR_HPP
#define OFFSETEDITOR_HPP

#include "common/structs.hpp"

#include "gui/screens/screen.hpp"

#include <3ds.h>
#include <vector>

class OffsetEditor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int Selection = 0;
	bool isReadMode = false;

	void Editu8();
	void Editu16();
	void Editu32();
	void Editu64();
	void Editu16String();
	void EditEncryptedInt32();
	
	void Readu8();
	void Readu16();
	void Readu32();
	void Readu64();
	void Readu16String();
	void ReadEncryptedInt32();

	std::vector<Structs::ButtonPos> mainButtons = {
		{10, 40, 140, 35, -1}, // u8.
		{10, 100, 140, 35, -1}, // u16.
		{10, 160, 140, 35, -1}, // u32.
		{170, 40, 140, 35, -1}, // u64.
		{170, 100, 140, 35, -1}, // u16 string.
		{170, 160, 140, 35, -1}, // EncryptedInt32 -> TODO.
	};
};

#endif