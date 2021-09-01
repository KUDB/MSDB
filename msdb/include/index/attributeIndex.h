#pragma once
#ifndef _MSDB_ATTRIBUTE_INDEX_H_
#define _MSDB_ATTRIBUTE_INDEX_H_

#include <pch.h>

namespace msdb
{
namespace core
{
enum class attrIndexType
{
	MMT,
	COMPASS,
	NONE
};

class attributeIndex
{
public:
	attributeIndex();

public:
	virtual attrIndexType getType() = 0;
};
using pAttrIndex = std::shared_ptr<attributeIndex>;
}		// core
}		// msdb
#endif	// _MSDB_ATTRIBUTE_INDEX_H_