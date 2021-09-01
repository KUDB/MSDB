#include <pch.h>
#include <array/arrayMgr.h>

namespace msdb
{
namespace core
{
arrayMgr::arrayMgr()
{
}

arrayMgr::~arrayMgr()
{
}

bool arrayMgr::hasAttributeIndex(arrayId arrId, attributeId attrId)
{
	assert(arrId != INVALID_ARRAY_ID);

	return this->attrIndies_.find(arrId) == this->attrIndies_.end()
		&& (this->attrIndies_[arrId]).find(attrId) == (this->attrIndies_[arrId]).end();
}

bool arrayMgr::hasDimensionIndex(arrayId arrId, dimensionId dimId)
{
	assert(arrId != INVALID_ARRAY_ID);

	return this->dimIndies_.find(arrId) != this->dimIndies_.end() &&
		this->dimIndies_[arrId].find(dimId) != this->dimIndies_[arrId].end();
}

pArrayDesc arrayMgr::getArrayDesc(arrayId arrId)
{
	assert(arrId != INVALID_ARRAY_ID);
	assert(this->arrDescs_.find(arrId) != this->arrDescs_.end());

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
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_NO_ATTR_INDEX, strMsg.c_str()));
	}

	if((this->attrIndies_[arrId]).find(attrId) == (this->attrIndies_[arrId]).end())
	{
		std::string strMsg = "No index for attribute (arrayId: " + std::to_string(arrId) + " / attributeId: " + std::to_string(attrId) + ")";
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_USER_QUERY_ERROR, MSDB_ER_NO_ATTR_INDEX, strMsg.c_str()));
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
void arrayMgr::setArrayDesc(arrayId id, pArrayDesc desc)
{
	assert(id != INVALID_ARRAY_ID);
	this->arrDescs_[id] = desc;
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
//bool arrayMgr::init()
//{
//	return true;
//}
}		// core
}		// msdb