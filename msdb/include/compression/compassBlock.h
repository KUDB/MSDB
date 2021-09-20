#pragma once
#ifndef _MSDB_COMPASSBLOCK_H_
#define _MSDB_COMPASSBLOCK_H_

#include <pch.h>
#include <array/memBlock.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
class compassBlock;
using pCompassBlock = std::shared_ptr<compassBlock>;

class compassBlock : public memBlock
{
public:
	compassBlock(pBlockDesc desc);
	virtual ~compassBlock();

public:
	template <typename Ty_>
	struct bin
	{
	public:
		std::vector<position_t> positional_;
		std::vector<Ty_> residual_;
	};

public:
	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		std::vector<compassBlock::bin<Ty_>> bins_;
		this->buildBinFronValue<Ty_>(bins_);

		assert(bins_.size() == this->numBins_ && "number of bins comparison at SerializeTy");
		for (size_t bi = 0; bi < this->numBins_; ++bi)
		{
			auto curBin = bins_[bi];

			if (curBin.positional_.size() == 0)
			{
				// No values
				bs << setw(1) << 0;
			} else
			{
				// Value exist
				bs << setw(1) << 1;

				this->serializePositional(bs, curBin.positional_);
				this->serialzieResidual<Ty_>(bs, curBin.residual_);
			}
		}
	}

	template<typename Ty_>
	void deserializeTy(bstream& bs)
	{
		std::vector<compassBlock::bin<Ty_>> bins_;
		bins_.resize(this->numBins_, bin<Ty_>());

		assert(bins_.size() == this->numBins_ && "number of bins comparison at deserializeTy");
		for (size_t bi = 0; bi < this->numBins_; ++bi)
		{
			auto curBin = &(bins_[bi]);

			char isExist = false;
			bs >> setw(1);
			bs >> isExist;

			if (isExist)
			{
				this->deserializePositional(bs, curBin->positional_);
				this->deserializeResidual<Ty_>(bs, curBin->residual_, curBin->positional_.size());
			}
		}

		this->buildArrayFromBin<Ty_>(bins_);
	}

private:
	template <typename Ty_>
	inline Ty_ getBinValueRange()
	{
		// To prevent overflow at uint64_t
		// Pow with '_TySize - 1'
		// Make double at returning
		uint64_t maxValue = pow(2, _TySize_ - 1);
		return (maxValue / this->numBins_) * 2;
	}

	template <typename Ty_>
	void buildBinFronValue(std::vector<compassBlock::bin<Ty_>>& bins_)
	{
		bins_.resize(this->numBins_, bin<Ty_>());

		Ty_ minValue = (Ty_)0x1 << (sizeof(Ty_) * CHAR_BIT - 1);
		Ty_ binValueRange = this->getBinValueRange<Ty_>();
		assert(binValueRange != 0 && binValueRange > 0);

		bool hasNegative = _TY_HAS_NEGATIVE_VALUE_;
		uint64_t negativeToPositive = ceil(this->numBins_ / 2.0);

		auto iit = this->getItemIterator();
		while (!iit->isEnd())
		{
			Ty_ value = (**iit).get<Ty_>();
			size_t binIndex = this->getBinIndexForValue(value, binValueRange, hasNegative, negativeToPositive);
			assert(binIndex >= 0);
			compassBlock::bin<Ty_>* curBin = &(bins_.at(binIndex));

			curBin->positional_.push_back(iit->seqPos());
			if (!hasNegative)
			{
				curBin->residual_.push_back(value % binValueRange);
			} else
			{
				Ty_ residual = value - (binIndex * binValueRange + minValue);
				curBin->residual_.push_back(residual);
				assert(residual >= 0);
			}

			++(*iit);
		}
	}

	template <typename Ty_>
	inline size_t getBinIndexForValue(Ty_ value, Ty_ binValueRange, bool hasNegative, size_t negativeToPositive = 0)
	{
		if (hasNegative)
		{
			return (size_t)floor(value / (double)binValueRange) + negativeToPositive;
		}

		return (uint64_t)(value / binValueRange);
	}

	template <typename Ty_>
	void buildArrayFromBin(std::vector<compassBlock::bin<Ty_>>& bins_)
	{
		Ty_ binValueRange = this->getBinValueRange<Ty_>();
		auto iit = this->getItemIterator();

		Ty_ minValue = (Ty_)0x1 << (_TySize_ - 1);	// 1000 0000
		position_t seqCapacity = iit->getCapacity();

		assert(bins_.size() == this->numBins_);
		for (size_t bi = 0; bi < this->numBins_; ++bi)
		{
			auto curBin = bins_[bi];
			Ty_ curBinBase;
			if (!_TY_HAS_NEGATIVE_VALUE_)
			{
				curBinBase = binValueRange * bi;
			} else
			{
				curBinBase = binValueRange * bi + minValue;
			}

			assert(curBin.positional_.size() == curBin.residual_.size());

			for (size_t i = 0; i < curBin.positional_.size(); ++i)
			{
				auto pos = curBin.positional_[i];
				auto residual = curBin.residual_[i];

				//assert(pos < seqCapacity);
				if (pos > seqCapacity)
				{
					BOOST_LOG_TRIVIAL(error) << "[" << bi << "](" << i << "): " << "Capacity: " << seqCapacity << ", pos: " << pos << ", residual:" << residual;
					return;
				}
				auto arrValue = iit->getAtSeqPos(pos);
				arrValue.set<Ty_>(curBinBase + residual);
			}
		}
	}

	// Expects positional vector is ordered
	void serializePositional(bstream& bs, std::vector<position_t>& positional);
	void deserializePositional(bstream& bs, std::vector<position_t>& positional);

	template <typename Ty_>
	void serialzieResidual(bstream& bs, std::vector<Ty_>& residual)
	{
		position_t maxResidual = 0;
		for (auto r : residual)
		{
			assert(r >= 0);
			if (maxResidual < r)
			{
				maxResidual = r;
			}
		}

		position_t bMaxResidual = std::max({ msb<position_t>(maxResidual), (unsigned char)1 });
		bs << setw(CHAR_BIT) << bMaxResidual;
		bs << setw(bMaxResidual);

		for (auto r : residual)
		{
			bs << r;
		}
	}

	template <typename Ty_>
	void deserializeResidual(bstream& bs, std::vector<Ty_>& residual, size_t numResiduals)
	{
		bit_cnt_type bMaxResidual = 0;
		Ty_ value;
		bs >> setw(CHAR_BIT) >> bMaxResidual;
		bs >> setw(bMaxResidual);

		while (numResiduals--)
		{
			bs >> value;
			residual.push_back(value);
		}
	}

public:
	void setNumBins(size_t numBins);

private:
	size_t numBins_;
};
}		// core
}		// msdb
#endif	// _MSDB_COMPASSBLOCK_H_