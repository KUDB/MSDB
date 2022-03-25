#pragma once
#ifndef _MSDB_ZIPCHUNK_H_
#define _MSDB_ZIPCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <compression/zipBlock.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
//class zipChunk;
//using pZipChunk = std::shared_ptr<zipChunk>;

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
}		// core
}		// msdb
#endif	// _MSDB_ZIPCHUNK_H_
