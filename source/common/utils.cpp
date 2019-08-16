#include "utils.hpp"
#include <memory>

// StringUtils.

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

// SMDH.

smdh_s* loadSMDH(u32 low, u32 high, u8 media)
{
    Handle fileHandle;

    u32 archPath[]                  = {low, high, media, 0x0};
    static constexpr u32 filePath[] = {0x0, 0x0, 0x2, 0x6E6F6369, 0x0};
    smdh_s* smdh                    = new smdh_s;

    FS_Path binArchPath = {PATH_BINARY, 0x10, archPath};
    FS_Path binFilePath = {PATH_BINARY, 0x14, filePath};

    Result res = FSUSER_OpenFileDirectly(&fileHandle, ARCHIVE_SAVEDATA_AND_CONTENT, binArchPath, binFilePath, FS_OPEN_READ, 0);
    if (R_SUCCEEDED(res))
    {
        u32 read;
        FSFILE_Read(fileHandle, &read, 0, smdh, sizeof(smdh_s));
    }
    else
    {
        delete smdh;
        smdh = NULL;
    }

    FSFILE_Close(fileHandle);
    return smdh;
}

// Archives.

// Check, if a Save is accessible on the Card or on the installed Title.
bool Archive::saveAccessible(FS_MediaType mediatype, u32 lowid, u32 highid)
{
    FS_Archive archive;
    Result res = save(&archive, mediatype, lowid, highid);
    if (R_SUCCEEDED(res))
    {
        FSUSER_CloseArchive(archive);
        return true;
    }
    return false;
}

// Save stuff.
Result Archive::save(FS_Archive* archive, FS_MediaType mediatype, u32 lowid, u32 highid)
{
    const u32 path[3] = {mediatype, lowid, highid};
    return FSUSER_OpenArchive(archive, ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path});
}