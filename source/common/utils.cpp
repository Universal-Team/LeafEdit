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
#include <memory>
#include <sstream>
#include <sys/stat.h>

extern std::string selectedSaveFolderEditor;
// StringUtils.

// Convert UTF-8 (up to 0xFFFF) to a single UTF-16 character.
std::u16string StringUtils::UTF8toUTF16(const std::string &text) {
    std::u16string out;
    char16_t c;

    for(uint i=0;i<text.size();) {
        if(!(text[i] & 0x80)) {
            c = text[i++];
        } else if((text[i] & 0xE0) == 0xC0) {
            c  = (text[i++] & 0x1F) << 6;
            c |=  text[i++] & 0x3F;
        } else if((text[i] & 0xF0) == 0xE0) {
            c  = (text[i++] & 0x0F) << 12;
            c |= (text[i++] & 0x3F) << 6;
            c |=  text[i++] & 0x3F;
        } else {
            i++; // out of range or something. (This only does up to 0xFFFF since it goes to a U16 anyways)
        }
        out += c;
    }
    return out;
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

std::vector<std::pair<std::string, s32>> EditorUtils::load_player_islandbox(int selectedplayer, int islandBox) {
	std::vector<std::pair<std::string, s32>> islandItemData;
	if (islandBox == 0) {
		for (int num = 0; num < 10; num++) {
			Item* item = &Save::Instance()->players[selectedplayer]->IslandBox[num];
			islandItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
		}
	} else {
		for (int num = islandBox*10; num < islandBox*10+10; num++) {
			Item* item = &Save::Instance()->players[selectedplayer]->IslandBox[num];
			islandItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
		}
	}

	return islandItemData;
}

std::vector<std::pair<std::string, s32>> EditorUtils::load_player_storageitems(int selectedplayer, int storage) {
	std::vector<std::pair<std::string, s32>> storageItemData;
	if (storage == 0) {
		for (int num = 0; num < 10; num++) {
			Item* item = &Save::Instance()->players[selectedplayer]->Storage[num];
			storageItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
		}
	} else {
		for (int num = storage*10; num < storage*10+10; num++) {
			Item* item = &Save::Instance()->players[selectedplayer]->Storage[num];
			storageItemData.push_back(std::make_pair(item->GetName(), item->GetSpritesheetID()));
		}
	}

	return storageItemData;
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
		char message [100];
		snprintf(message, sizeof(message), Lang::get("FIND_BACKUP").c_str(), path.c_str());
		Msg::DisplayWaitMsg(message);
	}
}


static inline u32   Pow2(u32 x)
{
    if (x <= 64)
        return 64;

    return 1u << (32 - __builtin_clz(x - 1));
}

C2D_Image C2DUtils::ImageDataToC2DImage(u32 *imageData, u32 width, u32 height, GPU_TEXCOLOR colorFormat) {

    u32     widthPow2 = Pow2(width);
    u32     heightPow2 = Pow2(height);
    u32   * buffer = (u32 *)linearAlloc(sizeof(u32) * widthPow2 * heightPow2);

    // Clear buffer
    C3D_SyncMemoryFill(buffer, 0, (u32 *)((u8 *)buffer + (sizeof(u32) * widthPow2 * heightPow2)), BIT(0) | GX_FILL_32BIT_DEPTH, nullptr, 0, nullptr, 0);
    GSPGPU_FlushDataCache(buffer, widthPow2 * heightPow2 * sizeof(u32));

    // Copy Data
    u32 *dst = buffer;
    u32 *src = imageData;

    for (u32 h = height; h > 0; h--)
    {
        memcpy(dst, src, width * sizeof(u32));
        dst += widthPow2;
        src += width;
    }

    GSPGPU_FlushDataCache(buffer, widthPow2 * heightPow2 * sizeof(u32));

    C3D_Tex *tex = new C3D_Tex();
    C3D_TexInit(tex, Pow2(width), Pow2(height), colorFormat);

    tex->param = GPU_TEXTURE_MAG_FILTER(GPU_NEAREST) | GPU_TEXTURE_MIN_FILTER(GPU_NEAREST)
        | GPU_TEXTURE_WRAP_S(GPU_CLAMP_TO_BORDER) | GPU_TEXTURE_WRAP_T(GPU_CLAMP_TO_BORDER);
    tex->border = 0xFFFFFFFF;

    C3D_SyncMemoryFill((u32 *)tex->data, 0, (u32 *)((u8 *)tex->data + tex->size), BIT(0) | (tex->fmt << 8), nullptr, 0, nullptr, 0);
    C3D_TexFlush(tex);

    C3D_SyncDisplayTransfer(buffer, GX_BUFFER_DIM(tex->width, tex->height), \
        (u32 *)tex->data, GX_BUFFER_DIM(tex->width, tex->height), TEXTURE_TRANSFER_FLAGS);

    C3D_TexFlush(tex);
    linearFree(buffer);

    Tex3DS_SubTexture *subtex = new Tex3DS_SubTexture();
    subtex->width = width;
    subtex->height = height;
    subtex->left = 0.f;
    subtex->top =  1.f;
    subtex->right = (float)width / (float)tex->width;
    subtex->bottom = 1.f - (float)height / (float)tex->height;

    C2D_Image image;
    image.tex = tex;
    image.subtex = subtex;

    return image;
}

void C2DUtils::C2D_ImageDelete(C2D_Image image)
{
    C3D_TexDelete(image.tex);
    delete image.tex;
    delete image.subtex;
}