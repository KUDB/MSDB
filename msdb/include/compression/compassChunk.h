#pragma once
#ifndef _MSDB_COMPASSCHUNK_H_
#define _MSDB_COMPASSCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <compression/compassBlock.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
class compassChunk;
using pCompassChunk = std::shared_ptr<compassChunk>;

class compassChunk : public flattenChunk
{
public:
	compassChunk(pChunkDesc desc);
	virtual ~compassChunk();

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if (blockItr->isExist())
			{
				pCompassBlock cpBlock = std::static_pointer_cast<compassBlock>(**blockItr);
				cpBlock->setNumBins(this->numBins_);
				try
				{
					cpBlock->serializeTy<Ty_>(bs);
				} catch (...)
				{
					std::cout << "Exception" << std::endl;
				}
			}

			++(*blockItr);
		}
	}

	template<class Ty_>
	void deserializeTy(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if (blockItr->isExist())
			{
				pCompassBlock cpBlock = std::static_pointer_cast<compassBlock>(**blockItr);
				cpBlock->setNumBins(this->numBins_);
				cpBlock->deserializeTy<Ty_>(bs);
			}

			++(*blockItr);
		}
	}

public:
	void setNumBins(size_t numBins);

private:
	size_t numBins_;
};
}	// core
}	// msdb
#endif	// _MSDB_COMPASSCHUNK_H_
