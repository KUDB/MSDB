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

attributeDesc::attributeDesc(attributeId id, std::string name, eleType type, compressionType compType)
	: id_(id), name_(name), type_(type), compType_(compType)
{
	this->typeSize_ = getEleSize(type);
	this->dataType_ = this->eleType2dataType(type_);
}

tinyxml2::XMLElement* attributeDesc::convertToXMLDoc(tinyxml2::XMLElement* node)
{
	node->SetAttribute(_MSDB_STR_ATTR_ID_, this->id_);
	node->SetAttribute(_MSDB_STR_ATTR_NAME_, this->name_.c_str());
	node->SetAttribute(_MSDB_STR_ATTR_TYPE_, eleTypeToString.at(this->type_));
	node->SetAttribute(_MSDB_STR_ATTR_TYPE_SIZE_, this->typeSize_);
	node->SetAttribute(_MSDB_STR_ATTR_COMP_TYPE_, compressionTypeToString(this->compType_));

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

	auto strCompType = xmlErrorHandler(node->Attribute(_MSDB_STR_ATTR_COMP_TYPE_));
	auto compType = compressionType::NONE;

	for (int i = 0; i < sizeof(compressionTypeStrings) / sizeof(const char*); ++i)
	{
		if (strcmp(strCompType, compressionTypeStrings[i]) == 0)
		{
			compType = (compressionType)i;
			break;
		}
	}

	return std::make_shared<attributeDesc>(id, name, attrType, compType);
}

std::string attributeDesc::toString()
{
	std::stringstream ss;
	ss << "id: " << this->id_ << ", name: " << this->name_ << ", type: " << eleTypeToString.at(this->type_);
	ss << ", comp type: " << compressionTypeToString(this->compType_);
	return ss.str();
}

bool attributeDesc::operator==(const attributeDesc& right_)
{
	if (this->id_ != right_.id_) return false;
	if (this->name_ != right_.name_) return false;
	if (this->type_ != right_.type_) return false;
	if (this->typeSize_ != right_.typeSize_) return false;
	if (this->compType_ != right_.compType_) return false;

	return true;
}
dataType attributeDesc::eleType2dataType(eleType eTy)
{
	switch (type_)
	{
	case eleType::BOOL:
	{
		return concreteTy<bool>();
	}
	case eleType::CHAR:
	case eleType::INT8:
	{
		return concreteTy<int8_t>();
	}
	case eleType::INT16:
	{
		return concreteTy<int16_t>();
	}
	case eleType::INT32:
	{
		return concreteTy<int32_t>();
	}
	case eleType::INT64:
	{
		return concreteTy<int64_t>();
	}
	case eleType::UINT8:
	{
		return concreteTy<uint8_t>();
	}
	case eleType::UINT16:
	{
		return concreteTy<uint16_t>();
	}
	case eleType::UINT32:
	{
		return concreteTy<uint32_t>();
	}
	case eleType::UINT64:
	{
		return concreteTy<uint64_t>();
	}
	case eleType::FLOAT:
	{
		return concreteTy<float>();
	}
	case eleType::DOUBLE:
	{
		return concreteTy<double>();
	}
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
}
}		// core
}		// msdb