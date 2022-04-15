#pragma once
#ifndef _MSDB_CPP_ARRAY_H_
#define _MSDB_CPP_ARRAY_H_

#include <pch.h>
#include <compression/compressionType.h>
#include <compression/materializedType.h>
#include <array/array.h>
#include <query/query.h>
#include <api_cpp/cpp_context.h.>
#include <index/attributeIndex.h>

namespace msdb
{
using typename core::eleType;
using typename core::compressionType;
using typename core::materializedType;
using typename core::attrIndexType;

class Array
{
public:
	Array(Context context, core::arrayId arrId);
	Array(Context context, std::string arrName);

public:
	inline core::pArrayDesc getDesc()
	{
		return this->arrDesc_;
	}

private:
	Context context_;
	core::pArrayDesc arrDesc_;
};

class ResultArray
{
public:
	ResultArray(Context context, core::pQuery qry);

	std::shared_ptr<std::vector<core::coor>> getDimBuffer();
	template <typename Ty_>
	std::shared_ptr<std::vector<Ty_>> getAttrBuffer(core::attributeId attrId)
	{
		return std::static_pointer_cast<std::vector<Ty_>>(this->qry_->getAttrBuffer<Ty_>(attrId));
	}
	inline core::pQuery getQuery()
	{
		return this->qry_;
	}
	inline core::pArrayDesc getArrayDesc()
	{
		return this->arrDesc_;
	}

public:
	void close();

private:
	Context context_;
	core::pArrayDesc arrDesc_;
	core::pQuery qry_;
};

void printResultArray(ResultArray& ra);

class DefDimension
{
public:
	DefDimension(std::string name, 
			  uint64_t start, uint64_t end, 
			  uint64_t chunkSize, uint64_t blockSize);

public:
	std::shared_ptr<core::dimensionDesc> getDesc();

private:
	std::shared_ptr<core::dimensionDesc> dimDesc_;
};

class DefAttribute
{
public:
	DefAttribute(std::string name, eleType type, 
				 materializedType matType = materializedType::FLATTEN,
				 compressionType compType = compressionType::NONE);

public:
	std::shared_ptr<core::attributeDesc> getDesc();

	operator core::attributeDesc();

private:
	std::shared_ptr<core::attributeDesc> attrDesc_;
};
}		// msdb
#endif	// _MSDB_CPP_ARRAY_H_