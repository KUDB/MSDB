#include <pch.h>
#include <array/configArrays.h>
#include <system/exceptions.h>

namespace msdb
{
namespace core
{
ConfigType configDiemnsion::getType()
{
	return ConfigType::DIMENSION;
}

void configDiemnsion::getDimensionDesc(pDimensionDesc desc)
{
	auto eleId = this->_root->FirstChildElement(_MSDB_XML_ELE_ID);
	desc->id_ = atoi(eleId->ToText()->Value());
	auto eleName = this->_root->FirstChildElement(_MSDB_XML_ELE_NAME);
	desc->name_ = eleName->ToText()->Value();
	auto eleStart = this->_root->FirstChildElement(_MSDB_XML_ELE_START);
	desc->start_ = atoi(eleId->ToText()->Value());
	auto eleEnd = this->_root->FirstChildElement(_MSDB_XML_ELE_END);
	desc->end_ = atoi(eleId->ToText()->Value());
}

ConfigType configAttribute::getType()
{
	return ConfigType::ATTRIBUTE;
}

void configAttribute::getAttributeDesc(pAttributeDesc desc)
{
	auto eleId = this->_root->FirstChildElement(_MSDB_XML_ELE_ID);
	desc->id_ = atoi(eleId->ToText()->Value());
	auto eleName = this->_root->FirstChildElement(_MSDB_XML_ELE_NAME);
	desc->name_ = eleName->ToText()->Value();
	//auto eleType = this->_root->FirstChildElement(_MSDB_XML_ELE_TYPE);
	//aDesc->_type
}

ConfigType configArray::getType()
{
	return ConfigType::ARRAY;
}

void configArray::getArrayDesc(pArrayDesc aDesc)
{
	auto dimDesc = this->_root->FirstChildElement(_MSDB_XML_DIMENSIONDESC);
	this->xmlChildExplore(dimDesc, reinterpret_cast<void*>(&(aDesc->dimDescs_)), &configjobs::getDimensionDescList);

	auto attrDesc = this->_root->FirstChildElement(_MSDB_XML_ATTRIBUTEDESC);
	this->xmlChildExplore(attrDesc, reinterpret_cast<void*>(&(aDesc->attrDescs_)), &configjobs::getAttributeDescList);
}

ConfigType configArrayList::getType()
{
	return ConfigType::ARRAYLIST;
}

void configArrayList::getArrayDescList(arrayDescs* list)
{
	this->xmlChildExplore(this->_root, reinterpret_cast<void*>(list), &configjobs::getArrayDesc);
}

namespace configjobs
{
void getArrayDesc(tinyxml2::XMLNode* root, void* list)
{
	_MSDB_TRY_BEGIN
		arrayDescs* aList = reinterpret_cast<arrayDescs*>(list);
	pArrayDesc aDesc;
	configArray(root).getArrayDesc(aDesc);
	aList->push_back(aDesc);
	_MSDB_CATCH_ALL
		_MSDB_RETHROW
		_MSDB_CATCH_END
}

void getAttributeDescList(tinyxml2::XMLNode* root, void* list)
{
	_MSDB_TRY_BEGIN
		dimensionDescs* dims = reinterpret_cast<dimensionDescs*>(list);
	pDimensionDesc dDesc;
	configDiemnsion(root).getDimensionDesc(dDesc);
	dims->push_back(dDesc);
	_MSDB_CATCH_ALL
		_MSDB_RETHROW
		_MSDB_CATCH_END
}

void getDimensionDescList(tinyxml2::XMLNode* root, void* list)
{
	_MSDB_TRY_BEGIN
	{
		attributeDescs * attrs = reinterpret_cast<attributeDescs*>(list);
		pAttributeDesc aDesc;
		configAttribute(root).getAttributeDesc(aDesc);
		attrs->push_back(aDesc);
	}
		_MSDB_CATCH_ALL
	{
		_MSDB_RETHROW
	}
		_MSDB_CATCH_END
}
}
}		// core
}		// msdb