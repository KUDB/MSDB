#include <pch.h>
#include <api_cpp/cpp_io_operators.h>
#include <api_cpp/cpp_operators.h>
#include <op/adapt_huffman_encode/adapt_huffman_encode_plan.h>
#include <op/adapt_huffman_decode/adapt_huffman_decode_plan.h>
//#include <op/compass_encode/compass_encode_plan.h>
//#include <op/compass_encode/compass_decode_plan.h>
#include <op/huffman_encode/huffman_encode_plan.h>
#include <op/huffman_encode/huffman_decode_plan.h>
#include <op/lzw_encode/lzw_encode_plan.h>
#include <op/lzw_encode/lzw_decode_plan.h>
#include <op/lzw_huffman_encode/lzw_huffman_encode_plan.h>
#include <op/lzw_huffman_encode/lzw_huffman_decode_plan.h>
#include <op/se_compression/se_compression_plan.h>
#include <op/se_compression/se_decompression_plan.h>
#include <op/se_huffman_encode/se_huffman_encode_plan.h>
#include <op/se_huffman_encode/se_huffman_decode_plan.h>
#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_encode/spiht_decode_plan.h>
#include <op/zip_save/zip_load_plan.h>
#include <op/zip_save/zip_save_plan.h>
#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <op/wavelet_encode/wavelet_decode_plan.h>
//#include <op/zfp_encode/zfp_encode_plan.h>
//#include <op/zfp_encode/zfp_decode_plan.h>
//#include <op/tthresh_encode/tthresh_encode_plan.h>
//#include <op/tthresh_encode/tthresh_decode_plan.h>
#include <op/mmt_build/mmt_build_plan.h>
#include <op/mmt_save/mmt_save_plan.h>
#include <op/mmt_load/mmt_load_plan.h>
#include <op/compass_index_build/compass_index_build_plan.h>

namespace msdb
{
/* ************************ */
/* BuildIndexOpr			*/
/* ************************ */
BuildIndexOpr::BuildIndexOpr(std::shared_ptr<AFLOperator> qry, attrIndexType indexType, core::eleDefault paramOne)
	: childQry_(qry), indexType_(indexType), paramOne_(paramOne), AFLOperator(qry->getArrayDesc())
{
}

std::shared_ptr<core::opPlan> BuildIndexOpr::getPlan()
{
	switch (this->indexType_)
	{
	case attrIndexType::MMT:
	{
		auto qryPlan = std::make_shared<core::mmt_build_plan>();
		core::parameters params = {
			std::make_shared<core::opParamPlan>(this->childQry_->getPlan()),
			std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
		};
		qryPlan->setParamSet(
			std::make_shared<core::mmt_build_plan_pset>(params));
		return qryPlan;
		break;
	}
	//case attrIndexType::COMPASS:
	//{
	//	auto qryPlan = std::make_shared<core::compass_index_build_plan>();
	//	core::parameters params = {
	//		std::make_shared<core::opParamPlan>(this->childQry_->getPlan()),
	//		std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::compass_index_build_plan_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	default:
		return nullptr;
	};
}
std::string BuildIndexOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << this->getPlan()->name() << "(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << "," << this->paramOne_ << ")";

	return ss.str();
}

std::shared_ptr<BuildIndexOpr> BuildIndex(std::shared_ptr<AFLOperator> qry, attrIndexType indexType, core::eleDefault paramOne)
{
	return std::make_shared<BuildIndexOpr>(qry, indexType, paramOne);
}

/* ************************ */
/* SaveIndexOpr				*/
/* ************************ */
SaveIndexOpr::SaveIndexOpr(Array arr, attrIndexType indexType)
	: indexType_(indexType), AFLOperator(arr.getDesc())
{
}

std::shared_ptr<core::opPlan> SaveIndexOpr::getPlan()
{
	switch (this->indexType_)
	{
	case attrIndexType::MMT:
	{
		auto qryPlan = std::make_shared<core::mmt_save_plan>();
		core::parameters params = {
			std::make_shared<core::opParamArray>(this->getArrayDesc())
		};
		qryPlan->setParamSet(
			std::make_shared<core::mmt_save_array_pset>(params));
		return qryPlan;
		break;
	}
	//case attrIndexType::COMPASS:
	//{
	//	auto qryPlan = std::make_shared<core::compass_index_build_plan>();
	//	core::parameters params = {
	//		std::make_shared<core::opParamPlan>(this->childQry_->getPlan()),
	//		std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::compass_index_build_plan_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	default:
		return nullptr;
	};
}
std::string SaveIndexOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << this->getPlan()->name() << "(" << std::endl;
	ss << strChildIndent << this->getArrayDesc()->name_ << "," << this->getPlan()->name() << ")";

	return ss.str();
}

