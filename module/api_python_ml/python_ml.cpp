#include "api_python_ml.h"
#include <iostream>
#include <utility>
#include <limits>
#include "python_ml.h"
#include <util/logger.h>
#include <string>

#include <op/update/update_plan.h>
#include <op/between/between_plan.h>
#include <api_cpp/cpp_operators.h>
#include <api_cpp/cpp_query.h>

#include <util/logger.h>

void* getItem(wchar_t* arrName, unsigned int attrId, long long idx, size_t batchSize)
{
	std::wstring wStrArrName(arrName);
	std::string strArrName(wStrArrName.begin(), wStrArrName.end());

	auto arrId = msdb::core::arrayMgr::instance()->getArrayId(strArrName);
	if (arrId == msdb::core::INVALID_ARRAY_ID)
	{
		BOOST_LOG_TRIVIAL(warning) << "PythonML::Invalid array name: " << strArrName;
		return nullptr;
	}

	msdb::Context ctx;
	auto loadAfl = msdb::Between(
		msdb::Load(msdb::Array(ctx, arrId)), 
		msdb::Domain(msdb::core::coordinates({idx}), msdb::core::coordinates({idx + 1})));
	auto qry = msdb::Query(loadAfl);
	qry.setRawResultOut();

	auto ra = qry.execute();
	if (qry.getStatus() != msdb::Query::Status::COMPLETE)
	{
		BOOST_LOG_TRIVIAL(warning) << "PythonML::Load query failed: ";
		return nullptr;
	}

	auto outArr = ra.getRawResult();
	auto cit = outArr->getChunkIterator(attrId);

	// TODO::Batch size, return multiple chunks
	cit->moveToSeqPos(idx);
	if (!cit->isExist())
	{
		BOOST_LOG_TRIVIAL(warning) << "PythonML::No chunk: " << idx;
		return nullptr;
	}

	auto bf = (*cit)->getBuffer();
	if (!bf->size() || batchSize == 0)
	{
		BOOST_LOG_TRIVIAL(warning) << "PythonML::Empty chunk buffer: " << idx;
		return nullptr;
	}

	void* ret = (void*)malloc(bf->size() * batchSize);
	if (ret == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "Memory allocation failed in pythonML>getItem.\n" << "(Memory size: " << bf->size() * batchSize << ")";
		return nullptr;
	}

	memcpy(ret, bf->getReadData(), bf->size());
	return ret;
}
