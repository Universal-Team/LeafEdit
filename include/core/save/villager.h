#pragma once

#ifndef VILLAGER_H
#define VILLAGER_H

#include <3ds.h>
#include <string>
#include "encryptedInt32.h"
#include "save.h"

class Villager {
public:
    Villager(void);
    ~Villager(void);
    Villager(const u32 offset, const u32 index);

    enum Personality : u8 {
        Lazy, Jock, Cranky, Smug, Normal, Peppy, Snooty, Uchi, NotSet
    };

    #pragma pack(push, 1)
    struct Villager_s {
        u16 Id; // 0 - 1 (2)
        Villager::Personality Personality; // 2 (1)
        u8 Unknown1[0x246B]; // 3 - 246D
        u32 Unknown2; // 24C2 - 24C5
        char16_t Catchphrase[11]; // Last character is null terminator | 24C6 - 24DB
        u8 Unknown3[8]; // 24DC - 24E3
        u8 Status; // Also general flags? | 24E4
        u8 Unknown4[9]; // 24E5 - 24ED
        char16_t TownName[9]; // Last character is null terminator | 24EE - 24FF
        u8 Unknown5[0x18]; // 2500 - 2517
    };
    #pragma pack(pop)

    u32 GetOffset(void) const;
    u32 GetIndex(void) const;

    u16 GetId(void) const;
    void SetId(const u16);

    void Write(void);

private:
    Villager_s* m_villagerData;

    u32 m_offset;
    u32 m_index;
};

#endif