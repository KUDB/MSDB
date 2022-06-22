#pragma once
#ifndef _MSDB_ARRAYDESC_H_
#define _MSDB_ARRAYDESC_H_

#include <pch.h>
#include <array/arrayId.h>
#include <array/attributeDesc.h>
#include <array/dimensionDesc.h>
#include <xml/tinyxml2.h>

namespace msdb
{
namespace core
{
#define _MSDB_STR_ARRAY_DESC_		"ARRAY_DESCRIPTION"
#define _MSDB_STR_ARR_ID_			"ARRAY_ID"
#define _MSDB_STR_ARR_NAME_			"ARRAY_NAME"
#define _MSDB_STR_ARR_TYPE_			"ARRAY_TYPE"

class arrayDesc;

using pArrayDesc = std::shared_ptr<arrayDesc>;

class arrayDesc : public std::enable_shared_from_this<arrayDesc>
{
public:
	arrayDesc();
	arrayDesc(const arrayId aid, const std::string arrayName, 
			  pDimensionDescs dimDescs, pAttributeDescs attrDescs);
	arrayDesc(const arrayDesc& src);
	arrayDesc(arrayDesc&& src) noexcept;
	~arrayDesc();

	friend void swap(arrayDesc& first, arrayDesc& second) noexcept;

public:
	pDimensionDescs getDimDescs();
	pAttributeDescs getAttrDescs();
	std::string toString(std::string strIndent = "");
	inline size_t getDSize()
	{
		return this->dimDescs_->size();
	}

	/**
	 * Save/load in XML file
	 */
	tinyxml2::XMLElement* convertToXMLDoc(std::shared_ptr<tinyxml2::XMLDocument> doc);
	static pArrayDesc buildDescFromXML(std::shared_ptr<tinyxml2::XMLDocument> doc);
	static pDimensionDescs buildDimensionDescsFromXML(tinyxml2::XMLElement*);
	static pAttributeDescs buildAttributeDescsFromXML(tinyxml2::XMLElement*);
	
public:
	//////////////////////////////
	// Operators
	// ***************************
	// Assign
	arrayDesc& operator=(const arrayDesc& src);
	arrayDesc& operator=(arrayDesc&& src) noexcept;

	// ***************************
	// Comparison
	friend bool operator == (const arrayDesc& lhs_, const arrayDesc& rhs_);

public:
	arrayId id_;
	std::string name_;

	pDimensionDescs dimDescs_;
	pAttributeDescs attrDescs_;
};

void swap(arrayDesc& first, arrayDesc& second) noexcept;
bool operator == (const arrayDesc& lhs_, const arrayDesc& rhs_);
bool operator!= (const arrayDesc& lhs_, const arrayDesc& rhs_);
}		// core
}		// msdb
#endif	// _MSDB_ARRAYDESC_H_