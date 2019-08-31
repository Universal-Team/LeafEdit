#include "common/utils.hpp"

#include "core/save/save.h"

#include <3ds.h>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

std::vector<std::string> g_villagerDatabase;

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


// Villager stuff.
u16 strToU16(std::string str) {
    u16 out;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> out;

    return out;
}

std::string languages[] = { "de", "en", "es", "fr", "it", "jp"};

void LoadVillagerDatabase(int lang) {
    g_villagerDatabase.clear();

    std::string currentLine;
        std::ifstream villagerDatabase("romfs:/lang/" + languages[lang] + "/villager.txt", std::ifstream::in);



    while (std::getline(villagerDatabase, currentLine)) {
        // TODO: Read default things like personality, catchphrases, & furniture.
        g_villagerDatabase.push_back(currentLine);
    }

    villagerDatabase.close();
}