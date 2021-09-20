#include <pch.h>
#include <op/se_compression/se_compression_plan.h>
#include <op/se_compression/se_compression_action.h>

namespace msdb
{
namespace core
{
se_compression_plan::se_compression_plan()
{
}

se_compression_plan::~se_compression_plan()
{
}

const char* se_compression_plan::name()
{
	return "se_compression_plan";
}

pAction se_compression_plan::makeAction()
{
	return std::make_shared<se_compression_action>();
}

se_compression_array_pset::se_compression_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb