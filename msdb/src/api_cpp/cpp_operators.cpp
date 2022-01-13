#include <pch.h>
#include <api_cpp/cpp_operators.h>
#include <op/insert/insert_plan.h>
#include <op/insert/insert_action.h>
#include <op/save/save_plan.h>
#include <op/load/load_plan.h>
#include <op/between/between_plan.h>
#include <op/copy_to_buffer/copy_to_buffer_plan.h>
#include <op/naive_filter/naive_filter_plan.h>
#include <op/index_filter/index_filter_plan.h>
#include <op/build/build_plan.h>
#include <op/consume/consume_plan.h>
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
	ss << strChildIndent << "\"" <<this->getArrayDesc()->name_ << "\"" << std::endl;

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

/* ************************ */
/* Save						*/
/* ************************ */
SaveOpr::SaveOpr(std::shared_ptr<AFLOperator> qry)
	: childQry_(qry), AFLOperator(qry->getArrayDesc())
{
}

std::shared_ptr<core::opPlan> SaveOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::save_plan>();

	core::parameters params = {
		std::make_shared<core::opParamPlan>(childQry_->getPlan())
	};
	qryPlan->setParamSet(std::make_shared<core::save_plan_pset>(params));
	return qryPlan;
}
std::string SaveOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Save(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << ")";

	return ss.str();
}
std::shared_ptr<SaveOpr> Save(std::shared_ptr<AFLOperator> qry)
{
	return std::make_shared<SaveOpr>(qry);
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

/* ************************ */
/* Between					*/
/* ************************ */
BetweenOpr::BetweenOpr(Array arr, Domain d)
	: domain_(d), childQry_(Load(arr)), AFLOperator(arr.getDesc())
{
}

BetweenOpr::BetweenOpr(std::shared_ptr<AFLOperator> qry, Domain d)
	: childQry_(qry), domain_(d), AFLOperator(qry->getArrayDesc())
{
}

std::shared_ptr<core::opPlan> BetweenOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::between_plan>();

	core::parameters params = {
		std::make_shared<core::opParamPlan>(childQry_->getPlan()),
		std::make_shared<core::opParamCoor>(std::make_shared<core::coor>(this->domain_.getStart())),
		std::make_shared<core::opParamCoor>(std::make_shared<core::coor>(this->domain_.getEnd()))
	};
	qryPlan->setParamSet(std::make_shared<core::between_plan_pset>(params));

	return qryPlan;
}
std::string BetweenOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Between(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << "," << std::endl;
	ss << strChildIndent << this->domain_.toString() << ")";
	return ss.str();
}
std::shared_ptr<BetweenOpr> Between(Array arr, Domain d)
{
	return std::make_shared<BetweenOpr>(arr, d);
}
std::shared_ptr<BetweenOpr> Between(std::shared_ptr<AFLOperator> qry, Domain d)
{
	return std::make_shared<BetweenOpr>(qry, d);
}

/* ************************ */
/* Build					*/
/* ************************ */
BuildOpr::BuildOpr(const core::arrayId aid, const std::string name, 
				   core::pDimensionDescs dims, core::pAttributeDescs attrs)
	: AFLOperator(std::make_shared<core::arrayDesc>(aid, name, dims, attrs))
{

}

std::shared_ptr<core::opPlan> BuildOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::build_plan>();

	core::parameters params = {
		std::make_shared<core::opParamArray>(this->getArrayDesc())
	};
	qryPlan->setParamSet(std::make_shared<core::build_array_pset>(params));

	return qryPlan;
}

std::string BuildOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Build(" << std::endl;
	ss << this->getArrayDesc()->toString(strChildIndent) << ")";

	return ss.str();
}

std::shared_ptr<BuildOpr> Build(const core::arrayId aid, const std::string name, 
								std::vector<DefDimension> dims, 
								std::vector<DefAttribute> attrs)
{
	core::pDimensionDescs outDims = std::make_shared<core::dimensionDescs>();
	core::dimensionId dId = 0;
	for(auto d : dims)
	{
		auto pDim = d.getDesc();
		pDim->id_ = dId++;
		outDims->push_back(pDim);
	}

	core::pAttributeDescs outAttrs = std::make_shared<core::attributeDescs>();
	core::attributeId aId = 0;
	for (auto a : attrs)
	{
		auto pAttr = a.getDesc();
		pAttr->id_ = aId++;
		outAttrs->push_back(pAttr);
	}

	return std::make_shared<BuildOpr>(aid, name, outDims, outAttrs);
}

