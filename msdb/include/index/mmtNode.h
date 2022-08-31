#pragma once
#ifndef _MSDB_MMTNODE_H_
#define _MSDB_MMTNODE_H_

#include <pch.h>
#include <index/attributeIndex.h>
#include <util/math.h>
#include <util/logger.h>
#include <io/bitstream.h>
#include <array/dimension.h>

namespace msdb
{
namespace core
{
class mmtNode;
using pMmtNode = std::shared_ptr<mmtNode>;

class mmtNode : public attributeIndex
{
public:
	//using TyType = Ty_;

	sig_bit_type bMax_;			// nth significant of max value
	sig_bit_type bMin_;			// nth significant of min value
	bit_cnt_type bMaxDelta_;	// bMax_ delta from a parent node
	bit_cnt_type bMinDelta_;	// bMin_ delta from a parent node
	bit_cnt_type order_;		// n
	bit_cnt_type childOrder_;	// order for child node
	bit_cnt_type bits_;			// required bits to represent min/max value
	boost::any max_;
	boost::any min_;
	boost::any realMax_;
	boost::any realMin_;
	bit_cnt_type jumpBits_;
	bool finished_;

	// For Test
	//dimension chunkCoor_;
	//dimension blockCoor_;
	//dimension nodeCoor_;
	size_t seqPos_;
	pMmtNode parent_;

public:
	mmtNode() : bMax_(0), bMin_(0), bits_(0x80), order_(1),
		bMaxDelta_(0), bMinDelta_(0), max_(0), min_(0),
		realMin_(0), realMax_(0), finished_(false),
		seqPos_(0), jumpBits_(0), childOrder_(0)
		//chunkCoor_(1), blockCoor_(1), nodeCoor_(1),
	{
	}

public:
	inline attrIndexType getType()
	{
		return attrIndexType::MMT;
	}

	// use function cause of unknown compile error
	inline void inDelta(bstream& bs)
	{
		bs >> setw(this->bits_) >> this->bMinDelta_ >> this->bMaxDelta_;
	}

	inline void outDelta(bstream& bs)
	{
		bs << setw(this->bits_) << this->bMinDelta_ << this->bMaxDelta_;
	}

	inline void inChildOrderChanged(bstream& bs)
	{
		bit_cnt_type orderChangedDelta = 0;
		bs >> setw(msb<sig_bit_type>(abs_(this->bMax_ - 1))) >> orderChangedDelta;
		if (orderChangedDelta)
		{
			this->childOrder_ = this->order_ + orderChangedDelta;
		} else
		{
			this->childOrder_ = 0;
		}
	}

