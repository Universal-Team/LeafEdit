	/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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

#include "config.hpp"
#include "gameLoader.hpp"
#include "gui.hpp"
#include "lang.hpp"
#include "msg.hpp"


u64 currentID;
u32 currentLowID;
u32 currentHighID;
u32 currentUniqueID;
FS_MediaType currentMedia;

bool GameLoader::checkTitle(u64 TitleID) {
	Result res = 0;
	static u32 titleCount;

	res = AM_GetTitleCount(MEDIATYPE_SD, &titleCount);
	if (R_FAILED(res))
	{
		return false; // cause failed.
	}

	std::vector<u64> ID;
	ID.resize(titleCount);

	// Get Title List.
	res = AM_GetTitleList(nullptr, MEDIATYPE_SD, titleCount, &ID[0]);
	if (R_FAILED(res))
	{
		return false; // cause failed.
	}
	// Check, if TitleID matches.
	if (std::find(ID.begin(), ID.end(), TitleID) != ID.end()) {
		currentID = TitleID;
		currentLowID = (u32)currentID;
		currentHighID = (u32)(currentID >> 32);
		currentUniqueID = (currentLowID >> 8);
		currentMedia = MEDIATYPE_SD;
		return true; // cause is found.
	} else {
		Msg::DisplayWarnMsg(Lang::get("TITLE_NOT_FOUND"));
		return false; // cause isn't found.
	}
}

bool GameLoader::scanCard(u64 TitleID)
{
	Result res = 0;
	u32 titleCount  = 0;
	// check for the cartridge.
	FS_CardType cardType;
	res = FSUSER_GetCardType(&cardType);
	if (R_SUCCEEDED(res))
	{
		if (cardType == CARD_CTR)
		{
			res = AM_GetTitleCount(MEDIATYPE_GAME_CARD, &titleCount);
			if (R_SUCCEEDED(res) && titleCount > 0)
			{
				std::vector<u64> ID;
				ID.resize(titleCount);
				res = AM_GetTitleList(NULL, MEDIATYPE_GAME_CARD, titleCount, &ID[0]);

				// check if this id is in our list
				if (R_SUCCEEDED(res) && std::find(ID.begin(), ID.end(), TitleID) != ID.end())
				{
					currentID = TitleID;
					currentLowID = (u32)currentID;
					currentHighID = (u32)(currentID >> 32);
					currentUniqueID = (currentLowID >> 8);
					currentMedia = MEDIATYPE_GAME_CARD;
					return true; // cause is found.
				} else {
					Msg::DisplayWarnMsg(Lang::get("TITLE_NOT_FOUND"));
					return false; // cause isn't found.
				}
			} else {
				Msg::DisplayWarnMsg(Lang::get("TITLE_NOT_FOUND"));
				return false; // cause isn't found.
			}
		} else {
			Msg::DisplayWarnMsg(Lang::get("TITLE_NOT_FOUND"));
			return false; // cause isn't found.
		}
	} else {
		Msg::DisplayWarnMsg(Lang::get("TITLE_NOT_FOUND"));
		return false; // cause isn't found.
	}
}

// Check for Updates of the old AC:NL Version.
void GameLoader::checkUpdate(void)
{
	if (Config::getBool("updateCheck") == false) {
		Result res = 0;
		u32 updateTitleCount;

		res = AM_GetTitleCount(MEDIATYPE_SD, &updateTitleCount);
		if (R_FAILED(res))
		{
			return;
		}

		// get title list and check if a title matches the ids we want
		std::vector<u64> updateIds;
		updateIds.resize(updateTitleCount);
		res    = AM_GetTitleList(nullptr, MEDIATYPE_SD, updateTitleCount, &updateIds[0]);
		if (R_FAILED(res))
		{
		return;
		}

		Msg::DisplayWarnMsg(Lang::get("GAMELOADER_CHECK"));

		if (std::find(updateIds.begin(), updateIds.end(), 0x0004000E00086200) != updateIds.end() //JPN.
			|| std::find(updateIds.begin(), updateIds.end(), 0x0004000E00086300) != updateIds.end() // USA.
			|| std::find(updateIds.begin(), updateIds.end(), 0x0004000E00086400) != updateIds.end() // EUR.
			|| std::find(updateIds.begin(), updateIds.end(), 0x0004000E00086500) != updateIds.end()) // KOR.
		{
			Msg::DisplayWarnMsg(Lang::get("UPDATE_FOUND"));
			Config::setBool("update", true);
		} else {
			Msg::DisplayWarnMsg(Lang::get("UPDATE_NOTFOUND"));
			Config::setBool("update", false);
		}
		Config::setBool("updateCheck", true);
		Config::save();
	}
}

// Check for Updates, even when the Update was already checked on first startup.
void GameLoader::updateCheck2(void) {
	Config::setBool("updateCheck", false);
	checkUpdate();
}