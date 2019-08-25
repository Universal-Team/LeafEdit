#pragma once

#ifndef ENCRYPTEDINT32
#define ENCRYPTEDINT32

class EncryptedInt32 {
public:
    EncryptedInt32();
    EncryptedInt32(u32 value);
    EncryptedInt32(u32 intLow, u32 intHigh);
    EncryptedInt32(u64 encryptedInt);

    u32 value;
    void encrypt(u32 &encryptedIntOut, u32 &encryptionDataOut);
    u32 decrypt(u32 intLow, u32 intHigh);
    u32 decrypt(u64 encryptedInt);

private:
    u8 calculateChecksum(u32 intHigh);
};

#endif