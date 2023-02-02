#pragma once
#ifndef _MSDB_API_CPP_IO_OPERATORS_H_
#define _MSDB_API_CPP_IO_OPERATORS_H_

#include <pch_api_cpp.h>
#include <api_cpp/cpp_operators.h>

namespace msdb
{
	/* ************************ */
	/* Save 					*/
	/* ************************ */
	class API_CPP_DLL_API SaveOpr : public AFLOperator
	{
	public:
		SaveOpr(std::shared_ptr<AFLOperator> qry);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
	};
	std::shared_ptr<SaveOpr> API_CPP_DLL_API Save(std::shared_ptr<AFLOperator> qry);



	/* ************************ */
	/* Between					*/
	/* ************************ */
	class API_CPP_DLL_API BetweenOpr : public AFLOperator
	{
	public:
		BetweenOpr(Array arr, Domain d);
		BetweenOpr(std::shared_ptr<AFLOperator> qry, Domain d);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
		Domain domain_;
	};
	std::shared_ptr<BetweenOpr> API_CPP_DLL_API Between(Array arr, Domain d);
	std::shared_ptr<BetweenOpr> API_CPP_DLL_API Between(std::shared_ptr<AFLOperator> qry, Domain d);

	/* ************************ */
	/* Build					*/
	/* ************************ */
	class API_CPP_DLL_API BuildOpr : public AFLOperator
	{
	public:
		BuildOpr(const core::arrayId aid, const std::string name,
			core::pDimensionDescs dims,
			core::pAttributeDescs attrs);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	public:
		//BuildOpr& AddAxis(id_t dimId, std::string axis, Coordinates dim, position_t chunkSize, position_t blockSize);
		//BuildOpr& AddAttribute(id_t attrId, std::string name, core::eleType eType);
		//BuildOpr& SetArray(id_t arrId, std::string name);
	};
	std::shared_ptr<BuildOpr> API_CPP_DLL_API Build(core::arrayId aid, std::string name,
		std::vector<DefDimension>, std::vector<DefAttribute>);

	/* ************************ */
	/* Filter					*/
	/* ************************ */
	class API_CPP_DLL_API FilterOpr : public AFLOperator
	{
	public:
		FilterOpr(std::shared_ptr<AFLOperator> qry, std::shared_ptr<PredicateImpl> pred);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
		std::shared_ptr<PredicateImpl> pred_;
	};
	std::shared_ptr<FilterOpr> API_CPP_DLL_API Filter(std::shared_ptr<AFLOperator> qry, std::shared_ptr<TermImpl> singleTerm);

	class API_CPP_DLL_API IndexFilterOpr : public AFLOperator
	{
	public:
		IndexFilterOpr(std::shared_ptr<AFLOperator> qry, std::shared_ptr<PredicateImpl> pred);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
		std::shared_ptr<PredicateImpl> pred_;
	};
	std::shared_ptr<IndexFilterOpr> API_CPP_DLL_API IndexFilter(std::shared_ptr<AFLOperator> qry, std::shared_ptr<TermImpl> singleTerm);


	/* ************************ */
	/* ToBuffer					*/
	/* ************************ */
	class API_CPP_DLL_API CopyToBufferOpr : public AFLOperator
	{
	public:
		CopyToBufferOpr(std::shared_ptr<AFLOperator> qry);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
	};
	std::shared_ptr<CopyToBufferOpr> API_CPP_DLL_API CopyToBuffer(std::shared_ptr<AFLOperator> qry);

	/* ************************ */
	/* Consume					*/
	/* ************************ */
	class API_CPP_DLL_API ConsumeOpr : public AFLOperator
	{
	public:
		ConsumeOpr(std::shared_ptr<AFLOperator> qry);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
	};
	std::shared_ptr<ConsumeOpr> API_CPP_DLL_API Consume(std::shared_ptr<AFLOperator> qry);

	/* ************************ */
	/* BuildIndexOpr			*/
	/* ************************ */
	class API_CPP_DLL_API BuildIndexOpr : public AFLOperator
	{
	public:
		BuildIndexOpr(std::shared_ptr<AFLOperator> qry, attrIndexType indexType, core::eleDefault paramOne);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
		attrIndexType indexType_;
		core::eleDefault paramOne_;
	};

