/*
MIT License
This file is part of NLTK
Copyright (c) 2018-2019 Slattz, Cuyler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "checksum.hpp"
#include "player.hpp"
#include "save.hpp"

#include <cstring>
#include <string>

#ifdef _3DS
#include "config.hpp"
#include "lang.hpp"
#include "msg.hpp"
#endif

Save* Save::m_pSave = nullptr;

Save::Save() { }

Save::~Save() {
	delete[] m_saveBuffer;
	m_saveBuffer = nullptr;
}

#ifdef _3DS
// For Card or Installed Title reading. Can only be used on 3DS and not DS(i) or so!
Save* Save::InitializeArchive(FS_Archive archive, bool init) {
	if (m_pSave != nullptr) {
		return m_pSave;
	}

	m_pSave = new Save;
	m_pSave->m_archive = archive;
	m_pSave->m_saveSize = 0;
	FS_Path path = fsMakePath(PATH_ASCII, "/garden_plus.dat");

	FSUSER_OpenFile(&m_pSave->m_handle, archive, path, FS_OPEN_READ | FS_OPEN_WRITE, 0);
	FSFILE_GetSize(m_pSave->m_handle, &m_pSave->m_saveSize);
	m_pSave->m_saveBuffer = new u8[m_pSave->m_saveSize];

	FSFILE_Read(m_pSave->m_handle, NULL, 0, m_pSave->m_saveBuffer, m_pSave->m_saveSize);

	m_pSave->m_changesMade = false;

	m_pSave->RegionLock.RawByte = m_pSave->ReadU8(0x621CE);
	m_pSave->RegionLock.DerivedID = m_pSave->RegionLock.RawByte & 0xF;
	m_pSave->RegionLock.RegionID = static_cast<CFG_Region>(m_pSave->RegionLock.RawByte >> 4);

	if (!init) {
		return m_pSave;
	}

	// Load Buildings.
	m_pSave->buildings = std::make_shared<BuildingArray>();

	// Load Players
	for (int i = 0; i < 4; i++) {
		u32 PlayerOffset = 0xA0 + (i * 0xA480);
		m_pSave->players[i] = std::make_shared<Player>(PlayerOffset, i);
	}

	// Load Villagers
	for (int i = 0; i < 10; i++) {
		m_pSave->villagers[i] = std::make_shared<Villager>(0x292D0 + (i * sizeof(Villager::Villager_s)), i);
	}

	m_pSave->town = std::make_shared<Town>();
	m_pSave->shop = std::make_shared<Shop>();

	return m_pSave;
}

bool Save::CommitArchive(bool close) {
	// For the Offset Editor. ;)
	if (Config::getBool("OffsetEditor") != true) {
		// Save Players
		for (int i = 0; i < 4; i++) {
			players[i]->Write();
		}
		// Save Villagers
		for (int i = 0; i < 10; i++) {
			villagers[i]->Write();
		}

		// Save Buildings.
		buildings->Write();

		town->Write();
		shop->Write();
	}
	
	// Update Checksums
	FixCRC32s();

	bool res = R_SUCCEEDED(FSFILE_Write(m_handle, NULL, 0, m_saveBuffer, m_saveSize, FS_WRITE_FLUSH));

	if (res) {
		m_changesMade = false;
	}

	if (close) {
		CloseArchive();
	}

	return res;
}

void Save::CloseArchive(void) {
	if (R_SUCCEEDED(FSFILE_Close(m_handle))) {
		FSUSER_ControlArchive(m_archive, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);
	}

	FSUSER_CloseArchive(m_archive);

	if (m_pSave != nullptr) {
		delete m_pSave;
		m_pSave = nullptr;
	}
}

//https://3dbrew.org/wiki/Title_list/DLC#Region_IDs
u8 Save::DeriveRegionLockID(u8 RegionID, u8 LanguageID) {
	if (RegionID == CFG_REGION_JPN) { //If region is JPN
		return 0;
	}

	else if (RegionID == CFG_REGION_USA) { //If region is USA
		switch (LanguageID) {
			case CFG_LANGUAGE_FR: //If lang is French
				return 3;
			case CFG_LANGUAGE_ES: //If lang is Spanish
				return 2;
			default: //If lang is English & other langs
				return 1;
		}
	}

	else if (RegionID == CFG_REGION_EUR) { //If region is EUR
		switch (LanguageID) {
			case CFG_LANGUAGE_FR: //If lang is French
				return 6;
			case CFG_LANGUAGE_DE: //If lang is German
				return 8;
			case CFG_LANGUAGE_IT: //If lang is Italian
				return 7;
			case CFG_LANGUAGE_ES: //If lang is Spanish
				return 5;
			default:
				return 4;
		}
	}

	else if (RegionID == CFG_REGION_KOR) { //If region is KOR
		return 9;
	}

	return 0;
}

bool Save::UpdateSaveRegion(void) {
	static constexpr u8 ACNLRegionIDs[11] = {0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0xFF};

	u8 SystemLanguage = 0xC; //0xC is ACNL default value || max+1
	u8 SystemRegion = 7; //7 is ACNL default value || max+1
	u8 RegionByte = 0;
	bool ret = false;
	CFGU_SecureInfoGetRegion(&SystemRegion);
	CFGU_GetSystemLanguage(&SystemLanguage);

	u8 DeriveID = DeriveRegionLockID(SystemRegion, SystemLanguage);
	if (DeriveID < 10) SystemRegion = ACNLRegionIDs[DeriveID];
	else SystemRegion = 4;

	RegionByte = (SystemRegion << 4) | (DeriveID & 0xF);

	if (RegionLock.RawByte != RegionByte) {
		ret = true;
		RegionLock.RegionID = static_cast<CFG_Region>(SystemRegion);
		RegionLock.DerivedID = DeriveID;
		RegionLock.RawByte = RegionByte;
	}

	return ret;
}

void Save::FixSaveRegion(void) {
	if (this->UpdateSaveRegion())
	{
		if (Msg::promptMsg(Lang::get("SAVEREGION_NOT_MATCH"))) {
			Write(0x621CE, RegionLock.RawByte);
		}
	}
}

#endif

// For RAW saves.
Save* Save::Initialize(const char *saveName, bool init) {
	if (m_pSave != nullptr) {
		return m_pSave;
	}

	m_pSave = new Save;
	m_pSave->m_saveSize = 0;
	m_pSave->m_saveBuffer = 0;

	FILE* savesFile = fopen(saveName, "rb");
	fseek(savesFile, 0, SEEK_END);
	m_pSave->m_saveSize = ftell(savesFile);
	fseek(savesFile, 0, SEEK_SET);
	m_pSave->m_saveBuffer = new u8[m_pSave->m_saveSize];
	fread(m_pSave->m_saveBuffer, 1, m_pSave->m_saveSize, savesFile);

	m_pSave->m_changesMade = false;
	#ifdef _3DS
	m_pSave->RegionLock.RawByte = m_pSave->ReadU8(0x621CE);
	m_pSave->RegionLock.DerivedID = m_pSave->RegionLock.RawByte & 0xF;
	m_pSave->RegionLock.RegionID = static_cast<CFG_Region>(m_pSave->RegionLock.RawByte >> 4);
	#endif
	if (!init) {
		return m_pSave;
	}

	// Load Buildings.
	m_pSave->buildings = std::make_shared<BuildingArray>();

	// Load Players
	for (int i = 0; i < 4; i++) {
		u32 PlayerOffset = 0xA0 + (i * 0xA480);
		m_pSave->players[i] = std::make_shared<Player>(PlayerOffset, i);
	}

	// Load Villagers
	for (int i = 0; i < 10; i++) {
		m_pSave->villagers[i] = std::make_shared<Villager>(0x292D0 + (i * sizeof(Villager::Villager_s)), i);
	}

	m_pSave->town = std::make_shared<Town>();
	m_pSave->shop = std::make_shared<Shop>();
	
	fclose(savesFile);
	m_pSave->m_saveFile = saveName;
	return m_pSave;
}

Save* Save::Instance() {
	if (!m_pSave) {
		m_pSave = new Save; // TODO: Should this be changed? We don't want an uninitialized save...
	}

	return m_pSave;
}

s8 Save::ReadS8(u32 offset) {
	return (s8) m_saveBuffer[offset];
}

u8 Save::ReadU8(u32 offset) {
	return m_saveBuffer[offset];
}

s16 Save::ReadS16(u32 offset) {
	return *(s16 *)(m_saveBuffer + offset);
}

u16 Save::ReadU16(u32 offset) {
	return *(u16 *)(m_saveBuffer + offset);
}

s32 Save::ReadS32(u32 offset) {
	return *(s32 *)(m_saveBuffer + offset);
}

u32 Save::ReadU32(u32 offset) {
	return *(u32 *)(m_saveBuffer + offset);
}

s64 Save::ReadS64(u32 offset) {
	return *(s64 *)(m_saveBuffer + offset);
}

u64 Save::ReadU64(u32 offset) {
	return *(u64 *)(m_saveBuffer + offset);
}

void Save::ReadArray(u8 *dst, u32 offset, u32 count) {
	u8 *src = m_saveBuffer + offset;
	memcpy(dst, src, count);
}

void Save::ReadArrayU16(u16 *dst, u32 offset, u32 count) {
	u16 *src = (u16 *)(m_saveBuffer + offset);
	memcpy(dst, src, count << 1);
}

std::u16string Save::ReadString(u32 offset, u32 maxSize) {
	return std::u16string(reinterpret_cast<char16_t *>(m_saveBuffer + offset), maxSize + 1);
}

u64 Save::GetSaveSize(void) {
	return m_saveSize;
}

u8* Save::GetRawSaveData(void) {
	return m_saveBuffer;
}

void Save::Close(void) {
	if (m_pSave != nullptr) {
		delete m_pSave;
		m_pSave = nullptr;
	}
}

// Actual Writing Stuff to the Save.

bool Save::Write(u32 offset, u8 *data, u32 count) {
	if (offset + count >= m_saveSize) {
		return false;
	}

	memcpy(m_saveBuffer + offset, data, count);
	m_changesMade = true;
	return true;
}


void Save::Write(u32 offset, s8 data) {
	m_saveBuffer[offset] = (u8)data;
}

void Save::Write(u32 offset, u8 data) {
	m_saveBuffer[offset] = data;
}

void Save::Write(u32 offset, s16 data) {
	*reinterpret_cast<s16*>(m_saveBuffer + offset) = data;
}

void Save::Write(u32 offset, u16 data) {
	*reinterpret_cast<u16*>(m_saveBuffer + offset) = data;
}

void Save::Write(u32 offset, s32 data) {
	*reinterpret_cast<s32*>(m_saveBuffer + offset) = data;
}

void Save::Write(u32 offset, u32 data) {
	*reinterpret_cast<u32*>(m_saveBuffer + offset) = data;
}

void Save::Write(u32 offset, s64 data) {
	*reinterpret_cast<s64*>(m_saveBuffer + offset) = data;
}

void Save::Write(u32 offset, u64 data) {
	*reinterpret_cast<u64*>(m_saveBuffer + offset) = data;
}

bool Save::Write(u32 offset, std::u16string str, u32 maxSize) {
	if (str.length() > maxSize + 1) {
		return false;
	}

	return Write(offset, (u8 *)str.data(), maxSize * 2);
}

bool Save::ChangesMade(void) {
	return m_changesMade;
}

/*
	NOTE: This should be removed at some point. It's a hack to allow the Player encryptedInts to say they've been changed.
*/
void Save::SetChangesMade(bool changesMade) {
	m_changesMade = changesMade;
}

bool Save::Commit(bool close) {
	// For the Offset Editor. ;)
	if (Config::getBool("OffsetEditor") != true) {
		// Save Players
		for (int i = 0; i < 4; i++) {
			players[i]->Write();
		}
		// Save Villagers
		for (int i = 0; i < 10; i++) {
			villagers[i]->Write();
		}
		// Save Buildings.
		buildings->Write();
		town->Write();
		shop->Write();
	}

	// Update Checksums
	FixCRC32s();
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