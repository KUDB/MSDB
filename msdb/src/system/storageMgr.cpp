#include <pch.h>
#include <system/storageMgr.h>
#include <system/exceptions.h>
#include <array/chunkId.h>
#include <array/configArrays.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
const char* strBasePath =	"../storage/";
const char* strConfigPath = "../storage/config/";
const char* strArrayPath =	"../storage/array/";

const char* strIndexFolder = "indies";
const char* strIndexFilExtension = ".msdbindex";

const char* strArrayConfigFile = "arrays.xml";
const char* strChunkFilExtension = ".chunk";

//void msdb::core::storageMgr::getConfigFile(std::string path)
//{
//	std::fstream fs;
//	fs.open(path, std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);
//
//	fs.close();
//	return;
//}

storageMgr::storageMgr()
{
	std::cout << "current dir: " << std::filesystem::current_path() << std::endl;

	this->basePath_ = filePath(strBasePath);
	this->configPath_ = filePath(strConfigPath);
	this->arrayPath_ = filePath(strArrayPath);

	this->indexFolder_ = filePath(strIndexFolder);
}

storageMgr::~storageMgr()
{
}

config* storageMgr::loadConfigFile(ConfigType type)
{
	tinyxml2::XMLDocument xmlDoc;

	switch (type)
	{
	case ConfigType::ARRAYLIST:
		xmlDoc.LoadFile(strArrayConfigFile);
		return new configArrayList(xmlDoc.FirstChild());
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_UNKNOWN_ERROR, MSDB_ER_UNKNOWN_ERROR));
	}

	return nullptr;
}

void storageMgr::saveConfigFile(config* cFile)
{
	switch(cFile->getType())
	{
	case ConfigType::ARRAYLIST:
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_UNKNOWN_ERROR, MSDB_ER_UNKNOWN_ERROR));
	}
}

void storageMgr::loadAttrIndex(arrayId arrId, attributeId attrId, pSerializable serialObj)
{
	std::ifstream fs;
	this->getIfstream(fs, this->getArrayIndexPath(arrId) / std::to_string(attrId),
					  strIndexFilExtension);
	serialObj->deserialize(fs);
	fs.close();
}

void storageMgr::saveAttrIndex(arrayId arrId, attributeId attrId, pSerializable serialObj)
{
	std::ofstream fs;
	this->getOfstream(fs, this->getArrayIndexPath(arrId) / std::to_string(attrId), 
					  strIndexFilExtension);
	serialObj->serialize(fs);
	BOOST_LOG_TRIVIAL(info) << "Save Attribute Index: " << serialObj->getSerializedSize() << " Bytes";
	fs.close();
}

void storageMgr::loadChunk(arrayId arrId, attributeId attrId, chunkId chkId, pSerializable serialObj)
{
	_MSDB_TRY_BEGIN
	{
		std::ifstream fs;
		this->getIfstream(fs, this->getChunkPath(arrId, attrId, chkId),
						  strChunkFilExtension);
		serialObj->deserialize(fs);
#ifndef NDEBUG
		BOOST_LOG_TRIVIAL(trace) << "Load Chunk[" << chkId << "] : " << serialObj->getSerializedSize() << " Bytes" << std::endl;
#endif

		fs.close();
	}
	_MSDB_CATCH(msdb_exception msex)
	{
		if(msex._error_category == MSDB_EC_IO_ERROR)
		{
			if(msex._error_code == MSDB_ER_CANNOT_OPEN_FILE)
			{
				// No chunk file, pass chunk
				BOOST_LOG_TRIVIAL(error) << "CATCH:: Load Chunk[" << chkId << "] : EMPTY";
			}
		}else
		{
			BOOST_LOG_TRIVIAL(error) << "CATCH:: Load Chunk[" << chkId << "] : MSDB EXCEPTION";
		}

		return;
	}
	_MSDB_CATCH_ALL
	{
		BOOST_LOG_TRIVIAL(error) << "CATCH:: Load Chunk[" << chkId << "] : EXCEPTION";
	}
	_MSDB_CATCH_END
}

// StorageMgr do not know the struct of chunk.
// Instead, it can deal with serializable objects.
// So, we seperate 'pChunk' into 'chunkId' and 'pSerializable'.
void storageMgr::saveChunk(arrayId arrId, attributeId attrId, chunkId chkId, pSerializable serialObj)
{
	_MSDB_TRY_BEGIN
	{
		std::ofstream fs;
		this->getOfstream(fs, this->getChunkPath(arrId, attrId, chkId),
						  strChunkFilExtension);
		serialObj->serialize(fs);
#ifndef NDEBUG
		BOOST_LOG_TRIVIAL(trace) << "Save Chunk[" << chkId << "] : " << serialObj->getSerializedSize() << " Bytes";
#endif
		fs.close();
	}
	_MSDB_CATCH_ALL
	{
		BOOST_LOG_TRIVIAL(error) << "Save Chunk[" << chkId << "] : EXCEPTION";

		return;
	}
	_MSDB_CATCH_END
}

filePath storageMgr::getArrayPath(arrayId arrId)
{
	return this->arrayPath_ / this->getArrayFolder(arrId);
}

filePath storageMgr::getArrayFolder(arrayId arrId)
{
	return filePath(std::to_string(arrId));
}

filePath storageMgr::getArrayIndexPath(arrayId arrId)
{
	return this->getArrayPath(arrId) / this->indexFolder_;
}

filePath storageMgr::getChunkPath(arrayId arrId, attributeId attrId, chunkId chkId)
{
	return this->getArrayPath(arrId) / std::to_string(attrId) / std::to_string(chkId);
}

void storageMgr::getOfstream(std::ofstream& fs, filePath fPath, const char* ext)
{
	fPath.replace_extension(ext);
	filePath dir = fPath.parent_path();
	if (!this->isExists(dir))
	{
		this->createDirs(dir);
	}

	fs.open(fPath, std::fstream::in | std::fstream::trunc | std::fstream::binary);
	if (!fs.is_open())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_IO_ERROR, MSDB_ER_CANNOT_OPEN_FILE, fPath.generic_string().c_str()));
	}
}

void storageMgr::getIfstream(std::ifstream& fs, filePath fPath, const char* ext)
{
	fPath.replace_extension(ext);
	fs.open(fPath, std::fstream::out | std::fstream::binary);
	if (!fs.is_open())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_IO_ERROR, MSDB_ER_CANNOT_OPEN_FILE, fPath.generic_string().c_str()));
	}
}

bool storageMgr::createDirs(filePath& fp)
{
	return std::filesystem::create_directories(fp);
}

bool storageMgr::removeFile(filePath& fp)
{
	return std::filesystem::remove(fp);
}

bool storageMgr::isFile(filePath& fp)
{
	return std::filesystem::is_regular_file(fp);
}

bool storageMgr::isDir(filePath& fp)
{
	return std::filesystem::is_directory(fp);
}

bool storageMgr::isExists(filePath& fp)
{
	return std::filesystem::exists(fp);
}
}		// core
}		// msdb