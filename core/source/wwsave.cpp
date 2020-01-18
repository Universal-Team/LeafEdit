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

#include "wwsave.h"

#include <cstring>
#include <string>

WWSave* WWSave::m_pSave = nullptr;

WWSave::WWSave() { }

WWSave* WWSave::Initialize(const char *saveName, bool init) {
	if (m_pSave != nullptr) {
		return m_pSave;
	}

	m_pSave = new WWSave;
	m_pSave->m_saveSize = 0;
	m_pSave->m_saveBuffer = 0;

    FILE* savesFile = fopen(saveName, "rb");
    fseek(savesFile, 0, SEEK_END);
    m_pSave->m_saveSize = ftell(savesFile);
    fseek(savesFile, 0, SEEK_SET);
    m_pSave->m_saveBuffer = new u8[m_pSave->m_saveSize];
    fread(m_pSave->m_saveBuffer, 1, m_pSave->m_saveSize, savesFile);

	if (!init) {
		return m_pSave;
	}

	fclose(savesFile);
	m_pSave->m_saveFile = saveName;
	return m_pSave;
}

WWSave* WWSave::Instance() {
	if (!m_pSave) {
		m_pSave = new WWSave; // TODO: Should this be changed? We don't want an uninitialized save...
	}

	return m_pSave;
}

u64 WWSave::GetSaveSize(void) {
	return m_saveSize;
}

u8* WWSave::GetRawSaveData(void) {
	return m_saveBuffer;
}

void WWSave::Close(void) {
	if (m_pSave != nullptr) {
		delete m_pSave;
		m_pSave = nullptr;
	}
}