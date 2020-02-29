#ifndef FILEBROWSE_HPP
#define FILEBROWSE_HPP

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

bool nameEndsWith(const std::string& name, const std::vector<std::string> extensionList);
void getDirectoryContents(std::vector<DirEntry>& dirContents);
void getDirectoryContents(std::vector<DirEntry>& dirContents, const std::vector<std::string> extensionList);
std::vector<std::string> getContents(const std::string &name, const std::vector<std::string> &extensionList);

namespace SaveBrowse {
	std::string searchForSave(const std::vector<std::string> SaveType, const std::string initialPath, const std::string Text);
}

#endif