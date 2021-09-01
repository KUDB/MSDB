#include <pch.h>
#include <op/spiht_decode/spiht_decode_plan.h>
#include <op/spiht_decode/spiht_decode_action.h>

namespace msdb
{
namespace core
{
// plan
spiht_decode_plan::spiht_decode_plan()
{
}

const char* spiht_decode_plan::name()
{
	return "spiht_decode_plan";
}

pAction spiht_decode_plan::makeAction()
{
	return std::make_shared<spiht_decode_action>();
}

// pset
spiht_decode_array_pset::spiht_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::CONST_TYPE);		// Target wtLevel
}
pArrayDesc spiht_decode_array_pset::inferSchema()
{
	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	eleDefault level;
	std::static_pointer_cast<opParamConst::paramType>(this->params_[1]->getParam())->getData(&level);

	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
pBitmapTree spiht_decode_array_pset::inferBottomUpBitmap()
{
	pArrayDesc desc = this->inferSchema();
	dimension chunkSpace = desc->getDimDescs()->getChunkSpace();
	dimension blockSpace = desc->getDimDescs()->getBlockSpace();
	dimension seChunkSpace = chunkSpace * blockSpace;

	return std::make_shared<bitmapTree>(seChunkSpace.area(), true);
}
}		// core
}		// msdb