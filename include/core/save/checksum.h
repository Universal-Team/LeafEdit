#pragma once

#ifndef CHECKSUM_H
#define CHECKSUM_H

typedef enum
{
    CRC_REFLECTED = 0, //Most common in ACNL checksums
    CRC_NORMAL = 1
} ChecksumType;

u32 CalculateCRC32Reflected(u8 *buf, u32 size);
u32 CalculateCRC32Normal(u8 *buf, u32 size);
bool VerifyCRC32(u32 crc, u8 *buf, u32 startOffset, u32 size, ChecksumType type = CRC_REFLECTED);
u32 UpdateCRC32(u32 startOffset, u32 size, ChecksumType type = CRC_REFLECTED);
void FixCRC32s();

#endif