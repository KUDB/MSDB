#include <pch.h>
#include <api_cpp/cpp_io_operators.h>
#include <api_cpp/cpp_operators.h>
#include <op/adapt_huffman_encode/adapt_huffman_encode_plan.h>
#include <op/adapt_huffman_decode/adapt_huffman_decode_plan.h>
#include <op/compass_encode/compass_encode_plan.h>
#include <op/compass_decode/compass_decode_plan.h>
#include <op/huffman_encode/huffman_encode_plan.h>
#include <op/huffman_decode/huffman_decode_plan.h>
#include <op/lzw_encode/lzw_encode_plan.h>
#include <op/lzw_decode/lzw_decode_plan.h>
#include <op/lzw_huffman_encode/lzw_huffman_encode_plan.h>
#include <op/lzw_huffman_decode/lzw_huffman_decode_plan.h>
#include <op/se_compression/se_compression_plan.h>
#include <op/se_decompression/se_decompression_plan.h>
#include <op/se_huffman_encode/se_huffman_encode_plan.h>
#include <op/se_huffman_decode/se_huffman_decode_plan.h>
#include <op/spiht_encode/spiht_encode_plan.h>
#include <op/spiht_decode/spiht_decode_plan.h>
#include <op/zip_load/zip_load_plan.h>
#include <op/zip_save/zip_save_plan.h>
#include <op/wavelet_encode/wavelet_encode_plan.h>
#include <op/wavelet_decode/wavelet_decode_plan.h>

#include <op/mmt_build/mmt_build_plan.h>
#include <op/compass_index_build/compass_index_build_plan.h>

namespace msdb
{
/* ************************ */
/* BuildIndexOpr			*/
/* ************************ */
BuildIndexOpr::BuildIndexOpr(Array arr, attrIndexType indexType)
	: indexType_(indexType), AFLOperator(arr.getDesc())
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
			std::make_shared<core::opParamArray>(this->getArrayDesc())
		};
		qryPlan->setParamSet(
			std::make_shared<core::mmt_build_array_pset>(params));
		return qryPlan;
		break;
	}
	case attrIndexType::COMPASS:
	{
		auto qryPlan = std::make_shared<core::compass_index_build_plan>();
		core::parameters params = {
			std::make_shared<core::opParamArray>(this->getArrayDesc())
		};
		qryPlan->setParamSet(
			std::make_shared<core::compass_index_build_array_pset>(params));
		return qryPlan;
		break;
	}
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
	ss << this->getArrayDesc()->name_ << ")";

	return ss.str();
}

std::shared_ptr<BuildIndexOpr> BuildIndex(Array arr, attrIndexType indexType)
{
	return std::make_shared<BuildIndexOpr>(arr, indexType);
}

/* ************************ */
/* CompOpr					*/
/* ************************ */
CompOpr::CompOpr(std::shared_ptr<AFLOperator> qry, compressionType compType)
	: childQry_(qry), compType_(compType), AFLOperator(qry->getArrayDesc())
{
}

std::shared_ptr<core::opPlan> CompOpr::getPlan()
{
	switch (this->compType_)
	{
	case compressionType::LZW:
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
	case compressionType::LZW_HUFFMAN:
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
	case compressionType::HUFFMAN:
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
	case compressionType::ADAPTHUFFMAN:
	{
		auto qryPlan = std::make_shared<core::adapt_huffman_encode_plan>();
		core::parameters params = {
			std::make_shared<core::opParamPlan>(childQry_->getPlan())
		};
		qryPlan->setParamSet(
			std::make_shared<core::adapt_huffman_encode_plan_pset>(params));
		return qryPlan;
		break;
	}
	case compressionType::ZIP:
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
CompOneParamOpr::CompOneParamOpr(std::shared_ptr<AFLOperator> qry, compressionType compType,
								 core::eleDefault paramOne)
	: childQry_(qry), compType_(compType), paramOne_(paramOne), AFLOperator(qry->getArrayDesc())
{
}

std::shared_ptr<core::opPlan> CompOneParamOpr::getPlan()
{
	switch (this->compType_)
	{
	case compressionType::COMPASS:
	{
		auto qryPlan = std::make_shared<core::compass_decode_plan>();
		core::parameters params = {
				std::make_shared<core::opParamPlan>(childQry_->getPlan()),
				std::make_shared<core::opParamConst>(
					std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE)),
			std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
		};
		qryPlan->setParamSet(
			std::make_shared<core::compass_decode_plan_pset>(params));
		return qryPlan;
		break;
	}
	case compressionType::SPIHT:
	{
		auto qryPlan = std::make_shared<core::spiht_decode_plan>();
		core::parameters params = {
			std::make_shared<core::opParamPlan>(childQry_->getPlan()),
			std::make_shared<core::opParamConst>(
				std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE)),
			std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
		};
		qryPlan->setParamSet(
			std::make_shared<core::spiht_decode_plan_pset>(params));
		return qryPlan;
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
	ss << this->childQry_->toString(depth + 1) << ",";
	ss << this->paramOne_ << ")";

	return ss.str();
}
CompTwoParamOpr::CompTwoParamOpr(std::shared_ptr<AFLOperator> qry, compressionType compType,
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
	case compressionType::SEACOW:
	{
		auto wtQryPlan = std::make_shared<core::se_compression_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(childQry_->getPlan()),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::se_compression_plan_pset>(params));
		}

		auto seQryPlan = std::make_shared<core::se_compression_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(wtQryPlan),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramTwo_, _ELE_DEFAULT_TYPE))
			};
			seQryPlan->setParamSet(
				std::make_shared<core::se_compression_plan_pset>(params));
		}
		return seQryPlan;
	}
	case compressionType::SEACOW_HUFFMAN:
	{
		auto wtQryPlan = std::make_shared<core::se_compression_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(childQry_->getPlan()),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			wtQryPlan->setParamSet(
				std::make_shared<core::se_compression_plan_pset>(params));
		}

		auto seQryPlan = std::make_shared<core::se_compression_plan>();
		{
			core::parameters params = {
					std::make_shared<core::opParamPlan>(wtQryPlan),
					std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramTwo_, _ELE_DEFAULT_TYPE))
			};
			seQryPlan->setParamSet(
				std::make_shared<core::se_compression_plan_pset>(params));
		}
		return seQryPlan;
		break;
	}
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
	ss << this->childQry_->toString(depth + 1) << ",";
	ss << this->paramOne_ << ",";
	ss << this->paramTwo_ << ")";

	return ss.str();
}