	std::shared_ptr<BuildIndexOpr> API_CPP_DLL_API BuildIndex(std::shared_ptr<AFLOperator> qry, attrIndexType compType, core::eleDefault paramOne);

	/* ************************ */
	/* SaveIndexOpr				*/
	/* ************************ */
	class API_CPP_DLL_API SaveIndexOpr : public AFLOperator
	{
	public:
		SaveIndexOpr(Array arr, attrIndexType indexType);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		attrIndexType indexType_;
	};

	std::shared_ptr<SaveIndexOpr> API_CPP_DLL_API SaveIndex(Array arr, attrIndexType compType);

	/* ************************ */
	/* LoadIndexOpr				*/
	/* ************************ */
	class API_CPP_DLL_API LoadIndexOpr : public AFLOperator
	{
	public:
		LoadIndexOpr(Array arr, attrIndexType indexType);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		attrIndexType indexType_;
	};

	std::shared_ptr<LoadIndexOpr> API_CPP_DLL_API LoadIndex(Array arr, attrIndexType compType);

	/* ************************ */
	/* CompOpr					*/
	/* ************************ */
	class API_CPP_DLL_API CompOpr : public AFLOperator
	{
	public:
		CompOpr(std::shared_ptr<AFLOperator> qry, encodingType compType);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
		encodingType compType_;
	};

	class API_CPP_DLL_API CompOneParamOpr : public AFLOperator
	{
	public:
		CompOneParamOpr(std::shared_ptr<AFLOperator> qry, encodingType compType,
			core::eleDefault paramOne);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
		encodingType compType_;
		core::eleDefault paramOne_;
	};

	class API_CPP_DLL_API CompTwoParamOpr : public AFLOperator
	{
	public:
		CompTwoParamOpr(std::shared_ptr<AFLOperator> qry, encodingType compType,
			core::eleDefault paramOne, core::eleDefault paramTwo);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
		encodingType compType_;
		core::eleDefault paramOne_;
		core::eleDefault paramTwo_;
	};

	std::shared_ptr<CompOpr> API_CPP_DLL_API Comp(
		std::shared_ptr<AFLOperator> qry, encodingType compType);

	std::shared_ptr<CompOneParamOpr> API_CPP_DLL_API Comp(
		std::shared_ptr<AFLOperator> qry, encodingType compType,
		core::eleDefault paramOne);

	std::shared_ptr<CompTwoParamOpr> API_CPP_DLL_API Comp(
		std::shared_ptr<AFLOperator> qry, encodingType compType,
		core::eleDefault paramOne, core::eleDefault paramTwo);

	/* ************************ */
	/* DecompOpr				*/
	/* ************************ */
	class API_CPP_DLL_API DecompOpr : public AFLOperator
	{
	public:
		DecompOpr(Array arr, encodingType compType);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
		encodingType compType_;
	};

	class API_CPP_DLL_API DecompOneParamOpr : public AFLOperator
	{
	public:
		DecompOneParamOpr(Array arr, encodingType compType,
			core::eleDefault paramOne);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
		encodingType compType_;
		core::eleDefault paramOne_;
	};

	class API_CPP_DLL_API DecompTwoParamOpr : public AFLOperator
	{
	public:
		DecompTwoParamOpr(Array arr, encodingType compType,
			core::eleDefault paramOne, core::eleDefault paramTwo);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);

	private:
		std::shared_ptr<AFLOperator> childQry_;
		encodingType compType_;
		core::eleDefault paramOne_;
		core::eleDefault paramTwo_;
	};

	std::shared_ptr<DecompOpr> API_CPP_DLL_API Decomp(
		Array arr, encodingType compType);

	std::shared_ptr<DecompOneParamOpr> API_CPP_DLL_API Decomp(
		Array arr, encodingType compType,
		core::eleDefault paramOne);

	std::shared_ptr<DecompTwoParamOpr> API_CPP_DLL_API Decomp(
		Array arr, encodingType compType,
		core::eleDefault paramOne, core::eleDefault paramTwo);
}		// msdb
#endif	// _MSDB_API_CPP_IO_OPERATORS_H_