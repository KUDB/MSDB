#pragma once
#ifndef _MSDB_CONFIGARRAYS_H_
#define _MSDB_CONFIGARRAYS_H_

#include <pch.h>
#include <array/arrayDesc.h>
#include <array/attributeDesc.h>
#include <array/dimensionDesc.h>
#include <system/config.h>

// Meta data for arrays is recorded in XML file at the config folder.

//	Array name
//	DimensionDesc
//		Dimension size
//		Dimensions
//			Dimension
//				Dimension name
//				Dimension type
//				Start
//				End
//				Chunk size
//	AttributeDesc
//		Attribute size
//		Attributes
//			Attribute
//				Attribute name
//				Attribute type

namespace msdb
{
namespace core
{
#define _MSDB_XML_ARRAYDESC			"array_desc"
#define _MSDB_XML_ARRAYS			"arrays"

#define _MSDB_XML_DIMENSIONDESC		"dimension_desc"
#define _MSDB_XML_DIMENSIONS		"dimensions"

#define _MSDB_XML_ATTRIBUTEDESC		"attribute_desc"
#define _MSDB_XML_ATTRIBUTES		"attributes"

using arrayDescs = std::vector<pArrayDesc>;

namespace configjobs
{
// config array
void getAttributeDescList(tinyxml2::XMLNode* root, void* list);
//void setAttributeDescList(attributes* list);

void getDimensionDescList(tinyxml2::XMLNode* root, void* list);
//void setDimensionDescList(dimensions* list);

// configArrayList
void getArrayDesc(tinyxml2::XMLNode* root, void* list);
}

class configDiemnsion : public config
{
public:
	configDiemnsion(tinyxml2::XMLNode* root) : config(root) {}
	ConfigType getType();

	void getDimensionDesc(pDimensionDesc desc);
	void setDimensionDesc(pDimensionDesc desc);
};

class configAttribute : public config
{
public:
	configAttribute(tinyxml2::XMLNode* root) : config(root) {}
	ConfigType getType();

	void getAttributeDesc(pAttributeDesc desc);
	void setAttributeDesc(pAttributeDesc desc);
};


class configArray : public config
{
public:
	configArray(tinyxml2::XMLNode* root) : config(root) {}
	ConfigType getType();
	
	void getArrayDesc(pArrayDesc desc);
	void setArrayDesc(pArrayDesc desc);
};

class configArrayList : public config
{
public:
	configArrayList(tinyxml2::XMLNode* root) : config(root) {}
	ConfigType getType();

	void getArrayDescList(arrayDescs* list);
	void setArrayDescList(arrayDescs* list);
};
}		// core
}		// msdb
#endif	// _MSDB_CONFIGARRAYS_H_