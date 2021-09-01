#pragma once
#ifndef _MSDB_OP_BETWEEN_PLAN_H_
#define _MSDB_OP_BETWEEN_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class between_plan : public opPlan
{
public:
	between_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class between_pset
{
protected:
	template<typename Ty_>
	pBitmapTree inferBottomUpAttrBitmap(pArrayDesc arrDesc, pAttributeDesc attrDesc, pBitmapTree boUpBitmap, pCoor sp, pCoor ep)
	{
		auto qRange = coorRange(*sp, *ep);

		dimension chunkSpace = arrDesc->getDimDescs()->getChunkSpace();
		dimension blockSpace = arrDesc->getDimDescs()->getBlockSpace();

		dimension chunkDims = arrDesc->getDimDescs()->getChunkDims();
		dimension blockDims = arrDesc->getDimDescs()->getBlockDims();

		size_t chunkNums = chunkSpace.area();
		size_t blockNums = blockSpace.area();

		dimension mmtNodeSpace = chunkSpace * blockSpace;

		coorItr cit(chunkSpace);
		coorItr bit(blockSpace);
		size_t dSize = arrDesc->getDSize();

		auto chunkBitmap = std::make_shared<bitmapTree>(chunkNums, false);
		auto chunkRange = coorRange(coor(chunkDims.size()), chunkDims);

		for (chunkId cid = 0; cid < chunkNums; ++cid)
		{
			if(boUpBitmap->isExist(cid))
			{
				auto chunkCoor = cit.seqToCoor(cid);
				auto chunkSp = chunkCoor * chunkDims;
				auto chunkEp = chunkSp + chunkDims;
				auto chunkRange = coorRange(chunkSp, chunkEp);

				if(chunkRange.isIntersect(qRange))
				{
					if(chunkRange.isFullyInside(qRange))
					{
						// Fully inside
						chunkBitmap->setExist(cid);
						auto blockBitmap = chunkBitmap->makeChild(cid, blockNums, true);

						if (boUpBitmap->hasChild(cid))
						{
							auto boUpBlockBitmap = boUpBitmap->getChild(cid);
							blockBitmap->andMerge(*boUpBlockBitmap);
						}
					}else
					{
						// Intersect
						chunkBitmap->setExist(cid);
						auto qRangeInChunk = qRange - chunkSp;

						// Check block bitmap exist
						if (boUpBitmap->hasChild(cid))
						{
							//auto blockBitmap = chunkBitmap->makeChild(cid, blockNums, true);

							auto boUpBlockBitmap = boUpBitmap->getChild(cid);
							auto blockBitmap = chunkBitmap->makeChild(cid, blockNums, false);
							for(blockId bid = 0; bid < blockNums; ++bid)
							{
								if (boUpBlockBitmap->isExist(bid))
								{
									auto blockCoor = bit.seqToCoor(bid);
									auto blockSp = blockCoor * blockDims;
									auto blockEp = blockSp + blockDims;
									auto blockRange = coorRange(blockSp, blockEp);
									
									if(blockRange.isIntersect(qRangeInChunk))
									{
										blockBitmap->setExist(bid);
									}
								}
							}
						} else
						{
							//auto blockBitmap = chunkBitmap->makeChild(cid, blockNums, true);

							auto blockBitmap = chunkBitmap->makeChild(cid, blockNums, false);
							for (blockId bid = 0; bid < blockNums; ++bid)
							{
								auto blockCoor = bit.seqToCoor(bid);
								auto blockSp = blockCoor * blockDims;
								auto blockEp = blockSp + blockDims;
								auto blockRange = coorRange(blockSp, blockEp);

								if (blockRange.isIntersect(qRangeInChunk))
								{
									blockBitmap->setExist(bid);
								}
							}
						}
					}
				}else
				{
					// Fully outside
					continue;
				}
			}
		}

		return chunkBitmap;
	}
};

//////////////////////////////
// between_array_pset
class between_array_pset : public opArrayParamSet, public between_pset
{
public:
	between_array_pset(parameters& pSet);

public:
	virtual pBitmapTree inferBottomUpBitmap() override;
};

//////////////////////////////
// index_filter_plan_pset
class between_plan_pset : public opPlanParamSet, public between_pset
{
public:
	between_plan_pset(parameters& pSet);

public:
	virtual pBitmapTree inferBottomUpBitmap() override;
};
}		// core
}		// msdb
#endif // _MSDB_OP_BETWEEN_PLAN_H_
