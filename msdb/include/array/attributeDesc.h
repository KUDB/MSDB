#pragma once
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

class attributeDesc;
using pAttributeDesc = std::shared_ptr<attributeDesc>;

class attributeDescs;
using pAttributeDescs = std::shared_ptr<attributeDescs>;

class attributeDescs : public std::vector<pAttributeDesc>, public std::enable_shared_from_this<attributeDescs>
{
public:
	attributeDescs();
	attributeDescs(const attributeDescs& mit);
};

class attributeDesc
{
public:
	// TODO::replace eleType->dataType
	attributeDesc(attributeId id, std::string name, eleType type, 
				  materializedType matType = materializedType::FLATTEN, compressionType compType = compressionType::NONE);

	// TODO::make copy constructor for attributeDesc
	// TODO::make assign operator for attributeDescs

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
	bool operator == (const attributeDesc& right_);

private:
	// TODO::replace eleType and erase this function
	dataType eleType2dataType(eleType eTy);

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
}		// core
}		// msdb
#endif