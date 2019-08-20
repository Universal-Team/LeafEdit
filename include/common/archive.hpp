#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

#include "utils.hpp"
#include <3ds.h>
#include "fsstream.hpp"


namespace Archive {
    Result init(void);
    void exit(void);

    FS_Archive sdmc(void);
    bool saveAccessible(FS_MediaType mediatype, u32 lowid, u32 highid);
    Result save(FS_Archive* archive, FS_MediaType mediatype, u32 lowid, u32 highid);
}

#endif