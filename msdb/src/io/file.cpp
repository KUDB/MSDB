#include <pch.h>
#include <io/file.h>
#include <io/io.h>

namespace msdb
{
namespace core
{
file::file(char const* path, int flags)
{
	_MSDB_TRY_IO_BEGIN
		this->fs_.open(path, flags);
		this->closed_ = false;
	_MSDB_CATCH_IO_END

	if (!this->fs_)
	{
		this->closed_ = false;
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_IO_ERROR, MSDB_ER_CANNOT_OPEN_FILE));
	}

	this->fm_ = fileManager::instance();
}

file::~file()
{
	this->close();
}

int file::close()
{
	if (this->closed_)
	{
		return 0;
	}

	_MSDB_TRY_IO_BEGIN
		this->fs_.close();
		this->closed_ = true;
	_MSDB_CATCH_IO_END

		return 0;
}

bool file::isClosed()
{
	if (this->closed_)
	{
		return true;
	}
	return false;
}
bool fileManager::openDir(char const* strPath)
{
	std::filesystem::path dirPath(strPath);
	std::filesystem::directory_entry dirEntry(dirPath);
	if(!dirEntry.exists())
	{
		if(!std::filesystem::create_directories(dirPath))
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_IO_ERROR, MSDB_ER_CANNOT_OPEN_DIR));
		}
	}
	return false;
}

bool fileManager::removeDir(char const* strPath)
{
	std::filesystem::path dirPath(strPath);
	std::filesystem::directory_entry dirEntry(dirPath);
	if(dirEntry.exists() && dirEntry.is_directory())
	{
		// the remove_all functions recursively remove all directory entries, 
		// then the entry itself.
		return std::filesystem::remove_all(dirPath);
	}
	return false;
}

bool fileManager::removeFile(char const* strPath)
{
	std::filesystem::path filePath(strPath);
	std::filesystem::directory_entry fileEntry(filePath);
	if (fileEntry.exists() && fileEntry.is_regular_file())
	{
		return std::filesystem::remove(filePath);
	}
	return false;
}

fileManager::fileEntries fileManager::getFileEntries(const char* strPath)
{
	return fileEntries();
}


bool fileManager::init()
{
	return false;
}
}		// core
}		// msdb