#include <pch.h>
#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_encode/spiht_encode_action.h>

namespace msdb
{
namespace core
{
// plan
spiht_encode_plan::spiht_encode_plan()
{
}

const char* spiht_encode_plan::name()
{
	return "spiht_encode_plan";
}

pAction spiht_encode_plan::makeAction()
{
	return std::make_shared<spiht_encode_action>(); 
}

// pset
spiht_encode_array_pset::spiht_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb