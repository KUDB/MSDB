#include <pch.h>
#include <array/attributeDesc.h>
#include <util/element.h>

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
}		// core
}		// msdb