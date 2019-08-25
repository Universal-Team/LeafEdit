#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "core/save/encryptedInt32.h"
#include "core/save/save.h"

class Player {
public:
    Player(void);
    ~Player(void);
    Player(u32 offset, u32 index);

    std::u16string Name;
    std::u16string TownName;

    EncryptedInt32 Wallet;
    EncryptedInt32 BankAmount;

    bool Exists();

    u32 m_offset;
    u32 m_index;
};

#endif
