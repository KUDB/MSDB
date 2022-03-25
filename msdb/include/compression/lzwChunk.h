#pragma once
#ifndef _MSDB_LZWCHUNK_H_
#define _MSDB_LZWCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <compression/lzwEncoder.h>
#include <compression/lzwBlock.h>
#include <io/bitstream.h>


namespace msdb
{
namespace core
{
//class lzwChunk;
//using pLzwChunk = std::shared_ptr<lzwChunk>;

template <typename Ty_>
class lzwChunk : public flattenChunk<Ty_>
{
public:
	using lzwCodeType = std::uint16_t;

public:
	lzwChunk(pChunkDesc desc)
		: flattenChunk<Ty_>(desc)
	{

	}
	virtual ~lzwChunk()
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
			auto blockObj = std::make_shared<lzwBlock<Ty_>>(desc);
			this->insertBlock(blockObj);
		}

		return this->blocks_[bId];
	}

public:
	virtual void serialize(std::ostream& os) override
	{
		bstream bs;
		switch (this->desc_->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->serializeTy<char>(bs);
			break;
		case eleType::INT8:
			this->serializeTy<int8_t>(bs);
			break;
		case eleType::INT16:
			this->serializeTy<int16_t>(bs);
			break;
		case eleType::INT32:
			this->serializeTy<int32_t>(bs);
			break;
		case eleType::INT64:
			this->serializeTy<int64_t>(bs);
			break;
		case eleType::UINT8:
			this->serializeTy<uint8_t>(bs);
			break;
		case eleType::UINT16:
			this->serializeTy<uint16_t>(bs);
			break;
		case eleType::UINT32:
			this->serializeTy<uint32_t>(bs);
			break;
		case eleType::UINT64:
			this->serializeTy<uint64_t>(bs);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}

		this->serializedSize_ = bs.capacity();
		this->getOutHeader()->serialize(os);
		os.write(bs.data(), bs.capacity());
	}
	virtual void deserialize(std::istream& is) override
	{
		this->getHeader()->deserialize(is);
		this->updateFromHeader();

		bstream bs;
		bs.resize(this->serializedSize_);
		is.read(bs.data(), this->serializedSize_);

		switch (this->desc_->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->deserializeTy<char>(bs);
			break;
		case eleType::INT8:
			this->deserializeTy<int8_t>(bs);
			break;
		case eleType::INT16:
			this->deserializeTy<int16_t>(bs);
			break;
		case eleType::INT32:
			this->deserializeTy<int32_t>(bs);
			break;
		case eleType::INT64:
			this->deserializeTy<int64_t>(bs);
			break;
		case eleType::UINT8:
			this->deserializeTy<uint8_t>(bs);
			break;
		case eleType::UINT16:
			this->deserializeTy<uint16_t>(bs);
			break;
		case eleType::UINT32:
			this->deserializeTy<uint32_t>(bs);
			break;
		case eleType::UINT64:
			this->deserializeTy<uint64_t>(bs);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	template<typename Ty_>
	void serializeTy(bstream& out)
	{
		lzwCoder<lzwCodeType> encoder;
		encoder.encode(out, (const unsigned char*)this->cached_->getReadData(), this->cached_->size());
	}

	template<class Ty_>
	void deserializeTy(bstream& in)
	{
		lzwCoder<lzwCodeType> decoder;
		decoder.decode((unsigned char*)this->cached_->getData(), this->cached_->size(), in);
	}

	//template<typename Ty_>
	//void serializeTy(std::stringstream& compressed)
	//{
	//	auto buf = (const char*)this->cached_->getReadData();
	//	auto length = this->cached_->size();
	//	std::map<std::pair<lzwCodeType, char>, lzwCodeType> dictionary;

	//	// "named" lambda function, used to reset the dictionary to its initial contents
	//	const auto reset_dictionary = [&dictionary]
	//	{
	//		dictionary.clear();

	//		const long int minc = std::numeric_limits<char>::min();
	//		const long int maxc = std::numeric_limits<char>::max();

	//		for (long int c = minc; c <= maxc; ++c)
	//		{
	//			// to prevent Undefined Behavior, resulting from reading and modifying
	//			// the dictionary object at the same time
	//			const lzwCodeType dictionary_size = dictionary.size();

	//			dictionary[{lzwChunk::dms, static_cast<char> (c)}] = dictionary_size;
	//		}
	//	};

	//	reset_dictionary();

	//	lzwCodeType i{ lzwChunk::dms }; // Index
	//	char c;

	//	for (size_t pos = 0; pos < length; ++pos)
	//	{
	//		c = buf[pos];

	//		// dictionary's maximum size was reached
	//		if (dictionary.size() == lzwChunk::dms)
	//		{
	//			reset_dictionary();
	//		}

	//		if (dictionary.count({ i, c }) == 0)
	//		{
	//			// to prevent Undefined Behavior, resulting from reading and modifying
	//			// the dictionary object at the same time
	//			const lzwCodeType dictionary_size = dictionary.size();

	//			dictionary[{i, c}] = dictionary_size;
	//			compressed.write(reinterpret_cast<const char*> (&i), sizeof(lzwCodeType));
	//			i = dictionary.at({ lzwChunk::dms, c });
	//		} else
	//		{
	//			i = dictionary.at({ i, c });
	//		}
	//	}

	//	if (i != lzwChunk::dms)
	//	{
	//		compressed.write(reinterpret_cast<const char*> (&i), sizeof(lzwCodeType));
	//	}
	//}

	//template<class Ty_>
	//void deserializeTy(std::stringstream& compressed)
	//{
	//	std::ostringstream os;
	//	std::vector<std::pair<lzwCodeType, char>> dictionary;

	//	// "named" lambda function, used to reset the dictionary to its initial contents
	//	const auto reset_dictionary = [&dictionary]
	//	{
	//		dictionary.clear();
	//		dictionary.reserve(lzwChunk::dms);

	//		const long int minc = std::numeric_limits<char>::min();
	//		const long int maxc = std::numeric_limits<char>::max();

	//		for (long int c = minc; c <= maxc; ++c)
	//		{
	//			dictionary.push_back({ lzwChunk::dms, static_cast<char> (c) });
	//		}
	//	};

	//	const auto rebuild_string = [&dictionary](lzwCodeType k) -> std::vector<char>
	//	{
	//		std::vector<char> s; // String

	//		while (k != lzwChunk::dms)
	//		{
	//			s.push_back(dictionary.at(k).second);
	//			k = dictionary.at(k).first;
	//		}

	//		std::reverse(s.begin(), s.end());
	//		return s;
	//	};

	//	reset_dictionary();

	//	lzwCodeType i{ lzwChunk::dms }; // Index
	//	lzwCodeType k; // Key

	//	while (compressed.read(reinterpret_cast<char*> (&k), sizeof(lzwCodeType)))
	//	{
	//		// dictionary's maximum size was reached
	//		if (dictionary.size() == lzwChunk::dms)
	//			reset_dictionary();

	//		if (k > dictionary.size())
	//		{
	//			throw std::runtime_error("invalid compressed code");
	//		}

	//		std::vector<char> s; // String

	//		if (k == dictionary.size())
	//		{
	//			dictionary.push_back({ i, rebuild_string(i).front() });
	//			s = rebuild_string(k);
	//		} else
	//		{
	//			s = rebuild_string(k);

	//			if (i != lzwChunk::dms)
	//				dictionary.push_back({ i, s.front() });
	//		}

	//		os.write(&s.front(), s.size());
	//		i = k;
	//	}

	//	if (!compressed.eof() || compressed.gcount() != 0)
	//	{
	//		throw std::runtime_error("corrupted compressed file");
	//	}

	//	memcpy(this->cached_->getData(), os.str().c_str(), this->cached_->size());
	//}

private:
	/// Dictionary Maximum Size (when reached, the dictionary will be reset)
	static const lzwCodeType dms{ std::numeric_limits<lzwCodeType>::max() };
};
}		// core
}		// msdb
#endif	// _MSDB_LZWCHUNK_H_