std::shared_ptr<SaveIndexOpr> SaveIndex(Array arr, attrIndexType indexType)
{
	return std::make_shared<SaveIndexOpr>(arr, indexType);
}

/* ************************ */
/* LoadIndexOpr				*/
/* ************************ */
LoadIndexOpr::LoadIndexOpr(Array arr, attrIndexType indexType)
	: indexType_(indexType), AFLOperator(arr.getDesc())
{
}

std::shared_ptr<core::opPlan> LoadIndexOpr::getPlan()
{
	switch (this->indexType_)
	{
	case attrIndexType::MMT:
	{
		auto qryPlan = std::make_shared<core::mmt_load_plan>();
		core::parameters params = {
			std::make_shared<core::opParamArray>(this->getArrayDesc())
		};
		qryPlan->setParamSet(
			std::make_shared<core::mmt_load_array_pset>(params));
		return qryPlan;
		break;
	}
	//case attrIndexType::COMPASS:
	//{
	//	auto qryPlan = std::make_shared<core::compass_index_build_plan>();
	//	core::parameters params = {
	//		std::make_shared<core::opParamPlan>(this->childQry_->getPlan()),
	//		std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::compass_index_build_plan_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	default:
		return nullptr;
	};
}
std::string LoadIndexOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << this->getPlan()->name() << "(" << std::endl;
	ss << strChildIndent << this->getArrayDesc()->name_ << "," << this->getPlan()->name() << ")";

	return ss.str();
}

std::shared_ptr<LoadIndexOpr> LoadIndex(Array arr, attrIndexType indexType)
{
	return std::make_shared<LoadIndexOpr>(arr, indexType);
}

/* ************************ */
/* CompOpr					*/
/* ************************ */
CompOpr::CompOpr(std::shared_ptr<AFLOperator> qry, encodingType compType)
	: childQry_(qry), compType_(compType), AFLOperator(qry->getArrayDesc())
{
}

std::shared_ptr<core::opPlan> CompOpr::getPlan()
{
	switch (this->compType_)
	{
	//////////////////////////////
	// Param Two
	//
	case encodingType::SEACOW:
	{
		auto wtQryPlan = std::make_shared<core::wavelet_encode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(childQry_->getPlan())
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::wavelet_encode_plan_pset>(params));
		}

		auto seQryPlan = std::make_shared<core::se_compression_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(wtQryPlan)
			};
			seQryPlan->setParamSet(
				std::make_shared<core::se_compression_plan_pset>(params));
		}
		return seQryPlan;
	}
#ifdef _MSDB_OP_SE_HUFFMAN_ENCODE_PLAN_H_
	case encodingType::SEACOW_HUFFMAN:
	{
		auto wtQryPlan = std::make_shared<core::wavelet_encode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(childQry_->getPlan())
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::wavelet_encode_plan_pset>(params));
		}

		auto seQryPlan = std::make_shared<core::se_huffman_encode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(wtQryPlan)
			};
			seQryPlan->setParamSet(
				std::make_shared<core::se_huffman_encode_plan_pset>(params));
		}
		return seQryPlan;
		break;
	}
