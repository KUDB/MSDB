#pragma once
#ifndef _MSDB_SPIHTBLOCK_H_
#define _MSDB_SPIHTBLOCK_H_

#include <pch.h>
#include <array/memBlock.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
class spihtBlock;
using pSpihtBlock = std::shared_ptr<spihtBlock>;

class spihtBlock : public memBlock
{
public:
	spihtBlock(pBlockDesc desc);
	virtual ~spihtBlock();

	template <typename Ty_>
	struct SpihtNode
	{
	public:
		Ty_ value_;
		Ty_ D_;
		Ty_ G_;
	};

//////////////////////////////
// Serializable
//////////////////////////////
public:
	void init(dimension& bandDims);
	
	template<typename Ty_>
	std::vector<Ty_> checkDescendants(bstream& bs, std::vector<SpihtNode<Ty_>>& arr, dimension& bandDims, coor parent_coor, size_t parent_index, Ty_ signBit, size_t curLevel)
	{
		std::vector<Ty_> parentDG(2);
		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		int sibling_size = (int)pow(2, dSize);
		dimension blockDims = this->desc_->dims_;
		curLevel++;

		coor my_coor(parent_coor);
		if (curLevel == 1)		// first band
		{
			my_coor /= 2;
			my_coor *= 2;
			for (int d = (int)dSize - 1; d >= 0; d--)
			{
				if (parent_coor[d] & 0x1)
				{
					my_coor[d] = my_coor[d] + bandDims[d];
				}
			}
		} else
		{
			my_coor *= 2;
		}
		size_t my_index = parent_index * sibling_size;
		
		for (int i = 0; i < sibling_size; i++)
		{
			itemItr->moveTo(my_coor);
			auto my_value = (**itemItr).get<Ty_>();

			if (my_value == signBit)  // negative
			{
				bs << 1;
			}
			else  // positive
			{	
				bs << 0;
			}

			// absolute 
			if ((my_value & signBit) && (my_value != signBit))
			{
				my_value = ~my_value;
				my_value += 1;
				my_value = my_value | signBit;
			}

			arr[my_index].value_ = my_value;

			if (curLevel == this->maxLevel_)
			{
				parentDG[0] = parentDG[0] | my_value;
			}
			else
			{
				std::vector<Ty_> childDG = this->checkDescendants(bs, arr, bandDims, my_coor, my_index, signBit, curLevel);		// for me
				arr[my_index].D_ = childDG[0];
				arr[my_index].G_ = childDG[1];

				parentDG[0] = parentDG[0] | arr[my_index].D_ | my_value;
				parentDG[1] = parentDG[1] | arr[my_index].D_;
			}

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				my_coor[d] = my_coor[d] + 1;
				if (my_coor[d] & 0x1)
				{
					break;
				}
				else
				{
					my_coor[d] = my_coor[d] - 2;
				}
			}	
			my_index++;
		}

