#include <pch_op.h>
#include <api_cpp/cpp_operators.h>
//#include <op/insert/insert_plan.h>
//#include <op/insert/insert_action.h>
//#include <op/save/save_plan.h>
#include <op/load/load_plan.h>
//#include <op/between/between_plan.h>
//#include <op/copy_to_buffer/copy_to_buffer_plan.h>
//#include <op/naive_filter/naive_filter_plan.h>
//#include <op/index_filter/index_filter_plan.h>
//#include <op/build/build_plan.h>
//#include <op/consume/consume_plan.h>
#include <array/dimensionDesc.h>
#include <array/attributeDesc.h>

namespace msdb
{
	/* ************************ */
	/* AFLOperator				*/
	/* ************************ */
	AFLOperator::AFLOperator(core::pArrayDesc arrDesc)
		: arrDesc_(arrDesc)
	{
	}

	std::string AFLOperator::toString(int depth)
	{
		if (depth == 0)
		{
			return "AFL>";
		}
		return "";
	}

	std::string AFLOperator::getIndentString(unsigned int depth)
	{
		std::stringstream ss;

		ss << "\t";
		for (int i = 0; i < depth; ++i)
		{
			ss << "    ";
		}

		return ss.str();
	}

	std::string AFLOperator::getStrStart()
	{
		return "AFL>";
	}

	/* ************************ */
	/* Load						*/
	/* ************************ */
	LoadOpr::LoadOpr(Array arr)
		: AFLOperator(arr.getDesc())
	{
	}

	std::shared_ptr<core::opPlan> LoadOpr::getPlan()
	{
		auto qryPlan = std::make_shared<core::load_plan>();

		core::parameters params = {
			std::make_shared<core::opParamArray>(this->getArrayDesc())
		};
		qryPlan->setParamSet(std::make_shared<core::load_array_pset>(params));

		return qryPlan;
	}
	std::string LoadOpr::toString(int depth)
	{
		std::string strIndent = this->getIndentString(depth);
		std::string strChildIndent = this->getIndentString(depth + 1);
		std::stringstream ss;
		ss << AFLOperator::toString(depth) << strIndent << "Load(" << this->getArrayDesc()->name_ << ")";

		return ss.str();
	}
	std::shared_ptr<LoadOpr> Load(Array arr)
	{
		return std::make_shared<LoadOpr>(arr);
	}
}		// msdb