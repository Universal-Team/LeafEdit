#ifndef FILEBROWSE_H
#define FILEBROWSE_H

#include <3ds.h>
#include <cstring>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <vector>

struct DirEntry {
	std::string name;
	std::string path;
	bool isDirectory;
	off_t size;
};

void getDirectoryContents(std::vector<DirEntry>& dirContents);

#endif