	inline void outChildOrderChanged(bstream& bs)
	{
		//////////
		// To change the child order, bMax == bMin.
		// Order has range [0 : max({bMax, bMin}) - 1] -> [0:bMax-1]
		//
		// this->bits_ = msb(bMax - bMin)
		// required bit = msb(abs(bMax));
		//
		if (this->childOrder_ > this->order_)
		{
			// Next order
			bs << setw(msb<sig_bit_type>(abs_(this->bMax_ - 1))) << this->childOrder_ - this->order_;
		} else
		{
			// cur node is last, end
			bs << setw(msb<sig_bit_type>(abs_(this->bMax_ - 1))) << 0x0;
		}
	}

public:
	template <typename Ty_>
	inline void inJumpedBits(bstream& bs, bit_cnt_type& jumpBits, Ty_& jumpValue)
	{
		jumpBits = 0;
		jumpValue = 0;

		if (this->childOrder_)
		{
			bit_cnt_type orderDelta = this->childOrder_ - this->order_;
			char inBit = 0;

			// make jumpBits
			bs >> setw(1);
			while (orderDelta)
			{
				++jumpBits;
				inBit = 0;
				bs >> inBit;
				jumpValue <<= 1;
				jumpValue |= inBit;

				if (inBit)
				{
					--orderDelta;
				}
			}

			// erase last significant bit
			jumpValue >>= 1;
			--jumpBits;

			// Postfix size should be >= 0
			assert(abs_(this->bMax_) - jumpBits - 1 >= 0);
			// Make to original value
			jumpValue <<= abs_(this->bMax_) - jumpBits - 1;
			this->jumpBits_ = jumpBits;
		} else
		{
			char inBit = 0;

			if (abs_(this->bMax_) > 1)
			{
				jumpBits = abs_(this->bMax_) - 1;
				bs >> setw(jumpBits) >> jumpValue;
			}

			bs >> setw(1) >> inBit;
			if (inBit != 1)
			{
				// Check flag bit
				std::stringstream errorMsg;
				errorMsg << "mmtNode.h::inJumpedBits(bstream& bs, bit_cnt_type& jumpBits, Ty_& jumpValue)" << std::endl;
				errorMsg << "HMMT: fail to decode jump bit, inBit should be set" << std::endl;
				errorMsg << "[bMax: " << static_cast<int>(abs_(this->bMax_)) << " / jumpBits: " << static_cast<int>(jumpBits) << " / jumpValue: " << static_cast<int>(jumpValue);
				BOOST_LOG_TRIVIAL(error) << errorMsg.str();
				_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(
					MSDB_EC_QUERY_ERROR, MSDB_ER_ATTR_INDEX_BUILD_FAIL, errorMsg.str()));
			}

			this->jumpBits_ = jumpBits;
		}
	}

public:
	template <typename Ty_>
	inline void outJumpedBits(bstream& bs, bit_cnt_type jumpBits, Ty_ jumpValue)
	{
		if (jumpBits)
		{
			Ty_ mask = ~((Ty_)-1 << jumpBits);
			bit_cnt_type postfixSize = abs_(this->bMax_) - jumpBits - 1;
			assert(postfixSize >= 0);
			bs << setw(jumpBits) << (Ty_)(abs_(jumpValue) >> (postfixSize));

			assert(abs_(this->bMax_) - jumpBits - 1 >= 0);
		}

		// Set flag bit
		bs << setw(1) << 0x1;
	}

	template <typename Ty_>
	inline void inMinMax(bstream& bs)
	{
		Ty_ min, max;
		bs >> setw(this->bits_) >> min >> max;
		this->min_ = min;
		this->max_ = max;
		this->realMin_ = min;
		this->realMax_ = max;
	}

	template <typename Ty_>
	inline void outMinMax(bstream& bs)
	{
		bs << setw(sizeof(Ty_) * CHAR_BIT);
		bs << this->getMin<Ty_>() << this->getMax<Ty_>();
	}

	template <typename Ty_>
	inline void initBits()
	{
		this->bits_ = sizeof(Ty_) * CHAR_BIT;
		this->order_ = 1;
		this->setMinMaxBits<Ty_>();
	}

	template <typename Ty_>
	inline void setMinMaxBits()
	{
		this->bMax_ = msb<Ty_>(abs_(boost::any_cast<Ty_>(max_)), this->order_) * SIGN(boost::any_cast<Ty_>(max_));
		this->bMin_ = msb<Ty_>(abs_(boost::any_cast<Ty_>(min_)), this->order_) * SIGN(boost::any_cast<Ty_>(min_));

#ifndef NDEBUG
		//if(this->bMax_ < 0 || this->bMin_ < 0)
		//{
		//	BOOST_LOG_TRIVIAL(warning) << "bMax: " << static_cast<int64_t>(this->bMax_) << ", bMin: " << static_cast<int64_t>(this->bMin_);
		//}
#endif
	}

	template <typename Ty_>
	inline Ty_ getMax()
	{
		return boost::any_cast<Ty_>(this->max_);
	}

	template <typename Ty_>
	inline Ty_ getMin()
	{
		return boost::any_cast<Ty_>(this->min_);
	}

