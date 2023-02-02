#pragma once
#ifndef _MSDB_ZIPCHUNK_H_
#define _MSDB_ZIPCHUNK_H_

#include <pch_op.h>
#include <array/flattenChunk.h>
#include <io/bitstream.h>
#include <util/ioutil.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class zipBlock : public flattenBlock<Ty_>
{
public:
	zipBlock(pBlockDesc desc)
		: flattenBlock<Ty_>(desc)
	{

	}
	virtual ~zipBlock()
	{

	}

public:
	template<typename Ty_>
	void serializeTy(std::stringstream& compressed)
	{
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;

		std::stringstream blockCompressed;
		std::stringstream origin;
		origin.write((const char*)this->getBuffer()->getReadData(), this->getBuffer()->size());
		out.push(boost::iostreams::zlib_compressor());
		out.push(origin);
		boost::iostreams::copy(out, blockCompressed);

		size_t mBlockSize = getSize(blockCompressed);
		compressed << static_cast<size_t>(mBlockSize);

		compressed << blockCompressed.rdbuf();
	}

	template<typename Ty_>
	void deserializeTy(std::stringstream& compressed)
	{
		size_t mBlockSize;
		compressed >> mBlockSize;

		assert(mBlockSize <= this->getBuffer()->size() * 2);

		char* tempBuffer = new char[mBlockSize];
		compressed.read(tempBuffer, mBlockSize);

		std::stringstream blockCompressed;
		blockCompressed.write(tempBuffer, mBlockSize);

		std::stringstream decompressed;
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
		out.push(boost::iostreams::zlib_decompressor());
		out.push(blockCompressed);
		boost::iostreams::copy(out, decompressed);

		delete[] tempBuffer;

		memcpy(this->getBuffer()->getData(), decompressed.str().c_str(), this->getBuffer()->size());
	}
};

template <typename Ty_>
class zipChunk : public flattenChunk<Ty_>
{
public:
	zipChunk(pChunkDesc desc)
		: flattenChunk<Ty_>(desc)
	{

	}
	virtual ~zipChunk()
	{

	}

public:
	virtual pBlock makeBlock(const blockId bId) override
	{
		assert(this->blockCapacity_ > bId);
		if (this->blocks_[bId] == nullptr)
		{
			// Make new one
			auto desc = this->getBlockDesc(bId);
			auto blockObj = std::make_shared<zipBlock<Ty_>>(desc);
			this->insertBlock(blockObj);
			return blockObj;
		}
		// Alread exist
		// Return old one
		return this->blocks_[bId];
	}

public:
	virtual void serialize(std::ostream& os) override
	{
		std::stringstream oss;
		switch (this->desc_->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->serializeTy<char>(oss);
			break;
		case eleType::INT8:
			this->serializeTy<int8_t>(oss);
			break;
		case eleType::INT16:
			this->serializeTy<int16_t>(oss);
			break;
		case eleType::INT32:
			this->serializeTy<int32_t>(oss);
			break;
		case eleType::INT64:
			this->serializeTy<int64_t>(oss);
			break;
		case eleType::UINT8:
			this->serializeTy<uint8_t>(oss);
			break;
		case eleType::UINT16:
			this->serializeTy<uint16_t>(oss);
			break;
		case eleType::UINT32:
			this->serializeTy<uint32_t>(oss);
			break;
		case eleType::UINT64:
			this->serializeTy<uint64_t>(oss);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}

		this->serializedSize_ = getSize(oss);
		this->getOutHeader()->serialize(os);
		os << oss.str();
	}

	virtual void deserialize(std::istream& is) override
	{
		this->getHeader()->deserialize(is);
		this->updateFromHeader();

		std::stringstream iss;
		iss << is.rdbuf();

		switch (this->desc_->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->deserializeTy<char>(iss);
			break;
		case eleType::INT8:
			this->deserializeTy<int8_t>(iss);
			break;
		case eleType::INT16:
			this->deserializeTy<int16_t>(iss);
			break;
		case eleType::INT32:
			this->deserializeTy<int32_t>(iss);
			break;
		case eleType::INT64:
			this->deserializeTy<int64_t>(iss);
			break;
		case eleType::UINT8:
			this->deserializeTy<uint8_t>(iss);
			break;
		case eleType::UINT16:
			this->deserializeTy<uint16_t>(iss);
			break;
		case eleType::UINT32:
			this->deserializeTy<uint32_t>(iss);
			break;
		case eleType::UINT64:
			this->deserializeTy<uint64_t>(iss);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	template<typename Ty_>
	void serializeTy(std::stringstream& compressed)
	{
		//////////////////////////////
		// Block IO
		//auto blockItr = this->getBlockIterator();
		//while (!blockItr->isEnd())
		//{
		//	if (blockItr->isExist())
		//	{
		//		pZipBlock zBlock = std::static_pointer_cast<zipBlock>(**blockItr);
		//		zBlock->serializeTy<Ty_>(compressed);
		//	}

		//	++(*blockItr);
		//}
		//////////////////////////////

		//////////////////////////////
		// Chunk IO
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;

		std::stringstream chunkCompressed;
		std::stringstream origin;
		origin.write((const char*)this->getBuffer()->getReadData(), this->getBuffer()->size());
		out.push(boost::iostreams::zlib_compressor());
		out.push(origin);
		boost::iostreams::copy(out, chunkCompressed);

		size_t mChunkSize = getSize(chunkCompressed);
		compressed << static_cast<size_t>(mChunkSize);

		compressed << chunkCompressed.rdbuf();
		chunkCompressed.clear();
		//////////////////////////////
	}

	template<class Ty_>
	void deserializeTy(std::stringstream& compressed)
	{
		//this->bufferAlloc();

		//////////////////////////////
		// Block IO
		//auto blockItr = this->getBlockIterator();
		//while (!blockItr->isEnd())
		//{
		//	if (blockItr->isExist())
		//	{
		//		pZipBlock zBlock = std::static_pointer_cast<zipBlock>(**blockItr);
		//		zBlock->deserializeTy<Ty_>(compressed);
		//	}

		//	++(*blockItr);
		//}
		//////////////////////////////

		//////////////////////////////
		// Chunk IO
		size_t mChunkSize;
		compressed >> mChunkSize;

		assert(mChunkSize <= this->getBuffer()->size());

		char* tempBuffer = new char[mChunkSize];
		compressed.read(tempBuffer, mChunkSize);

		std::stringstream chunkCompressed;
		chunkCompressed.write(tempBuffer, mChunkSize);

		std::stringstream decompressed;
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
		out.push(boost::iostreams::zlib_decompressor());
		out.push(chunkCompressed);
		boost::iostreams::copy(out, decompressed);

		delete[] tempBuffer;

		memcpy(this->getBuffer()->getData(), decompressed.str().c_str(), this->getBuffer()->size());
		//////////////////////////////
	}
};

//////////////////////////////
// zipChunkFactory
//
// To make concreteType Chunk
//
template <typename Ty_>
class zipChunkFactory : public chunkFactory
{
public:
	zipChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};

//////////////////////////////
// zipChunkFactoryBuilder
//
class zipChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	zipChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<zipChunkFactory<Ty_>>();
	}
};

//////////////////////////////
// zipChunkType
// 
// - ChunkType for zipChunk
//
class zipChunkType : public chunkType
{
public:
	zipChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "zipChunk";
	}
};
}		// core
}		// msdb

#include "zipChunk.hpp"

#endif	// _MSDB_ZIPCHUNK_H_
