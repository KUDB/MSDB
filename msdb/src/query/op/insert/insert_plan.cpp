#include <pch.h>
#include <op/insert/insert_plan.h>
#include <op/insert/insert_action.h>

namespace msdb
{
namespace core
{
insert_plan::insert_plan()
{
}

insert_plan::~insert_plan()
{
}

const char* insert_plan::name()
{
	return "insert";
}

pAction insert_plan::makeAction()
{
	return std::make_shared<insert_action>();
}

insert_array_file_pset::insert_array_file_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 3);
	assert(this->params_[1]->type() == opParamType::ENUM);
	assert(this->params_[2]->type() == opParamType::STRING);
}
insert_array_multi_attr_file_pset::insert_array_multi_attr_file_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 3);
	assert(this->params_[1]->type() == opParamType::ENUM);
	assert(this->params_[2]->type() == opParamType::CONTAINER);
}
insert_array_multi_attr_memory_pset::insert_array_multi_attr_memory_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 3);
	assert(this->params_[1]->type() == opParamType::ENUM);
	assert(this->params_[2]->type() == opParamType::CONTAINER);			// memory pointer
	//assert(this->params_[3]->type() == opParamType::CONST_TYPE);		// number of size
}
}		// core

/* ************************ */
/* Insert					*/
/* ************************ */
InsertOpr::InsertOpr(Array arr, core::parameters params, InsertOprType type)
	: params_(params), type_(type), AFLOperator(arr.getDesc())
{
}
std::shared_ptr<core::opPlan> InsertOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::insert_plan>();

	switch (this->type_)
	{
	case single_from_file:
	{
		qryPlan->setParamSet(std::make_shared<core::insert_array_file_pset>(this->params_));
		break;
	}
	case multi_from_file:
	{
		qryPlan->setParamSet(std::make_shared<core::insert_array_multi_attr_file_pset>(this->params_));
		break;
	}
	case multi_from_memory:
	{
		qryPlan->setParamSet(std::make_shared<core::insert_array_multi_attr_memory_pset>(this->params_));
		break;
	}
	default:
	{
		// TODO::throw exception
	}
	}

	return qryPlan;
}
std::string InsertOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Insert(" << std::endl;
	ss << strChildIndent << "\"" << this->getArrayDesc()->name_ << "\"" << std::endl;

	switch (this->type_)
	{
	case single_from_file:
	{
		assert(this->params_[2]->type() == core::opParamType::STRING);
		// File path
		ss << strChildIndent << "\"" << std::static_pointer_cast<std::string>(this->params_[2]->getParam())->c_str() << "\")";

		break;
	}
	case multi_from_file:
	{
		// TODO::print parameters for multi attribute from file
		//ss << strChildIndent << (int64_t)this->mem_.get() << std::endl;		// print pointer address
		//ss << strChildIndent << this->size_ << ")";
		break;
	}
	case multi_from_memory:
	{
		// TODO::print parameters for multi attribute from memory
		break;
	}
	default:
	{

	}
	}
	// TODO:: switch case according to the type of params


	ss << "\")";

	return ss.str();
}
std::shared_ptr<InsertOpr> Insert(Array arr, std::string filePath)
{
	core::parameters params = {
		std::make_shared<core::opParamArray>(arr.getDesc()),
		std::make_shared<core::opParamEnum<core::opInsertType>>(core::opInsertType::FILE),
		std::make_shared<core::opParamString>(std::make_shared<std::string>(filePath))
	};

	return std::make_shared<InsertOpr>(arr, params, InsertOprType::single_from_file);
}
// TODO::Insert multi-attribute from file
std::shared_ptr<InsertOpr> Insert(Array arr, core::insert_array_multi_attr_file_pset::containerType attrFiles)
{
	//core::parameters params = {
	//	std::make_shared<core::opParamArray>(arr.getDesc()),
	//	std::make_shared<core::opParamEnum>(std::make_shared<std::string>(core::opInsertTypeToString(core::opInsertType::FILE))),
	//	std::make_shared<core::opParamContainer<
	//		core::insert_array_multi_attr_file_pset::keyType, core::insert_array_multi_attr_file_pset::valueType>>(attrFiles)
	//};

	core::parameters params = {
		std::make_shared<core::opParamArray>(arr.getDesc()),
		std::make_shared<core::opParamEnum<core::opInsertType>>(core::opInsertType::FILE)
	};

	return std::make_shared<InsertOpr>(arr, params, InsertOprType::multi_from_file);
}
// TODO::Insert multi-attribute from memory
std::shared_ptr<InsertOpr> Insert(Array arr, core::insert_array_multi_attr_memory_pset::containerType attrMem)
{
	auto pMemAttrs = std::make_shared<core::insert_array_multi_attr_memory_pset::containerType>(attrMem);

	core::parameters params = {
		std::make_shared<core::opParamArray>(arr.getDesc()),
		std::make_shared<core::opParamEnum<core::opInsertType>>(core::opInsertType::MEMORY),
		std::make_shared<core::opParamContainer<
			core::insert_array_multi_attr_memory_pset::keyType, core::insert_array_multi_attr_memory_pset::valueType>>(pMemAttrs)
	};

	//std::make_shared<core::opParamMemory>(mem),
		//std::make_shared<core::opParamConst>(std:k:make_shared<core::stableElement>(&size, _ELE_DEFAULT_TYPE))

	return std::make_shared<InsertOpr>(arr, params, InsertOprType::multi_from_memory);
}
//////////
// Deprecated: OLD Synopsis that can insert only one attribute from memory.
//
//std::shared_ptr<InsertOpr> Insert(Array arr, std::shared_ptr<void> mem, int64_t size)
//{
//	core::parameters params = {
//		std::make_shared<core::opParamArray>(arr.getDesc()),
//		std::make_shared<core::opParamEnum>(std::make_shared<std::string>(core::opInsertTypeToString(core::opInsertType::MEMORY))),
//		std::make_shared<core::opParamMemory>(mem),
//		std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&size, _ELE_DEFAULT_TYPE))
//	};
//
//	return std::make_shared<InsertOpr>(arr, params);
//}
}		// msdb