	template <typename Ty_>
	inline Ty_ getRealMax()
	{
		return boost::any_cast<Ty_>(this->realMax_);
	}

	template <typename Ty_>
	inline Ty_ getRealMin()
	{
		return boost::any_cast<Ty_>(this->realMin_);
	}

	template <typename Ty_>
	inline void print()
	{
		BOOST_LOG_TRIVIAL(debug) << static_cast<int64_t>(this->getMin<Ty_>())
			<< "(" << static_cast<int64_t>(this->bMin_) << ", " << static_cast<int64_t>(this->bMinDelta_) << ")~"
			<< static_cast<int64_t>(this->getMax<Ty_>())
			<< "(" << static_cast<int64_t>(this->bMax_) << ", " << static_cast<int64_t>(this->bMaxDelta_) << ")"
			<< " / b: " << static_cast<int64_t>(this->bits_)
			<< " / bTy: " << static_cast<int64_t>(sizeof(Ty_) * CHAR_BIT)
			<< " / or: " << static_cast<int64_t>(this->order_)
			<< " / real: " << static_cast<int64_t>(this->getRealMin<Ty_>()) << "~" << static_cast<int64_t>(this->getRealMax<Ty_>());
	}

	template <typename Ty_>
	inline std::string toString()
	{
		std::stringstream ss;

		ss << static_cast<int64_t>(this->getMin<Ty_>())
			<< "(" << static_cast<int64_t>(this->bMin_) << ", " << static_cast<int64_t>(this->bMinDelta_) << ")~"
			<< static_cast<int64_t>(this->getMax<Ty_>())
			<< "(" << static_cast<int64_t>(this->bMax_) << ", " << static_cast<int64_t>(this->bMaxDelta_) << ")"
			<< " / b: " << static_cast<int64_t>(this->bits_)
			<< " / bTy: " << static_cast<int64_t>(sizeof(Ty_) * CHAR_BIT)
			<< " / or: " << static_cast<int64_t>(this->order_) << ", cor: " << static_cast<int64_t>(this->childOrder_)
			<< " / real: " << static_cast<int64_t>(this->getRealMin<Ty_>()) << "~" << static_cast<int64_t>(this->getRealMax<Ty_>()) << "\n";
			//<< this->chunkCoor_.toString() << " / " << this->blockCoor_.toString() << " / " << this->nodeCoor_.toString();

		return ss.str();
	}
};

template <typename Ty_>
void nodeUpdateWhenChildOrderChanged(pMmtNode curNode, bit_cnt_type jumpBits, Ty_ jumpValue)
{
	// As child order changed, bMax_ == bMin_
	// Just use one of them (bMax_, bMin_)
	bit_cnt_type tySize = sizeof(Ty_) * CHAR_BIT;
	bit_cnt_type prefixSize = sizeof(Ty_) * CHAR_BIT - abs_(curNode->bMax_) + 1;
	bit_cnt_type postfixSize = abs_(curNode->bMax_) - jumpBits - 1;
	Ty_ jumpMask = (~((Ty_)-1 << jumpBits)) << postfixSize;
	Ty_ prefixMask = ((~jumpMask) >> postfixSize) << postfixSize;

	Ty_ min_ = curNode->getMin<Ty_>();
	Ty_ max_ = curNode->getMax<Ty_>();

	if (curNode->childOrder_ == 0)
	{
		min_ = (Ty_)((prefixMask & abs_((Ty_)min_)) | (jumpMask & abs_((Ty_)jumpValue)));
		max_ = (Ty_)((prefixMask & abs_((Ty_)max_)) | (jumpMask & abs_((Ty_)jumpValue)));

	} else if (max_ >= 0)
	{
		min_ = (Ty_)((prefixMask & abs_((Ty_)min_)) | (jumpMask & abs_((Ty_)jumpValue)));
		max_ = getMaxBoundary<Ty_>(max_, curNode->childOrder_, curNode->bMax_ - jumpBits);
	} else
	{
		min_ = getMinBoundary<Ty_>(min_, curNode->childOrder_, curNode->bMin_ + jumpBits);
		max_ = (Ty_)((prefixMask & abs_((Ty_)max_)) | (jumpMask & abs_((Ty_)jumpValue)));
	}

	//////////
	// SIGN check for negative values
	//
	//    - As they were wrapped with abs_() negative values should change to positive value. 
	//
	//////////
	// NOTE:: special case of -128
	//
	// In case of min_ = -128 (1000 0000)
	// masked min could be negative value
	// after * SING() -> it changed to positive value even it was negative
	//
	if (SIGN(min_) != SIGN(curNode->bMin_))
		min_ = (Ty_)min_ * SIGN(curNode->bMin_);
	if (SIGN(max_) != SIGN(curNode->bMax_))
		max_ = (Ty_)max_ * SIGN(curNode->bMax_);

	curNode->min_ = min_;
	curNode->max_ = max_;

	curNode->jumpBits_ = jumpBits;

	//////////
	// NOTE:: Simple mask could raise an error
	//
	// prevNode: max: 124(0111 1100)
	// curNode: realmax: 94(0101 1110), order = 1
	//			estimateMax: 127(0111 1111)
	//			as estimateMax > parentMax 
	//			max: 124(0111 1100)
	// --------------------
	// If jump = 2
	// 124(0111 1100)
	//  94(  01     )
	// --------------------
	//  92(0101 1100) <- but its wrong
	//  95(0101 1111) <- this shold be right
	//
}

//////////////////////////////
// updateChildNodeOrder
//////////////////////////////
//
// Find next 'different' significant bit
// If next order significant bits are same on min/max, move on to the next order
// 
// return number of jumped bits
//
template <typename Ty_>
bit_cnt_type updateChildNodeOrder(pMmtNode curNode)
{
	if (curNode->bMax_ != curNode->bMin_)
	{
		// order not changed
		curNode->childOrder_ = curNode->order_;
		return 0;
	}

	if (curNode->bMax_ == 0 && curNode->bMin_ == 0)
	{
		curNode->childOrder_ = 0;
		return 0;
	}

	//////////////////////////////
	// order changed
	//
	// min/max has same sign
	// if positive: max has same or higher significant bit then min
	// if negative:	min has same or higher significant bit then min 
	Ty_ myMax = curNode->getRealMax<Ty_>();
	Ty_ myMin = curNode->getRealMin<Ty_>();
	bit_cnt_type bMax = 0;
	bit_cnt_type bMin = 0;
	bit_cnt_type childOrder = curNode->order_;

	do
	{
		++childOrder;
		bMax = msb<Ty_>(abs_(myMax), childOrder);
		bMin = msb<Ty_>(abs_(myMin), childOrder);

		// Positive: check bMax
		// Negative: check bMin
		if ((myMax >= 0 && bMax == 0) || (myMin < 0 && bMin == 0))
		{
			// If the higher significant bit is 0 
			// then this is the last bit
			curNode->childOrder_ = 0;
			return abs_(curNode->bMax_) - 1;		// curNode->bMax_ == curNode->bMin_
		}

		// child order cannot be larger then Ty_ data type size
		assert(childOrder <= sizeof(Ty_) * CHAR_BIT);
	} while (bMax == bMin);

	curNode->childOrder_ = childOrder;

	// Now, bMax != bMin
	//BOOST_LOG_TRIVIAL(debug) << static_cast<int>(abs_(curNode->bMax_) - bMax - 1) << "/" << static_cast<int>(abs_(curNode->bMin_) - bMin - 1);
	return std::min({ abs_(curNode->bMax_) - bMax - 1, abs_(curNode->bMin_) - bMin - 1 });	// return jumped bits
}
}		// core
}		// msdb
#endif	// _MSDB_MMTNODE_H_
