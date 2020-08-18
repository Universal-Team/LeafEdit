/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
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

#include "common.hpp" // For the common Headers.
#include "coreUtils.hpp"
#include "jpeg.hpp"
#include "Sav.hpp"
#include "saveUtils.hpp"
#include "utils.hpp"

#include <algorithm> // For std::max.
#include <ctime> // For the time string.
#include <dirent.h> // For mkdir.

extern std::shared_ptr<Sav> save;
extern std::unique_ptr<Config> config;

// Fix Invalid Buildings and replace them with "Empty".
void CoreUtils::FixInvalidBuildings(void) {
	if (save != nullptr) { // Make sure save is not nullpointer.
		u32 offset = 0;
		bool ask = false;
		if (save->getType() == SaveType::WA || save->getType() == SaveType::NL) {
			if (save->getType() == SaveType::NL) {
				offset = 0x80+0x049528;
			} else if (save->getType() == SaveType::WA) {
				offset = 0x04be88;
			}

			for (int i = 0; i < 58; i++) {
				u8 building = save->savePointer()[(offset + (i * 4))]; // Get building IDs.
				if ((building >= 0x12 && building <= 0x4B) || building > 0xFC) {
					if (!ask) {
						ask = true;
						if (!Msg::promptMsg(Lang::get("INVALID_BUILDINGS"))) {
							return;
						}
					}

					SaveUtils::Write<u32>(save->savePointer(), offset + (i * 4), static_cast<u32>(0x000000FC)); // Write empty building.
				}
			}
		}
	}
}

//https://3dbrew.org/wiki/Title_list/DLC#Region_IDs
u8 CoreUtils::DeriveRegionLockID(u8 RegionID, u8 LanguageID) {
	if (RegionID == CFG_REGION_JPN) { // If region is JPN
		return 0;

	} else if (RegionID == CFG_REGION_USA) { // If region is USA
		switch (LanguageID) {
			case CFG_LANGUAGE_FR: // If lang is French
				return 3;
			case CFG_LANGUAGE_ES: // If lang is Spanish
				return 2;
			default: // If lang is English & other langs
				return 1;
		}

	} else if (RegionID == CFG_REGION_EUR) { // If region is EUR
		switch (LanguageID) {
			case CFG_LANGUAGE_FR: // If lang is French
				return 6;
			case CFG_LANGUAGE_DE: // If lang is German
				return 8;
			case CFG_LANGUAGE_IT: // If lang is Italian
				return 7;
			case CFG_LANGUAGE_ES: // If lang is Spanish
				return 5;
			default:
				return 4;
		}

	} else if (RegionID == CFG_REGION_KOR) { // If region is KOR
		return 9;
	}

	return 0;
}

// Update the Save Region.
bool CoreUtils::UpdateSaveRegion(Region_Lock &regionLock) {
	static constexpr u8 ACNLRegionIDs[11] = {0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0xFF};

	u8 SystemLanguage = 0xC; // 0xC is ACNL default value || max+1
	u8 SystemRegion = 7; // 7 is ACNL default value || max+1
	u8 RegionByte = 0;
	bool ret = false;
	CFGU_SecureInfoGetRegion(&SystemRegion);
	CFGU_GetSystemLanguage(&SystemLanguage);

	u8 DeriveID = DeriveRegionLockID(SystemRegion, SystemLanguage);
	if (DeriveID < 10) SystemRegion = ACNLRegionIDs[DeriveID];
	else SystemRegion = 4;

	RegionByte = (SystemRegion << 4) | (DeriveID & 0xF);

	if (regionLock.RawByte != RegionByte) {
		ret = true;
		regionLock.RegionID = static_cast<CFG_Region>(SystemRegion);
		regionLock.DerivedID = DeriveID;
		regionLock.RawByte = RegionByte;
	}

	return ret;
}

// Fix the Save Region.
void CoreUtils::FixSaveRegion(Region_Lock &regionLock) {
	if (save != nullptr) {
		if (save->getType() == SaveType::WA) {
			if (UpdateSaveRegion(regionLock)) {
				if (Msg::promptMsg(Lang::get("REGION_NOT_MATCH"))) {
					save->savePointer()[0x621CE] = regionLock.RawByte;
				}
			}
		}
	}
}

/*
C2D_Image CoreUtils::LoadPlayerTPC(std::shared_ptr<Player> player) {
	if (player != nullptr || player->tpcImage() != nullptr) {
		if (player->hasTPC()) {
			// Load.
			return LoadPlayerPicture(player->tpcImage());
		} else {
			return {nullptr};
		}
	} else {
		return {nullptr};
	}
}*/

void CoreUtils::createBackup() {
	// Make sure save is not nullpointer.
	if (save != nullptr) {
		if (config->createBackups()) {
			std::string fileName;
			switch(save->getType()) {
				case SaveType::WW:
					fileName = "ACWW.sav";
					break;
				case SaveType::NL:
					fileName = "garden.dat";
					break;
				case SaveType::WA:
					fileName = "garden_plus.dat";
					break;
				case SaveType::UNUSED:
					fileName = "?";
					break;
			}

			if (fileName != "?") {
				Msg::DisplayMsg(Lang::get("CREATING_BACKUP"));
				char stringTime[15] = {0};
				time_t unixTime = time(NULL);
				struct tm* timeStruct = gmtime((const time_t*)&unixTime);
				std::strftime(stringTime, 14, "%Y%m%d%H%M%S", timeStruct);
				std::string path = "/3ds/LeafEdit/Backups/" + std::string(stringTime);
				mkdir(path.c_str(), 0777); // Create folder.
				path += "/" + fileName;

				FILE *file = fopen(path.c_str(), "w");
				fwrite(save->rawData().get(), 1, save->getLength(), file);
				fclose(file);

				char message[200];
				snprintf(message, sizeof(message), Lang::get("BACKUP_RESULT").c_str(), path.c_str());
				Msg::DisplayWaitMsg(message);
			}
		}
	}
}

