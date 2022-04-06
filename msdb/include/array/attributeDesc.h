#pragma once
#ifndef _MSDB_ATTRIBUTEDESC_H_
#define _MSDB_ATTRIBUTEDESC_H_

#include <pch.h>
#include <util/element.h>
#include <util/dataType.h>
#include <array/attributeId.h>
#include <array/chunkType.h>
#include <compression/compressionType.h>
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
	attributeDesc(attributeId id, std::string name, eleType type, compressionType compType = compressionType::NONE);

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
	std::list<chunkType> chunkTypeLineage_;
	std::map<std::string, std::string> optionalParams_;
	size_t typeSize_;
	compressionType compType_;	// compression type, default:NONE
};
}		// core
}		// msdb
#endif