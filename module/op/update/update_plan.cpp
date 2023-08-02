#include <pch_op.h>
#include <op/update/update_plan.h>
#include <op/update/update_action.h>

namespace msdb
{
namespace core
{
/* ************************ */
/* fast_update_plan			*/
/* ************************ */
fast_update_plan::fast_update_plan()
{
}

fast_update_plan::~fast_update_plan()
{
}

const char* fast_update_plan::name()
{
	return "fast_update";
}

pAction fast_update_plan::makeAction()
{
	return std::make_shared<fast_update_action>();
}

////////////////////////////////////////////////
namespace op
{
namespace update
{
pset_file::pset_file(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 4);
	assert(this->params_[1]->type() == opParamType::STRING);
	assert(this->params_[2]->type() == opParamType::COOR);
	assert(this->params_[3]->type() == opParamType::COOR);
}
pset_files::pset_files(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 4);
	assert(this->params_[pset_files::IDX_CONTAINER]->type() == opParamType::CONTAINER);
	assert(this->params_[pset_files::IDX_START_COOR]->type() == opParamType::COOR);
	assert(this->params_[pset_files::IDX_END_COOR]->type() == opParamType::COOR);
}
}		// update
}		// op
}		// core
////////////////////////////////////////////////

/* ************************ */
/* Fast Update Operator		*/
/* ************************ */
FastUpdateOpr::FastUpdateOpr(Array arr, core::parameters params)
	: params_(params), AFLOperator(arr.getDesc())
{
}
std::shared_ptr<core::opPlan> FastUpdateOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::fast_update_plan>();

	if (this->params_.size() < 4)
	{
		_MSDB_THROW(
			_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT,
				std::string("FastUpdate requires 4 parameters, but has ") + std::to_string(this->params_.size())));
	}

	switch (this->params_[1]->type())
	{
	case core::opParamType::STRING:
	{
		// Single file
		qryPlan->setParamSet(std::make_shared<core::op::update::pset_file>(this->params_));
		break;
	}
	case core::opParamType::CONTAINER:
	{
		// Multi file
		qryPlan->setParamSet(std::make_shared<core::op::update::pset_files>(this->params_));
		break;
	}
	default:
	{
		_MSDB_THROW(
			_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT,
				std::string("Parameter 2 for 'FastUpdate' is wrong. (Expect STRING or CONTAINER)")));
	}
	}

	return qryPlan;
}
std::string FastUpdateOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Update(" << std::endl;
	ss << strChildIndent << "\"" << this->getArrayDesc()->name_ << "\"" << std::endl;

	if (this->params_[1]->type() == core::opParamType::STRING)
	{
		// Single file
		ss << strChildIndent << "\"" << std::static_pointer_cast<std::string>(this->params_[1]->getParam())->c_str() << "\")" << std::endl;
	}
	else if(this->params_[1]->type() == core::opParamType::CONTAINER)
	{
		// Multiple files
		auto files = std::static_pointer_cast<core::op::update::pset_files::containerType>(this->params_[1]->getParam());

		for (auto& it : *files)
		{
			auto& key = it.first;
			auto& value = it.second;

			ss << strChildIndent << "{attr " << key << ": \"" << value << "\"}\n";
		}
	}

	ss << strChildIndent << "(" << std::static_pointer_cast<core::opParamCoor>(this->params_[2]->getParam()) 
		<< " - " << std::static_pointer_cast<core::opParamCoor>(this->params_[3]->getParam()) << ")" << std::endl;

	// Multi attribute files
	ss << "\")";

	return ss.str();
}
std::shared_ptr<FastUpdateOpr> FastUpdate(Array arr, std::string filePath, 
	const Coordinates start, const Coordinates end)
{
	core::parameters params = {
		std::make_shared<core::opParamArray>(arr.getDesc()),
		std::make_shared<core::opParamString>(std::make_shared<std::string>(filePath)),
		core::opParamCoor::make(start.getCoor()),
		core::opParamCoor::make(end.getCoor()),
	};

	return std::make_shared<FastUpdateOpr>(arr, params);
}
std::shared_ptr<FastUpdateOpr> FastUpdate(Array arr, core::op::update::pset_files::containerType attrFiles, 
	const Coordinates start, const Coordinates end)
{
	auto pFileAttrs = std::make_shared<core::op::update::pset_files::containerType>(attrFiles);

	core::parameters params = {
		std::make_shared<core::opParamArray>(arr.getDesc()),
		std::make_shared<core::opParamContainer<
			core::op::update::pset_files::keyType, core::op::update::pset_files::valueType>>(pFileAttrs),
		core::opParamCoor::make(start.getCoor()),
		core::opParamCoor::make(end.getCoor()),
	};

	return std::make_shared<FastUpdateOpr>(arr, params);
}
}		// msdb