// All Palettes.
/* Color format seems to be: RGBA. */
static const u32 NLPaletteColors[] = {
	0xFFEEFFFF, 0xFF99AAFF, 0xEE5599FF, 0xFF66AAFF, 0xFF0066FF, 0xBB4477FF, 0xCC0055FF, 0x990033FF, 0x552233FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFBBCCFF, 0xFF7777FF, 0xDD3311FF, 0xFF5544FF, 0xFF0000FF, 0xCC6666FF, 0xBB4444FF, 0xBB0000FF, 0x882222FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xEEEEEEFF,
	0xDDCCBBFF, 0xFFCC66FF, 0xDD6622FF, 0xFFAA22FF, 0xFF6600FF, 0xBB8855FF, 0xDD4400FF, 0xBB4400FF, 0x663311FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xDDDDDDFF,
	0xFFEEDDFF, 0xFFDDCCFF, 0xFFCCAAFF, 0xFFBB88FF, 0xFFAA88FF, 0xDD8866FF, 0xBB6644FF, 0x995533FF, 0x884422FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xCCCCCCFF,
	0xFFCCFFFF, 0xEE88FFFF, 0xCC66DDFF, 0xBB88CCFF, 0xCC00FFFF, 0x996699FF, 0x8800AAFF, 0x550077FF, 0x330044FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xBBBBBBFF,
	0xFFBBFFFF, 0xFF99FFFF, 0xDD22BBFF, 0xFF55EEFF, 0xFF00CCFF, 0x885577FF, 0xBB0099FF, 0x880066FF, 0x550044FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xAAAAAAFF,
	0xDDBB99FF, 0xCCAA77FF, 0x774433FF, 0xAA7744FF, 0x993300FF, 0x773322FF, 0x552200FF, 0x331100FF, 0x221100FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x999999FF,
	0xFFFFCCFF, 0xFFFF77FF, 0xDDDD22FF, 0xFFFF00FF, 0xFFDD00FF, 0xCCAA00FF, 0x999900FF, 0x887700FF, 0x555500FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x888888FF,
	0xDDBBFFFF, 0xBB99EEFF, 0x6633CCFF, 0x9955FFFF, 0x6600FFFF, 0x554488FF, 0x440099FF, 0x220066FF, 0x221133FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x777777FF,
	0xBBBBFFFF, 0x8899FFFF, 0x3333AAFF, 0x3355EEFF, 0x0000FFFF, 0x333388FF, 0x0000AAFF, 0x111166FF, 0x000022FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x666666FF,
	0x99EEBBFF, 0x66CC77FF, 0x226611FF, 0x44AA33FF, 0x008833FF, 0x557755FF, 0x225500FF, 0x113322FF, 0x002211FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x555555FF,
	0xDDFFBBFF, 0xCCFF88FF, 0x88AA55FF, 0xAADD88FF, 0x88FF00FF, 0xAABB99FF, 0x66BB00FF, 0x559900FF, 0x336600FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x444444FF,
	0xBBDDFFFF, 0x77CCFFFF, 0x335599FF, 0x6699FFFF, 0x1177FFFF, 0x4477AAFF, 0x224477FF, 0x002277FF, 0x001144FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x333333FF,
	0xAAFFFFFF, 0x55FFFFFF, 0x0088BBFF, 0x55BBCCFF, 0x00CCFFFF, 0x4499AAFF, 0x006688FF, 0x004455FF, 0x002233FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x222222FF,
	0xCCFFEEFF, 0xAAEEDDFF, 0x33CCAAFF, 0x55EEBBFF, 0x00FFCCFF, 0x77AAAAFF, 0x00AA99FF, 0x008877FF, 0x004433FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x000000FF,
	0xAAFFAAFF, 0x77FF77FF, 0x66DD44FF, 0x00FF00FF, 0x22DD22FF, 0x55BB55FF, 0x00BB00FF, 0x008800FF, 0x224422FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

/* Color format seems to be: BRGA. */
static const u32 WWPaletteColors[] = {
	0xFF0000FF, 0xFF7331FF, 0xFFAD00FF, 0xFFFF00FF, 0xADFF00FF, 0x52FF00FF, 0x00FF00FF, 0x00AD52FF, 0x0052ADFF, 0x0000FFFF, 0x5200FFFF, 0xAD00FFFF, 0xFF00FFFF, 0x000000FF, 0xFFFFFFFF,
	0xFF7B7BFF, 0xFFB57BFF, 0xFFE77BFF, 0xFFFF7BFF, 0xDEFF7BFF, 0xADFF7BFF, 0x7BFF7BFF, 0x52AD84FF, 0x5284ADFF, 0x7B7BFFFF, 0xB57BFFFF, 0xE77BFFFF, 0xFF7BFFFF, 0x000000FF, 0xFFFFFFFF,
	0xA50000FF, 0xA53100FF, 0xA57300FF, 0xA5A500FF, 0x73A500FF, 0x31A500FF, 0x00A500FF, 0x005221FF, 0x002152FF, 0x0000A5FF, 0x3100A5FF, 0x7300A5FF, 0xA500A5FF, 0x000000FF, 0xFFFFFFFF,
	0x009C00FF, 0x5ACE6BFF, 0xB5FFDEFF, 0x009C6BFF, 0x52CEA5FF, 0xADFFD6FF, 0x0052ADFF, 0x2984D6FF, 0x5AADFFFF, 0x0000FFFF, 0x4A6BFFFF, 0x314ADEFF, 0x1821B5FF, 0x00008CFF, 0xFFFFFFFF,
	0xAD7300FF, 0xD6AD42FF, 0xFFDE8CFF, 0xFF0839FF, 0xFF4A6BFF, 0xFF949CFF, 0xAD00FFFF, 0xD663FFFF, 0xFFCEFFFF, 0xFFBD9CFF, 0xDE9473FF, 0xBD634AFF, 0x9C3921FF, 0x7B1000FF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF5200FF, 0xFFB55AFF, 0xFFEFADFF, 0x7B1000FF, 0xA54A31FF, 0xD6846BFF, 0xFFBD9CFF, 0x5AADFFFF, 0x84C6FFFF, 0xADE7FFFF, 0xD6FFFFFF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x00FF00FF, 0x42FF42FF, 0x8CFF8CFF, 0xD6FFD6FF, 0x0000FFFF, 0x4242FFFF, 0x8C8CFFFF, 0xD6D6FFFF, 0xFF0000FF, 0xFF4242FF, 0xFF8C8CFF, 0xFFD6D6FF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x003100FF, 0x426342FF, 0x849C84FF, 0xC6D6C6FF, 0x7B1000FF, 0xA54A29FF, 0xD68C5AFF, 0xFFC68CFF, 0xD6B500FF, 0xE7CE39FF, 0xF7DE7BFF, 0xFFF7BDFF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x0000FFFF, 0xFF0000FF, 0xFFFF00FF, 0x4242FFFF, 0xFF4242FF, 0xFFFF42FF, 0x8C8CFFFF, 0xFF8C8CFF, 0xFFFF8CFF, 0xD6D6FFFF, 0xFFD6D6FF, 0xFFFFD6FF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x00FF00FF, 0x0000FFFF, 0xFF00FFFF, 0x42FF42FF, 0x4242FFFF, 0xFF42FFFF, 0x8CFF8CFF, 0x8C8CFFFF, 0xFF8CFFFF, 0xD6FFD6FF, 0xD6D6FFFF, 0xFFD6FFFF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF7B00FF, 0xFFFF00FF, 0x84FF00FF, 0x00FF00FF, 0x00847BFF, 0x0000FFFF, 0x7B00FFFF, 0xFF94FFFF, 0xD6B500FF, 0xBD1000FF, 0x5A1000FF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x109463FF, 0x087B52FF, 0x108C39FF, 0x319C31FF, 0xCEA54AFF, 0xCE9439FF, 0xBD8C4AFF, 0xD68C31FF, 0xAD734AFF, 0x8C5A31FF, 0x6B4229FF, 0x84EFFFFF, 0x31CEEFFF, 0x00A5C6FF, 0xFFFFFFFF,
	0xD6DEE7FF, 0xB5CEDEFF, 0xE7EFEFFF, 0xF7F7F7FF, 0x84737BFF, 0x948C6BFF, 0x847B63FF, 0x9C845AFF, 0x739CB5FF, 0xFF2929FF, 0xFFFF00FF, 0x9421FFFF, 0x009CBDFF, 0x000000FF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xF7EFEFFF, 0xE7DEDEFF, 0xD6CECEFF, 0xC6B5B5FF, 0xB5A5A5FF, 0xA59494FF, 0x9C8484FF, 0x8C6B6BFF, 0x7B5A5AFF, 0x6B4A4AFF, 0x5A3131FF, 0x4A2121FF, 0x421010FF, 0x310000FF,
	0xFFFFFFFF, 0xEFEFEFFF, 0xDEDEDEFF, 0xCECECEFF, 0xB5B5B5FF, 0xA5A5A5FF, 0x949494FF, 0x848484FF, 0x6B6B6BFF, 0x5A5A5AFF, 0x4A4A4AFF, 0x313131FF, 0x212121FF, 0x101010FF, 0x000000FF,
	0xFF8C7BFF, 0xFF0000FF, 0xFF7B00FF, 0xFFFF00FF, 0x008400FF, 0x00FF00FF, 0x0000FFFF, 0x009CFFFF, 0xD600FFFF, 0xFF6BFFFF, 0x9C0000FF, 0xFF9400FF, 0xFFBD94FF, 0x000000FF, 0xFFFFFFFF
};

C2D_Image CoreUtils::patternImage(std::shared_ptr<PatternImage> image, SaveType ST) {
	u32 *buffer = (u32*)linearAlloc(sizeof(u32) * 32 * 32); // Allocate Buffer.

	if (image) {
		switch(ST) {
			case SaveType::NL:
			case SaveType::WA:
				for (int i = 0; i < 0x200; i++) {
					buffer[i * 2] = NLPaletteColors[image->getPaletteColor(image->getPixel(i, false))]; // Left pixel.
					buffer[i * 2 + 1] = NLPaletteColors[image->getPaletteColor(image->getPixel(i, true))]; // Right pixel.
				}
				break;
			case SaveType::WW:
				for (int i = 0; i < 0x200; i++) {
					buffer[i * 2] = WWPaletteColors[std::max<u8>(0, image->getPaletteColor(image->getPixel(i, false)) - 1)]; // Left pixel.
					buffer[i * 2 + 1] = WWPaletteColors[std::max<u8>(0, image->getPaletteColor(image->getPixel(i, true)) - 1)]; // Right pixel.
				}
				break;
			case SaveType::UNUSED:
				linearFree(buffer); // Free buffer cause unneeded.
				return {nullptr};
		}


		C2D_Image tmp = C2DUtils::ImageDataToC2DImage(buffer, 32, 32, GPU_RGBA8);
		linearFree(buffer); // Free buffer cause unneeded.
		return tmp;
	} else {

		linearFree(buffer); // Free buffer cause unneeded.
		return {nullptr};
	}
}

/* Generate an empty pattern using the empty pattern files of the romfs. */
void CoreUtils::generateEmptyPattern(SaveType ST, WWRegion region, std::shared_ptr<u8[]> &data) {
	data = nullptr; // Reset here.
	std::string path;

	switch(ST) {
		case SaveType::WW:
			/* Switch Region. */
			switch(region) {
				case WWRegion::JPN_REV0:
				case WWRegion::JPN_REV1:
					path = "romfs:/pattern/empty/ww_jpn.acww";
					break;
				case WWRegion::USA_REV0:
				case WWRegion::USA_REV1:
				case WWRegion::EUR_REV1:
					path = "romfs:/pattern/empty/ww_eur.acww";
					break;
				case WWRegion::KOR_REV1:
					path = "romfs:/pattern/empty/ww_kor.acww";
					break;
				case WWRegion::UNKNOWN:
					return; // Because invalid.
			}
			break;

		case SaveType::NL:
		case SaveType::WA:
			path = "romfs:/pattern/empty/nl.acnl";
			break;
		case SaveType::UNUSED:
			return; // Because invalid.
	}

	FILE *file = fopen(path.c_str(), "rb");
	fseek(file, 0, SEEK_END);
	u32 size = ftell(file);
	fseek(file, 0, SEEK_SET);
	data = std::shared_ptr<u8[]>(new u8[size]);
	fread(data.get(), 1, size, file);
	fclose(file);
}