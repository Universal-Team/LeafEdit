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

#pragma once

#ifndef WWSAVE_H
#define WWSAVE_H

#include "types.h"

#include <string>

class WWSave {
public:
	static WWSave* Initialize(const char *saveName, bool init);
	static WWSave* Instance();
	u8* GetRawSaveData(void);
	u64 GetSaveSize(void);
	void Close(void);

private:
	u8 *m_saveBuffer;
	u64 m_saveSize;
	const char *m_saveFile;
	WWSave(void);
	WWSave(WWSave const&) {};
	WWSave& operator=(WWSave const&) { return *WWSave::Instance(); }; // Is this right?
	static WWSave* m_pSave;
};

#endif