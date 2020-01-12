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

#include "common/config.hpp"
#include "common/io.hpp"
#include "common/utils.hpp"

#include "core/save/item.h"
#include "core/save/save.h"

#include "gui/colors.hpp"
#include "gui/msg.hpp"

#include "lang/lang.hpp"

#include <3ds.h>
#include <ctime>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <sys/stat.h>

extern std::string selectedSaveFolderEditor;
// StringUtils.

std::u16string StringUtils::UTF8toUTF16(const char* src)
{
	char16_t tmp[256] = {0};
	utf8_to_utf16((uint16_t*)tmp, (uint8_t*)src, 256);
	return std::u16string(tmp);
}

static std::string utf16DataToUtf8(const char16_t* data, size_t size, char16_t delim = 0)
{
	std::string ret;
	ret.reserve(size);
	char addChar[4] = {0};
	for (size_t i = 0; i < size; i++)
	{
		if (data[i] == delim)
		{
			return ret;
		}
		else if (data[i] < 0x0080)
		{
			addChar[0] = data[i];
			addChar[1] = '\0';
		}
		else if (data[i] < 0x0800)
		{
			addChar[0] = 0xC0 | ((data[i] >> 6) & 0x1F);
			addChar[1] = 0x80 | (data[i] & 0x3F);
			addChar[2] = '\0';
		}
		else
		{
			addChar[0] = 0xE0 | ((data[i] >> 12) & 0x0F);
			addChar[1] = 0x80 | ((data[i] >> 6) & 0x3F);
			addChar[2] = 0x80 | (data[i] & 0x3F);
			addChar[3] = '\0';
		}
		ret.append(addChar);
	}
	return ret;
}

std::string StringUtils::UTF16toUTF8(const std::u16string& src)
{
	return utf16DataToUtf8(src.data(), src.size());
}

std::string StringUtils::format(std::string fmt_str, ...)
{
	va_list ap;
	char* fp = NULL;
	va_start(ap, fmt_str);
	vasprintf(&fp, fmt_str.c_str(), ap);
	va_end(ap);
	std::unique_ptr<char, decltype(free)*> formatted(fp, free);
	return std::string(formatted.get());
}

std::vector<u32> EditorUtils::findPlayerReferences(Player *player) {
	// Read u16 ID + Name
	u16 *dataArray = new u16[11];
	for (u32 i = 0; i < 11; i++) {
		dataArray[i] = Save::Instance()->ReadU16(player->m_offset + 0x55A6 + i * 2);
	}

	std::vector<u32> references = std::vector<u32>();
	u32 Size = Save::Instance()->GetSaveSize() - 22;
	for (u32 i = 0; i < Size; i += 2) { // subtract 22 so we don't read past the end of the file
		bool foundMatch = true;
		for (int x = 0; x < 11; x++) {
			if (Save::Instance()->ReadU16(i + x * 2) != dataArray[x]) {
				foundMatch = false;
				break;
			}
		}
		// add the offset to the vector
		if (foundMatch) {
			references.push_back(i);
			i += 20; // skip the rest of this data
		}
	}

	delete[] dataArray;
	return references;
}

std::vector<std::pair<std::string, s32>> EditorUtils::load_player_invitems(int selectedplayer) {
	std::vector<std::pair<std::string, s32>> inventoryItemData;
	for (int num = 0; num < 16; num++) {
		Item* item = &Save::Instance()->players[selectedplayer]->Pockets[num];
		inventoryItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
	}

	return inventoryItemData;
}


std::vector<std::pair<std::string, s32>> EditorUtils::load_player_dresseritems(int selectedplayer, int dresser) {
	std::vector<std::pair<std::string, s32>> dresserItemData;
	if (dresser == 0) {
		for (int num = 0; num < 10; num++) {
			Item* item = &Save::Instance()->players[selectedplayer]->Dresser[num];
			dresserItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
		}
	} else {
		for (int num = dresser*10; num < dresser*10+10; num++) {
			Item* item = &Save::Instance()->players[selectedplayer]->Dresser[num];
			dresserItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
		}
	}

	return dresserItemData;
}


// Villager stuff.
u16 strToU16(std::string str) {
	u16 out;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> out;

	return out;
}

void Utils::colorLogic(int mode) {
	if (mode == 0) {
		DARKER_COLOR = DARKER_BLUE;
		LIGHT_COLOR = LIGHT_BLUE;
		LIGHTER_COLOR = LIGHTER_BLUE;
		SELECTED_COLOR = SELECTED_BLUE;
		UNSELECTED_COLOR = UNSELECTED_BLUE;
		Config::colorMode = 1;
	} else if (Config::colorMode == 1) {
		DARKER_COLOR = DARKER_DEEPBLUE;
		LIGHT_COLOR = LIGHT_DEEPBLUE;
		LIGHTER_COLOR = LIGHTER_DEEPBLUE;
		SELECTED_COLOR = SELECTED_DEEPBLUE;
		UNSELECTED_COLOR = UNSELECTED_DEEPBLUE;
		Config::colorMode = 2;
	} else if (Config::colorMode == 2) {
		DARKER_COLOR = DARKER_DEEPRED;
		LIGHT_COLOR = LIGHT_DEEPRED;
		LIGHTER_COLOR = LIGHTER_DEEPRED;
		SELECTED_COLOR = SELECTED_DEEPRED;
		UNSELECTED_COLOR = UNSELECTED_DEEPRED;
		Config::colorMode = 3;
	} else {
		DARKER_COLOR = DARKER_GREEN;
		LIGHT_COLOR = LIGHT_GREEN;
		LIGHTER_COLOR = LIGHTER_GREEN;
		SELECTED_COLOR = SELECTED_GREEN;
		UNSELECTED_COLOR = UNSELECTED_GREEN;
		Config::colorMode = 0;
	}
}

void Utils::createBackup(void) {
	if (Msg::promptMsg2(Lang::get("CREATE_BACKUP"))) {
		char stringTime[15]   = {0};
    	time_t unixTime       = time(NULL);
    	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
    	std::strftime(stringTime, 14, "%Y%m%d%H%M%S", timeStruct);
		std::string path = "/LeafEdit/Backups/" + std::string(stringTime);
		mkdir(path.c_str(), 0777); // Create folder.
		path += "/garden_plus.dat";
		io::copyFile(Archive::sdmc(), Archive::sdmc(), StringUtils::UTF8toUTF16(selectedSaveFolderEditor.c_str()), StringUtils::UTF8toUTF16(path.c_str()));
		// Display at the end, where the backup is.
		Msg::DisplayWaitMsg(Lang::get("FIND_BACKUP") + "\n\n" + path);
	}
}