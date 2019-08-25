#include "common/archive.hpp"

#include "core/save/checksum.h"
#include "core/save/player.h"
#include "core/save/save.h"

#include <3ds.h>
#include <cstring>
#include <string>


Save* Save::m_pSave = nullptr;

Save::Save() { }

Save::~Save() {
    for (auto player : players) {  
        delete player;
        player = nullptr;
    }

    delete[] m_saveBuffer;
    m_saveBuffer = nullptr;
}

Save* Save::Initialize(const char *saveName, bool init) {
    if (m_pSave != nullptr) {
        return m_pSave;
    }

    m_pSave = new Save;

    m_pSave->m_saveSize = 0;
    FS_Path path = fsMakePath(PATH_ASCII, saveName);

    FSUSER_OpenFile(&m_pSave->m_handle, Archive::sdmc(), path, FS_OPEN_READ | FS_OPEN_WRITE, 0);
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

    // Load Players
    for (int i = 0; i < 4; i++) {
        u32 PlayerOffset = 0xA0 + (i * 0xA480);
        m_pSave->players[i] = new Player(PlayerOffset, i);
    }

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