#include "common/archive.hpp"

static FS_Archive mSdmc;

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

Result Archive::init(void)
{
	return FSUSER_OpenArchive(&mSdmc, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
}

void Archive::exit(void)
{
	FSUSER_CloseArchive(mSdmc);
}

FS_Archive Archive::sdmc(void)
{
	return mSdmc;
}