#include <pch.h>
#include <op/wavelet_decode/wavelet_decode_plan.h>
#include <op/wavelet_decode/wavelet_decode_action.h>

namespace msdb
{
namespace core
{
wavelet_decode_plan::wavelet_decode_plan()
{
}
const char* wavelet_decode_plan::name()
{
	return "wavelet_decode";
}
pAction wavelet_decode_plan::makeAction()
{
	auto wdAction = std::make_shared<wavelet_decode_action>();
	return wdAction;
}

wavelet_decode_array_pset::wavelet_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[0]->type() == opParamType::ARRAY);
	assert(this->params_[1]->type() == opParamType::CONST_TYPE);		// Target wtLevel
}

pArrayDesc wavelet_decode_array_pset::inferSchema()
{
	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(
		this->params_[0]->getParam());
	pArrayDesc aInferDesc = std::make_shared<opParamArray::paramType>(
		*aSourceDesc);

	return aInferDesc;
}

wavelet_decode_plan_pset::wavelet_decode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::CONST_TYPE);		// Target wtLevel
}

pBitmapTree wavelet_decode_plan_pset::inferBottomUpBitmap()
{
	auto fromBottom = this->getSourcePlanBottomUpBitmap();

	auto aDesc = this->inferSchema();
	dimension chunkSpace = aDesc->getDimDescs()->getChunkSpace();
	dimension blockSpace = aDesc->getDimDescs()->getBlockSpace();
	dimension seChunkSpace = chunkSpace * blockSpace;

	auto inChunkItr = coorItr(seChunkSpace);
	auto outChunkItr = coorItr(chunkSpace);
	auto outBlockItr = coorItr(blockSpace);

	pBitmapTree outBitmap = std::make_shared<bitmapTree>(chunkSpace.area(), false);

	while(!inChunkItr.isEnd())
	{
		if(fromBottom->isExist(inChunkItr.seqPos()))
		{
			dimension blockCoor = inChunkItr.coor() % blockSpace;
			dimension chunkCoor = (inChunkItr.coor() - blockCoor) / blockSpace;	// Divide operator for dimension is rounded up by default
			outChunkItr.moveTo(chunkCoor);
			outBlockItr.moveTo(blockCoor);

			outBitmap->setExist(outChunkItr.seqPos());

			if(!outBitmap->hasChild(outChunkItr.seqPos()))
			{
				outBitmap->makeChild(outChunkItr.seqPos(), blockSpace.area(), false);
			}

			auto blockBitmap = outBitmap->getChild(outChunkItr.seqPos());
			blockBitmap->setExist(outBlockItr.seqPos());
		}

		++inChunkItr;
	}

	return outBitmap;
}

pBitmapTree wavelet_decode_plan_pset::inferTopDownBitmap(pBitmapTree fromParent)
{
	auto aDesc = this->inferSchema();
	dimension chunkSpace = aDesc->getDimDescs()->getChunkSpace();
	dimension blockSpace = aDesc->getDimDescs()->getBlockSpace();
	dimension seChunkSpace = chunkSpace * blockSpace;

	auto inChunkItr = coorItr(chunkSpace);
	auto inBlockItr = coorItr(blockSpace);
	auto outChunkItr = coorItr(seChunkSpace);

	pBitmapTree outBitmap = std::make_shared<bitmapTree>(seChunkSpace.area(), false);

	while(!inChunkItr.isEnd())
	{
		if(fromParent->isExist(inChunkItr.seqPos()))
		{
			if(fromParent->isTree() && fromParent->hasChild(inChunkItr.seqPos()))
			{
				auto child = fromParent->getChild(inChunkItr.seqPos());
				inBlockItr.moveToStart();
				while (!inBlockItr.isEnd())
				{
					if(child->isExist(inBlockItr.seqPos()))
					{
						dimension outChunkCoor = inBlockItr.coor() + inChunkItr.coor() * blockSpace;
						outChunkItr.moveTo(outChunkCoor);
						outBitmap->setExist(outChunkItr.seqPos());
						outBitmap->makeChild(outChunkItr.seqPos(), 1, true);
					}
					++inBlockItr;
				}
			}else
			{
				// Set all block exsit
				inBlockItr.moveToStart();
				while (!inBlockItr.isEnd())
				{
					dimension outChunkCoor = inBlockItr.coor() + inChunkItr.coor() * blockSpace;
					outChunkItr.moveTo(outChunkCoor);
					outBitmap->setExist(outChunkItr.seqPos());
					outBitmap->makeChild(outChunkItr.seqPos(), 1, true);	// make block bitmap true
					++inBlockItr;
				}
			}
		}
		++inChunkItr;
	}

	return outBitmap;
}
}		// core
}		// msdb