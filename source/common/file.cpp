/*
MIT License
This file is part of NLTK
Copyright (c) 2018-2019 Slattz, Cuyler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "common/file.hpp"

#include <3ds.h>
using namespace std;

void File::PathFixups(std::string &filePath) {
    if (filePath.substr(0, 6) == "sdmc:/")
        filePath.erase(0, 6);

    if (filePath.front() != '/')
        filePath.insert(0, "/");
}

int File::Create(std::string filePath) {
    PathFixups(filePath);

    FS_Archive sdmcArch = FS::GetSDMCArchive();
    FS_Path FSPath = fsMakePath(PATH_ASCII, filePath.c_str());
    return FSUSER_CreateFile(sdmcArch, FSPath, 0, 0);
}

int File::Rename(std::string OldFilePath, std::string NewFilePath) {
    PathFixups(OldFilePath);
    PathFixups(NewFilePath);

    FS_Archive sdmcArch = FS::GetSDMCArchive();
    FS_Path OldFSPath = fsMakePath(PATH_ASCII, OldFilePath.c_str());
    FS_Path NewFSPath = fsMakePath(PATH_ASCII, NewFilePath.c_str());
    return FSUSER_RenameFile(sdmcArch, OldFSPath, sdmcArch, NewFSPath);
}

int File::Remove(std::string filePath) {
    PathFixups(filePath);

    FS_Archive sdmcArch = FS::GetSDMCArchive();
    FS_Path FSPath = fsMakePath(PATH_ASCII, filePath.c_str());
    return FSUSER_DeleteFile(sdmcArch, FSPath);
}

bool File::Exists(std::string filePath) {
    Handle FileHandle = 0;

    PathFixups(filePath);
    FS_Archive sdmcArch = FS::GetSDMCArchive();
    FS_Path FSPath = fsMakePath(PATH_ASCII, filePath.c_str());

    Result res = FSUSER_OpenFile(&FileHandle, sdmcArch, FSPath, File::READ, 0);
    if (R_SUCCEEDED(res)) {
        FSFILE_Close(FileHandle);
        return true;
    }

    return false;
}

int File::Open(File &output, const std::string &filePath, int mode) {
    output.m_Path = filePath;
    output.m_Mode = mode;
    fstream::openmode OpenMode;

    if ((mode & File::CREATE) && !Exists(filePath)) 
        Create(filePath);

    if (mode & File::READ) OpenMode|=fstream::in;
    if (mode & File::WRITE) OpenMode|=fstream::out;
    if (mode & File::APPEND) OpenMode|=fstream::app;
    if (mode & File::TRUNCATE) OpenMode|=fstream::trunc;
    if (mode & File::BINARY) OpenMode|=fstream::binary;

    output.m_File = new fstream();
    output.m_File->open(output.m_Path, OpenMode);
    if (!output.m_File->good()) return OPResult::FAILED;
    output.m_FileIsOpen = output.m_File->is_open();
    if (!output.m_FileIsOpen) {
        return OPResult::FILE_NOT_OPEN;
    }

    return OPResult::SUCCESS;
}

File::File(void) {
    m_File = nullptr;
    m_Path = "\0";
    m_Mode = 0;
    m_FileIsOpen = false;
}

File::File(const std::string &filePath, int mode) : File() {
    Open(*this, filePath, mode);
}

File::~File(void) {
    this->Close();
}

int File::Close(void) {
    if (!m_FileIsOpen) 
        return OPResult::FILE_NOT_OPEN;

    m_File->flush();
    m_File->close();

    if (!m_File->good()) 
        return OPResult::FAILED;
    
    return OPResult::SUCCESS;
}

int File::Read(void *buffer, u32 size) {
    if (size == 0 || buffer == nullptr) return OPResult::INVALID_ARG;
    if (!m_FileIsOpen) return OPResult::FILE_NOT_OPEN;
    if (!(m_Mode&File::READ)) return OPResult::INVALID_MODE;
        
    m_File->read((char*)buffer, size);
    if (m_File->good()) return OPResult::SUCCESS;

    return OPResult::FAILED;
}

int File::Write(const void *data, u32 size) {

    if (size == 0 || data == nullptr) return OPResult::INVALID_ARG;
    if (!m_FileIsOpen) return OPResult::FILE_NOT_OPEN;
    if (!(m_Mode&File::WRITE)) return OPResult::INVALID_MODE;

    m_File->write((char*)data, size);
    if(m_File->good()) {
        if (m_Mode&File::SYNC) m_File->flush();
        return OPResult::SUCCESS;
    }
    return OPResult::FAILED;
}

int File::Flush(void) const {
    if (!m_FileIsOpen) return OPResult::FILE_NOT_OPEN;
    m_File->flush();
    if (m_File->good()) return OPResult::SUCCESS;
    return OPResult::FAILED;
}

u64 File::GetSize(void) const {
    if (!m_FileIsOpen) return OPResult::FILE_NOT_OPEN;
    m_File->seekg(0, m_File->end);
    u64 size = m_File->tellg();
    m_File->seekg(0);
    return size;
}

bool File::IsOpen(void) const {
    return m_FileIsOpen;
}

int File::Seek(s64 offset, SeekPos origin) {
    if (!m_FileIsOpen) return OPResult::FILE_NOT_OPEN;
    fstream::seekdir way;

    if (origin == SeekPos::SET) way = m_File->beg;
    else if (origin == SeekPos::END) way = m_File->end;
    else way = m_File->cur; //Assume Current Pos

    m_File->seekg(offset, way);
    if (m_File->good())
        return OPResult::SUCCESS;

    else
        return OPResult::INVALID_ARG;
}

u64 File::Tell(void) const {
    return m_File->tellg();
}

void File::Rewind(void) {
    m_File->seekg(0);
}

static FS_Archive sdmcArch = 0;

FS_Archive FS::GetSDMCArchive(void) {
    return sdmcArch;
}