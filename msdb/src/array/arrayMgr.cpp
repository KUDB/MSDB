#include <pch.h>
#include <array/arrayMgr.h>
#include <system/storageMgr.h>

namespace msdb
{
namespace core
{
arrayMgr::arrayMgr()
{
	this->loadAllArrayDesc();
}

arrayMgr::~arrayMgr()
{
	this->saveAllArrayDesc();
}

bool arrayMgr::hasAttributeIndex(arrayId arrId, attributeId attrId)
{
	if (arrId == INVALID_ARRAY_ID || !this->attrIndies_.count(arrId))
	{
		BOOST_LOG_TRIVIAL(warning) << "Array Mgr doesn't have an attribute index where the array id is " << std::to_string(arrId) << ".";
		return false;
	}

	return this->attrIndies_.find(arrId) == this->attrIndies_.end()
		&& (this->attrIndies_[arrId]).find(attrId) == (this->attrIndies_[arrId]).end();
}

bool arrayMgr::hasDimensionIndex(arrayId arrId, dimensionId dimId)
{
	if (arrId == INVALID_ARRAY_ID || !this->dimIndies_.count(arrId))
	{
		BOOST_LOG_TRIVIAL(warning) << "Array Mgr doesn't have a dimension index where the array id is " << arrId << ".";
		return false;
	}

	return this->dimIndies_.find(arrId) != this->dimIndies_.end() &&
		this->dimIndies_[arrId].find(dimId) != this->dimIndies_[arrId].end();
}

arrayId arrayMgr::getArrayId(std::string arrName)
{
	if (!this->arrNames_.count(arrName))
	{
		BOOST_LOG_TRIVIAL(warning) << "Array Mgr doesn't have an array named " << arrName << ".";
		return INVALID_ARRAY_ID;
	}

	return this->arrNames_[arrName];
}

pArrayDesc arrayMgr::getArrayDesc(arrayId arrId)
{
	if (arrId == INVALID_ARRAY_ID || !this->arrDescs_.count(arrId))
	{
		BOOST_LOG_TRIVIAL(warning) << "Array Mgr doesn't have an array desc where the array id is " << arrId << ".";
	}

	return this->arrDescs_[arrId];
}
pAttrIndex arrayMgr::getAttributeIndex(arrayId arrId, attributeId attrId)
{
	assert(arrId != INVALID_ARRAY_ID);
	//assert(this->attrIndies_.find(arrId) != this->attrIndies_.end());
	//assert(this->attrIndies_[arrId].find(attrId) != this->attrIndies_[arrId].end());	// nextWork attributeId exist

	if(this->attrIndies_.find(arrId) == this->attrIndies_.end())
	{
		std::string strMsg = "No index for array (arrayId: " + std::to_string(arrId) + ")";
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NO_ATTR_INDEX, strMsg.c_str()));
	}

	if((this->attrIndies_[arrId]).find(attrId) == (this->attrIndies_[arrId]).end())
	{
		std::string strMsg = "No index for attribute (arrayId: " + std::to_string(arrId) + " / attributeId: " + std::to_string(attrId) + ")";
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NO_ATTR_INDEX, strMsg.c_str()));
	}

	return (this->attrIndies_[arrId])[attrId];
}
pDimensionIndex arrayMgr::getDimensionIndex(arrayId arrId, dimensionId dimId)
{
	assert(arrId != INVALID_ARRAY_ID);
	assert(this->dimIndies_.find(arrId) != this->dimIndies_.end());	// Check arrayId exist
	assert(this->dimIndies_[arrId].find(dimId) != this->dimIndies_[arrId].end());	// nextWork dimensionId exist

	return (this->dimIndies_[arrId])[dimId];
}
void arrayMgr::setArrayDesc(arrayId arrId, pArrayDesc desc)
{
	assert(arrId != INVALID_ARRAY_ID);
	this->arrDescs_[arrId] = desc;
}
void arrayMgr::setArrayName(std::string arrName, arrayId arrId)
{
	assert(arrId != INVALID_ARRAY_ID);
	this->arrNames_[arrName] = arrId;
}
void arrayMgr::registArray(pArrayDesc arrDesc)
{
	this->setArrayDesc(arrDesc->id_, arrDesc);
	this->setArrayName(arrDesc->name_, arrDesc->id_);
}
void arrayMgr::removeArray(arrayId arrId)
{
	assert(arrId != INVALID_ARRAY_ID);

	if (this->arrDescs_.count(arrId))
	{
		auto arrDesc = this->arrDescs_[arrId];

		this->arrNames_.erase(arrDesc->name_);
		this->attrIndies_.erase(arrId);
		this->dimIndies_.erase(arrId);

		this->arrDescs_.erase(arrId);
	}

	storageMgr::instance()->removeArrayFiles(arrId);
}
void arrayMgr::setAttributeIndex(arrayId id, attributeId attrId, pAttrIndex aIndex)
{
	assert(id != INVALID_ARRAY_ID);
	(this->attrIndies_[id])[attrId] = aIndex;
}
void arrayMgr::setDimensionIndex(arrayId id, dimensionId dimId, pDimensionIndex dIndex)
{
	assert(id != INVALID_ARRAY_ID);
	(this->dimIndies_[id])[dimId] = dIndex;
}
void arrayMgr::flushAttributeIndex(arrayId arrId, attributeId attrId)
{
	assert(arrId != INVALID_ARRAY_ID);
	(this->attrIndies_[arrId]).erase(attrId);
}
void arrayMgr::flushDimensionIndex(arrayId arrId, dimensionId dimId)
{
	assert(arrId != INVALID_ARRAY_ID);
	(this->dimIndies_[arrId]).erase(dimId);
}
void arrayMgr::saveAllArrayDesc()
{
	for (auto& arr : this->arrDescs_)
	{
		storageMgr::instance()->saveArrayDesc(arr.second);
	}
}
void arrayMgr::loadAllArrayDesc()
{
	auto arrDescList = storageMgr::instance()->loadAllArrayDescs();

	for (auto& desc : arrDescList)
	{
		this->registArray(desc);
	}
}
void arrayMgr::saveArrayDesc(arrayId arrId)
{
	storageMgr::instance()->saveArrayDesc(this->getArrayDesc(arrId));
}
//bool arrayMgr::init()
//{
//	return true;
//}
}		// core
}		// msdb