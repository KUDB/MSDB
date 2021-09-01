#pragma once
#ifndef _MSDB_ARRAYMGR_H_
#define _MSDB_ARRAYMGR_H_

#include <pch.h>
#include <util/singleton.h>
#include <array/array.h>
#include <index/attributeIndex.h>
#include <index/dimensionIndex.h>

namespace msdb
{
namespace core
{
class arrayMgr : public singleton<arrayMgr>
{
private:
	arrayMgr();

	~arrayMgr();

public:
	bool hasAttributeIndex(arrayId arrId, attributeId attrId);
	bool hasDimensionIndex(arrayId arrId, dimensionId dimId);

	pArrayDesc getArrayDesc(arrayId arrId);
	pAttrIndex getAttributeIndex(arrayId arrId, attributeId attrId);
	pDimensionIndex getDimensionIndex(arrayId arrId, dimensionId dimId);

	void setArrayDesc(arrayId arrId, pArrayDesc desc);
	void setAttributeIndex(arrayId arrId, attributeId attrId, pAttrIndex aIndex);
	void setDimensionIndex(arrayId arrId, dimensionId dimId, pDimensionIndex dIndex);

	void flushAttributeIndex(arrayId arrId, attributeId attrId);
	void flushDimensionIndex(arrayId arrId, dimensionId dimId);

	template <class Aty_>
	pArray makeArray(const arrayId arrId, const std::string arrName,
					 std::vector<std::string> dimNames, const dimension dims, const dimension chunkDims, const dimension blockDims,
					 std::vector<std::string> attrNames, std::vector<eleType> attrTypes)
	{
		assert(dimNames.size() == dims.size() == chunkDims.size() == blockDims.size());
		assert(attrNames.size() == attrTypes.size());

		dimensionDescs dimDescs;
		for (dimensionId dId = 0; dId < dimNames.size(); ++dId)
		{
			dimDescs.push_back(std::make_shared<dimensionDesc>(dId, dimNames[dId], 0, dims[dId], chunkDims[dId], blockDims[dId]));
		}

		attributeDescs attrDescs;
		for (attributeId aId = 0; aId < attrNames.size(); ++aId)
		{
			attrDescs.push_back(std::make_shared<attributeDesc>(aId, attrNames[aId], attrTypes[aId]));
		}

		pArrayDesc arrDesc = std::make_shared<arrayDesc>(arrId, arrName.c_str(), dimDescs, attrDescs);
		std::shared_ptr<Aty_> arr = std::make_shared<Aty_>(arrDesc);

		// Register array at the arrayMgr.
		this->setArrayDesc(arrId, arrDesc);

		return arr;
	}

	template <class Aty_>
	pArray makeArray(pArrayDesc arrDesc)
	{
		std::shared_ptr<Aty_> arr = std::make_shared<Aty_>(arrDesc);

		this->setArrayDesc(arrDesc->id_, arrDesc);

		return arr;
	}

public:
	std::map<arrayId, pArrayDesc> arrDescs_;
	std::map<arrayId, std::map<attributeId, pAttrIndex>> attrIndies_;
	std::map<arrayId, std::map<dimensionId, pDimensionIndex>> dimIndies_;

protected:
	friend singleton<arrayMgr>;
};
}		// core
}		// msdb
#endif	// _MSDB_ARRAYMGR_H_