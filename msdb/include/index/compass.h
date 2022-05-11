#pragma once
#ifndef _MSDB_COMPASS_H_
#define _MSDB_COMPASS_H_

#include <pch.h>
#include <index/attributeIndex.h>
#include <util/element.h>
#include <array/chunkIterator.h>

#define _STR_PARAM_COMPASS_BINS_	"compass_bins"

namespace msdb
{
namespace core
{
class compassIndex;
using pCompassIndex = std::shared_ptr<compassIndex>;

class compassBlockIndex;
using pCompassBlockIndex = std::shared_ptr<compassBlockIndex>;

class compassBin
{
public:
	size_t numElements = 0;
};

class compassBlockIndex
{
public:
	compassBlockIndex(size_t numBins, size_t binValueRange, bool hasNegative, uint64_t negativeToPositive = 0);

public:
	compassBin& at(size_t index)
	{
		return this->blockBins_[index];
	}

public:
	inline size_t getNumBins()
	{
		return this->numBins_;
	}

	inline size_t getBinIndexForValue(uint64_t value)
	{
		if (this->hasNegative_)
		{
			//assert((uint64_t)(value / this->binValueRange_ + this->negativeToPositive_) < this->numBins_);
			//return (uint64_t)(value / this->binValueRange_ + this->negativeToPositive_);

			assert(floor((int64_t)value / (double)this->binValueRange_) + this->negativeToPositive_ < this->numBins_);
			return (size_t)floor((int64_t)value / (double)this->binValueRange_) + this->negativeToPositive_;
		}

		assert((uint64_t)(value / this->binValueRange_) < this->numBins_);
		return (uint64_t)(value / this->binValueRange_);
	}

protected:
	size_t numBins_;
	size_t binValueRange_;
	bool hasNegative_;
	uint64_t negativeToPositive_;

protected:
	std::vector<compassBin> blockBins_;
};

class compassIndex : public attributeIndex
{
public:
	using size_type = size_t;

public:
	compassIndex(const eleType eType, const size_type numBins)
		: eType_(eType), numBins_(numBins)
	{
	}

public:
	static pCompassIndex createIndex(const eleType eType, const size_type numBins);

public:
	inline attrIndexType getType()
	{
		return attrIndexType::COMPASS;
	}

	inline eleType getEleType()
	{
		return this->eType_;
	}
	inline size_type getNumBins()
	{
		return this->numBins_;
	}
	inline void setNumBins(size_type numBins)
	{
		this->numBins_ = numBins;
	}

public:
	virtual void build(pChunkIterator& it) = 0;

protected:
	eleType eType_;
	size_type numBins_;
};

template <typename Ty_>
class compassIndexImpl : public compassIndex
{
public:
	using size_type = size_t;

public:
	compassIndexImpl(const eleType eType, const size_type numBins)
		: compassIndex(eType, numBins)
	{
	}

public:
	inline size_t getNumBins()
	{
		return this->numBins_;
	}

protected:
	inline size_t getBinIndexForValue(Ty_ value, Ty_ binValueRange, bool hasNegative, size_t negativeToPositive = 0)
	{
		if (hasNegative)
		{
			//assert((uint64_t)(value / this->binValueRange_ + this->negativeToPositive_) < this->numBins_);
			//return (uint64_t)(value / this->binValueRange_ + this->negativeToPositive_);

			assert(floor(value / (double)binValueRange) + negativeToPositive < this->numBins_);
			return (size_t)floor(value / (double)binValueRange) + negativeToPositive;
		}

		assert((uint64_t)(value / binValueRange) < this->numBins_);
		return (uint64_t)(value / binValueRange);
	}

	inline Ty_ getBinValueRange()
	{
		// To prevent overflow at uint64_t
		// Pow with '_TySize - 1'
		// Make double at returning
		uint64_t maxValue = pow(2, _TySize_ - 1);
		return (maxValue / this->numBins_) * 2;
	}

public:
	pCompassBlockIndex at(chunkId cid, blockId bid)
	{
		return arrayBins_[cid][bid];
	}

	void build(pChunkIterator& cit) override
	{
		this->arrayBins_.clear();
		this->arrayBins_.resize(cit->getCapacity());

		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				auto bit = (*cit)->getBlockIterator();
				auto cid = cit->seqPos();

				this->arrayBins_[cid].clear();
				this->arrayBins_[cid].resize(bit->getCapacity());

				while (!bit->isEnd())
				{
					if (bit->isExist())
					{
						this->buildBlockIndex(cid, **bit);
					}

					++(*bit);
				}
			}

			++(*cit);
		}
	}

	void buildBlockIndex(const chunkId cid, pBlock myBlock)
	{
		Ty_ binValueRange = this->getBinValueRange();
		uint64_t negativeToPositive = ceil(this->numBins_ / 2.0);
		bool hasNegative = _TY_HAS_NEGATIVE_VALUE_;
		Ty_ minValue = (Ty_)0x1 << (sizeof(Ty_) * CHAR_BIT - 1);

		assert(binValueRange != 0 && binValueRange > 0);

		auto blockIndex = std::make_shared<compassBlockIndex>(this->numBins_, binValueRange, _TY_HAS_NEGATIVE_VALUE_, negativeToPositive);

		this->arrayBins_[cid][myBlock->getId()] = blockIndex;
		//std::vector<compassBin<Ty_>>* blockBins = (arrayBins_[cid][myBlock->getId()]);

		auto iit = myBlock->getItemIterator();
		while (!iit->isEnd())
		{
			Ty_ value = (**iit).get<Ty_>();
			auto binIndex = this->getBinIndexForValue(value, binValueRange, hasNegative, negativeToPositive);

			compassBin* curBin = &(blockIndex->at(binIndex));
			curBin->numElements++;

			++(*iit);
		}
	}

private:
	std::vector<std::vector<std::shared_ptr<compassBlockIndex>>> arrayBins_;
};
}		// core
}		// msdb
#endif	// _MSDB_COMPASS_H_