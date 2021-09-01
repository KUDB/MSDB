#pragma once
#ifndef _MSDB_ARRAYDESC_H_
#define _MSDB_ARRAYDESC_H_

#include <pch.h>
#include <array/arrayId.h>
#include <array/attributeDesc.h>
#include <array/dimensionDesc.h>

namespace msdb
{
namespace core
{
class arrayDesc;

using pArrayDesc = std::shared_ptr<arrayDesc>;

class arrayDesc : public std::enable_shared_from_this<arrayDesc>
{
public:
	arrayDesc();
	arrayDesc(const arrayId aid, const std::string arrayName, 
			  pDimensionDescs dimDescs, pAttributeDescs attrDescs);
	arrayDesc(const arrayDesc& mit);
	~arrayDesc();

public:
	pDimensionDescs getDimDescs();
	pAttributeDescs getAttrDescs();

	size_t getDSize();

public:
	arrayId id_;
	std::string name_;

	pDimensionDescs dimDescs_;
	pAttributeDescs attrDescs_;
};
}		// core
}		// msdb
#endif	// _MSDB_ARRAYDESC_H_