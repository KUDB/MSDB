﻿#include <pch.h>
#include <op/between/between_action.h>
#include <array/arrayMgr.h>
#include <array/flattenChunk.h>
#include <array/flattenArray.h>

namespace msdb
{
namespace core
{
between_action::between_action()
{
}

between_action::~between_action()
{
}

const char* between_action::name()
{
	return "between";
}

pArray between_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	pArray inArr = inputArrays[0];
	//pArray outArr = arrayMgr::instance()->makeArray<flattenArray>(this->getArrayDesc());
	pArray outArr = std::make_shared<flattenArray>(this->getArrayDesc());
	pCoor sp = std::static_pointer_cast<coor>(this->params_[1]->getParam());
	pCoor ep = std::static_pointer_cast<coor>(this->params_[2]->getParam());
	range betweenRange(*sp, *ep);

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		auto chunkItr = inArr->getChunkIterator(attr->id_);
		while (!chunkItr->isEnd())
		{
			if(chunkItr->isExist())
			{
				//std::cout << "[" << chunkItr->seqPos() << "]: exist / ";
				auto inChunk = (**chunkItr);
				//std::cout << "**ChunkItr / " << std::endl;
				auto chunkRange = inChunk->getChunkRange();
				//std::cout << "chunkRange /" << std::endl;

				if (chunkRange.isIntersect(betweenRange))
				{
					/**
					 * Intersect chunk
					 *
					 * Note: Do not copy blockBitmap from inChunk to outChunk.
					 *       makeBlock() of the outChunk automatically set
					 *       the exist block bit in its blockBitmap.
					 */
					auto outChunk = outArr->makeChunk(attr->id_, inChunk->getId());
					outChunk->setChunkDesc(inChunk->getDesc());
					outChunk->bufferCopy(inChunk);
					//outChunk->bufferRef(inChunk);

					if (chunkRange.isFullyInside(betweenRange))
					{
						this->fullyInsideChunk(outChunk, inChunk);
					}else
					{
						this->betweenChunk(outChunk, inChunk, betweenRange);
					}
				}

				////////////////////////////////
				//auto blockItr = inChunk->getBlockIterator();
				//while (!blockItr->isEnd())
				//{
				//	if (blockItr->isExist())
				//	{
				//		auto inBlock = (**blockItr);
				//		auto inDesc = inBlock->getDesc();

				//		auto outBlock = outChunk->makeBlock(inBlock->getId());
				//		auto outDesc = outBlock->getDesc();

				//		outDesc->setIsp(inDesc->getIsp());
				//		outDesc->setIep(inDesc->getIep());
				//		outBlock->copyBitmap(inBlock->getBitmap());
				//	}

				//	++(*blockItr);
				//}
			}

			++(*chunkItr);
		}
	}
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}

void between_action::betweenChunk(pChunk outChunk, pChunk inChunk, range& betweenRange)
{
	auto blockItr = inChunk->getBlockIterator();
	auto betweenRangeInChunk = betweenRange;
	betweenRangeInChunk.move(inChunk->getDesc()->sp_ * -1);

	while (!blockItr->isEnd())
	{
		if(blockItr->isExist())
		{
			auto inBlock = (**blockItr);
			auto blockRange = inBlock->getBlockRange();

			if (blockRange.isIntersect(betweenRangeInChunk))
			{
				auto outBlock = outChunk->makeBlock(inBlock->getId());

				if (blockRange.isFullyInside(betweenRangeInChunk))
				{
					fullyInsideBlock(outBlock, inBlock);
				} else
				{
					this->betweenBlock(outBlock, inBlock, betweenRangeInChunk);
				}
			}
		}

		++(*blockItr);
	}
}
void between_action::fullyInsideChunk(pChunk outChunk, pChunk inChunk)
{
	outChunk->makeBlocks(*inChunk->getBlockBitmap());
}
void between_action::betweenBlock(pBlock outBlock, pBlock inBlock, range& betweenRangeInChunk)
{
	auto inDesc = inBlock->getDesc();
	auto outDesc = outBlock->getDesc();

	outDesc->setIsp(getOutsideCoor(inDesc->getSp(), betweenRangeInChunk.getSp()));
	outDesc->setIep(getInsideCoor(inDesc->getEp(), betweenRangeInChunk.getEp()));
	outDesc->setIsp(outDesc->getIsp() - outDesc->getSp());	// Isp: global -> local
	outDesc->setIep(outDesc->getIep() - outDesc->getSp());
	outBlock->initEmptyBitmap();

	range itRange(outDesc->getIsp(), outDesc->getIep());
	auto iit = inBlock->getItemRangeIterator(itRange);
	auto oit = outBlock->getItemRangeIterator(itRange);
	
	while(!oit->isEnd())
	{
		if(iit->isExist())
		{
			oit->setExist();
		}
		
		++(*iit);
		++(*oit);
	}
}
void between_action::fullyInsideBlock(pBlock outBlock, pBlock inBlock)
{
	auto inDesc = inBlock->getDesc();
	auto outDesc = outBlock->getDesc();

	outDesc->setIsp(inDesc->getIsp());
	outDesc->setIep(inDesc->getIep());
	outBlock->copyBitmap(inBlock->getBitmap());
}
}		// core
}		// msdb