#pragma once
#ifndef _MSDB_ELEMENT_H_
#define _MSDB_ELEMENT_H_

#include <pch.h>

namespace msdb
{
namespace core
{
#define _ELE_DEFAULT_TYPE		eleType::INT64
using eleDefault = int64_t;
using eleSize = size_t;

class stableElement;
using pStableElement = std::shared_ptr<stableElement>;

enum class eleType
{
	EMPTY,		// cannot use NULL
	BOOL,
	CHAR,
	INT8,
	INT16,
	INT32,
	INT64,
	UINT8,
	UINT16,
	UINT32,
	UINT64,
	FLOAT,
	DOUBLE
};

// Use stableElement if you need to hold a value.
class element
{
public:
	element(eleType type = _ELE_DEFAULT_TYPE);

	element(void* ptr, eleType type = _ELE_DEFAULT_TYPE);

	element(const element& mit);

	element& operator=(const element& mit);

public:
	bool		getBool()	const { return get<bool>(); }
	char		getChar()	const { return get<char>(); }
	int8_t		getInt8()	const { return get<int8_t>(); }
	int16_t		getInt16()	const { return get<int16_t>(); }
	int32_t		getInt32()	const { return get<int32_t>(); }
	int64_t		getInt64()	const { return get<int64_t>(); }
	uint8_t		getUint8()	const { return get<uint8_t>(); }
	uint16_t	getUint16()	const { return get<uint16_t>(); }
	uint32_t	getUint32()	const { return get<uint32_t>(); }
	uint64_t	getUint64()	const { return get<uint64_t>(); }
	float		getFloat()	const { return get<float>(); }
	double		getDouble()	const { return get<double>(); }

	void		setBool(bool v)			{ set<bool>(v); }
	void		setChar(char v)			{ set<char>(v); }
	void		setInt8(int8_t v)		{ set<int8_t>(v); }
	void		setInt16(int16_t v)		{ set<int16_t>(v); }
	void		setInt32(int32_t v)		{ set<int32_t>(v); }
	void		setInt64(int64_t v)		{ set<int64_t>(v); }
	void		setUint8(uint8_t v)		{ set<uint8_t>(v); }
	void		setUint16(uint16_t v)	{ set<uint16_t>(v); }
	void		setUint32(uint32_t v)	{ set<uint32_t>(v); }
	void		setUint64(uint64_t v)	{ set<uint64_t>(v); }
	void		setDateTime(uint64_t v) { set<uint64_t>(v); }
	void		setFloat(float v)		{ set<float>(v); }
	void		setDouble(double v)		{ set<double>(v); }

	void getData(void* output);
	//void* getData();

public:
	template<class Ty_>
	inline Ty_& get()
	{
		return *(reinterpret_cast<Ty_*>(this->ptr_));
	}

	template<class Ty_>
	inline Ty_ const& get() const
	{
		return const_cast<element*>(this)->get<Ty_>();
	}

	template<class Ty_>
	inline void set(const Ty_& v)
	{
		*reinterpret_cast<Ty_*>(this->ptr_) = v;
	}

public:
	void getData_Bool(void* output)
	{
		*(bool*)output = this->getBool();
	}

	void getData_1Byte(void* output)
	{
		*(uint8_t*)output = this->getUint8();
	}

	void getData_2Byte(void* output)
	{
		*(uint16_t*)output = this->getUint16();
	}

	void getData_4Byte(void* output)
	{
		*(uint32_t*)output = this->getUint32();
	}

	void getData_8Byte(void* output)
	{
		*(uint64_t*)output = this->getUint64();
	}
	// TODO :: operator comparison
	// TODO :: operator assignment

protected:
	typedef void(element::* gFunc)(void*);
	gFunc findGetFunc(eleType type);
	void (element::* getFunc)(void*);

protected:
	const size_t getCurrentTypeSize() const
	{
		if (this->getFunc == nullptr)	return 0;
		if (this->getFunc == &element::getData_Bool)		return sizeof(bool);
		if (this->getFunc == &element::getData_1Byte)		return sizeof(int8_t);
		if (this->getFunc == &element::getData_2Byte)		return sizeof(int16_t);
		if (this->getFunc == &element::getData_4Byte)		return sizeof(int32_t);
		if (this->getFunc == &element::getData_8Byte)		return sizeof(int64_t);
	}

protected:
	void* ptr_;

private:
	friend class stableElement;
};

// hold element in new assigned storage space
class stableElement : public element, public std::enable_shared_from_this<stableElement>
{
public:
	stableElement(eleType type = _ELE_DEFAULT_TYPE);

	stableElement(void* ptr, eleType type = _ELE_DEFAULT_TYPE);

	stableElement(const stableElement& mit);

	virtual ~stableElement();

public:
	stableElement& operator=(const element& mit);
	// Comparison
	//bool operator> (const element& mit);


	eleType getEleType();

	bool isInt();		// all integer from int8_t to int64_t
	bool isFloat();		// float, double

private:
	eleType type_;
};

eleSize getEleSize(eleType type);
}		// core
}		// msdb
#endif	// _MSDB_ELEMENT_H_
