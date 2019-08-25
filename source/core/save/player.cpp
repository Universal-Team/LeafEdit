
#include <3ds.h>
#include <string>
#include "core/save/player.h"


Player::Player() { }

Player::~Player()
{
}

Player::Player(u32 offset, u32 index) {
    this->m_offset = offset;
    this->m_index = index;

    this->Name = Save::Instance()->ReadString(offset + 0x55A8, 8);
    this->TownName = Save::Instance()->ReadString(offset + 0x55BE, 8);

    this->Wallet = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6F08));
    this->BankAmount = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6b8c));
}

bool Player::Exists() {
    return Save::Instance()->ReadU16(this->m_offset + 0x55A6) != 0;
}