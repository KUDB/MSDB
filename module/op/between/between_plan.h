#pragma once
#ifndef _MSDB_OP_BETWEEN_PLAN_H_
#define _MSDB_OP_BETWEEN_PLAN_H_

#include <pch_op.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <api_cpp/cpp_operators.h>
#include <api_cpp/cpp_array.h>
#include <api_cpp/cpp_domain.h>
#include "between_action.h"

namespace msdb
{
namespace core
{
class OP_API between_plan : public opPlan
{
public:
	between_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class OP_API between_pset
{
protected:
	template<typename Ty_>
	pBitmapTree inferBottomUpAttrBitmap(pArrayDesc arrDesc, pAttributeDesc attrDesc, pBitmapTree boUpBitmap, pCoor sp, pCoor ep)
	{
		// SP and EP can be smaller than array dimension size.
		auto asp = op::between::adjustDomain(arrDesc->getDimDescs(), *sp, false);
		auto aep = op::between::adjustDomain(arrDesc->getDimDescs(), *ep, true);
		auto qRange = range(asp, aep);

		dimension chunkSpace = arrDesc->getDimDescs()->getChunkSpace();
		dimension blockSpace = arrDesc->getDimDescs()->getBlockSpace();

		dimension chunkDims = arrDesc->getDimDescs()->getChunkDims();
		dimension blockDims = arrDesc->getDimDescs()->getBlockDims();

		size_t chunkNums = chunkSpace.area();
		size_t blockNums = blockSpace.area();

		dimension mmtNodeSpace = chunkSpace * blockSpace;

		mdItr cit(chunkSpace);
		mdItr bit(blockSpace);
		size_t dSize = arrDesc->getDSize();

		auto chunkBitmap = std::make_shared<bitmapTree>(chunkNums, false);

		for (chunkId cid = 0; cid < chunkNums; ++cid)
		{
			if(boUpBitmap->isExist(cid))
			{
				auto chunkCoor = cit.seqToCoor(cid);
				auto chunkSp = chunkCoor * chunkDims;
				auto chunkEp = chunkSp + chunkDims - 1;
				auto chunkRange = range(chunkSp, chunkEp);

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
									auto blockRange = range(blockSp, blockEp);
									
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
								auto blockRange = range(blockSp, blockEp);

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
class OP_API between_array_pset : public opArrayParamSet, public between_pset
{
public:
	between_array_pset(parameters& pSet);

public:
	virtual pBitmapTree inferBottomUpBitmap() override;
};

//////////////////////////////
// index_filter_plan_pset
class OP_API between_plan_pset : public opPlanParamSet, public between_pset
{
public:
	between_plan_pset(parameters& pSet);

public:
	virtual pBitmapTree inferBottomUpBitmap() override;
};
}		// core

/* ************************ */
/* CPP API Operator	        */
/* ************************ */
class OP_API BetweenOpr : public AFLOperator
{
public:
	BetweenOpr(Array arr, Domain d);
	BetweenOpr(std::shared_ptr<AFLOperator> qry, Domain d);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	Domain domain_;
};
std::shared_ptr<BetweenOpr> OP_API Between(Array arr, Domain d);
std::shared_ptr<BetweenOpr> OP_API Between(std::shared_ptr<AFLOperator> qry, Domain d);
}		// msdb
#endif // _MSDB_OP_BETWEEN_PLAN_H_
