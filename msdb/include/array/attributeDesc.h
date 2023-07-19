#pragma once
#ifndef _MSDB_ATTRIBUTEDESC_H_
#define _MSDB_ATTRIBUTEDESC_H_

#include <pch.h>
#include <array/attributeId.h>
#include <util/element.h>
#include <util/dataType.h>
#include <encoding/encodingType.h>
#include <compression/materializedType.h>
#include <system/exceptions.h>
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

std::pair<std::string, std::string> make_attr_param(std::string key, std::string value);

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
				  const materializedType matType = materializedType::FLATTEN, const encodingType compType = encodingType::NONE,
				  const paramType& optionalParams = paramType());
	attributeDesc(const attributeDesc& src);
	attributeDesc(attributeDesc&& src) noexcept;

public:
	// TODO:: set comp type at compression operator actions
	/**
	 * Getter 
	 */
	inline const paramType& getOptionalParams() const
	{
		return this->optionalParams_;
	}
	inline void setParam(std::string key, std::string value)
	{
		this->optionalParams_[key] = value;
	}

	inline const std::string& getParam(const std::string& key) const
	{
		if (this->optionalParams_.find(key) == this->optionalParams_.end())
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NO_PARAM, std::string("AttributeDesc: ") + key));
			return "";
		}
		return this->optionalParams_.at(key);
	}

	inline const attributeId& getId() const
	{
		return this->id_;
	}
	inline const std::string& getName() const
	{
		return this->name_;
	}
	inline const dataType& getDataType() const
	{
		return this->dataType_;
	}
	inline const int getCompType() const
	{
		return this->compType_;
	}
	inline const materializedType& getMatType() const
	{
		return this->matType_;
	}

	/**
	 * Setter
	 */
	inline void setCompType(int compType)
	{
		this->compType_ = compType;
	}
	inline attributeId& setId(const attributeId attrId)
	{
		return this->id_ = attrId;
	}

	/**
	 * Save/load in XML file
	 */
	tinyxml2::XMLElement* convertToXMLDoc(tinyxml2::XMLElement* node);
	static pAttributeDesc buildDescFromXML(tinyxml2::XMLElement* node);
	std::string toString() const;

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
	eleType type_;				// TODO::eleType is now deprecated, replace eleType->util/dataType
	dataType dataType_;			// TODO::Move the variable to private section
	//std::list<chunkType> chunkTypeLineage_;		// dependency problem, chunkType->chunkFactory->chunk->attributeDesc
	paramType optionalParams_;
	size_t typeSize_;
	int compType_;				// compression chunk type, default:NONE
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