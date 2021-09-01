#pragma once
#ifndef _MSDB_SERIALIZABLE_H_
#define _MSDB_SERIALIZABLE_H_

#include <pch.h>

namespace msdb
{
namespace core
{
using header_version_t = size_t;
using header_size_t = size_t;
using body_size_t = size_t;

class serialHeader;
using pHeader = std::shared_ptr<serialHeader>;

class serializable;
using pSerializable = std::shared_ptr<serializable>;

class serialHeader
{
public:
	serialHeader();

public:
	virtual void serialize(std::ostream& os) = 0;
	virtual void deserialize(std::istream& is) = 0;

public:
	header_version_t version_;
	header_size_t headerSize_;
	body_size_t bodySize_;
};	// serialHeader

class serializable
{
public:
	using size_type = size_t;

public:
	serializable(pHeader myHeader);

public:
	size_type getSerializedSize();
	void setSerializedSize(size_type size);

	pHeader getHeader();		// header without any update
	pHeader getInHeader();		// header used to deserialize
	pHeader getOutHeader();		// header used to serialize
	
	virtual void updateToHeader() = 0;
	virtual void updateFromHeader() = 0;
	
	virtual void serialize(std::ostream& os) = 0;
	virtual void deserialize(std::istream& is) = 0;

private:
	pHeader myHeader_;

protected:
	size_type serializedSize_;
};	// serializable
}		// core
}		// msdb
#endif	// _MSDB_SERIALIZABLE_H_