#endif	//_MSDB_OP_SE_HUFFMAN_ENCODE_PLAN_H_
	//////////////////////////////

	//////////////////////////////
	// Param One
	//
	//case encodingType::COMPASS:
	//{
	//	auto qryPlan = std::make_shared<core::compass_encode_plan>();
	//	core::parameters params = {
	//			std::make_shared<core::opParamPlan>(childQry_->getPlan())
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::compass_encode_plan_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	case encodingType::SPIHT:
	{
		auto wtQryPlan = std::make_shared<core::wavelet_encode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(childQry_->getPlan())
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::wavelet_encode_plan_pset>(params));
		}

		auto spihtQryPlan = std::make_shared<core::spiht_encode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(wtQryPlan)
			};
			spihtQryPlan->setParamSet(
				std::make_shared<core::spiht_encode_plan_pset>(params));
		}
		return spihtQryPlan;
		return nullptr;
		break;
	}
	//////////////////////////////

	//////////////////////////////
	// No Param
	//
	case encodingType::LZW:
	{
		auto qryPlan = std::make_shared<core::lzw_encode_plan>();
		core::parameters params = {
			std::make_shared<core::opParamPlan>(childQry_->getPlan())
		};
		qryPlan->setParamSet(
			std::make_shared<core::lzw_encode_plan_pset>(params));
		return qryPlan;
		break;
	}
	case encodingType::LZW_HUFFMAN:
	{
		auto qryPlan = std::make_shared<core::lzw_huffman_encode_plan>();
		core::parameters params = {
			std::make_shared<core::opParamPlan>(childQry_->getPlan())
		};
		qryPlan->setParamSet(
			std::make_shared<core::lzw_huffman_encode_plan_pset>(params));
		return qryPlan;
		break;
	}
	case encodingType::HUFFMAN:
	{
		auto qryPlan = std::make_shared<core::huffman_encode_plan>();
		core::parameters params = {
			std::make_shared<core::opParamPlan>(childQry_->getPlan())
		};
		qryPlan->setParamSet(
			std::make_shared<core::huffman_encode_plan_pset>(params));
		return qryPlan;
		break;
	}
	//case encodingType::ADAPTHUFFMAN:
	//{
	//	auto qryPlan = std::make_shared<core::adapt_huffman_encode_plan>();
	//	core::parameters params = {
	//		std::make_shared<core::opParamPlan>(childQry_->getPlan())
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::adapt_huffman_encode_plan_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	case encodingType::ZIP:
	{
		auto qryPlan = std::make_shared<core::zip_save_plan>();
		core::parameters params = {
			std::make_shared<core::opParamPlan>(childQry_->getPlan())
		};
		qryPlan->setParamSet(
			std::make_shared<core::zip_save_plan_pset>(params));
		return qryPlan;
		break;
	}
	//case encodingType::ZFP:
	//{
	//	auto qryPlan = std::make_shared<core::zfp_encode_plan>();
	//	core::parameters params = {
	//		std::make_shared<core::opParamPlan>(childQry_->getPlan())
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::zfp_encode_plan_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	//case encodingType::TTHRESH:
	//{
	//	auto qryPlan = std::make_shared<core::tthresh_encode_plan>();
	//	core::parameters params = {
	//		std::make_shared<core::opParamPlan>(childQry_->getPlan())
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::tthresh_encode_plan_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	//////////////////////////////
	default:
		return nullptr;
	};
}
std::string CompOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Comp(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << ")";

	return ss.str();
}
CompOneParamOpr::CompOneParamOpr(std::shared_ptr<AFLOperator> qry, encodingType compType,
								 core::eleDefault paramOne)
	: childQry_(qry), compType_(compType), paramOne_(paramOne), AFLOperator(qry->getArrayDesc())
{
}

std::shared_ptr<core::opPlan> CompOneParamOpr::getPlan()
{
	switch (this->compType_)
	{
	//case encodingType::COMPASS:
	//{
	//	auto qryPlan = std::make_shared<core::compass_encode_plan>();
	//	core::parameters params = {
	//			std::make_shared<core::opParamPlan>(childQry_->getPlan()),
	//			std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::compass_encode_plan_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	case encodingType::SPIHT:
	{
		auto wtQryPlan = std::make_shared<core::wavelet_encode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(childQry_->getPlan()),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::wavelet_encode_plan_pset>(params));
		}

		auto spihtQryPlan = std::make_shared<core::spiht_encode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(wtQryPlan)
			};
			spihtQryPlan->setParamSet(
				std::make_shared<core::spiht_encode_plan_pset>(params));
		}
		return spihtQryPlan;
		return nullptr;
		break;
	}
	default:
		return nullptr;
	};
}
std::string CompOneParamOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Comp(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << "," << this->paramOne_ << ")";

	return ss.str();
}
CompTwoParamOpr::CompTwoParamOpr(std::shared_ptr<AFLOperator> qry, encodingType compType,
								 core::eleDefault paramOne, core::eleDefault paramTwo)
	: childQry_(qry), compType_(compType), 
	paramOne_(paramOne), paramTwo_(paramTwo), 
	AFLOperator(qry->getArrayDesc())
{
}

std::shared_ptr<core::opPlan> CompTwoParamOpr::getPlan()
{
	switch (this->compType_)
	{
	case encodingType::SEACOW:
	{
		auto wtQryPlan = std::make_shared<core::wavelet_encode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(childQry_->getPlan()),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::wavelet_encode_plan_pset>(params));
		}

		auto seQryPlan = std::make_shared<core::se_compression_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(wtQryPlan)
			};
			seQryPlan->setParamSet(
				std::make_shared<core::se_compression_plan_pset>(params));
		}
		return seQryPlan;
	}
