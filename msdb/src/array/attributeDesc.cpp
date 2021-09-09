#include <pch.h>
#include <typeinfo>
#include <array/attributeDesc.h>
#include <util/element.h>
#include <system/exceptions.h>
#include <xml/xmlFile.h>

namespace msdb
{
namespace core
{
attributeDesc::attributeDesc(attributeId id, std::string name, eleType type)
	: id_(id), name_(name), type_(type)
{
	this->typeSize_ = getEleSize(type);
}

attributeDescs::attributeDescs()
{
}

attributeDescs::attributeDescs(const attributeDescs& mit)
{
	for (auto desc : mit)
	{
		this->push_back(std::make_shared<attributeDesc>(*desc));
	}
}

tinyxml2::XMLElement* attributeDesc::convertToXMLDoc(tinyxml2::XMLElement* node)
{
	node->SetAttribute(_MSDB_STR_ATTR_ID_, this->id_);
	node->SetAttribute(_MSDB_STR_ATTR_NAME_, this->name_.c_str());
	node->SetAttribute(_MSDB_STR_ATTR_TYPE_, eleTypeToString.at(this->type_));
	node->SetAttribute(_MSDB_STR_ATTR_TYPE_SIZE_, this->typeSize_);

	return node;
}

pAttributeDesc attributeDesc::buildDescFromXML(tinyxml2::XMLElement* node)
{
	auto id = node->IntAttribute(_MSDB_STR_ATTR_ID_);
	auto name = xmlErrorHandler(node->Attribute(_MSDB_STR_ATTR_NAME_));
	auto eleTypeName = std::string(xmlErrorHandler(node->Attribute(_MSDB_STR_ATTR_TYPE_)));
	auto typeSize = node->IntAttribute(_MSDB_STR_ATTR_TYPE_SIZE_);
	core::eleType attrType;

	for (auto bucket : eleTypeToString)
	{
		if (strcmp(bucket.second, eleTypeName.c_str()) == 0)
		{
			attrType = bucket.first;
			break;
		}
	}

	return std::make_shared<attributeDesc>(id, name, attrType);
}

std::string attributeDesc::toString()
{
	std::stringstream ss;
	ss << "id: " << this->id_ << ", name: " << this->name_ << ", type: " << eleTypeToString.at(this->type_);
	return ss.str();
}

bool attributeDesc::operator==(const attributeDesc& right_)
{
	if (this->id_ != right_.id_) return false;
	if (this->name_ != right_.name_) return false;
	if (this->type_ != right_.type_) return false;
	if (this->typeSize_ != right_.typeSize_) return false;

	return true;
}
}		// core
}		// msdb