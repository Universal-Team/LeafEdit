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
#include "jpeg.h"
#include "Sav.hpp"
#include "saveUtils.hpp"
#include "utils.hpp"

#include <ctime> // For the time string.
#include <dirent.h> // For mkdir.

extern std::shared_ptr<Sav> save;
extern std::unique_ptr<Config> config;

// Fix Invalid Buildings and replace them with "Empty".
void CoreUtils::FixInvalidBuildings(void) {
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
					if (!Msg::promptMsg("Invalid Buildings have been found on your save.\nDo you like to fix them through Empty Buildings?")) {
						return;
					}
				}

				SaveUtils::Write<u32>(save->savePointer(), offset + (i * 4), static_cast<u32>(0x000000FC)); // Write empty building.
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
	if (save->getType() == SaveType::WA) {
		if (UpdateSaveRegion(regionLock))
		{
			if (Msg::promptMsg("The region does not match the one from your console.\nWould you like to fix it?")) {
				save->savePointer()[0x621CE] = regionLock.RawByte;
			}
		}
	}
}

C2D_Image CoreUtils::LoadPlayerTPC(std::shared_ptr<Player> player) {
	if (player->tpcImage() != nullptr) {
		if (player->hasTPC()) {
			// Load.
			return LoadPlayerPicture(player->tpcImage());
		} else {
			return {nullptr};
		}
	} else {
		return {nullptr};
	}
}

void CoreUtils::createBackup() {
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
			Msg::DisplayWarnMsg("Create Backup... Please wait.");
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
		}
	}
}