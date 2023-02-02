#include "project_plan.h"
#include "project_action.h"

namespace msdb
{
namespace op
{
project_plan::project_plan()
{
}
project_plan::~project_plan()
{
}
const char* project_plan::name()
{
	return "project";
}

pAction project_plan::makeAction()
{
	return std::make_shared<project_action>();
}

project_pset::project_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 2);

	assert(this->params_[1]->type() == opParamType::STRING_LIST);
}

projectOpr::projectOpr(Array arr, project_pset::containerType& attrList)
	: childQry_(msdb::Load(arr)), attrList_(std::make_shared<project_pset::containerType>(attrList)), AFLOperator(arr.getDesc())
{
}
projectOpr::projectOpr(std::shared_ptr<AFLOperator> qry, project_pset::containerType& attrList)
	: childQry_(qry), attrList_(std::make_shared<project_pset::containerType>(attrList)), AFLOperator(qry->getArrayDesc())
{
}
std::shared_ptr<core::opPlan> projectOpr::getPlan()
{
	auto qryPlan = std::make_shared<project_plan>();

	core::parameters params = {
		std::make_shared<core::opParamPlan>(this->childQry_->getPlan()),
		std::make_shared<core::opParamStringList>(this->attrList_)
	};
	qryPlan->setParamSet(std::make_shared<project_pset>(params));

	return qryPlan;
}
std::string projectOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Project(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << "," << std::endl;
	ss << strChildIndent;
	for (auto attr : *this->attrList_)
	{
		ss << attr;
	}
	ss << ")";
	return ss.str();
}
std::shared_ptr<projectOpr> Project(Array arr, project_pset::containerType& attrList)
{
	return std::make_shared<projectOpr>(arr, attrList);
}
std::shared_ptr<projectOpr> Project(std::shared_ptr<AFLOperator> qry, project_pset::containerType& attrList)
{
	return std::make_shared<projectOpr>(qry, attrList);
}

REGISTER_USER_DEFINED_OPERATOR_FACTORY(project, project_plan);
}		// op
}		// msdb
