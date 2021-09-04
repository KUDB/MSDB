#include <pch.h>
#include <query/query.h>

namespace msdb
{
namespace core
{
query::query()
{
	this->timer_ = std::make_shared<timer>();
}
pTimer query::getTimer()
{
	return this->timer_;
}
void query::setOutArrayDesc(pArrayDesc arrDesc)
{
	if(arrDesc_ != nullptr)
	{
		// TODO::throw exception
	}

	this->arrDesc_ = arrDesc;
}
void query::setDimBuffer(std::shared_ptr<std::vector<coor>> buffer)
{
	this->dimBuffer_ = buffer;
}
void query::setAttrBuffer(attributeId attrId, outBuffer buffer)
{
	if (arrDesc_ == nullptr)
	{
		// TODO::throw exception
	}

	this->attrBuffers_[attrId] = buffer;
}
std::shared_ptr<std::vector<coor>> query::getDimBuffer()
{
	return this->dimBuffer_;
}
}	// core
}	// msdb