#include "smdh.hpp"

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