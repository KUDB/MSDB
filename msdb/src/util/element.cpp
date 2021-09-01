#include <pch.h>
#include <util/element.h>
#include <system/exceptions.h>

namespace msdb
{
namespace core
{
eleSize getEleSize(eleType type)
{
	switch(type)
	{
	case eleType::EMPTY:
		return 0;
	case eleType::BOOL:
		return sizeof(bool);
	case eleType::CHAR:
		return sizeof(char);
	case eleType::INT8:
		return sizeof(int8_t);
	case eleType::INT16:
		return sizeof(int16_t);
	case eleType::INT32:
		return sizeof(int32_t);
	case eleType::INT64:
		return sizeof(int64_t);
	case eleType::UINT8:
		return sizeof(uint8_t);
	case eleType::UINT16:
		return sizeof(uint16_t);
	case eleType::UINT32:
		return sizeof(uint32_t);
	case eleType::UINT64:
		return sizeof(uint64_t);
	case eleType::FLOAT:
		return sizeof(float);
	case eleType::DOUBLE:
		return sizeof(double);
	}
	
	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_UNKNOWN_ELEMENT_TYPE));
}
element::element(eleType type)
	: ptr_(nullptr)
{
	this->getFunc = this->findGetFunc(type);
}
element::element(void* ptr, eleType type)
	: ptr_(ptr)
{
	this->getFunc = this->findGetFunc(type);
}

element::element(const element& mit)
	: ptr_(mit.ptr_), getFunc(mit.getFunc)
{
}


element& element::operator=(const element& mit)
{
	this->ptr_ = mit.ptr_;
	this->getFunc = mit.getFunc;

	return (*this);
}

void element::getData(void* output)
{
	(this->*getFunc)(output);
}

//void* element::getData()
//{
//	return this->ptr_;
//}

element::gFunc element::findGetFunc(eleType type)
{
	static void(element:: * eleGetDataFPointer[13])(void*) = {
		nullptr,					// empty
		&element::getData_Bool,		// bool
		&element::getData_1Byte,	// char
		&element::getData_1Byte,	// int8
		&element::getData_2Byte,	// int16
		&element::getData_4Byte,	// int32
		&element::getData_8Byte,	// int64
		&element::getData_1Byte,	// uint8
		&element::getData_2Byte,	// uint16
		&element::getData_4Byte,	// uint32
		&element::getData_8Byte,	// uint64
		&element::getData_4Byte,	// float		WARNING
		&element::getData_8Byte,	// double		WARNING
	};

	return eleGetDataFPointer[static_cast<int>(type)];
}

stableElement::stableElement(eleType type)
	: type_(type)
{
	size_t size = getEleSize(type);
	this->ptr_ = static_cast<void*>(new char[size]);
}

stableElement::stableElement(void* ptr, eleType type)
	: element(ptr, type), type_(type)
{
	size_t size = getEleSize(type);
	this->ptr_ = static_cast<void*>(new char[size]);
	memcpy(this->ptr_, ptr, size);
}

stableElement::stableElement(const stableElement& mit)
	: element(mit), type_(mit.type_)
{
	this->ptr_ = new char[mit.getCurrentTypeSize()];
	memcpy(this->ptr_, mit.ptr_, mit.getCurrentTypeSize());
}


//stableElement& stableElement::operator=(const stableElement& mit)
//{
//	if (this->getCurrentTypeSize() != mit.getCurrentTypeSize())
//	{
//		delete[] this->ptr_;
//		this->ptr_ = new char[mit.getCurrentTypeSize()];
//	}
//
//	memcpy(this->ptr_, mit.ptr_, mit.getCurrentTypeSize());
//	this->getFunc = mit.getFunc;
//	
//	return (*this);
//}

stableElement& stableElement::operator=(const element& mit)
{
	if (this->getCurrentTypeSize() != mit.getCurrentTypeSize())
	{
		delete[] this->ptr_;
		this->ptr_ = new char[mit.getCurrentTypeSize()];
	}

	memcpy(this->ptr_, mit.ptr_, mit.getCurrentTypeSize());
	this->getFunc = mit.getFunc;

	return (*this);
}

stableElement::~stableElement()
{
	delete[] this->ptr_;
}
eleType stableElement::getEleType()
{
	return this->type_;
}
bool stableElement::isInt()
{
	switch (this->type_)
	{
	case eleType::INT8:
	case eleType::INT16:
	case eleType::INT32:
	case eleType::INT64:
	case eleType::UINT8:
	case eleType::UINT16:
	case eleType::UINT32:
	case eleType::UINT64:
		return true;
	}
	return false;
}
bool stableElement::isFloat()
{
	switch(this->type_)
	{
	case eleType::FLOAT:
	case eleType::DOUBLE:
		return true;
	}

	return false;
}
}		// core
}		// msdb