#pragma once
#ifndef _MSDB_INDEX_FILTER_PLAN_H_
#define _MSDB_INDEX_FILTER_PLNA_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <array/arrayMgr.h>
#include <index/mmt.h>
#include <parse/predicate.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// index_filter_plan
class index_filter_plan : public opPlan
{
public:
	index_filter_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

//////////////////////////////
// index_filter_array_pset
//
// Base class for pSet class of exe_act_ind_index_filter operator
class index_filter_pset
{
protected:
	template<typename Ty_>
	pBitmapTree inferBottomUpAttrBitmap(pArrayDesc arrDesc, pAttributeDesc attrDesc, pBitmapTree myBitmap, pPredicate inPredicate)
	{
		inPredicate->setEvaluateFunc(attrDesc->type_);

		dimension chunkSpace = arrDesc->getDimDescs()->getChunkSpace();
		dimension blockSpace = arrDesc->getDimDescs()->getBlockSpace();
		dimension mmtNodeSpace = chunkSpace * blockSpace;

		mdItr cit(chunkSpace);
		mdItr bit(blockSpace);
		size_t dSize = arrDesc->getDSize();

		auto arrIndex = arrayMgr::instance()->getAttributeIndex(arrDesc->id_, attrDesc->id_);
		if (arrIndex->getType() == attrIndexType::MMT)
		{
			auto pMmtIndex = std::static_pointer_cast<MinMaxTreeImpl<Ty_>>(arrIndex);
			auto curLevel = pMmtIndex->getMaxLevel();
			auto blockLevel = pMmtIndex->getBlockLevel();

			if (inPredicate->evaluateNode(pMmtIndex->getNode(0, curLevel)))		// Check root node
			{
				// Target chunk exists
				// Start searching nodes
				std::vector<std::vector<bool>> nodes(curLevel + 1);
				size_t childs = (size_t)pow(2, dSize);

				//////////////////////////////
				// Level (mmtLevel)
				nodes[curLevel] = std::vector<bool>({ true });

				//////////////////////////////
				// Level (mmtLevel - 1)
				curLevel -= 1;
				if (curLevel >= 0)
				{
					this->evaluateFirstLevel(inPredicate, pMmtIndex, nodes, curLevel, dSize);
				}

				//////////////////////////////
				// Level (mmtLevel - 2) ~ 0
				curLevel -= 1;
				while (curLevel >= blockLevel)
				{
					this->evaluateChildLevel(inPredicate, pMmtIndex, nodes, curLevel, dSize);
					curLevel -= 1;
				}

				return this->inferBoUpBitmapChildLevel(myBitmap, nodes, pMmtIndex,
													   chunkSpace, blockSpace);
			} else
			{
				// No target chunk
				return std::make_shared<bitmapTree>(chunkSpace.area(), false);
			}
		} else if (arrIndex->getType() == attrIndexType::COMPASS)
		{
			auto cpIndex = std::static_pointer_cast<compassIndexImpl<Ty_>>(arrIndex);
			auto numBins = cpIndex->getNumBins();

			while (!cit.isEnd())
			{
				if (myBitmap->isExist(cit.seqPos()))
				{
					bit.moveToStart();
					bool chunkExist = false;

					if (!myBitmap->hasChild(cit.seqPos()))
					{
						// No child bitmap means that all blocks are exist
						myBitmap->makeChild(cit.seqPos(), blockSpace.area(), true);
					}

					auto myBlockBitmap = myBitmap->getChild(cit.seqPos());
					while (!bit.isEnd())
					{
						if (myBlockBitmap->isExist(bit.seqPos()) && inPredicate->evaluateCompassBin(cpIndex->at(cit.seqPos(), bit.seqPos())))
						{
							chunkExist = true;
						} else
						{
							myBlockBitmap->setNull(bit.seqPos());
						}
						++bit;
					}

					if (!chunkExist)
					{
						myBitmap->setNull(cit.seqPos());
						myBitmap->makeChild(cit.seqPos(), blockSpace.area(), false);
					}
				}

				++cit;
			}

			return myBitmap;
		} else
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
	}

	template <typename Ty_>
	void evaluateFirstLevel(std::shared_ptr<predicate> inPredicate,
							std::shared_ptr<MinMaxTreeImpl<Ty_>>pMmtIndex,
							std::vector<std::vector<bool>>& nodes,
							size_t curLevel, size_t dSize)
	{
		auto nit = pMmtIndex->getNodeIterator(curLevel);
		nodes[curLevel] = std::vector<bool>(nit.getCapacity(), false);

		while (!nit.isEnd())
		{
			if (inPredicate->evaluateNode((*nit)))
			{
				nodes[curLevel][nit.seqPos()] = true;
			} else
			{
				nodes[curLevel][nit.seqPos()] = false;
			}
			++nit;
		}
	}

