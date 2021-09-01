#include <pch.h>
#include <array/arrayDesc.h>

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
size_t arrayDesc::getDSize()
{
	return this->dimDescs_->size();
}
}		// core
}		// msdb