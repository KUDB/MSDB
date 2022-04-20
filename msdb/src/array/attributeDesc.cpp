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
//////////////////////////////
// attributeDescs
//
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

// Friends
bool operator==(const attributeDescs& lhs_, const attributeDescs& rhs_)
{
	if (lhs_.size() != rhs_.size())		return false;

	auto size = lhs_.size();
	for (int i = 0; i < size; ++i)
	{
		_MSDB_TRY_BEGIN
		{
			// shared_ptr obj
			if (lhs_.at(i) != rhs_.at(i))
			{
				if (lhs_.at(i) == nullptr || rhs_.at(i) == nullptr)	return false;
				if (*(lhs_.at(i)) != *(rhs_.at(i)))					return false;
			}
		}_MSDB_CATCH(std::out_of_range e)
		{
			return false;
		}_MSDB_CATCH_END
	}

	return true;
}
bool operator!=(const attributeDescs& lhs_, const attributeDescs& rhs_)
{
	if (lhs_ == rhs_)
		return false;

	return true;
}

//////////////////////////////
// attributeDesc
//
attributeDesc::attributeDesc()
	: id_(0), name_(""), type_(eleType::EMPTY),
	matType_(materializedType::FLATTEN), compType_(compressionType::NONE)
{
	this->typeSize_ = 0;
	this->dataType_ = concreteTy<bool>();
}

attributeDesc::attributeDesc(attributeId id, std::string name,
							 eleType type,
							 materializedType matType, compressionType compType)
	: id_(id), name_(name), type_(type), matType_(matType), compType_(compType)
{
	this->typeSize_ = getEleSize(type_);
	this->dataType_ = this->eleType2dataType(type_);
}

attributeDesc::attributeDesc(const attributeDesc& src)
	: id_(src.id_), name_(src.name_), type_(src.type_),
	matType_(src.matType_), compType_(src.compType_),
	typeSize_(src.typeSize_), dataType_(src.dataType_),
	optionalParams_(src.optionalParams_)
{

}

attributeDesc::attributeDesc(attributeDesc&& src) noexcept
	: attributeDesc()
{
	swap(*this, src);
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
	// TODO::materializedType, compressionType 
	auto matType = materializedType::FLATTEN;
	auto compType = compressionType::NONE;

	for (int i = 0; i < sizeof(compressionTypeStrings) / sizeof(const char*); ++i)
	{
		if (strcmp(strCompType, compressionTypeStrings[i]) == 0)
		{
			compType = (compressionType)i;
			break;
		}
	}

	return std::make_shared<attributeDesc>(id, name, attrType, matType, compType);
}

std::string attributeDesc::toString()
{
	std::stringstream ss;
	ss << "id: " << this->id_ << ", name: " << this->name_ << ", type: " << eleTypeToString.at(this->type_);
	ss << ", comp type: " << compressionTypeToString(this->compType_);
	return ss.str();
}

// TODO::Deprecate
dataType attributeDesc::eleType2dataType(eleType eTy)
{
	// NO Type for eleType::EMPTY
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

// ***************************
// Friends
void swap(attributeDesc& lhs_, attributeDesc& rhs_)
{
	using std::swap;

	swap(lhs_.id_, rhs_.id_);
	swap(lhs_.name_, rhs_.name_);

	swap(lhs_.type_, rhs_.type_);
	swap(lhs_.dataType_, rhs_.dataType_);
	swap(lhs_.optionalParams_, rhs_.optionalParams_);
	swap(lhs_.typeSize_, rhs_.typeSize_);
	swap(lhs_.compType_, rhs_.compType_);
	swap(lhs_.matType_, rhs_.matType_);
}

bool operator==(const attributeDesc& lhs_, const attributeDesc& rhs_)
{
	if (lhs_.id_ != rhs_.id_) return false;
	if (lhs_.name_ != rhs_.name_) return false;

	if (lhs_.type_ != rhs_.type_) return false;
	if (lhs_.dataType_.index() != rhs_.dataType_.index()) return false;
	if (lhs_.typeSize_ != rhs_.typeSize_) return false;
	if (lhs_.compType_ != rhs_.compType_) return false;
	if (lhs_.matType_ != rhs_.matType_) return false;

	if (lhs_.optionalParams_ != rhs_.optionalParams_) return false;

	return true;
}
bool operator!=(const attributeDesc& lhs_, const attributeDesc& rhs_)
{
	if (lhs_ == rhs_)
		return false;

	return true;
}
}		// core
}		// msdb