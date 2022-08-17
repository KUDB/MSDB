#pragma once
#include "sz3Chunk.h"
#include <SZ3/sz3lib.hpp>
#include <io/io_util.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// sz3Block
//
// ===========================
// Serializable
template<typename Ty_>
inline void sz3Block<Ty_>::serialize(bstream& bs)
{
	_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "sz3Block does not use serialize(bstream& bs) function"));
}

template<typename Ty_>
inline void sz3Block<Ty_>::deserialize(bstream& bs)
{
	_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "sz3Block does not use deserialize(bstream& bs) function"));
}

template<typename Ty_>
void sz3Block<Ty_>::serialize(std::ostream& os)
{
	// TODO::Support float, double
	size_t dimSize = this->getDSize();
	auto dims = this->getDesc()->dims_;

	size_t typeSize = 0;
	switch (sizeof(Ty_))
	{
	case 1:
	case 2:
	case 4:
		typeSize = 32;
		break;
	case 8:
		typeSize = 64;
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "SZ3 only support Ty_ size=1,2,4,8"));
	}

	std::string strTypeSize = std::to_string(typeSize);
	std::string dim0, dim1, dim2, dim3;

	// Dimension check
	// TODO::There could be dim 0 size error
	auto testOut = dims[0] % (typeSize / (sizeof(Ty_) * CHAR_BIT));
	assert(dims[0] % (typeSize / (sizeof(Ty_) * CHAR_BIT)) == 0);

	const char** argv = nullptr;
	size_t argc = 12;
	switch (dimSize)
	{
	case 0:
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NO_DIMENSION, "SZ3 not supports dimension size=0"));
		break;
	}
	case 1:
	{
		dim0 = std::to_string(dims[0] / (typeSize / (sizeof(Ty_) * CHAR_BIT)));
		const char*temp[] = {
			"", "-I", strTypeSize.c_str(), "-i", "IN_PATH", "-z", "OUT_PATH", "-M", "ABS", "0.99",
			"-1", dim0.c_str(), NULL };
		argv = temp;
		argc = 12;
		break;
	}
	case 2:
	{
		dim0 = std::to_string(dims[0] / (typeSize / (sizeof(Ty_) * CHAR_BIT)));
		dim1 = std::to_string(dims[1]);
		const char* temp[] = { 
			"", "-I", strTypeSize.c_str(), "-i", "IN_PATH", "-z", "OUT_DATA_PATH", "-M", "ABS", "0.99",
			"-2", dim0.c_str(), dim1.c_str(), NULL };
		argv = temp;
		argc = 13;
		break;
	}
	case 3:
	{
		dim0 = std::to_string(dims[0] / (typeSize / (sizeof(Ty_) * CHAR_BIT)));
		dim1 = std::to_string(dims[1]);
		dim2 = std::to_string(dims[2]);
		const char* temp[] = {
			"", "-I", strTypeSize.c_str(), "-i", "IN_PATH", "-z", "OUT_DATA_PATH", "-M", "ABS", "0.99",
			"-3", dim0.c_str(), dim1.c_str(), dim2.c_str(), NULL };
		argv = temp;
		argc = 14;
		break;
	}
	case 4:
	{
		dim0 = std::to_string(dims[0] / (typeSize / (sizeof(Ty_) * CHAR_BIT)));
		dim1 = std::to_string(dims[1]);
		dim2 = std::to_string(dims[2]);
		dim3 = std::to_string(dims[3]);
		const char* temp[] = {
			"", "-I", strTypeSize.c_str(), "-i", "IN_PATH", "-z", "OUT_DATA_PATH", "-M", "ABS", "0.99",
			"-4", dim0.c_str(), dim1.c_str(), dim2.c_str(), dim3.c_str(), NULL };
		argv = temp;
		argc = 15;
		break;
	}
	default:
	{
		size_t dimRest = dims.area() / dims[0] / dims[1] / dims[2];
		dim0 = std::to_string(dims[0] / (typeSize / (sizeof(Ty_) * CHAR_BIT)));
		dim1 = std::to_string(dims[1]);
		dim2 = std::to_string(dims[2]);
		dim3 = std::to_string(dimRest);
		const char* temp[] = {
			"", "-I", strTypeSize.c_str(), "-i", "IN_PATH", "-z", "OUT_DATA_PATH", "-M", "ABS", "0.99", 
			"-4", dim0.c_str(), dim1.c_str(), dim2.c_str(), dim3.c_str(), NULL };
		argv = temp;
		argc = 15;
		break;
	}
	}

	char* out = nullptr;
	size_t outBytes = sz3::sz3lib(argc, argv, (char*)this->getBuffer()->getData(), &out, this->getBuffer()->size());

	os << outBytes;
	os.write(out, outBytes);
}

template<typename Ty_>
void sz3Block<Ty_>::deserialize(std::istream& is)
{
	// TODO::Support float, double
	size_t dimSize = this->getDSize();
	auto dims = this->getDesc()->dims_;

	size_t typeSize = 0;
	switch (sizeof(Ty_))
	{
	case 1:
	case 2:
	case 4:
		typeSize = 32;
		break;
	case 8:
		typeSize = 64;
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "SZ3 only support Ty_ size=1,2,4,8"));
	}

	std::string strTypeSize = std::to_string(typeSize);
	const char* argv[] = {"", "-I", strTypeSize.c_str(), "-z", "IN_PATH", "-o", "OUT_PATH" };
	size_t argc = 7;

	size_t inBytes = 0;
	is >> inBytes;
	this->getDesc()->mSize_ = inBytes;


	char* decodeOut = nullptr;
	std::shared_ptr<char[]> in(new char[inBytes]);

	size_t decodeBytes = sz3::sz3lib(argc, argv, in.get(), &decodeOut, inBytes);

	memcpy(this->getBuffer()->getData(), decodeOut, decodeBytes);
}
// ===========================

//////////////////////////////
// sz3Chunk
//
// ===========================
// Serializable
template<typename Ty_>
inline void sz3Chunk<Ty_>::serialize(std::ostream& os)
{
	std::stringstream oss;
	std::vector<size_t> blockSize;

	// Serialize Block
	auto it = this->getBlockIterator();
	while (!it->isEnd())
	{
		std::static_pointer_cast<sz3Block<Ty_>>(**it)->serialize(oss);
		blockSize.push_back((*it)->getDesc()->mSize_);
		++(*it);
	}

	// Get serialized Size
	this->serializedSize_ = oss.tellp();

	// Write data on ostream
	this->getOutHeader()->serialize(os);

	// for : blockSize
	// os << blockSize[i];
	os << oss.str();
}
template<typename Ty_>
inline void sz3Chunk<Ty_>::deserialize(std::istream& is)
{
	this->getHeader()->deserialize(is);
	this->updateFromHeader();

	std::stringstream oss;
	oss << is.rdbuf();

	auto it = this->getBlockIterator();
	while (!it->isEnd())
	{
		std::static_pointer_cast<sz3Block<Ty_>>(**it)->deserialize(oss);

		++(*it);
	}
}
// ===========================

//////////////////////////////
// sz3ChunkFactory
//
template <typename Ty_>
pChunk sz3ChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<sz3Chunk<Ty_>>(cDesc);
}
}		// core
}		// msdb