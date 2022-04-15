#include <pch.h>
#include <array/arrayDesc.h>
#include <xml/xmlFile.h>

namespace msdb
{
namespace core
{
arrayDesc::arrayDesc()
{
	// TODO::
}

arrayDesc::arrayDesc(const arrayId aid, const std::string arrayName,
					 pDimensionDescs dimDescs,
					 pAttributeDescs attrDescs)
	: id_(aid), name_(arrayName), dimDescs_(dimDescs), attrDescs_(attrDescs)
{
}

arrayDesc::arrayDesc(const arrayDesc& mit)
	: id_(mit.id_), name_(mit.name_)
{
	this->dimDescs_ = std::make_shared<dimensionDescs>();
	this->attrDescs_ = std::make_shared<attributeDescs>();

	for (size_t i = 0; i < mit.dimDescs_->size(); i++)
	{
		this->dimDescs_->push_back(std::make_shared<dimensionDesc>(*mit.dimDescs_->at(i)));
	}

	for (size_t i = 0; i < mit.attrDescs_->size(); i++)
	{
		this->attrDescs_->push_back(std::make_shared<attributeDesc>(*mit.attrDescs_->at(i)));
	}
}

arrayDesc::arrayDesc(arrayDesc&& src) noexcept
	: arrayDesc()
{
	swap(*this, src);
}

arrayDesc::~arrayDesc()
{
	// TODO::
}

pDimensionDescs arrayDesc::getDimDescs()
{
	return this->dimDescs_;
}

pAttributeDescs arrayDesc::getAttrDescs()
{
	return this->attrDescs_;
}
std::string arrayDesc::toString(std::string strIndent)
{
	std::stringstream ss;
	ss << strIndent << "id: " << this->id_ << ", name: " << this->name_ << std::endl;

	ss << strIndent << "<" << std::endl;
	for (int i = 0; i < this->attrDescs_->size(); ++i)
	{
		ss << strIndent << "    " << this->attrDescs_->at(i)->toString();
		if (i != this->attrDescs_->size() - 1)
		{
			ss << ", ";
		}
		ss << std::endl;
	}
	ss << strIndent << ">" << std::endl;
	ss << strIndent << "[" << std::endl;
	for (int i = 0; i < this->dimDescs_->size(); ++i)
	{
		ss << strIndent << "    " <<  this->dimDescs_->at(i)->toString();
		if (i != this->dimDescs_->size() - 1)
		{
			ss << ", ";
		}
		ss << std::endl;
	}
	ss << strIndent << "]";

	return ss.str();
}

/**
 * Save/load in XML file
 */
tinyxml2::XMLElement* arrayDesc::convertToXMLDoc(std::shared_ptr<tinyxml2::XMLDocument> doc)
{
	auto desc = doc->NewElement(_MSDB_STR_ARRAY_DESC_);

	desc->SetAttribute(_MSDB_STR_ARR_ID_, this->id_);
	desc->SetAttribute(_MSDB_STR_ARR_NAME_, this->name_.c_str());
	
	auto dimDescs = doc->NewElement(_MSDB_STR_DIMENSION_DESCS_);
	auto attrDescs = doc->NewElement(_MSDB_STR_ATTRIBUTE_DESCS_);

	for (auto dimDesc : *this->dimDescs_)
	{
		dimDescs->LinkEndChild(dimDesc->convertToXMLDoc(doc->NewElement(_MSDB_STR_DIM_DESC_)));
	}

	for (auto attrDesc : *this->attrDescs_)
	{
		attrDescs->LinkEndChild(attrDesc->convertToXMLDoc(doc->NewElement(_MSDB_STR_ATTR_DESC_)));
	}

	desc->LinkEndChild(dimDescs);
	desc->LinkEndChild(attrDescs);

	return desc;
}
std::shared_ptr<arrayDesc> arrayDesc::buildDescFromXML(std::shared_ptr<tinyxml2::XMLDocument> doc)
{
	_MSDB_TRY_BEGIN
	{
		auto nodeArrDesc = xmlErrorHandler(doc->FirstChildElement(_MSDB_STR_ARRAY_DESC_));

		auto aid = nodeArrDesc->IntAttribute(_MSDB_STR_ARR_ID_);
		auto arrName = xmlErrorHandler(nodeArrDesc->Attribute(_MSDB_STR_ARR_NAME_));
		
		auto dimDescs = buildDimensionDescsFromXML(nodeArrDesc);
		auto attrDescs = buildAttributeDescsFromXML(nodeArrDesc);

		return std::make_shared<arrayDesc>(aid, arrName, dimDescs, attrDescs);
	}
	_MSDB_CATCH_ALL
	{
		// TODO:: Log error
	}
	_MSDB_CATCH_END

	return nullptr;
}
pDimensionDescs arrayDesc::buildDimensionDescsFromXML(tinyxml2::XMLElement* nodeArrDesc)
{
	pDimensionDescs dimDescs = std::make_shared<dimensionDescs>();
	auto nodeDimDescc = xmlErrorHandler(nodeArrDesc->FirstChildElement(_MSDB_STR_DIMENSION_DESCS_));
	auto nodeDimDesc = xmlErrorHandler(nodeDimDescc->FirstChildElement(_MSDB_STR_DIM_DESC_));
	do
	{
		dimDescs->push_back(dimensionDesc::buildDescFromXML(nodeDimDesc));
		nodeDimDesc = nodeDimDesc->NextSiblingElement();
	} while (nodeDimDesc != nullptr);
	
	return dimDescs;
}
pAttributeDescs arrayDesc::buildAttributeDescsFromXML(tinyxml2::XMLElement* nodeArrDesc)
{
	pAttributeDescs attrDescs = std::make_shared<attributeDescs>();
	auto nodeAttrDescc = xmlErrorHandler(nodeArrDesc->FirstChildElement(_MSDB_STR_ATTRIBUTE_DESCS_));
	auto nodeAttrDesc = xmlErrorHandler(nodeAttrDescc->FirstChildElement(_MSDB_STR_ATTR_DESC_));
	do
	{
		attrDescs->push_back(attributeDesc::buildDescFromXML(nodeAttrDesc));
		nodeAttrDesc = nodeAttrDesc->NextSiblingElement();
	} while (nodeAttrDesc != nullptr);

	return attrDescs;
}

//////////////////////////////
// Operators
// ***************************
// Assign
arrayDesc& arrayDesc::operator=(const arrayDesc& src)
{
	if (this == &src)
	{
		return *this;
	}

	arrayDesc temp(src);
	swap(*this, temp);
	return *this;
}
arrayDesc& arrayDesc::operator=(arrayDesc&& src) noexcept
{
	arrayDesc temp(std::move(src));
	swap(*this, temp);
	return *this;
}

bool arrayDesc::operator==(const arrayDesc& right_)
{
	if (this->id_ != right_.id_) return false;
	if (this->name_ != right_.name_) return false;

	pDimensionDescs leftDim = this->dimDescs_;
	pDimensionDescs rightDim = right_.dimDescs_;
	if (leftDim->size() != rightDim->size()) return false;
	
	for (int i = 0; i < leftDim->size(); i++)
	{
		if (!(*leftDim->at(i) == *rightDim->at(i))) return false;
	}

	pAttributeDescs leftAttr = this->attrDescs_;
	pAttributeDescs rightAttr = right_.attrDescs_;
	if (leftAttr->size() != rightAttr->size()) return false;

	for (int i = 0; i < leftAttr->size(); i++)
	{
		if (!(*leftAttr->at(i) == *rightAttr->at(i))) return false;
	}

	return true;
}
void swap(arrayDesc& first, arrayDesc& second) noexcept
{
	using std::swap;

	swap(first.id_, second.id_);
	swap(first.name_, second.name_);

	swap(first.dimDescs_, second.dimDescs_);
	swap(first.attrDescs_, second.attrDescs_);
}
}		// core
}		// msdb