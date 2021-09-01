#include <pch.h>
#include <io/serializable.h>

namespace msdb
{
namespace core
{
serializable::serializable(pHeader myHeader)
	: myHeader_(myHeader), serializedSize_(myHeader->bodySize_)
{
}
serializable::size_type serializable::getSerializedSize()
{
	return this->serializedSize_;
}

void serializable::setSerializedSize(size_type size)
{
	this->serializedSize_ = size;
}

pHeader serializable::getInHeader()
{
	this->updateFromHeader();
	return this->myHeader_;
}
pHeader serializable::getOutHeader()
{
	this->updateToHeader();
	return this->myHeader_;
}
pHeader serializable::getHeader()
{
	return this->myHeader_;
}
serialHeader::serialHeader()
	: headerSize_(0), bodySize_(0), version_(0)
{
}
}		// core
}		// msdb