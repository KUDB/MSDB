#include <pch.h>
#include <system/storageMgr.h>
#include <system/exceptions.h>
#include <array/arrayMgr.h>
#include <array/chunkId.h>
#include <array/configArrays.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
const char* strBasePath =	"../storage/";
const char* strConfigPath = "config/";
const char* strArrayPath =	"array/";

const char* strIndexFolder = "indies";
const char* strIndexFilExtension = ".msdbindex";

const char* strArrayConfigFile = "arrays.xml";
const char* strChunkFilExtension = ".chunk";

const std::string storageMgr::extArrayConfig = ".msdbarray";

storageMgr::storageMgr()
{
	std::cout << "current dir: " << std::filesystem::current_path() << std::endl;

	this->basePath_ = filePath(strBasePath);
	this->configPath_ = filePath(std::string(strConfigPath, strConfigPath).c_str());
	this->arrayPath_ = filePath(strArrayPath);

	this->indexFolder_ = filePath(strIndexFolder);

	this->initSystemConfig();
}

storageMgr::~storageMgr()
{
}


void storageMgr::saveArrayDesc(pArrayDesc arrDesc)
{
	auto pXmlDoc = std::make_shared<tinyxml2::XMLDocument>();
	auto pXmlEle = arrDesc->convertToXMLDoc(pXmlDoc);
	pXmlDoc->LinkEndChild(pXmlEle);

	auto path = this->getBasePath() / filePath(std::to_string(arrDesc->id_) + this->extArrayConfig);
	auto strPath = path.c_str();

	pXmlDoc->SaveFile(path.u8string().c_str());
}

void storageMgr::initSystemConfig()
{
}

std::vector<pArrayDesc> storageMgr::loadAllArrayDescs()
{
	std::vector<pArrayDesc> arrDescList;

	auto path = this->getBasePath() / filePath("*" + this->extArrayConfig);
	auto arrayFiles = this->getFiles(path);

	for (auto f : arrayFiles)
	{
		arrDescList.push_back(this->loadArrayDesc(f));
	}

	return arrDescList;
}

pArrayDesc storageMgr::loadArrayDesc(filePath descPath)
{
	auto pXmlDoc = std::make_shared<tinyxml2::XMLDocument>();
	pXmlDoc->LoadFile(descPath.u8string().c_str());

	return arrayDesc::buildDescFromXML(pXmlDoc);
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

filePath storageMgr::getBasePath()
{
	return this->basePath_;
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
std::vector<filePath> storageMgr::getFiles(filePath& fp)
{
	std::vector<filePath> out;
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(fp))
	{
		out.push_back(entry.path());
	}

	return out;
}
}		// core
}		// msdb