	template <typename Ty_>
	void evaluateChildLevel(std::shared_ptr<predicate> inPredicate,
							std::shared_ptr<MinMaxTreeImpl<Ty_>>pMmtIndex,
							std::vector<std::vector<bool>>& nodes,
							size_t curLevel, size_t dSize)
	{
		size_t prevLevel = curLevel + 1;
		size_t childs = (size_t)pow(2, dSize);
		auto pcit = pMmtIndex->getNodeIterator(prevLevel);
		auto cit = pMmtIndex->getNodeIterator(curLevel);
		nodes[curLevel] = std::vector<bool>(cit.getCapacity(), false);

		auto curNodeSpace = pMmtIndex->getNodeSpace(curLevel);

		while (!pcit.isEnd())
		{
			// nextWork parent
			if (nodes[prevLevel][pcit.seqPos()])
			{
				auto childBase = pcit.coor();
				for (dimensionId d = 0; d < dSize; ++d)
				{
					childBase[d] *= 2;
				}

				// Iterate childs
				for (size_t cID = 0; cID < childs; cID++)
				{
					// Set child coordinate and move to cit
					auto cur = childBase;
					for (dimensionId d = 0; d < dSize; ++d)
					{
						if (cID & ((dimensionId)0x1 << d))
						{
							cur[d] += 1;
						}
					}

					if (cur < curNodeSpace)
					{
						// Check current coordinate is in curNodeSpace.
						// Node space with odd number can cause error in here.
						cit.moveTo(cur);

						if (inPredicate->evaluateNode((*cit)))
						{
							nodes[curLevel][cit.seqPos()] = true;
						}
						else
						{
							nodes[curLevel][cit.seqPos()] = false;
						}
					}
				}
			}
			++pcit;
		}
	}

	template <typename Ty_>
	pBitmapTree inferBoUpBitmapChildLevel(pBitmapTree myBitmap,
										  std::vector<std::vector<bool>>& nodes,
										  std::shared_ptr<MinMaxTreeImpl<Ty_>>pMmtIndex,
										  dimension& chunkSpace, dimension& blockSpace)
	{
		size_t chunkNums = chunkSpace.area();
		size_t blockNums = blockSpace.area();
		size_t chunkLevel = pMmtIndex->getChunkLevel();
		size_t blockLevel = pMmtIndex->getBlockLevel();

		//auto chunkBitmap = std::make_shared<bitmapTree>(chunkNums, false);
		auto chunkItr = mdItr(chunkSpace);
		auto blockItr = mdItr(blockSpace);
		auto nodeItr = mdItr(chunkSpace * blockSpace);

		for (chunkId cid = 0; cid < chunkNums; ++cid)
		{
			auto chunkCoor = chunkItr.seqToCoor(cid);
			if (myBitmap->isExist(cid) && nodes[chunkLevel][cid])
			{
				if (!myBitmap->hasChild(cid))
				{
					// No child bitmap means that all blocks are exist
					myBitmap->makeChild(cid, blockNums, true);
				}

				auto blockBitmap = myBitmap->getChild(cid);
				for (blockId bid = 0; bid < blockNums; ++bid)
				{
					auto blockCoor = blockItr.seqToCoor(bid);
					auto nodeCoor = chunkCoor * blockSpace + blockCoor;
					if (!blockBitmap->isExist(bid) || !nodes[blockLevel][nodeItr.coorToSeq(nodeCoor)])
					{
						blockBitmap->setNull(bid);
					}
				}
			} else
			{
				myBitmap->setNull(cid);
				myBitmap->makeChild(cid, blockNums, false);
			}
		}

		return myBitmap;
	}
};

//////////////////////////////
// index_filter_array_pset
class index_filter_array_pset : public opArrayParamSet, public index_filter_pset
{
public:
	index_filter_array_pset(parameters& pSet);

public:
	virtual pBitmapTree inferBottomUpBitmap() override;
};

//////////////////////////////
// index_filter_plan_pset
class index_filter_plan_pset : public opPlanParamSet, public index_filter_pset
{
public:
	index_filter_plan_pset(parameters& pSet);

public:
	virtual pBitmapTree inferBottomUpBitmap() override;
};
}		// core
}		// msdb
#endif	// _MSDB_INDEX_FILTER_PLAN_H_