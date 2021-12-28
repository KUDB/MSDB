#pragma once
#ifndef _MSDB_STORAGEMGR_H_
#define _MSDB_STORAGEMGR_H_

#include <pch.h>
#include <array/arrayId.h>
#include <array/chunkId.h>
#include <array/attributeId.h>
#include <util/singleton.h>
#include <io/bitstream.h>
#include <io/serializable.h>
#include <system/config.h>
#include <array/arrayDesc.h>

namespace msdb
{
namespace core
{
using storage_version_type = unsigned int;
const storage_version_type MSDB_STORAGE_FORMAT_VERSION = 1;

extern const char* strBasePath;
extern const char* strConfigPath;
extern const char* strArrayPath;

extern const char* strIndexFolder;
extern const char* strIndexFilExtension;

extern const char* strArrayConfigFile;

using filePath = std::filesystem::path;

class storageMgr : public singleton<storageMgr>
{
private:
	storageMgr();

	~storageMgr();

public:
	//config* loadConfigFile(ConfigType type);
	//void saveConfigFile(config* cFile);
	void initSystemConfig();
	std::vector<pArrayDesc> loadAllArrayDescs();
	pArrayDesc loadArrayDesc(const filePath descPath);
	void saveArrayDesc(pArrayDesc arrDesc);

	void loadAttrIndex(const arrayId arrId, const attributeId attrId, pSerializable serialObj);
	void saveAttrIndex(const arrayId arrId, const attributeId attrId, pSerializable serialObj);

	void loadChunk(const arrayId arrId, const attributeId attrId, const chunkId chkId, pSerializable serialObj);
	void saveChunk(const arrayId arrId, const attributeId attrId, const chunkId chkId, pSerializable serialObj);

protected:
	filePath getBasePath();
	filePath getArrayRelativePath(const arrayId arrId);
	filePath getArrayFolder(const arrayId arrId);
	filePath getArrayIndexPath(const arrayId arrId);
	filePath getChunkPath(const arrayId arrId, const attributeId attrId, const chunkId chkId);

	void getOfstream(std::ofstream& fs, filePath fPath, const char* ext);
	void getIfstream(std::ifstream& fs, filePath fPath, const char* ext);

	// STD::FILESYSTEM
	static bool createDirs(const filePath& fp);
	static bool removeFile(const filePath& fp);
	static bool isFile(const filePath& fp);
	static bool isDir(const filePath& fp);
	static bool isExists(const filePath& fp);
	static std::vector<filePath> getFiles(const filePath& fp);

protected:
	//bool init();

	// Paths
	filePath basePath_;
	filePath configPath_;
	filePath arrayPath_;

	// Folders
	filePath indexFolder_;

private:
	static const std::string extArrayConfig;

protected:
	friend singleton<storageMgr>;
};
}		// core
}		// msdb
#endif	//_MSDB_STORAGEMGR_H_
