
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

    this->PlayerId = Save::Instance()->ReadU16(offset + 0x55A6);
    this->Name = Save::Instance()->ReadString(offset + 0x55A8, 8);
    this->TownName = Save::Instance()->ReadString(offset + 0x55BE, 8);
    this->Gender = Save::Instance()->ReadU16(offset + 0x55BA);
    this->TownId = Save::Instance()->ReadU16(offset + 0x55BC);

    this->WalletOld = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6e38)); // Old Ver.
    this->Wallet = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6F08)); // Welcome Amiibo.

    this->BankAmountOld = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6b6c)); // Old Ver.
    this->BankAmount = EncryptedInt32(Save::Instance()->ReadU64(offset + 0x6b8c)); // Welcome Amiibo.
}

void Player::Write() {
    u32 encryptedInt = 0;
    u32 encryptionData = 0;

    Save::Instance()->Write(this->m_offset + 0x55A6, this->PlayerId);
    Save::Instance()->Write(this->m_offset + 0x55A8, this->Name, 8);
    Save::Instance()->Write(this->m_offset + 0x55BA, this->Gender);
    Save::Instance()->Write(this->m_offset + 0x55BC, this->TownId);
    Save::Instance()->Write(this->m_offset + 0x55BE, this->TownName, 8);

    this->Wallet.encrypt(encryptedInt, encryptionData);
    Save::Instance()->Write(this->m_offset + 0x6F08, encryptedInt);
    Save::Instance()->Write(this->m_offset + 0x6F0C, encryptionData);
}

bool Player::Exists() {
    return Save::Instance()->ReadU16(this->m_offset + 0x55A6) != 0;
}