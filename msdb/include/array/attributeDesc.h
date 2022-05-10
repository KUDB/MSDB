﻿#pragma once
#ifndef _MSDB_ATTRIBUTEDESC_H_
#define _MSDB_ATTRIBUTEDESC_H_

#include <pch.h>
#include <array/attributeId.h>
#include <util/element.h>
#include <util/dataType.h>
#include <compression/compressionType.h>
#include <compression/materializedType.h>
//#include <array/chunkType.h>
#include <xml/tinyxml2.h>

namespace msdb
{
namespace core
{
#define _MSDB_STR_ATTRIBUTE_DESCS_	"ATTRIBUTE_DESCRIPTIONS"
#define _MSDB_STR_ATTR_DESC_		"ATTRIBUTE_DESCRIPTION"
#define _MSDB_STR_ATTR_ID_			"ATTRIBUTE_ID"
#define _MSDB_STR_ATTR_NAME_		"ATTRIBUTE_NAME"
#define _MSDB_STR_ATTR_TYPE_		"ATTRIBUTE_TYPE"
#define _MSDB_STR_ATTR_TYPE_SIZE_	"ATTRIBUTE_TYPE_SIZE"
#define _MSDB_STR_ATTR_COMP_TYPE_	"ATTRIBUTE_COMPRESSION_TYPE"
#define _MSDB_STR_ATTR_MAT_TYPE_	"ATTRIBUTE_MATERIALIZED_TYPE"
#define _MSDB_STR_ATTR_OPTIONAL_PARAMS_	"ATTRIBUTE_OPTIONAL_PARAMS"

// helper type for the visitor
template<class... Ts>
struct variantVisitOperator : Ts...
{
	using Ts::operator()...; 
};
// explicit deduction guide (not needed as of C++20)
template<class... Ts> 
variantVisitOperator(Ts...)->variantVisitOperator<Ts...>;

//////////////////////////////
class attributeDesc;
using pAttributeDesc = std::shared_ptr<attributeDesc>;

class attributeDescs;
using pAttributeDescs = std::shared_ptr<attributeDescs>;

class attributeDescs : public std::vector<pAttributeDesc>, public std::enable_shared_from_this<attributeDescs>
{
public:
	attributeDescs();
	attributeDescs(const attributeDescs& mit);

public:
	friend bool operator==(const attributeDescs& lhs_, const attributeDescs& rhs_);
};

bool operator==(const attributeDescs& lhs_, const attributeDescs& rhs_);
bool operator!=(const attributeDescs& lhs_, const attributeDescs& rhs_);


class attributeDesc
{
public:
	using paramType = std::map<std::string, std::string>;

public:
	// TODO::replace eleType->dataType
	attributeDesc();
	attributeDesc(const attributeId id, const std::string name, const dataType type, 
				  const materializedType matType = materializedType::FLATTEN, const compressionType compType = compressionType::NONE,
				  const paramType& optionalParams = paramType());
	attributeDesc(const attributeDesc& src);
	attributeDesc(attributeDesc&& src) noexcept;

public:
	// TODO:: set comp type at compression operator actions
	inline void setCompType(compressionType compType)
	{
		this->compType_ = compType;
	}
	inline compressionType getCompType()
	{
		return this->compType_;
	}
	/**
	 * Save/load in XML file
	 */
	tinyxml2::XMLElement* convertToXMLDoc(tinyxml2::XMLElement* node);
	static pAttributeDesc buildDescFromXML(tinyxml2::XMLElement* node);
	std::string toString();

public:
	friend void swap(attributeDesc& lhs_, attributeDesc& rhs_);
	friend bool operator==(const attributeDesc& lhs_, const attributeDesc& rhs_);

private:
	// TODO::replace eleType and erase this function
	dataType eleType2dataType(const eleType& eTy);
	eleType dataType2eleType(const dataType& dTy);

public:
	attributeId id_;
	std::string name_;
	eleType type_;				// TODO::Deprecated, replace eleType->util/dataType
	dataType dataType_;
	//std::list<chunkType> chunkTypeLineage_;		// dependency problem, chunkType->chunkFactory->chunk->attributeDesc
	std::map<std::string, std::string> optionalParams_;
	size_t typeSize_;
	compressionType compType_;	// compression chunk type, default:NONE
	materializedType matType_;	// materialized chunk type, default:flatten
};

void swap(attributeDesc& lhs_, attributeDesc& rhs_);
bool operator==(const attributeDesc& lhs_, const attributeDesc& rhs_);
bool operator!=(const attributeDesc& lhs_, const attributeDesc& rhs_);

class dataTyConvertor
{
public:
	inline eleType operator()(concreteTy<bool>) { return eleType::BOOL;	}
	inline eleType operator()(concreteTy<char>) { return eleType::CHAR; }
	inline eleType operator()(concreteTy<int8_t>) { return eleType::INT8; }
	inline eleType operator()(concreteTy<uint8_t>) { return eleType::UINT8; }
	inline eleType operator()(concreteTy<int16_t>) { return eleType::INT16; }
	inline eleType operator()(concreteTy<uint16_t>) { return eleType::UINT16; }
	inline eleType operator()(concreteTy<int32_t>) { return eleType::INT32; }
	inline eleType operator()(concreteTy<uint32_t>) { return eleType::UINT32; }
	inline eleType operator()(concreteTy<int64_t>) { return eleType::INT64; }
	inline eleType operator()(concreteTy<uint64_t>) { return eleType::UINT64; }
	inline eleType operator()(concreteTy<float>) { return eleType::FLOAT; }
	inline eleType operator()(concreteTy<double>) { return eleType::DOUBLE; }
};
}		// core
}		// msdb
#endif