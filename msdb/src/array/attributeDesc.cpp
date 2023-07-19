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

std::pair<std::string, std::string> make_attr_param(std::string key, std::string value)
{
	return std::make_pair<>(key, value);
}
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
	matType_(materializedType::FLATTEN), compType_(encodingType::NONE)
{
	this->typeSize_ = 0;
	this->dataType_ = concreteTy<bool>();
}

attributeDesc::attributeDesc(const attributeId id, const std::string name, 
							 const dataType type, const materializedType matType, const encodingType compType, 
							 const paramType& optionalParams)
	: id_(id), name_(name), dataType_(type), matType_(matType), compType_(compType), optionalParams_(optionalParams)
{
	this->type_ = this->dataType2eleType(type);
	this->typeSize_ = getEleSize(type_);
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
	node->SetAttribute(_MSDB_STR_ATTR_TYPE_, reinterpret_cast<iTy*>(&this->dataType_)->toString().c_str());
	node->SetAttribute(_MSDB_STR_ATTR_MAT_TYPE_, materializedTypeToString(this->matType_));
	node->SetAttribute(_MSDB_STR_ATTR_COMP_TYPE_, encodingTypeToString(this->compType_));
	
	tinyxml2::XMLElement* paramNode = node->GetDocument()->NewElement(_MSDB_STR_ATTR_OPTIONAL_PARAMS_);
	for (auto it = this->optionalParams_.begin(); it != this->optionalParams_.end(); ++it)
	{
		auto param = *it;
		paramNode->SetAttribute(param.first.c_str(), param.second.c_str());
	}
	node->InsertEndChild(paramNode);

	return node;
}

pAttributeDesc attributeDesc::buildDescFromXML(tinyxml2::XMLElement* node)
{
	auto id = node->IntAttribute(_MSDB_STR_ATTR_ID_);
	auto name = xmlErrorHandler(node->Attribute(_MSDB_STR_ATTR_NAME_));
	auto strDataType = std::string(xmlErrorHandler(node->Attribute(_MSDB_STR_ATTR_TYPE_)));
	dataType dataType = string2dataType(strDataType);

	auto strCompType = xmlErrorHandler(node->Attribute(_MSDB_STR_ATTR_COMP_TYPE_));
	auto strMatType = xmlErrorHandler(node->Attribute(_MSDB_STR_ATTR_MAT_TYPE_));
	auto matType = materializedType::FLATTEN;		// Set default
	auto compType = encodingType::NONE;			// Set default
	for (int i = 0; i < sizeof(materializedTypeStrings) / sizeof(const char*); ++i)
	{
		if (strcmp(strMatType, materializedTypeStrings[i]) == 0)
		{
			matType = (materializedType)i;
			break;
		}
	}
	for (int i = 0; i < sizeof(encodingTypeStrings) / sizeof(const char*); ++i)
	{
		if (strcmp(strCompType, encodingTypeStrings[i]) == 0)
		{
			compType = (encodingType)i;
			break;
		}
	}

	attributeDesc::paramType optionalParam;
	auto paramNode = node->FirstChildElement(_MSDB_STR_ATTR_OPTIONAL_PARAMS_);
	const tinyxml2::XMLAttribute* paramAttr;
	paramAttr = paramNode->FirstAttribute();
	while (paramAttr != nullptr)
	{
		optionalParam.insert(
			std::make_pair<std::string, std::string>(
				std::string(paramAttr->Name()), std::string(paramAttr->Value())));
		paramAttr = paramAttr->Next();
	}


	return std::make_shared<attributeDesc>(id, name, dataType, matType, compType, optionalParam);
}

std::string attributeDesc::toString() const
{
	std::stringstream ss;
	ss << "id: " << this->id_ << ", name: " << this->name_ << ", type: " << eleTypeToString.at(this->type_);
	ss << ", comp type: " << encodingTypeToString(this->compType_);
	return ss.str();
}

// TODO::Deprecate
dataType attributeDesc::eleType2dataType(const eleType& eTy)
{
	// NO Type for eleType::EMPTY
	switch (eTy)
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

eleType attributeDesc::dataType2eleType(const dataType& dTy)
{
	return std::visit(dataTyConvertor(), dTy);
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