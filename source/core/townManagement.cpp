/*
*   This file is part of LeafEdit
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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

#include "common/archive.hpp"
#include "common/common.hpp"
#include "common/config.hpp"
#include "common/io.hpp" // Backup & Restore Part.
#include "common/title.hpp"
#include "common/utils.hpp"

#include "core/townManagement.hpp"

#include "gui/keyboard.hpp" // For the Input Stuff.
#include "gui/screens/screenCommon.hpp"

#include "lang/langStrings.h" // For the Strings.

#include <3ds.h>

extern bool WelcomeAmiibo;
extern bool isROMHack;

// Backup the current Game. If Update Found "Welcome-Amiibo" Folder -> If not "Old" Folder.
Result TownManagement::BackupTown(u64 ID, FS_MediaType Media, u32 lowID, u32 highID)
{
	static bool alreadyexists = false;
	Result res		= 0;
	FS_Archive archive;
		res = Archive::save(&archive, Media, lowID, highID); // Get the current Archive.

	if (R_SUCCEEDED(res)) {
		std::u16string customPath;

		std::string saveName = Input::getString(20, Lang::typeName.c_str());
		customPath += StringUtils::UTF8toUTF16("/LeafEdit/Towns");

		customPath += StringUtils::UTF8toUTF16("/");

		// Check for the game.
		if (isROMHack == true) {
			customPath += StringUtils::UTF8toUTF16("Welcome-Luxury");
		} else if (isROMHack == false) {
			if (ID == OldJPN || ID == OldUSA || ID == OldEUR || ID == OldKOR) {
				if (WelcomeAmiibo == false) {
					customPath += StringUtils::UTF8toUTF16("Old");
				} else if (WelcomeAmiibo == true) {
					customPath += StringUtils::UTF8toUTF16("Welcome-Amiibo");
				}
			} else {
				if (ID == WelcomeAmiiboUSA || ID == WelcomeAmiiboEUR || ID == WelcomeAmiiboJPN || ID == WelcomeAmiiboKOR) {
					customPath += StringUtils::UTF8toUTF16("Welcome-Amiibo");
				}
			}
		}

		std::u16string folderPath = customPath;
		folderPath += StringUtils::UTF8toUTF16("/");
		folderPath += StringUtils::UTF8toUTF16(saveName.c_str());
		Msg::DisplayMsg(Lang::messages2[7]);
		if (io::directoryExists(Archive::sdmc(), folderPath) == false) {
			alreadyexists = false;
			res = io::createDirectory(Archive::sdmc(), folderPath);
			if (R_FAILED(res)) {
				FSUSER_CloseArchive(archive);
				Msg::DisplayWaitMsg(Lang::messages[0]);
				return res;
			}
		} else if (io::directoryExists(Archive::sdmc(), folderPath) == true) {
			alreadyexists = true;
		}


		std::u16string savePath = folderPath;
		savePath += StringUtils::UTF8toUTF16("/");

		if (alreadyexists == true) {
			if (Msg::promptMsg(Lang::messages2[11])) {
				res = io::copyDirectory(archive, Archive::sdmc(), StringUtils::UTF8toUTF16("/"), savePath);
					if (R_FAILED(res)) {
						FSUSER_CloseArchive(archive);
						FSUSER_DeleteDirectoryRecursively(Archive::sdmc(), fsMakePath(PATH_UTF16, folderPath.data()));
						Msg::DisplayWaitMsg(Lang::messages[1]);
						return res;
					}
				FSUSER_CloseArchive(archive);
				Msg::DisplayWaitMsg(Lang::messages[2]);
			}

		} else if (alreadyexists == false) {
			res = io::copyDirectory(archive, Archive::sdmc(), StringUtils::UTF8toUTF16("/"), savePath);
				if (R_FAILED(res)) {
					FSUSER_CloseArchive(archive);
					FSUSER_DeleteDirectoryRecursively(Archive::sdmc(), fsMakePath(PATH_UTF16, folderPath.data()));
					Msg::DisplayWaitMsg(Lang::messages[1]);
					return res;
				}
				FSUSER_CloseArchive(archive);
				Msg::DisplayWaitMsg(Lang::messages[2]);
			}
	}
	return 0;
	alreadyexists = false;
}

// Clear the current Save Data
Result TownManagement::CreateNewTown(FS_MediaType Media, u64 TID, u32 lowID, u32 highID, u32 uniqueID)
{
	FS_Archive archive;
	Result res		= 0;
	Archive::save(&archive, Media, lowID, highID); // Get the current Archive.

	std::u16string dstPath = StringUtils::UTF8toUTF16("/");

	FSUSER_DeleteDirectoryRecursively(archive, fsMakePath(PATH_UTF16, dstPath.data()));

	res = FSUSER_ControlArchive(archive, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);
		if (R_FAILED(res)) {
			FSUSER_CloseArchive(archive);
			Msg::DisplayWaitMsg(Lang::messages[3]);
			return res;
		}


	u8 out;
	u64 secureValue = ((u64)SECUREVALUE_SLOT_SD << 32) | (uniqueID << 8);
	res = FSUSER_ControlSecureSave(SECURESAVE_ACTION_DELETE, &secureValue, 8, &out, 1);
		if (R_FAILED(res)) {
			FSUSER_CloseArchive(archive);
			Msg::DisplayWaitMsg(Lang::messages[4]);
			return res;
		}
	LaunchTown(Media, TID);
	return 0;
}

// Restore and Launch the selected Town.
Result TownManagement::LaunchTown(FS_MediaType Mediatype, u64 TID)
{
	Result res		= 0;
		u8 param[0x300];
		u8 hmac[0x20];
		memset(param, 0, sizeof(param));
		memset(hmac, 0, sizeof(hmac));

		APT_PrepareToDoApplicationJump(0, TID, Mediatype);
		res = APT_DoApplicationJump(param, sizeof(param), hmac);
		if (R_FAILED(res)) {
			Msg::DisplayWaitMsg(Lang::messages[6]);
			return res;
		}
	return 0;
}

// Restore the selected Town.
Result TownManagement::RestoreTown(u64 ID, FS_MediaType Media, u32 lowID, u32 highID, u32 uniqueID, std::string saveFolder)
{
	Result res		= 0;
	FS_Archive archive;
		res =  Archive::save(&archive, Media, lowID, highID); // Get the current Archive.

		if (R_SUCCEEDED(res)) {
			std::u16string customPath;
			customPath += StringUtils::UTF8toUTF16("/LeafEdit/Towns");
			customPath += StringUtils::UTF8toUTF16("/");

		// Check for the game.
			if (isROMHack == true) {
				customPath += StringUtils::UTF8toUTF16("Welcome-Luxury/");
			} else if (isROMHack == false) {
				if (ID == OldJPN || ID == OldUSA || ID == OldEUR || ID == OldKOR) {
					if (WelcomeAmiibo == false) {
						customPath += StringUtils::UTF8toUTF16("Old/");
					} else if (WelcomeAmiibo == true) {
						customPath += StringUtils::UTF8toUTF16("Welcome-Amiibo/");
					}
				} else {
					if (ID == WelcomeAmiiboUSA || ID == WelcomeAmiiboEUR || ID == WelcomeAmiiboJPN || ID == WelcomeAmiiboKOR) {
						customPath += StringUtils::UTF8toUTF16("Welcome-Amiibo/");
					}
				}
			}

				std::u16string srcPath = customPath;
				srcPath += StringUtils::UTF8toUTF16(saveFolder.c_str());
				srcPath += StringUtils::UTF8toUTF16("/");
				std::u16string dstPath = StringUtils::UTF8toUTF16("/");

				FSUSER_DeleteDirectoryRecursively(archive, fsMakePath(PATH_UTF16, dstPath.data()));
					  Msg::DisplayMsg(Lang::messages2[8]);
				res = io::copyDirectory(Archive::sdmc(), archive, srcPath, dstPath);
					if (R_FAILED(res)) {
						Msg::DisplayWaitMsg(Lang::messages[1]);
						FSUSER_CloseArchive(archive);
						return res;
					}


			res = FSUSER_ControlArchive(archive, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);
				if (R_FAILED(res)) {
					FSUSER_CloseArchive(archive);
					Msg::DisplayWaitMsg(Lang::messages[3]);
					return res;
				}


			u8 out;
			u64 secureValue = ((u64)SECUREVALUE_SLOT_SD << 32) | (uniqueID << 8);
			res = FSUSER_ControlSecureSave(SECURESAVE_ACTION_DELETE, &secureValue, 8, &out, 1);
				if (R_FAILED(res)) {
					FSUSER_CloseArchive(archive);
					Msg::DisplayWaitMsg(Lang::messages[4]);
					return res;
				}
			}
	Msg::DisplayWaitMsg(Lang::messages[5]);
	return 0;
}

// Other Useful stuff?

void TownManagement::DeleteBackup(u64 ID, std::string backup) {
	std::u16string customPath;
	customPath += StringUtils::UTF8toUTF16("/LeafEdit/Towns");

	customPath += StringUtils::UTF8toUTF16("/");

		// Check for the game.
		if (isROMHack == true) {
			customPath += StringUtils::UTF8toUTF16("Welcome-Luxury");
		} else if (isROMHack == false) {
			if (ID == OldJPN || ID == OldUSA || ID == OldEUR || ID == OldKOR) {
				if (WelcomeAmiibo == false) {
					customPath += StringUtils::UTF8toUTF16("Old");
				} else if (WelcomeAmiibo == true) {
					customPath += StringUtils::UTF8toUTF16("Welcome-Amiibo");
				}
			} else {
				if (ID == WelcomeAmiiboUSA || ID == WelcomeAmiiboEUR || ID == WelcomeAmiiboJPN || ID == WelcomeAmiiboKOR) {
					customPath += StringUtils::UTF8toUTF16("Welcome-Amiibo");
				}
			}
		}

	std::u16string backupPath = customPath;
	backupPath += StringUtils::UTF8toUTF16("/");
	backupPath += StringUtils::UTF8toUTF16(backup.c_str());

	FSUSER_DeleteDirectoryRecursively(Archive::sdmc(), fsMakePath(PATH_UTF16, backupPath.data()));
}