		return parentDG;
	}

	template<typename Ty_>
	void decode_signsetting(bstream& bs, std::vector<SpihtNode<Ty_>>& arr, dimension& bandDims, coor parent_coor, size_t parent_index, Ty_ signBit, size_t curLevel)
	{
		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		int sibling_size = (int)pow(2, dSize);
		dimension blockDims = this->desc_->dims_;
		curLevel++;

		coor my_coor(parent_coor);
		if (curLevel == 1)		// first band
		{
			my_coor /= 2;
			my_coor *= 2;
			for (int d = (int)dSize - 1; d >= 0; d--)
			{
				if (parent_coor[d] & 0x1)
				{
					my_coor[d] = my_coor[d] + bandDims[d];
				}
			}
		}
		else
		{
			my_coor *= 2;
		}
		size_t my_index = parent_index * sibling_size;

		char codeBit;
		for (int i = 0; i < sibling_size; i++)
		{
			itemItr->moveTo(my_coor);
			auto my_value = arr[my_index].value_;

			bs >> codeBit;

			if (codeBit)
			{
				my_value = my_value | signBit;
			}

			arr[my_index].value_ = my_value;

			if (curLevel != this->maxLevel_)
			{
				this->decode_signsetting<Ty_>(bs, arr, bandDims, my_coor, my_index, signBit, curLevel);
			}

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				my_coor[d] = my_coor[d] + 1;
				if (my_coor[d] & 0x1)
				{
					break;
				}
				else
				{
					my_coor[d] = my_coor[d] - 2;
				}
			}
			my_index++;
		}
	}

	template<typename Ty_>
	void decode_travel(std::vector<SpihtNode<Ty_>>& arr, dimension& bandDims, coor parent_coor, size_t parent_index, Ty_ signBit, size_t curLevel)
	{
		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		int sibling_size = (int)pow(2, dSize);
		dimension blockDims = this->desc_->dims_;
		curLevel++;

		coor my_coor(parent_coor);
		if (curLevel == 1)		// first band
		{
			my_coor /= 2;
			my_coor *= 2;
			for (int d = (int)dSize - 1; d >= 0; d--)
			{
				if (parent_coor[d] & 0x1)
				{
					my_coor[d] = my_coor[d] + bandDims[d];
				}
			}
		}
		else
		{
			my_coor *= 2;
		}
		size_t my_index = parent_index * sibling_size;

		for (int i = 0; i < sibling_size; i++)
		{
			itemItr->moveTo(my_coor);
			auto my_value = arr[my_index].value_;

			// absolute 
			if ((my_value & signBit) && (my_value != signBit))
			{
				my_value = ~my_value;
				my_value += 1;
				my_value = my_value | signBit;
			}
			
			(**itemItr).set<Ty_>(my_value);

			if (curLevel != this->maxLevel_)
			{
				this->decode_travel<Ty_>(arr, bandDims, my_coor, my_index, signBit, curLevel);
			}
			
			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				my_coor[d] = my_coor[d] + 1;
				if (my_coor[d] & 0x1)
				{
					break;
				}
				else
				{
					my_coor[d] = my_coor[d] - 2;
				}
			}
			my_index++;
		}
	}

	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		bs << setw(1);
		dimension bandDims(this->desc_->dims_);
		bandDims /= pow(2, this->maxLevel_);

		this->init(bandDims);
		this->encode_progress<Ty_>(bs, bandDims);
	}

	template<class Ty_>
	void encode_progress(bstream& bs, dimension& bandDims)
	{
		size_t maxStep = sizeof(Ty_) * 8;
		Ty_ signBit = (Ty_)0x1 << (maxStep - 1);
		Ty_ stepBit = (Ty_)0x1 << (maxStep - 2);

		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		dimension blockDims = this->desc_->dims_;

		size_t block_num = 1;
		size_t band_num = 1;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			block_num *= blockDims[d];
			band_num *= bandDims[d];
		}
		size_t noChild_num = band_num / 4;
		std::vector<SpihtNode<Ty_>> arr(block_num);

		// descendants travel 
		coor band_coor(dSize);
		size_t index = 0;
		for (size_t i = 0; i < band_num; i++)
		{
			itemItr->moveTo(band_coor);
			auto value = (**itemItr).get<Ty_>();

			if (value == signBit)  // negative
			{
				bs << 1;
			}
			else  // positive
			{
				bs << 0;
			}

			// absolute 
			if ((value & signBit) && (value != signBit))
			{
				value = ~value;
				value += 1;
				value = value | signBit;
			}
			arr[index].value_ = value;

			if (index >= noChild_num)
			{
				std::vector<Ty_> DG = this->checkDescendants<Ty_>(bs, arr, bandDims, band_coor, index, signBit, 0);
				arr[index].D_ = DG[0];
				arr[index].G_ = DG[1];
			}

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				band_coor[d] = band_coor[d] + 2;
				if (band_coor[d] >= bandDims[d])
				{
					band_coor[d] = band_coor[d] - bandDims[d];
					if (d == 0)
					{
						for (int dd = (int)dSize - 1; dd >= 0; dd--)
						{
							band_coor[dd] = band_coor[dd] + 1;
							if (band_coor[dd] == 1)
							{
								break;
							}
							else
							{
								band_coor[dd] = 0;
							}
						}
					}
				}
				else
				{
					break;
				}
			}	
			index++;
		}

		for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
		{
			size_t LSP_size = this->LSP_.size();
			this->encode_sigpass<Ty_>(bs, arr, bandDims, signBit, stepBit);
			this->encode_refinepass<Ty_>(bs, arr, stepBit, LSP_size);
			stepBit = stepBit >> 1;
		}
	}

	template<class Ty_>
	void encode_sigpass(bstream& bs, std::vector<SpihtNode<Ty_>>& arr, dimension& bandDims, Ty_ signBit, Ty_ stepBit)
	{
		size_t dSize = this->getDSize();
		int sibling_size = (int)pow(2, dSize);
		dimension blockDims = this->desc_->dims_;

		size_t block_num = 1;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			block_num *= blockDims[d];
		}

		// LIP
		size_t LIP_size = this->LIP_.size();
		for (size_t i = 0; i < LIP_size; i++)
		{
			size_t LIP_index = this->LIP_.front();
			this->LIP_.pop_front();
			Ty_ LIP_value = arr[LIP_index].value_;

			if (LIP_value & stepBit)
			{
				bs << 1;
				if (LIP_value & signBit)  // -
				{
					bs << 1;
				} else  // +
				{
					bs << 0;
				}
				this->LSP_.push_back(LIP_index);
			} else
			{
				bs << 0;
				this->LIP_.push_back(LIP_index);
			}
		}

		// LIS
		std::list<size_t> TMP_;
		std::list<char> TMP_TYPE_;
		while (this->LIS_.size() != 0)
		{
			size_t LIS_index = this->LIS_.front();
			this->LIS_.pop_front();
			Ty_ LIS_value = arr[LIS_index].value_;
			size_t child_index = LIS_index * sibling_size;

			char LIS_type = this->LIS_TYPE_.front();
			this->LIS_TYPE_.pop_front();
			if (LIS_type == 'A')	// type A
			{
				if (arr[LIS_index].D_ & stepBit)
				{
					bs << 1;

					bool haveGrand = true;
					if (child_index * sibling_size >= block_num)
					{
						haveGrand = false;
					}

					for (size_t i = 0; i < sibling_size; i++)
					{
						Ty_ child_value = arr[child_index].value_;

						if (child_value & stepBit)
						{
							bs << 1;
							if (child_value & signBit)  // -
							{
								bs << 1;
							} else  // +
							{
								bs << 0;
							}
							this->LSP_.push_back(child_index);
						} else
						{
							bs << 0;
							this->LIP_.push_back(child_index);
						}

						child_index++;
					}

					if (haveGrand)
					{
						this->LIS_.push_back(LIS_index);
						this->LIS_TYPE_.push_back('B');
					}
				} else
				{
					bs << 0;
					TMP_.push_back(LIS_index);
					TMP_TYPE_.push_back(LIS_type);
				}
			} else    // type B
			{
				if (arr[LIS_index].G_ & stepBit)
				{
					bs << 1;

					for (size_t i = 0; i < sibling_size; i++)
					{
						this->LIS_.push_back(child_index);
						this->LIS_TYPE_.push_back('A');

						child_index++;
					}
				} else
				{
					bs << 0;
					TMP_.push_back(LIS_index);
					TMP_TYPE_.push_back(LIS_type);
				}
			}
		}
		this->LIS_ = TMP_;
		this->LIS_TYPE_ = TMP_TYPE_;
	}

	template<class Ty_>
	void encode_refinepass(bstream& bs, std::vector<SpihtNode<Ty_>>& arr, Ty_ stepBit, size_t LSP_size)
	{
		std::list<size_t> TMP_ = this->LSP_;
		for (size_t i = 0; i < LSP_size; i++)
		{
			size_t LSP_index = TMP_.front();
			TMP_.pop_front();
			Ty_ LSP_value = arr[LSP_index].value_;

			if (LSP_value & stepBit)
			{
				bs << 1;
			} else
			{
				bs << 0;
			}
		}
	}

	template<typename Ty_>
	void deserializeTy(bstream& bs)
	{
		bs >> setw(1);
		dimension bandDims(this->desc_->dims_);
		bandDims /= pow(2, this->maxLevel_);

		this->init(bandDims);
		this->decode_progress<Ty_>(bs, bandDims);
	}

	template<class Ty_>
	void decode_progress(bstream& bs, dimension& bandDims)
	{
		size_t maxStep = sizeof(Ty_) * 8;
		Ty_ signBit = (Ty_)0x1 << (maxStep - 1);
		Ty_ stepBit = (Ty_)0x1 << (maxStep - 2);
		char codeBit;

		auto itemItr = this->getItemIterator();
		size_t dSize = this->getDSize();
		dimension blockDims = this->desc_->dims_;

		size_t block_num = 1;
		size_t band_num = 1;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			block_num *= blockDims[d];
			band_num *= bandDims[d];
		}
		size_t noChild_num = band_num / 4;
		std::vector<SpihtNode<Ty_>> arr(block_num);

		// signBit setting 
		coor sband_coor(dSize);
		size_t sindex = 0;
		for (size_t i = 0; i < band_num; i++)
		{
			itemItr->moveTo(sband_coor);
			auto value = arr[sindex].value_;

			bs >> codeBit;

			if (codeBit)
			{
				value = value | signBit;
			}

			arr[sindex].value_ = value;

			if (sindex >= noChild_num)
			{
				this->decode_signsetting<Ty_>(bs, arr, bandDims, sband_coor, sindex, signBit, 0);
			}

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				sband_coor[d] = sband_coor[d] + 2;
				if (sband_coor[d] >= bandDims[d])
				{
					sband_coor[d] = sband_coor[d] - bandDims[d];
					if (d == 0)
					{
						for (int dd = (int)dSize - 1; dd >= 0; dd--)
						{
							sband_coor[dd] = sband_coor[dd] + 1;
							if (sband_coor[dd] == 1)
							{
								break;
							}
							else
							{
								sband_coor[dd] = 0;
							}
						}
					}
				}
				else
				{
					break;
				}
			}
			sindex++;
		}

		
		for (size_t curStep = 0; curStep < maxStep - 1; curStep++)
		{
			size_t LSP_size = this->LSP_.size();
			this->decode_sigpass<Ty_>(bs, arr, bandDims, signBit, stepBit);
			this->decode_refinepass<Ty_>(bs, arr, stepBit, LSP_size);
			stepBit = stepBit >> 1;
		}

		// descendants travel 
		coor band_coor(dSize);
		size_t index = 0;
		for (size_t i = 0; i < band_num; i++)
		{
			itemItr->moveTo(band_coor);
			auto value = arr[index].value_;

			// absolute 
			if ((value & signBit) && (value != signBit))
			{
				value = ~value;
				value += 1;
				value = value | signBit;
			}
			
			(**itemItr).set<Ty_>(value);

			if (index >= noChild_num)
			{
				this->decode_travel<Ty_>(arr, bandDims, band_coor, index, signBit, 0);
			}

			for (int d = (int)dSize - 1; d >= 0; d--)	// iteration(?)
			{
				band_coor[d] = band_coor[d] + 2;
				if (band_coor[d] >= bandDims[d])
				{
					band_coor[d] = band_coor[d] - bandDims[d];
					if (d == 0)
					{
						for (int dd = (int)dSize - 1; dd >= 0; dd--)
						{
							band_coor[dd] = band_coor[dd] + 1;
							if (band_coor[dd] == 1)
							{
								break;
							}
							else
							{
								band_coor[dd] = 0;
							}
						}
					}
				}
				else
				{
					break;
				}
			}
			index++;
		}
	}

	template<class Ty_>
	void decode_sigpass(bstream& bs, std::vector<SpihtNode<Ty_>>& arr, dimension& bandDims, Ty_ signBit, Ty_ stepBit)
	{
		size_t dSize = this->getDSize();
		int sibling_size = (int)pow(2, dSize);
		dimension blockDims = this->desc_->dims_;
		char codeBit;

		size_t block_num = 1;
		for (int d = (int)dSize - 1; d >= 0; d--)
		{
			block_num *= blockDims[d];
		}

		// LIP
		size_t LIP_size = this->LIP_.size();
		for (size_t i = 0; i < LIP_size; i++)
		{
			size_t LIP_index = this->LIP_.front();
			this->LIP_.pop_front();
			Ty_ LIP_data = arr[LIP_index].value_;

			bs >> codeBit;

			if (codeBit)
			{
				LIP_data = LIP_data ^ stepBit;

				bs >> codeBit;

				if (codeBit)  // -
				{
					LIP_data = LIP_data | signBit;
				}

				arr[LIP_index].value_ = LIP_data;
				this->LSP_.push_back(LIP_index);
			} else
			{
				this->LIP_.push_back(LIP_index);
			}
		}

		// LIS
		std::list<size_t> TMP_;
		std::list<char> TMP_TYPE_;
		coor grand_coor(dSize);
		while (this->LIS_.size() != 0)
		{
			size_t LIS_index = this->LIS_.front();
			this->LIS_.pop_front();
			Ty_ LIS_data = arr[LIS_index].value_;
			size_t child_index = LIS_index * sibling_size;

			char LIS_type = this->LIS_TYPE_.front();
			this->LIS_TYPE_.pop_front();
			if (LIS_type == 'A')	// type A
			{
				bs >> codeBit;

				if (codeBit)
				{
					bool haveGrand = true;
					if (child_index * sibling_size >= block_num)
					{
						haveGrand = false;
					}

					for (size_t i = 0; i < sibling_size; i++)
					{
						Ty_ child_value = arr[child_index].value_;

						bs >> codeBit;

						if (codeBit)
						{
							child_value = child_value ^ stepBit;

							bs >> codeBit;

							if (codeBit)  // -
							{
								child_value = child_value | signBit;
							}

							arr[child_index].value_ = child_value;
							this->LSP_.push_back(child_index);
						} else
						{
							this->LIP_.push_back(child_index);
						}

						child_index++;
					}

					if (haveGrand)
					{
						this->LIS_.push_back(LIS_index);
						this->LIS_TYPE_.push_back('B');
					}
				} else
				{
					TMP_.push_back(LIS_index);
					TMP_TYPE_.push_back(LIS_type);
				}
			} else    // type B
			{
				bs >> codeBit;

				if (codeBit)
				{
					for (size_t i = 0; i < sibling_size; i++)
					{
						this->LIS_.push_back(child_index);
						this->LIS_TYPE_.push_back('A');

						child_index++;
					}
				} else
				{
					TMP_.push_back(LIS_index);
					TMP_TYPE_.push_back(LIS_type);
				}
			}
		}
		this->LIS_ = TMP_;
		this->LIS_TYPE_ = TMP_TYPE_;
	}

	template<class Ty_>
	void decode_refinepass(bstream& bs, std::vector<SpihtNode<Ty_>>& arr, Ty_ stepBit, size_t LSP_size)
	{
		char codeBit;

		std::list<size_t> TMP_ = this->LSP_;
		for (size_t i = 0; i < LSP_size; i++)
		{
			size_t LSP_index = TMP_.front();
			TMP_.pop_front();
			Ty_ LSP_value = arr[LSP_index].value_;

			bs >> codeBit;

			if (codeBit)
			{
				LSP_value = LSP_value ^ stepBit;
				arr[LSP_index].value_ = LSP_value;
			}
		}
	}

public:
	void setLevel(size_t maxLevel);

private:
	std::list<size_t> LIP_;
	std::list<size_t> LIS_;
	std::list<char> LIS_TYPE_;
	std::list<size_t> LSP_;

	size_t maxLevel_;
};
}		// core
}		// msdb
#endif	// _MSDB_SPIHTBLOCK_H_
