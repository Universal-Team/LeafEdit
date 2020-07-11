/*
*   This file is part of LeafEdit-Core
*   Copyright (C) 2020 Universal-Team
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

#include "checksum.hpp"
#include "saveUtils.hpp"
#include "types.hpp"

#include <cstring>
#include <string>

const u32 crcTable_1[256] = { //Polynomial: 0x1EDC6F41, 0xFFFFFFFF initial value, 0xFFFFFFFF xor, Input Reflection, Output Reflection
	0x00000000, 0xF26B8303, 0xE13B70F7, 0x1350F3F4, 0xC79A971F,
	0x35F1141C, 0x26A1E7E8, 0xD4CA64EB, 0x8AD958CF, 0x78B2DBCC,
	0x6BE22838, 0x9989AB3B, 0x4D43CFD0, 0xBF284CD3, 0xAC78BF27,
	0x5E133C24, 0x105EC76F, 0xE235446C, 0xF165B798, 0x30E349B,
	0xD7C45070, 0x25AFD373, 0x36FF2087, 0xC494A384, 0x9A879FA0,
	0x68EC1CA3, 0x7BBCEF57, 0x89D76C54, 0x5D1D08BF, 0xAF768BBC,
	0xBC267848, 0x4E4DFB4B, 0x20BD8EDE, 0xD2D60DDD, 0xC186FE29,
	0x33ED7D2A, 0xE72719C1, 0x154C9AC2, 0x061C6936, 0xF477EA35,
	0xAA64D611, 0x580F5512, 0x4B5FA6E6, 0xB93425E5, 0x6DFE410E,
	0x9F95C20D, 0x8CC531F9, 0x7EAEB2FA, 0x30E349B1, 0xC288CAB2,
	0xD1D83946, 0x23B3BA45, 0xF779DEAE, 0x05125DAD, 0x1642AE59,
	0xE4292D5A, 0xBA3A117E, 0x4851927D, 0x5B016189, 0xA96AE28A,
	0x7DA08661, 0x8FCB0562, 0x9C9BF696, 0x6EF07595, 0x417B1DBC,
	0xB3109EBF, 0xA0406D4B, 0x522BEE48, 0x86E18AA3, 0x748A09A0,
	0x67DAFA54, 0x95B17957, 0xCBA24573, 0x39C9C670, 0x2A993584,
	0xD8F2B687, 0x0C38D26C, 0xFE53516F, 0xED03A29B, 0x1F682198,
	0x5125DAD3, 0xA34E59D0, 0xB01EAA24, 0x42752927, 0x96BF4DCC,
	0x64D4CECF, 0x77843D3B, 0x85EFBE38, 0xDBFC821C, 0x2997011F,
	0x3AC7F2EB, 0xC8AC71E8, 0x1C661503, 0xEE0D9600, 0xFD5D65F4,
	0x0F36E6F7, 0x61C69362, 0x93AD1061, 0x80FDE395, 0x72966096,
	0xA65C047D, 0x5437877E, 0x4767748A, 0xB50CF789, 0xEB1FCBAD,
	0x197448AE, 0x0A24BB5A, 0xF84F3859, 0x2C855CB2, 0xDEEEDFB1,
	0xCDBE2C45, 0x3FD5AF46, 0x7198540D, 0x83F3D70E, 0x90A324FA,
	0x62C8A7F9, 0xB602C312, 0x44694011, 0x5739B3E5, 0xA55230E6,
	0xFB410CC2, 0x092A8FC1, 0x1A7A7C35, 0xE811FF36, 0x3CDB9BDD,
	0xCEB018DE, 0xDDE0EB2A, 0x2F8B6829, 0x82F63B78, 0x709DB87B,
	0x63CD4B8F, 0x91A6C88C, 0x456CAC67, 0xB7072F64, 0xA457DC90,
	0x563C5F93, 0x082F63B7, 0xFA44E0B4, 0xE9141340, 0x1B7F9043,
	0xCFB5F4A8, 0x3DDE77AB, 0x2E8E845F, 0xDCE5075C, 0x92A8FC17,
	0x60C37F14, 0x73938CE0, 0x81F80FE3, 0x55326B08, 0xA759E80B,
	0xB4091BFF, 0x466298FC, 0x1871A4D8, 0xEA1A27DB, 0xF94AD42F,
	0x0B21572C, 0xDFEB33C7, 0x2D80B0C4, 0x3ED04330, 0xCCBBC033,
	0xA24BB5A6, 0x502036A5, 0x4370C551, 0xB11B4652, 0x65D122B9,
	0x97BAA1BA, 0x84EA524E, 0x7681D14D, 0x2892ED69, 0xDAF96E6A,
	0xC9A99D9E, 0x3BC21E9D, 0xEF087A76, 0x1D63F975, 0x0E330A81,
	0xFC588982, 0xB21572C9, 0x407EF1CA, 0x532E023E, 0xA145813D,
	0x758FE5D6, 0x87E466D5, 0x94B49521, 0x66DF1622, 0x38CC2A06,
	0xCAA7A905, 0xD9F75AF1, 0x2B9CD9F2, 0xFF56BD19, 0x0D3D3E1A,
	0x1E6DCDEE, 0xEC064EED, 0xC38D26C4, 0x31E6A5C7, 0x22B65633,
	0xD0DDD530, 0x0417B1DB, 0xF67C32D8, 0xE52CC12C, 0x1747422F,
	0x49547E0B, 0xBB3FFD08, 0xA86F0EFC, 0x5A048DFF, 0x8ECEE914,
	0x7CA56A17, 0x6FF599E3, 0x9D9E1AE0, 0xD3D3E1AB, 0x21B862A8,
	0x32E8915C, 0xC083125F, 0x144976B4, 0xE622F5B7, 0xF5720643,
	0x07198540, 0x590AB964, 0xAB613A67, 0xB831C993, 0x4A5A4A90,
	0x9E902E7B, 0x6CFBAD78, 0x7FAB5E8C, 0x8DC0DD8F, 0xE330A81A,
	0x115B2B19, 0x020BD8ED, 0xF0605BEE, 0x24AA3F05, 0xD6C1BC06,
	0xC5914FF2, 0x37FACCF1, 0x69E9F0D5, 0x9B8273D6, 0x88D28022,
	0x7AB90321, 0xAE7367CA, 0x5C18E4C9, 0x4F48173D, 0xBD23943E,
	0xF36E6F75, 0x0105EC76, 0x12551F82, 0xE03E9C81, 0x34F4F86A,
	0xC69F7B69, 0xD5CF889D, 0x27A40B9E, 0x79B737BA, 0x8BDCB4B9,
	0x988C474D, 0x6AE7C44E, 0xBE2DA0A5, 0x4C4623A6, 0x5F16D052,
	0xAD7D5351
};

const u32 crcTable_2[256] = { //Polynomial: 0x04C11DB7, 0x00000000 initial value, 0xFFFFFFFF xor, No Input Reflection, No Output Reflection
	0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC,
	0x17C56B6B, 0x1A864DB2, 0x1E475005, 0x2608EDB8, 0x22C9F00F,
	0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A,
	0x384FBDBD, 0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9,
	0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75, 0x6A1936C8,
	0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3,
	0x709F7B7A, 0x745E66CD, 0x9823B6E0, 0x9CE2AB57, 0x91A18D8E,
	0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
	0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84,
	0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D, 0xD4326D90, 0xD0F37027,
	0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022,
	0xCA753D95, 0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1,
	0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D, 0x34867077,
	0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C,
	0x2E003DC5, 0x2AC12072, 0x128E9DCF, 0x164F8078, 0x1B0CA6A1,
	0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
	0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB,
	0x6F52C06C, 0x6211E6B5, 0x66D0FB02, 0x5E9F46BF, 0x5A5E5B08,
	0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D,
	0x40D816BA, 0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E,
	0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692, 0x8AAD2B2F,
	0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044,
	0x902B669D, 0x94EA7B2A, 0xE0B41DE7, 0xE4750050, 0xE9362689,
	0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
	0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683,
	0xD1799B34, 0xDC3ABDED, 0xD8FBA05A, 0x690CE0EE, 0x6DCDFD59,
	0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C,
	0x774BB0EB, 0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F,
	0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53, 0x251D3B9E,
	0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5,
	0x3F9B762C, 0x3B5A6B9B, 0x0315D626, 0x07D4CB91, 0x0A97ED48,
	0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
	0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2,
	0xE6EA3D65, 0xEBA91BBC, 0xEF68060B, 0xD727BBB6, 0xD3E6A601,
	0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604,
	0xC960EBB3, 0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7,
	0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B, 0x9B3660C6,
	0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD,
	0x81B02D74, 0x857130C3, 0x5D8A9099, 0x594B8D2E, 0x5408ABF7,
	0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
	0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD,
	0x6C47164A, 0x61043093, 0x65C52D24, 0x119B4BE9, 0x155A565E,
	0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B,
	0x0FDC1BEC, 0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088,
	0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654, 0xC5A92679,
	0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12,
	0xDF2F6BCB, 0xDBEE767C, 0xE3A1CBC1, 0xE760D676, 0xEA23F0AF,
	0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
	0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5,
	0x9E7D9662, 0x933EB0BB, 0x97FFAD0C, 0xAFB010B1, 0xAB710D06,
	0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03,
	0xB1F740B4
};

// Calculate the CRC32 reflected type.
u32 Checksum::CalculateCRC32Reflected(u8 *buf, u32 size) {
	u32 crc = 0xFFFFFFFF;
	while (size-- != 0) {
		crc = crcTable_1[(crc ^ *buf) & 0xFF] ^ (crc >> 8);
		buf++;
	}

	return ~crc;
}

// Calculate the CRC32 normal type.
u32 Checksum::CalculateCRC32Normal(u8 *buf, u32 size) {
	u32 crc = 0;
	while (size-- != 0) {
		crc = crcTable_2[((crc >> 24) ^ *buf) & 0xFF] ^ (crc << 8);
		buf++;
	}

	return ~crc;
}

// Verify the CRC32.
bool Checksum::VerifyCRC32(u32 crc, u8 *buf, u32 startOffset, u32 size, ChecksumType type) {
	if (type == CRC_NORMAL) {
		return CalculateCRC32Normal(buf + startOffset + 4, size) == crc;
	}

	return CalculateCRC32Reflected(buf + startOffset + 4, size) == crc;
}

// Update the CRC32.
u32 Checksum::UpdateCRC32(u8 *rawData, u32 startOffset, u32 size, ChecksumType type) {
	u32 crc32 = 0;
	if (type == CRC_NORMAL) {
		crc32 = CalculateCRC32Normal(rawData + startOffset + 4, size);
	} else {
		crc32 = CalculateCRC32Reflected(rawData + startOffset + 4, size);
	}

	SaveUtils::Write<u32>(rawData, startOffset, crc32); // write calculated crc32
	return crc32;
}

// Fix the CRC32 for AC:WA.
void Checksum::FixCRC32s(u8 *data) {
	UpdateCRC32(data, 0x80, 0x1C); // Save Header

	// Rehash players
	for (int i = 0; i < 4; i++) {
		UpdateCRC32(data, 0xA0 + (0xA480 * i), 0x6B84);				// Players Checksum1
		UpdateCRC32(data, 0xA0 + (0xA480 * i) + 0x6B88, 0x38F4);	// Players Checksum2
	}

	UpdateCRC32(data, 0x292A0, 0x22BC8);	// VillagerData Checksum
	UpdateCRC32(data, 0x4BE80, 0x44B8);		// GeneralTownData Checksum
	UpdateCRC32(data, 0x53424, 0x1E4D8);	// ItemAndAcreData Checksum
	UpdateCRC32(data, 0x71900, 0x20);		// Unknown1 Checksum
	UpdateCRC32(data, 0x71924, 0xBE4);		// Unknown2 Checksum
	UpdateCRC32(data, 0x73954, 0x16188);	// LetterStorage Checksum

	UpdateCRC32(data, 0x5033C, 0x28F0, CRC_NORMAL);	// Unknown3 Checksum
	UpdateCRC32(data, 0x52C30, 0x7F0, CRC_NORMAL);	// Unknown4 Checksum
	UpdateCRC32(data, 0x7250C, 0x1444, CRC_NORMAL);	// Unknown5 Checksum
}

// Fix the CRC32 for AC:NL.
void Checksum::FixNLCRC32s(u8 *data) {
	UpdateCRC32(data, 0x80, 0x1C); // Save Header
	// Rehash players
	for (int i = 0; i < 4; i++) {
		UpdateCRC32(data, 0xA0 + ((0x9E90 + 0x80) * i), 0x6B64);			// Players Checksum1
		UpdateCRC32(data, 0xA0 + ((0x9E90 + 0x80) * i) + 0x6B68, 0x33A4);	// Players Checksum2
	}

	UpdateCRC32(data, 0x27C60 + 0x80, 0x218B0);	// VillagerData Checksum
	UpdateCRC32(data, 0x49520 + 0x80, 0x44B8);	// GeneralTownData Checksum
	UpdateCRC32(data, 0x4D9DC + 0x80, 0x1E420);	// ItemAndAcreData Checksum
	UpdateCRC32(data, 0x6BE00 + 0x80, 0x20);	// Unknown1 Checksum
	UpdateCRC32(data, 0x6BE24 + 0x80, 0x13AF8);	// ?
}

// Wild World.
// Calculate AC:WW's Checksum.
u16 Checksum::CalculateWW(const u16 *buffer, u64 size, uint checksumOffset) {
	if ((checksumOffset & 1) == 1)	return 0; // checksumOffset must be 16-bit aligned!

	u16 checksum = 0;
	for (uint i = 0; i < size; i++) {
		if (i == checksumOffset) continue;
		checksum += buffer[i];
	}

	return (u16) -checksum;
}

// Verify AC:WW's Checksum.
bool Checksum::VerifyWW(const u16 *buffer, u64 size, u16 currentChecksum, uint checksumOffset) {
	if (CalculateWW(buffer, size, checksumOffset) == currentChecksum) return true;
	else	return false;
}

// Update AC:WW's Checksum.
void Checksum::UpdateWWChecksum(WWRegion region, u8 *saveBuffer, u16 *buffer, u64 size) {
	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			SaveUtils::Write<u16>(saveBuffer, 0x15FDC, CalculateWW(buffer, size, 0xAFEE));
			break;
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			SaveUtils::Write<u16>(saveBuffer, 0x12220, CalculateWW(buffer, size, 0x9110));
			break;
		case WWRegion::KOR_REV1:
			SaveUtils::Write<u16>(saveBuffer, 0x173F8, CalculateWW(buffer, size, 0xB9FC));
			break;
		case WWRegion::UNKNOWN:
			break;
	}
}