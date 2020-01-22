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

#include "wwsave.hpp"
#include "wwChecksum.hpp"

#include <cstring>
#include <string>
#include <vector>

WWSave* WWSave::m_pSave = nullptr;

WWSave::WWSave() { }

WWSave::~WWSave() {
	for (auto player : players) {
		delete player;
		player = nullptr;
	}

	delete[] m_saveBuffer;
	m_saveBuffer = nullptr;
}

std::vector<std::string> WwCharacterDictionary = {
    "\0", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
    "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e",
    "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
    "v", "w", "x", "y", "z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "⨍",
    "s̊", "Œ", "Ž", "š", "œ", "ž", "Ÿ", "À", "Á", "Â", "Ã", "Ä", "Å", "Æ", "Ç", "È",
    "É", "Ê", "Ë", "Ì", "Í", "Î", "Ï", "Đ", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö", "Ø", "Ù",
    "Ú", "Û", "Ü", "Ý", "Þ", "ß", "à", "á", "â", "ã", "ä", "å", "æ", "ç", "è", "é",
    "ê", "ë", "ì", "í", "î", "ï", "ð", "ñ", "ò", "ó", "ô", "õ", "ö", "ø", "ù", "ú",
    "û", "ü", "ý", "þ", "ÿ", " ", "\n", "!", "“", "#", "$", "%", "&", "´", "(", ")",
    "*", "+", ",", "-", ".", "/", ":", ";", "<", "=", ">", "?", "@", "[", "{", "]",
    "|", "_", "}", "、", "˷", "…", "~", "£", "†", "‡", "^", "‰", "⟨", "`", "”", "•",
    "‒", "'", "—", "\"", "™", "⟩", " ", "˜", "¥", "╎", "§", "¡", "¢", "£", "¨", "©",
    "ª", "«", "¬", "–", "®", "°", "±", "²", "³", "‾", "ˢ", "µ", "¶", "→", "¹", "º",
    "»", "･", "¼", "½", "¾", "", "", "", "", "¿", "×", "÷", "💧", "★", "❤", "♪",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
};

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

	// Load Players.
	for (int i = 0; i < 4; i++) {
		u32 PlayerOffset = 0x000C + (i * 0x228C);
		m_pSave->players[i] = new WWPlayer(PlayerOffset, i);
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

s8 WWSave::ReadS8(u32 offset) {
	return (s8) m_saveBuffer[offset];
}

u8 WWSave::ReadU8(u32 offset) {
	return m_saveBuffer[offset];
}

s16 WWSave::ReadS16(u32 offset) {
	return *(s16 *)(m_saveBuffer + offset);
}

u16 WWSave::ReadU16(u32 offset) {
	return *(u16 *)(m_saveBuffer + offset);
}

s32 WWSave::ReadS32(u32 offset) {
	return *(s32 *)(m_saveBuffer + offset);
}

u32 WWSave::ReadU32(u32 offset) {
	return *(u32 *)(m_saveBuffer + offset);
}

s64 WWSave::ReadS64(u32 offset) {
	return *(s64 *)(m_saveBuffer + offset);
}

u64 WWSave::ReadU64(u32 offset) {
	return *(u64 *)(m_saveBuffer + offset);
}

void WWSave::ReadArray(u8 *dst, u32 offset, u32 count) {
	u8 *src = m_saveBuffer + offset;
	memcpy(dst, src, count);
}

void WWSave::ReadArrayU16(u16 *dst, u32 offset, u32 count) {
	u16 *src = (u16 *)(m_saveBuffer + offset);
	memcpy(dst, src, count << 1);
}

std::u16string WWSave::ReadString(u32 offset, u32 maxSize) {
	return std::u16string(reinterpret_cast<char16_t *>(m_saveBuffer + offset), maxSize + 1);
}

// Actual Writing Stuff to the Save.

bool WWSave::Write(u32 offset, u8 *data, u32 count) {
	if (offset + count >= m_saveSize) {
		return false;
	}

	memcpy(m_saveBuffer + offset, data, count);
	m_changesMade = true;
	return true;
}

bool WWSave::Write(u32 offset, s8 data) {
	return Write(offset, (u8 *)&data, 1);
}

bool WWSave::Write(u32 offset, u8 data) {
	return Write(offset, &data, 1);
}

bool WWSave::Write(u32 offset, s16 data) {
	return Write(offset, (u8 *)&data, 2);
}

bool WWSave::Write(u32 offset, u16 data) {
	return Write(offset, (u8 *)&data, 2);
}

bool WWSave::Write(u32 offset, s32 data) {
	return Write(offset, (u8 *)&data, 4);
}

bool WWSave::Write(u32 offset, u32 data) {
	return Write(offset, (u8 *)&data, 4);
}

bool WWSave::Write(u32 offset, s64 data) {
	return Write(offset, (u8 *)&data, 8);
}

bool WWSave::Write(u32 offset, u64 data) {
	return Write(offset, (u8 *)&data, 8);
}

bool WWSave::Write(u32 offset, std::u16string str, u32 maxSize) {
	if (str.length() > maxSize + 1) {
		return false;
	}

	return Write(offset, (u8 *)str.data(), maxSize * 2);
}

bool WWSave::ChangesMade(void) {
	return m_changesMade;
}

/*
	NOTE: This should be removed at some point. It's a hack to allow the Player encryptedInts to say they've been changed.
*/
void WWSave::SetChangesMade(bool changesMade) {
	m_changesMade = changesMade;
}

bool WWSave::Commit(bool close) {

	// Save Player.
	for (int i = 0; i < 4; i++) {
		players[i]->Write();
	}
	// Update checksums.
	WWChecksum::UpdateChecksum();
	FILE *saveFile = fopen(m_saveFile, "rb+");
	bool res = R_SUCCEEDED(fwrite(m_saveBuffer, 1, m_saveSize, saveFile));

	if (res) {
		m_changesMade = false;
	}

	if (close) {
		Close();
	}

	fclose(saveFile);
	return res;
}