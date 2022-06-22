#pragma once
#include "tthreshChunk.h"
#include <tthreshCompressor.h>
#include <io/io_util.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// tthreshBlock
//
// ===========================
// Serializable
template<typename Ty_>
inline void tthreshBlock<Ty_>::serialize(bstream& bs)
{
	_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "tthreshBlock does not use serialize(bstream& bs) function"));
}

template<typename Ty_>
inline void tthreshBlock<Ty_>::deserialize(bstream& bs)
{
	_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "tthreshBlock does not use deserialize(bstream& bs) function"));
}

template<typename Ty_>
void tthreshBlock<Ty_>::serialize(std::ostream& os)
{
	tthresh::tthreshCompressor comp;

	std::string io_type;
	if (typeid(Ty_) == typeid(float))
	{
		io_type = "float";
	}
	else if (typeid(Ty_) == typeid(double))
	{
		io_type = "double";
	}
	else
	{
		switch (sizeof(Ty_))
		{
			case sizeof(unsigned char) :
			{
				io_type = "uchar";
				break;
			}
			case sizeof(unsigned short) :
			{
				io_type = "ushort";
				break;
			}
			case sizeof(int) :
			{
				io_type = "int";
				break;
			}
			default:
			{
				io_type = "int";
				break;
			}
		}
	}
	
	auto myDim = this->getDesc()->dims_.toVector();
	std::vector<uint32_t> inputDim = {myDim.begin(), myDim.end()};

	double target_value = 0;
	size_t skip_bytes = 0;
	bool verbose = false;
	bool debug = false;

	std::stringstream oss;
	//ostream* pOss = &oss;

	comp.compress(inputDim, (char*)this->cached_->getData(), (size_t)this->getDesc()->mSize_, oss, io_type,
				  tthresh::Target::eps, target_value, skip_bytes, 
				  verbose, debug);
	uint64_t streamSize = size_of_stream(oss);
	os << streamSize;
	os << oss.str();

	//std::vector<uint32_t> dims, char* input, size_t inputSize, std::ostream& output, string io_type,
	//	Target target, double target_value, size_t skip_bytes,
	//	bool verbose = false, bool debug = false
}

template<typename Ty_>
void tthreshBlock<Ty_>::deserialize(std::istream& is)
{
	tthresh::tthreshCompressor comp;

	std::stringstream oss;
	double* data = nullptr;

	std::vector<tthresh::Slice> cutout;

	bool autocrop = false;
	bool verbose = false;
	bool debug = false;

	uint64_t streamSize = 0;
	is >> streamSize;

	char* inData = nullptr;
	try
	{
		inData = new char[streamSize];
		is.read(inData, streamSize);

		std::stringstream iss;
		iss.write(inData, streamSize);
		iss.seekg(0);

		comp.decompress(iss, oss, data, cutout, autocrop, verbose, debug);

		memcpy(this->cached_->getData(), oss.str().c_str(), this->getDesc()->mSize_);
	}
	catch (...)
	{
		if (inData)
		{
			delete[] inData;
		}

		throw;
	}
}
// ===========================

//////////////////////////////
// tthreshChunk
//
// ===========================
// Serializable
template<typename Ty_>
inline void tthreshChunk<Ty_>::serialize(std::ostream& os)
{
	std::stringstream oss;
	std::vector<size_t> blockSize;

	// Serialize Block
	auto it = this->getBlockIterator();
	while (!it->isEnd())
	{
		std::static_pointer_cast<tthreshBlock<Ty_>>(**it)->serialize(oss);
		// (*it)->getBlock mSize;
		// blockSize.push_back(blockSize);
		++(*it);
	}

	// Get serialized Size
	oss.seekg(0, ios::end);
	this->serializedSize_ = oss.tellp();
	
	// Write data on ostream
	this->getOutHeader()->serialize(os);

	// for : blockSize
	// os << blockSize[i];
	os << oss.str();
}
template<typename Ty_>
inline void tthreshChunk<Ty_>::deserialize(std::istream& is)
{
	this->getHeader()->deserialize(is);
	this->updateFromHeader();

	std::stringstream oss;
	oss << is.rdbuf();

	auto it = this->getBlockIterator();
	while (!it->isEnd())
	{
		std::static_pointer_cast<tthreshBlock<Ty_>>(**it)->deserialize(oss);

		++(*it);
	}
}
// ===========================

//////////////////////////////
// tthreshChunkFactory
//
template <typename Ty_>
pChunk tthreshChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<tthreshChunk<Ty_>>(cDesc);
}
}		// core
}		// msdb