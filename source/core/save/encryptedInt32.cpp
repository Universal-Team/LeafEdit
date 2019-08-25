#include <3ds.h>
#include <random>
#include "core/save/encryptedInt32.h"

EncryptedInt32::EncryptedInt32() { }

EncryptedInt32::EncryptedInt32(u32 unencryptedValue) {
    value = unencryptedValue;
}

EncryptedInt32::EncryptedInt32(u32 intLow, u32 intHigh) {
    decrypt(intLow, intHigh);
}

EncryptedInt32::EncryptedInt32(u64 encryptedInt)
    : EncryptedInt32(static_cast<u32>(encryptedInt & 0xFFFFFFFF), static_cast<u32>((encryptedInt >> 32) & 0xFFFFFFFF)) { }

u8 EncryptedInt32::calculateChecksum(u32 intLow) {
    return static_cast<u8>(((intLow & 0xFF) + ((intLow >> 8) & 0xFF) + ((intLow >> 16) & 0xFF) + ((intLow >> 24) & 0xFF) + 0xBA) & 0xFF);
}

u32 EncryptedInt32::decrypt(u64 encryptedInt) {
    return EncryptedInt32::decrypt(static_cast<u32>(encryptedInt & 0xFFFFFFFF), static_cast<u32>((encryptedInt >> 32) & 0xFFFFFFFF));
}

u32 EncryptedInt32::decrypt(u32 intLow, u32 intHigh) {
    u16 adjustValue = static_cast<u16>(intHigh & 0xFFFF);
    u8 shiftValue = static_cast<u8>((intHigh >> 16) & 0xFF);
    u8 checksum = static_cast<u8>((intHigh >> 24) & 0xFF);

    u8 calculatedChecksum = calculateChecksum(intLow);

    if (calculatedChecksum != checksum) {
        // TODO: Log
        printf("\x1b[29;4HInvalid encrypted int checksum! Expected: %02X | Got: %02X", calculatedChecksum, checksum);
        return 0;
    }

    u8 leftShiftCount = static_cast<u8>(0x1C - shiftValue);
    u8 rightShiftCount = static_cast<u8>(0x20 - leftShiftCount);

    value = (((intLow << leftShiftCount) + (intLow >> rightShiftCount)) - (adjustValue + 0x8F187432));
    return value;
}

void EncryptedInt32::encrypt(u32 &encryptedIntOut, u32 &encryptionDataOut) {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 0x10000 - 1);
    std::uniform_int_distribution<std::mt19937::result_type> dist2(0, 0x1A - 1);

    u16 adjustValue = static_cast<u16>(dist(rng) & 0xFFFF);
    u8 shiftValue = static_cast<u8>(dist(rng) & 0x19);

    u32 encryptedInt = (value + adjustValue + 0x8F187432);
    encryptedInt = (encryptedInt >> (0x1C - shiftValue)) + (encryptedInt << (shiftValue + 4));
    u32 encryptionData = (calculateChecksum(encryptedInt) << 24) | (shiftValue << 16) | adjustValue;

    printf("\x1b[27;4HEncrypted Int value: %08X", static_cast<unsigned int>(encryptedInt));
    printf("\x1b[28;4HEncrypted Data value: %08X", static_cast<unsigned int>(encryptionData));

    encryptedIntOut = encryptedInt;
    encryptionDataOut = encryptionData;
}