#ifdef _MSDB_OP_SE_HUFFMAN_ENCODE_PLAN_H_
	case encodingType::SEACOW_HUFFMAN:
	{
		auto wtQryPlan = std::make_shared<core::wavelet_encode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(childQry_->getPlan()),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::wavelet_encode_plan_pset>(params));
		}

		auto seQryPlan = std::make_shared<core::se_huffman_encode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(wtQryPlan)
			};
			seQryPlan->setParamSet(
				std::make_shared<core::se_huffman_encode_plan_pset>(params));
		}
		return seQryPlan;
		break;
	}
#endif
	default:
		return nullptr;
	};
}
std::string CompTwoParamOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << "Comp(" << std::endl;
	ss << this->childQry_->toString(depth + 1) << "," << this->paramOne_ << "," << this->paramTwo_ << ")";

	return ss.str();
}

std::shared_ptr<CompOpr> Comp(
	std::shared_ptr<AFLOperator> qry, encodingType compType)
{
	return std::make_shared<CompOpr>(qry, compType);
}
std::shared_ptr<CompOneParamOpr> Comp(
	std::shared_ptr<AFLOperator> qry, encodingType compType,
	core::eleDefault paramOne)
{
	return std::make_shared<CompOneParamOpr>(qry, compType, paramOne);
}
std::shared_ptr<CompTwoParamOpr> Comp(
	std::shared_ptr<AFLOperator> qry, encodingType compType,
	core::eleDefault paramOne, core::eleDefault paramTwo)
{
	return std::make_shared<CompTwoParamOpr>(qry, compType, paramOne, paramTwo);
}

/* ************************ */
/* DecompOpr				*/
/* ************************ */
DecompOpr::DecompOpr(Array arr, encodingType compType)
	: compType_(compType), AFLOperator(arr.getDesc())
{
}

std::shared_ptr<core::opPlan> DecompOpr::getPlan()
{
	switch (this->compType_)
	{
	case encodingType::LZW:
	{
		auto qryPlan = std::make_shared<core::lzw_decode_plan>();
		core::parameters params = {
			std::make_shared<core::opParamArray>(this->getArrayDesc())
		};
		qryPlan->setParamSet(
			std::make_shared<core::lzw_decode_array_pset>(params));
		return qryPlan;
		break;
	}
	case encodingType::LZW_HUFFMAN:
	{
		auto qryPlan = std::make_shared<core::lzw_huffman_decode_plan>();
		core::parameters params = {
			std::make_shared<core::opParamArray>(this->getArrayDesc())
		};
		qryPlan->setParamSet(
			std::make_shared<core::lzw_huffman_decode_array_pset>(params));
		return qryPlan;
		break;
	}
	case encodingType::HUFFMAN:
	{
		auto qryPlan = std::make_shared<core::huffman_decode_plan>();
		core::parameters params = {
			std::make_shared<core::opParamArray>(this->getArrayDesc())
		};
		qryPlan->setParamSet(
			std::make_shared<core::huffman_decode_array_pset>(params));
		return qryPlan;
		break;
	}
	//case encodingType::ADAPTHUFFMAN:
	//{
	//	auto qryPlan = std::make_shared<core::adapt_huffman_decode_plan>();
	//	core::parameters params = {
	//		std::make_shared<core::opParamArray>(this->getArrayDesc())
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::adapt_huffman_decode_array_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	case encodingType::ZIP:
	{
		auto qryPlan = std::make_shared<core::zip_load_plan>();
		core::parameters params = {
			std::make_shared<core::opParamArray>(this->getArrayDesc())
		};
		qryPlan->setParamSet(
			std::make_shared<core::zip_load_array_pset>(params));
		return qryPlan;
		break;
	}
	//case encodingType::ZFP:
	//{
	//	auto qryPlan = std::make_shared<core::zfp_decode_plan>();
	//	core::parameters params = {
	//		std::make_shared<core::opParamArray>(this->getArrayDesc())
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::zfp_decode_array_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	//case encodingType::TTHRESH:
	//{
	//	auto qryPlan = std::make_shared<core::tthresh_decode_plan>();
	//	core::parameters params = {
	//		std::make_shared<core::opParamArray>(this->getArrayDesc())
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::tthresh_decode_array_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	default:
		return nullptr;
	};
}
std::string DecompOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << this->getPlan()->name() << "(" << std::endl;
	ss << strChildIndent << this->getArrayDesc()->name_ << ")";

	return ss.str();
}
DecompOneParamOpr::DecompOneParamOpr(Array arr, encodingType compType,
									 core::eleDefault paramOne)
	: compType_(compType), paramOne_(paramOne), AFLOperator(arr.getDesc())
{
}