//BuildOpr& BuildOpr::AddAxis(id_t dimId, std::string axis, Coordinate dim, position_t chunkSize, position_t blockSize)
//{
//	//arrDesc_->dimDescs_->push_back(std::make_shared<core::dimensionDesc>(dimId, axis, dim.getCoor().at(0), dim.getCoor().at(1), chunkSize, blockSize));
//	//core::arrayMgr::instance()->setArrayDesc(arrDesc_->id_, arrDesc_);
//	return *this;
//}
//
//BuildOpr& BuildOpr::AddAttribute(id_t attrId, std::string name, core::eleType eType)
//{
//	//arrDesc_->attrDescs_->push_back(std::make_shared<core::attributeDesc>(attrId, name, eType));
//	//core::arrayMgr::instance()->setArrayDesc(arrDesc_->id_, arrDesc_);
//	return *this;
//}
//
//BuildOpr& BuildOpr::SetArray(id_t arrId, std::string name)
//{
//	//arrDesc_->id_ = arrId;
//	//arrDesc_->name_ = name;
//	//core::arrayMgr::instance()->setArrayDesc(arrId, arrDesc_);
//	return *this;
//}

//std::shared_ptr<BuildOpr> Build()
//{
//	return std::make_shared<BuildOpr>();
//}

/* ************************ */
/* Filter					*/
/* ************************ */
FilterOpr::FilterOpr(std::shared_ptr<AFLOperator> qry, std::shared_ptr<PredicateImpl> pred)
	: childQry_(qry), pred_(pred), AFLOperator(qry->getArrayDesc())
{
}
std::shared_ptr<core::opPlan> FilterOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::naive_filter_plan>();

	core::parameters params = {
		std::make_shared<core::opParamPlan>(childQry_->getPlan()),
		std::make_shared<core::opParamPredicate>(this->pred_->getPredicate())
	};
	qryPlan->setParamSet(std::make_shared<core::naive_filter_plan_pset>(params));

	return qryPlan;
}
std::string FilterOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Filter(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << "," << std::endl;
	ss << strChildIndent << this->pred_->toString() << ")";

	return ss.str();
}
std::shared_ptr<FilterOpr> Filter(std::shared_ptr<AFLOperator> qry, std::shared_ptr<TermImpl> singleTerm)
{
	return std::make_shared<FilterOpr>(qry, std::make_shared<PredicateImpl>(std::make_shared<core::singlePredicate>(singleTerm->getTerm())));
}

IndexFilterOpr::IndexFilterOpr(std::shared_ptr<AFLOperator> qry, std::shared_ptr<PredicateImpl> pred)
	: childQry_(qry), pred_(pred), AFLOperator(qry->getArrayDesc())
{
}
std::shared_ptr<core::opPlan> IndexFilterOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::index_filter_plan>();

	core::parameters params = {
		std::make_shared<core::opParamPlan>(childQry_->getPlan()),
		std::make_shared<core::opParamPredicate>(this->pred_->getPredicate())
	};
	qryPlan->setParamSet(std::make_shared<core::index_filter_plan_pset>(params));

	return qryPlan;
}
std::string IndexFilterOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "IndexFilter(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << "," << std::endl;
	ss << strChildIndent << this->pred_->toString() << ")";

	return ss.str();
}
std::shared_ptr<IndexFilterOpr> IndexFilter(std::shared_ptr<AFLOperator> qry, std::shared_ptr<TermImpl> singleTerm)
{
	return std::make_shared<IndexFilterOpr>(qry, std::make_shared<PredicateImpl>(std::make_shared<core::singlePredicate>(singleTerm->getTerm())));
}


/* ************************ */
/* ToBuffer					*/
/* ************************ */
CopyToBufferOpr::CopyToBufferOpr(std::shared_ptr<AFLOperator> qry)
	: childQry_(qry), AFLOperator(qry->getArrayDesc())
{
}
std::shared_ptr<core::opPlan> CopyToBufferOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::copy_to_buffer_plan>();

	core::parameters params = {
		std::make_shared<core::opParamPlan>(childQry_->getPlan())
	};
	qryPlan->setParamSet(std::make_shared<core::copy_to_buffer_pset>(params));
	
	return qryPlan;
}
std::string CopyToBufferOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "CopyToBuffer(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << ")";

	return ss.str();
}
std::shared_ptr<CopyToBufferOpr> CopyToBuffer(std::shared_ptr<AFLOperator> qry)
{
	return std::make_shared<CopyToBufferOpr>(qry);
}

/* ************************ */
/* Consume					*/
/* ************************ */
ConsumeOpr::ConsumeOpr(std::shared_ptr<AFLOperator> qry)
	: childQry_(qry), AFLOperator(qry->getArrayDesc())
{
}
std::shared_ptr<core::opPlan> ConsumeOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::consume_plan>();

	core::parameters params = {
		std::make_shared<core::opParamPlan>(childQry_->getPlan())
	};
	qryPlan->setParamSet(std::make_shared<core::consume_plan_pset>(params));

	return qryPlan;
}
std::string ConsumeOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Consume(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << ")";

	return ss.str();
}
std::shared_ptr<ConsumeOpr> Consume(std::shared_ptr<AFLOperator> qry)
{
	return std::make_shared<ConsumeOpr>(qry);
}
}		// msdb