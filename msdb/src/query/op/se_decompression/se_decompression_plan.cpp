#include <pch.h>
#include <op/se_decompression/se_decompression_plan.h>
#include <op/se_decompression/se_decompression_action.h>

#include <util/logger.h >

namespace msdb
{
namespace core
{
se_decompression_plan::se_decompression_plan()
{
}

se_decompression_plan::~se_decompression_plan()
{
}

const char* se_decompression_plan::name()
{
	return "se_decompression_plan";
}

pAction se_decompression_plan::makeAction()
{
	return std::make_shared<se_decompression_action>();
}

se_decompression_array_pset::se_decompression_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[0]->type() == opParamType::ARRAY);				// source array
	assert(this->params_[1]->type() == opParamType::CONST_TYPE);		// Target wtLevel
}
pArrayDesc se_decompression_array_pset::inferSchema()
{
	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	// TODO:: Fix wrong schema
	// It did not consider wtArray schema.
	// So do 'spiht_decode' class.

	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
pBitmapTree se_decompression_array_pset::inferBottomUpBitmap()
{
	pArrayDesc desc = this->inferSchema();
	dimension chunkSpace = desc->getDimDescs()->getChunkSpace();
	dimension blockSpace = desc->getDimDescs()->getBlockSpace();
	dimension seChunkSpace = chunkSpace * blockSpace;

	//BOOST_LOG_TRIVIAL(debug) << "Se Decomp Array Chunk Space: " << seChunkSpace.area();

	return std::make_shared<bitmapTree>(seChunkSpace.area(), true);
}
}		// core
}		// msdb