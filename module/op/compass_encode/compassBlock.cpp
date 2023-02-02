#include <pch_op.h>
#include <op/compass_encode/compassBlock.h>

namespace msdb
{
namespace core
{
//compassBlock::compassBlock(pBlockDesc desc)
//	: flattenBlock<element>(desc), numBins_(0)
//{
//}
//
//compassBlock::~compassBlock()
//{
//}

//void compassBlock::serializePositional(bstream& bs, std::vector<position_t>& positional)
//{
//	position_t maxGap = 0;
//	position_t prevP = positional[0];
//	for (auto p : positional)
//	{
//		if (p - prevP > maxGap)
//		{
//			maxGap = p - prevP;
//		}
//		prevP = p;
//	}
//
//	auto bFirstPosition = std::max({ msb<position_t>(positional[0]), (unsigned char)1 });
//	auto bMaxGap = std::max({ msb<position_t>(maxGap), (unsigned char)1 });
//
//	bs << setw(CHAR_BIT) << bFirstPosition << bMaxGap;
//	bs << setw(bFirstPosition) << positional[0];	// set first position
//	bs << setw(bMaxGap);
//
//	prevP = positional[0];
//	size_t numPositions = positional.size();
//	for (size_t i = 1; i < numPositions; ++i)
//	{
//		position_t p = positional[i];
//		assert((p == 0 || (p > prevP && p - prevP > 0)) && "check positional vector is ordered");
//		bs << (p - prevP);
//		prevP = p;
//	}
//	bs << 0;
//}

//void compassBlock::deserializePositional(bstream& bs, std::vector<position_t>& positional)
//{
//	bit_cnt_type bMaxGap = 0;
//	bit_cnt_type bFirstPosition = 0;
//
//	position_t p = 0;
//	position_t prevP = 0;
//
//	bs >> setw(CHAR_BIT) >> bFirstPosition >> bMaxGap;
//	bs >> setw(bFirstPosition) >> p;		// get first position
//	bs >> setw(bMaxGap);
//	positional.push_back(p);
//	prevP = p;
//
//	do
//	{
//		bs >> p;
//		if (!p)
//		{
//			break;
//		}
//		positional.push_back(p + prevP);
//		prevP += p;
//
//	} while (true);
//}

//void compassBlock::setNumBins(size_t numBins)
//{
//	assert(numBins % 2 == 0);
//	this->numBins_ = numBins;
//}
}		// core
}		// msdb