std::shared_ptr<core::opPlan> DecompOneParamOpr::getPlan()
{
	switch (this->compType_)
	{
	//case encodingType::COMPASS:
	//{
	//	auto qryPlan = std::make_shared<core::compass_decode_plan>();
	//	core::parameters params = {
	//			std::make_shared<core::opParamArray>(this->getArrayDesc()),
	//			std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
	//	};
	//	qryPlan->setParamSet(
	//		std::make_shared<core::compass_decode_array_pset>(params));
	//	return qryPlan;
	//	break;
	//}
	case encodingType::SPIHT:
	{
		auto spQryPlan = std::make_shared<core::spiht_decode_plan>();
		{
			core::parameters params = {
				std::make_shared<core::opParamArray>(this->getArrayDesc()),
				std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			spQryPlan->setParamSet(
				std::make_shared<core::spiht_decode_array_pset>(params));
		}

		auto wtQryPlan = std::make_shared<core::wavelet_decode_plan>();
		{
			core::parameters params = {
				std::make_shared<core::opParamPlan>(spQryPlan),
				std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::wavelet_decode_plan_pset>(params));
		}

		return wtQryPlan;
		break;
	}
	default:
		return nullptr;
	};
}
std::string DecompOneParamOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << this->getPlan()->name() << "(" << std::endl;
	ss << strChildIndent << this->getArrayDesc()->name_ << "," << this->paramOne_ << ")";

	return ss.str();
}
DecompTwoParamOpr::DecompTwoParamOpr(Array arr, encodingType compType,
									 core::eleDefault paramOne, core::eleDefault paramTwo)
	: compType_(compType),
	paramOne_(paramOne), paramTwo_(paramTwo), 
	AFLOperator(arr.getDesc())
{
}

std::shared_ptr<core::opPlan> DecompTwoParamOpr::getPlan()
{
	switch (this->compType_)
	{
	case encodingType::SEACOW:
	{
		auto seQryPlan = std::make_shared<core::se_decompression_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamArray>(this->getArrayDesc()),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramTwo_, _ELE_DEFAULT_TYPE))
			};
			seQryPlan->setParamSet(
				std::make_shared<core::se_decompression_array_pset>(params));
		}

		auto wtQryPlan = std::make_shared<core::wavelet_decode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(seQryPlan),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::wavelet_decode_plan_pset>(params));
		}

		return wtQryPlan;
		break;
	}
#ifdef _MSDB_SE_HUFFMAN_DECODE_PLAN_H_
	case encodingType::SEACOW_HUFFMAN:
	{
		auto seQryPlan = std::make_shared<core::se_huffman_decode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamArray>(this->getArrayDesc()),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramTwo_, _ELE_DEFAULT_TYPE))
			};
			seQryPlan->setParamSet(
				std::make_shared<core::se_huffman_decode_array_pset>(params));
		}

		auto wtQryPlan = std::make_shared<core::wavelet_decode_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(seQryPlan),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::wavelet_decode_plan_pset>(params));
		}

		return wtQryPlan;
		break;
	}
#endif	// _MSDB_SE_HUFFMAN_DECODE_PLAN_H_
	default:
		return nullptr;
	};
}
std::string DecompTwoParamOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	auto p = this->getPlan();
	ss << AFLOperator::toString(depth) << strIndent << p->name();
	ss << "(" << std::endl;
	ss << strChildIndent << this->getArrayDesc()->name_ << "," << this->paramOne_ << "," << this->paramTwo_ << ")";

	return ss.str();
}
std::shared_ptr<DecompOpr> Decomp(
	Array arr, encodingType compType)
{
	return std::make_shared<DecompOpr>(arr, compType);
}
std::shared_ptr<DecompOneParamOpr> Decomp(
	Array arr, encodingType compType,
	core::eleDefault paramOne)
{
	return std::make_shared<DecompOneParamOpr>(arr, compType, paramOne);
}
std::shared_ptr<DecompTwoParamOpr> Decomp(
	Array arr, encodingType compType,
	core::eleDefault paramOne, core::eleDefault paramTwo)
{
	return std::make_shared<DecompTwoParamOpr>(arr, compType, paramOne, paramTwo);
}
}		// msdb