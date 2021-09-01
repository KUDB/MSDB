#pragma once
#ifndef _MSDB_MATH_H_
#define _MSDB_MATH_H_

#include <pch.h>

namespace msdb
{
namespace core
{
#define SIGN(val)	((val >= 0) ? 1 : -1)

#define _TySize_	(sizeof(Ty_) * CHAR_BIT)
#define _TY_HAS_NEGATIVE_VALUE_		((Ty_)-1 < 0)

// Use abs_ instead of std::abs
// std::abs make compile error
//  - C2668: ambigous call to overloeaded function.
// As std::abs has no function for unsigned integer types.
// When we use template, we don't know what 'Ty_' type is.
// Just try to make an absolute value for all 'Ty_' types.

_NODISCARD int abs_(int num) noexcept;
_NODISCARD long int abs_(long int num) noexcept;
_NODISCARD long long int abs_(long long int num) noexcept;
_NODISCARD float abs_(float num) noexcept;
_NODISCARD double abs_(double num) noexcept;
_NODISCARD unsigned int abs_(unsigned int num) noexcept;
_NODISCARD unsigned long int abs_(unsigned long int num) noexcept;
_NODISCARD unsigned long long int abs_(unsigned long long int num) noexcept;

int64_t intDivCeil(int64_t li, int64_t ri);

using bit_cnt_type = unsigned char;		// number of bit
using sig_bit_type = char;				// significant bit

unsigned long long calcMaxLimit(bit_cnt_type bits);
unsigned long long calcMinLimit(bit_cnt_type bits);

template<typename Ty_, typename size_type = std::conditional_t<sizeof(Ty_) < 32, unsigned char, unsigned int > ,
	size_t Bits_ = sizeof(Ty_)* CHAR_BIT>
	size_type msb(const Ty_ inValue, const size_type inOrder = 1)
{
	assert(inOrder > 0);
	if (inValue == 0)	return 0;		// 0 has no significant bit

	Ty_ value = inValue;
	size_type order = inOrder;
	Ty_ mask = (Ty_)0x1 << (Bits_ - 1);
	size_type i = Bits_ + 1;
	while (order != 0 && i-- != 0)
	{
		(mask & value) && --order;
		value <<= 1;
	}

	if((Ty_)abs_(inValue) < 0 && i == Bits_)
	{
		// For sign values:
		// Char type(8 bits)
		// -128 = 1000 0000
		//        N|------|
		// msb should be 7 not 8.
		// 8th bit is a sign bit, not significant bit.
		return Bits_ - 1;
	}

	if(i == (size_type)-1)
	{
		return 0;
	}

	return i;
}

template <typename Ty_>
bit_cnt_type getPrefixPosForPrevLimit(Ty_ prevLimit, bit_cnt_type order)
{
	assert(order > 0);

	Ty_ absPrevLimit = abs_(prevLimit);
	sig_bit_type bits = msb<Ty_>(absPrevLimit);
	Ty_ v = 0x1 << (sizeof(Ty_) * 8 - 1);
	if (absPrevLimit == v)
	{
		// For sign values:
		// Char type(8 bits)
		// -128 = 1000 0000
		//        N|------|
		// msb should be 7 not 8.
		// 8th bit is a sign bit, not significant bit.

		return sizeof(Ty_) * 8 + 1 - order;
	}

	Ty_ mask = (unsigned long long)1 << (bits - 1);
	size_t prefixPos = bits + 1;

	while (prefixPos > 0 && order > 1)
	{
		--prefixPos;
		(absPrevLimit & mask) && --order;
		mask >>= 1;
	}
	return (bit_cnt_type)prefixPos;
}

// Return Max Limit value where num of bits is provided.
// PrevLimit is always equal or large than maxBoundary
template <typename Ty_>
Ty_ getMaxBoundary(Ty_ prevLimit, sig_bit_type sigBitPos)
{
	if (sigBitPos == 0)
	{
		return 0;
	} else if (sigBitPos > 0)
	{
		Ty_ v = static_cast<Ty_>(calcMaxLimit(sigBitPos));
		return std::min({ static_cast<Ty_>(calcMaxLimit(sigBitPos)), prevLimit });
	} else
	{
		Ty_ v = static_cast<Ty_>(calcMinLimit(-sigBitPos) * -1);
		return std::min({ static_cast<Ty_>(calcMinLimit(-sigBitPos) * -1), prevLimit });
	}
}

// PrevLimit is always equal or less than minBoundary
template <typename Ty_>
Ty_ getMinBoundary(Ty_ prevLimit, sig_bit_type sigBitPos)
{
	if (sigBitPos == 0)
	{
		return 0;
	} else if (sigBitPos > 0)
	{
		return std::max({ static_cast<Ty_>(calcMinLimit(sigBitPos)), prevLimit });
	} else
	{
		// For sign value:
		// -127 = 1000 0001
		//      =>0111 1111 (127)
		// -128 = 1000 0000
		// but, sigBitPos = 7
		// To cover this, std::max({ static_cast<Ty_>(calcMaxLimit(-sigBitPos) * -1 - 1), prevLimit });
		// Minus one from the calcMaxLimit
		return std::max({ static_cast<Ty_>(calcMaxLimit(-sigBitPos) * -1), prevLimit });
	}
}

template <typename Ty_>
Ty_ getPositiveMaxBoundary(Ty_ prevLimit, bit_cnt_type order, sig_bit_type sigBitPos)
{
	/*
	*  Here is an example:
	*
	*  Suppose prevLimit (1001 1111 1010), order (3), significantBit (3)
	*  ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
	*  │ 1│ 0│ 0│ 1│ 1│ 1│ 1│ 1│ 1│ 0│ 1│ 0│
	*  └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
	*             ▲        ▲
	*        prefixpos     sigbit
	*  ==================================
	*  Result:
	*              ┌─────┬─────────────────┐
	*              │ 0's │ 1's             │
	*  ┌──┬──┬──┬──┼──┬──┼──┬──┬──┬──┬──┬──┤
	*  │ 1│ 0│ 0│ 1│ 0│ 0│ 1│ 1│ 1│ 1│ 1│ 1│
	*  ├──┴──┴──┴──┼──┴──┼──┴──┴──┴──┴──┴──┤
	*  │prefix_copy│     │ maxLimit        │
	*  └───────────┴─────┴─────────────────┘
	*             ▲        ▲
	*        prefixpos     sigbit
	*/

	bit_cnt_type prefixPos = getPrefixPosForPrevLimit(prevLimit, order);
	assert(prefixPos >= sigBitPos);

	size_t prefixSize = _TySize_ - prefixPos + 1;
	Ty_ prefixMask = (Ty_)(~((Ty_)-1 << prefixSize) << (_TySize_ - prefixSize));
	Ty_ sigBitMaxLimit = calcMaxLimit(sigBitPos);

	return (prevLimit & prefixMask) | calcMaxLimit(sigBitPos);
}

template <typename Ty_>
Ty_ getPositiveMinBoundary(Ty_ prevLimit, bit_cnt_type order, sig_bit_type sigBitPos)
{
	bit_cnt_type prefixPos = getPrefixPosForPrevLimit(prevLimit, order);
#ifndef NDEBUG
	if(prefixPos < sigBitPos)
	{
		BOOST_LOG_TRIVIAL(error) << "getPositiveMinBoundary assert(prefixPos >= sigBitPos): " << static_cast<int>(prefixPos) << ", " << static_cast<int>(sigBitPos) << "/prevLimit: " << static_cast<int64_t>(prevLimit) << ", order: " << static_cast<int>(order);
	}
	assert(prefixPos >= sigBitPos);
#endif

	size_t prefixSize = _TySize_ - prefixPos + 1;
	Ty_ prefixMask = (Ty_)(~((Ty_)-1 << prefixSize) << (_TySize_ - prefixSize));
	Ty_ sigBitMinLimit = calcMinLimit(sigBitPos);

	return (prevLimit & prefixMask) | calcMinLimit(sigBitPos);
}

// Return Max Limit value where num of bits is provided.
template <typename Ty_>
Ty_ getMaxBoundary(Ty_ prevLimit, bit_cnt_type order, sig_bit_type sigBitPos)
{
	if(order == 1)
	{
		return getMaxBoundary(prevLimit, sigBitPos);
	}
	
#ifndef NDEBUG
	// After order 1, signs of prevLimit and sigBitPos are same
	if (!(sigBitPos == 0 || SIGN(prevLimit) == SIGN(sigBitPos)))
	{
		BOOST_LOG_TRIVIAL(warning) << "prevLimit: " << prevLimit << ", sigBitPos: " << sigBitPos;
		assert(sigBitPos == 0 || SIGN(prevLimit) == SIGN(sigBitPos));
	}
#endif

	if (prevLimit == 0)
	{
		return 0;
	} else if (prevLimit > 0)
	{
		return std::min({ getPositiveMaxBoundary<Ty_>(prevLimit, order, sigBitPos), prevLimit });
	} else
	{
		Ty_ value = (Ty_)(getPositiveMinBoundary<Ty_>(abs_(prevLimit), order, abs_(sigBitPos)));
		return std::min({ (Ty_)(value * -1), prevLimit });
	}
}

template <typename Ty_>
Ty_ getMinBoundary(Ty_ prevLimit, bit_cnt_type order, sig_bit_type sigBitPos)
{
	if (order == 1)
	{
		return getMinBoundary(prevLimit, sigBitPos);
	}

#ifndef NDEBUG
	// After order 1, signs of prevLimit and sigBitPos are same
	if(!(sigBitPos == 0 || SIGN(prevLimit) == SIGN(sigBitPos)))
	{
		BOOST_LOG_TRIVIAL(warning) << "prevLimit: " << prevLimit << ", sigBitPos: " << sigBitPos;
		assert(sigBitPos == 0 || SIGN(prevLimit) == SIGN(sigBitPos));
	}
#endif

	if (prevLimit == 0)
	{
		return 0;
	} else if (prevLimit > 0)
	{
		return std::max({ getPositiveMinBoundary<Ty_>(prevLimit, order, sigBitPos), prevLimit });
	} else
	{
		Ty_ value = (Ty_)(getPositiveMaxBoundary<Ty_>(abs_(prevLimit), order, abs_(sigBitPos)));
		return std::max({ (Ty_)(value * -1), prevLimit });
	}
}

template <typename Ty_>
inline Ty_ getMaxValue()
{
	if(_TY_HAS_NEGATIVE_VALUE_)
	{
		return ((Ty_)~(0x0)) ^ (Ty_)(0x1 << (_TySize_ - 1));
	}

	return (Ty_)~(0x0);
}

template <typename Ty_>
inline Ty_ getMinValue()
{
	if(_TY_HAS_NEGATIVE_VALUE_)
	{
		return (Ty_)(0x1 << (_TySize_ - 1));
	}

	return 0;
}
}		// core
}		// msdb
#endif	// _MSDB_MATH_H_