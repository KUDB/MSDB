#pragma once
#ifndef _MSDB_FILE_H_
#define _MSDB_FILE_H_

#include <pch.h>
#include <util/singleton.h>

namespace msdb
{
namespace core
{
class file;
class fileManager;

class file
{
public:
	using byteSize = size_t;

public:
	file(char const* strPath, int flags);

	/**

	*/
	~file();

	int close();

	bool isClosed();

	byteSize read(void* data, byteSize size, byteSize offs);
	byteSize wirte(const void* data, byteSize size, byteSize offs);

private:
	std::fstream fs_;
	bool closed_;
	fileManager* fm_;		// pointer to filemanager
};

class fileManager : public singleton<fileManager>
{
public:
	using files = std::vector<file>;
	using fileEntries = std::vector<std::filesystem::directory_entry>;

	bool openDir(const char* strPath);
	bool removeDir(const char* strPath);

	bool removeFile(char const* strPath);

	fileEntries getFileEntries(const char* strPath);

protected:
	bool init();

private:
	
};
}		// core
}		// msdb
#endif	// _MSDB_FILE_H_