std::shared_ptr<CompOpr> Comp(
	std::shared_ptr<AFLOperator> qry, compressionType compType)
{
	return std::make_shared<CompOpr>(qry, compType);
}
std::shared_ptr<CompOneParamOpr> Comp(
	std::shared_ptr<AFLOperator> qry, compressionType compType,
							  int paramOne)
{
	return std::make_shared<CompOneParamOpr>(qry, compType, paramOne);
}
std::shared_ptr<CompTwoParamOpr> Comp(
	std::shared_ptr<AFLOperator> qry, compressionType compType,
	int paramOne, int paramTwo)
{
	return std::make_shared<CompTwoParamOpr>(qry, compType, paramOne, paramTwo);
}

/* ************************ */
/* DecompOpr				*/
/* ************************ */
DecompOpr::DecompOpr(Array arr, compressionType compType)
	: compType_(compType), AFLOperator(arr.getDesc())
{
}

std::shared_ptr<core::opPlan> DecompOpr::getPlan()
{
	switch (this->compType_)
	{
	case compressionType::LZW:
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
	case compressionType::LZW_HUFFMAN:
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
	case compressionType::HUFFMAN:
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
	case compressionType::ADAPTHUFFMAN:
	{
		auto qryPlan = std::make_shared<core::adapt_huffman_decode_plan>();
		core::parameters params = {
			std::make_shared<core::opParamArray>(this->getArrayDesc())
		};
		qryPlan->setParamSet(
			std::make_shared<core::adapt_huffman_decode_array_pset>(params));
		return qryPlan;
		break;
	}
	case compressionType::ZIP:
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
	ss << this->getArrayDesc()->name_ << ")";

	return ss.str();
}
DecompOneParamOpr::DecompOneParamOpr(Array arr, compressionType compType,
									 core::eleDefault paramOne)
	: compType_(compType), paramOne_(paramOne), AFLOperator(arr.getDesc())
{
}

std::shared_ptr<core::opPlan> DecompOneParamOpr::getPlan()
{
	switch (this->compType_)
	{
	case compressionType::COMPASS:
	{
		auto qryPlan = std::make_shared<core::compass_decode_plan>();
		core::parameters params = {
				std::make_shared<core::opParamArray>(this->getArrayDesc()),
				std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
		};
		qryPlan->setParamSet(
			std::make_shared<core::compass_decode_array_pset>(params));
		return qryPlan;
		break;
	}
	case compressionType::SPIHT:
	{
		auto spQryPlan = std::make_shared<core::spiht_decode_plan>();
		{
			core::parameters params = {
				std::make_shared<core::opParamArray>(this->getArrayDesc()),
				std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			spQryPlan->setParamSet(
				std::make_shared<core::spiht_decode_plan_pset>(params));
		}

		auto wtQryPlan = std::make_shared<core::wavelet_decode_plan>();
		{
			core::parameters params = {
				std::make_shared<core::opParamPlan>(spQryPlan),
				std::make_shared<core::opParamConst>(std::make_shared<core::stableElement>(&this->paramOne_, _ELE_DEFAULT_TYPE))
			};
			spQryPlan->setParamSet(
				std::make_shared<core::wavelet_decode_array_pset>(params));
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
	ss << this->getArrayDesc()->name_ << ",";
	ss << this->paramOne_ << ")";

	return ss.str();
}
DecompTwoParamOpr::DecompTwoParamOpr(Array arr, compressionType compType,
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
	case compressionType::SEACOW:
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
	case compressionType::SEACOW_HUFFMAN:
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
	default:
		return nullptr;
	};
}
std::string DecompTwoParamOpr::toString(int depth)
{
	std::string strIndent = this->getIndentString(depth);
	std::string strChildIndent = this->getIndentString(depth + 1);
	std::stringstream ss;
	ss << AFLOperator::toString(depth) << strIndent << this->getPlan()->name() << "(" << std::endl;
	ss << this->getArrayDesc()->name_ << ",";
	ss << this->paramOne_ << ",";
	ss << this->paramTwo_ << ")";

	return ss.str();
}
std::shared_ptr<DecompOpr> Decomp(
	Array arr, compressionType compType)
{
	return std::make_shared<DecompOpr>(arr, compType);
}
std::shared_ptr<DecompOneParamOpr> Decomp(
	Array arr, compressionType compType,
	core::eleDefault paramOne)
{
	return std::make_shared<DecompOneParamOpr>(arr, compType, paramOne);
}
std::shared_ptr<DecompTwoParamOpr> Decomp(
	Array arr, compressionType compType,
	core::eleDefault paramOne, core::eleDefault paramTwo)
{
	return std::make_shared<DecompTwoParamOpr>(arr, compType, paramOne, paramTwo);
}
}		// msdb