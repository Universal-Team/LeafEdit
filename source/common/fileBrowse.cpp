#include "common/fileBrowse.h"

#include "gui/msg.hpp"

#include <3ds.h>
#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>

int file_count = 0;
uint selectedFile = 0;
int keyRepeatDelay = 3;
bool dirChanged = true;
std::vector<DirEntry> dirContents;

off_t getFileSize(const char *fileName)
{
	FILE* fp = fopen(fileName, "rb");
	off_t fsize = 0;
	if (fp) {
		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);			// Get source file's size
		fseek(fp, 0, SEEK_SET);
	}
	fclose(fp);

	return fsize;
}

bool dirEntryPredicate(const DirEntry& lhs, const DirEntry& rhs) {
	if(!lhs.isDirectory && rhs.isDirectory) {
		return false;
	}
	if(lhs.isDirectory && !rhs.isDirectory) {
		return true;
	}
	return strcasecmp(lhs.name.c_str(), rhs.name.c_str()) < 0;
}

void getDirectoryContents(std::vector<DirEntry>& dirContents) {
	struct stat st;

	dirContents.clear();

	DIR *pdir = opendir ("."); 
	
	if (pdir == NULL) {
		Msg::DisplayWarnMsg("Unable to open the directory.");
			gspWaitForVBlank();
	} else {

		while(true) {
			DirEntry dirEntry;

			struct dirent* pent = readdir(pdir);
			if(pent == NULL) break;

			stat(pent->d_name, &st);
			if (strcmp(pent->d_name, "..") != 0) {
				dirEntry.name = pent->d_name;
				dirEntry.isDirectory = (st.st_mode & S_IFDIR) ? true : false;
				if (!dirEntry.isDirectory) {
					dirEntry.size = getFileSize(dirEntry.name.c_str());
				}

				if (dirEntry.name.compare(".") != 0) {
					dirContents.push_back (dirEntry);
				}
			}

		}
		
		closedir(pdir);
	}
	std::sort(dirContents.begin(), dirContents.end(), dirEntryPredicate);
}