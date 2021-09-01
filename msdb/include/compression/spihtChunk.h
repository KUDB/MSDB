#pragma once
#ifndef _MSDB_SPIHTCHUNK_H_
#define _MSDB_SPIHTCHUNK_H_

#include <pch.h>
#include <array/memChunk.h>
#include <compression/spihtBlock.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
class spihtChunk;
using pSpihtChunk = std::shared_ptr<spihtChunk>;

class spihtChunk : public memChunk
{
public:
	spihtChunk(pChunkDesc desc);
	virtual ~spihtChunk();

	void initBufferZero();

protected:
	virtual void makeBuffer();

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	//virtual pChunkItemIterator getItemIterator();
	//virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range);

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			// TODO::check isExist()
			std::static_pointer_cast<spihtBlock>(**blockItr)->setLevel(this->maxLevel_);
			this->blockSerialize<Ty_>(bs, (**blockItr));
			++(*blockItr);
		}
	}

	template <typename Ty_>
	void blockSerialize(bstream& bs, pBlock curBlock)
	{
		pSpihtBlock spBlock = std::static_pointer_cast<spihtBlock>(curBlock);
		spBlock->serializeTy<Ty_>(bs);
	}

	template<class Ty_>
	void deserialize(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if(blockItr->isExist())
			{
				std::static_pointer_cast<spihtBlock>(**blockItr)->setLevel(this->maxLevel_);
				this->blockDeserialize<Ty_>(bs, (**blockItr));
			}
			
			++(*blockItr);
		}
	}

	template <typename Ty_>
	void blockDeserialize(bstream& bs, pBlock curBlock)
	{
		pSpihtBlock spBlock = std::static_pointer_cast<spihtBlock>(curBlock);
		spBlock->deserializeTy<Ty_>(bs);
	}

public:
	inline void setLevel(size_t maxLevel)
	{
		this->maxLevel_ = maxLevel;
	}
	inline size_t getLevel()
	{
		return this->maxLevel_;
	}

protected:
	size_t maxLevel_;
};
}		// core
}		// msdb
#endif	// _MSDB_SPIHTCHUNK_H_