#include <pch_op.h>
#include <op/between/between_plan.h>
#include <op/between/between_action.h>

namespace msdb
{
namespace core
{
	// plan
between_plan::between_plan()
{
}

const char* between_plan::name()
{
	return "between_plan";
}

pAction between_plan::makeAction()
{
	return std::make_shared<between_action>();
}

//////////////////////////////
// Array Pset
//
between_array_pset::between_array_pset(parameters& pSet)
	: opArrayParamSet(pSet), between_pset()
{
	assert(this->params_.size() == 3);
	assert(this->params_[1]->type() == opParamType::COOR);
	assert(this->params_[2]->type() == opParamType::COOR);
}

//pArrayDesc between_array_pset::inferSchema()
//{
//	auto aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
//	auto aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);
//
//	return aInferDesc;
//}

pBitmapTree between_array_pset::inferBottomUpBitmap()
{
	pArrayDesc arrDesc = this->inferSchema();
	auto inSp = std::static_pointer_cast<coor>(this->params_[1]->getParam());
	auto inEp = std::static_pointer_cast<coor>(this->params_[2]->getParam());

	dimension chunkSpace = arrDesc->getDimDescs()->getChunkSpace();
	size_t chunkNums = chunkSpace.area();
	auto chunkBitmap = std::make_shared<bitmapTree>(chunkNums, true);

	for (auto attrDesc : *arrDesc->attrDescs_)
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			return this->inferBottomUpAttrBitmap<char>(arrDesc, attrDesc, chunkBitmap, inSp, inEp);
			break;
		case eleType::INT8:
			return this->inferBottomUpAttrBitmap<int8_t>(arrDesc, attrDesc, chunkBitmap, inSp, inEp);
			break;
		case eleType::INT16:
			return this->inferBottomUpAttrBitmap<int16_t>(arrDesc, attrDesc, chunkBitmap, inSp, inEp);
			break;
		case eleType::INT32:
			return this->inferBottomUpAttrBitmap<int32_t>(arrDesc, attrDesc, chunkBitmap, inSp, inEp);
			break;
		case eleType::INT64:
			return this->inferBottomUpAttrBitmap<int64_t>(arrDesc, attrDesc, chunkBitmap, inSp, inEp);
			break;
		case eleType::UINT8:
			return this->inferBottomUpAttrBitmap<uint8_t>(arrDesc, attrDesc, chunkBitmap, inSp, inEp);
			break;
		case eleType::UINT16:
			return this->inferBottomUpAttrBitmap<uint16_t>(arrDesc, attrDesc, chunkBitmap, inSp, inEp);
			break;
		case eleType::UINT32:
			return this->inferBottomUpAttrBitmap<uint32_t>(arrDesc, attrDesc, chunkBitmap, inSp, inEp);
			break;
		case eleType::UINT64:
			return this->inferBottomUpAttrBitmap<uint64_t>(arrDesc, attrDesc, chunkBitmap, inSp, inEp);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}
	return nullptr;
}

//////////////////////////////
// Plan Pset
//
between_plan_pset::between_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet), between_pset()
{
	assert(this->params_.size() == 3);
	assert(this->params_[1]->type() == opParamType::COOR);
	assert(this->params_[2]->type() == opParamType::COOR);
}

pBitmapTree between_plan_pset::inferBottomUpBitmap()
{
	pArrayDesc arrDesc = this->inferSchema();
	auto boUpBitmap = std::make_shared<bitmapTree>(*(this->getSourcePlanBottomUpBitmap()));
	auto inSp = std::static_pointer_cast<coor>(this->params_[1]->getParam());
	auto inEp = std::static_pointer_cast<coor>(this->params_[2]->getParam());

	for (auto attrDesc : *arrDesc->attrDescs_)
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			return this->inferBottomUpAttrBitmap<char>(arrDesc, attrDesc, boUpBitmap, inSp, inEp);
			break;
		case eleType::INT8:
			return this->inferBottomUpAttrBitmap<int8_t>(arrDesc, attrDesc, boUpBitmap, inSp, inEp);
			break;
		case eleType::INT16:
			return this->inferBottomUpAttrBitmap<int16_t>(arrDesc, attrDesc, boUpBitmap, inSp, inEp);
			break;
		case eleType::INT32:
			return this->inferBottomUpAttrBitmap<int32_t>(arrDesc, attrDesc, boUpBitmap, inSp, inEp);
			break;
		case eleType::INT64:
			return this->inferBottomUpAttrBitmap<int64_t>(arrDesc, attrDesc, boUpBitmap, inSp, inEp);
			break;
		case eleType::UINT8:
			return this->inferBottomUpAttrBitmap<uint8_t>(arrDesc, attrDesc, boUpBitmap, inSp, inEp);
			break;
		case eleType::UINT16:
			return this->inferBottomUpAttrBitmap<uint16_t>(arrDesc, attrDesc, boUpBitmap, inSp, inEp);
			break;
		case eleType::UINT32:
			return this->inferBottomUpAttrBitmap<uint32_t>(arrDesc, attrDesc, boUpBitmap, inSp, inEp);
			break;
		case eleType::UINT64:
			return this->inferBottomUpAttrBitmap<uint64_t>(arrDesc, attrDesc, boUpBitmap, inSp, inEp);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}
	return nullptr;
}
}		// core


/* ************************ */
/* CPP API Operator	        */
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
}		// msdb