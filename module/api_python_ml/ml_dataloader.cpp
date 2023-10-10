#include <api_python_ml.h>
#include <ml_dataloader.h>
#include <algorithm>
#include <random>
#include <numeric>
#include <array/arrayMgr.h>

#include <op/update/update_plan.h>
#include <op/between/between_plan.h>
#include <api_cpp/cpp_operators.h>
#include <api_cpp/cpp_query.h>

namespace msdb
{
namespace api_python_ml
{
ml_dataloader::ml_dataloader(const core::arrayId xArrId, const core::arrayId yArrId, const size_t itemBufferSize,
	const size_t batchSize, const size_t xBufferSize, const size_t yBufferSize)
	: _xArrId(xArrId), _yArrId(yArrId), _xAttrId(0), _yAttrId(0), _itemBufferSize(itemBufferSize),
	_batchSize(batchSize), _xBufferSize(xBufferSize), _yBufferSize(yBufferSize), _numItems(0),
	_isPreloadActive(false), _lastGetIdx(0)
{
	auto xArrDesc = core::arrayMgr::instance()->getArrayDesc(xArrId);
	auto yArrDesc = core::arrayMgr::instance()->getArrayDesc(yArrId);

	if (xArrDesc == nullptr || yArrDesc == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << ": Invalid array id";
		if (!xArrDesc)
		{
			BOOST_LOG_TRIVIAL(error) << "Can't find the array with id: " << xArrId;
		}
		if (!yArrDesc)
		{
			BOOST_LOG_TRIVIAL(error) << "Can't find the array with id: " << yArrId;
		}
		_numItems = 0;
		return;
	}

	if (xArrDesc->getDimDescs()->size() < 2)
	{
		BOOST_LOG_TRIVIAL(error) << "The dimension of xArray should be LE than two.";
		return;
	}
	if (yArrDesc->getDimDescs()->size() < 2)
	{
		BOOST_LOG_TRIVIAL(error) << "The dimension of yArray should be LE than two.";
		return;
	}

	auto xIdDim = xArrDesc->getDimDescs()->at(0);
	auto yIdDim = yArrDesc->getDimDescs()->at(0);

	if (xIdDim->getLength() != yIdDim->getLength())
	{
		BOOST_LOG_TRIVIAL(error) << "Two array (x: " << xArrId << ", y: " << yArrId << ") have different number of items.\n"
			<< "[x: " << xIdDim << ", y: " << yIdDim << "]";
		return;
 	}

	_numItems = xIdDim->getLength();

	// Init vecSequence
	_vecSeq.resize(_numItems);
	std::iota(_vecSeq.begin(), _vecSeq.end(), 0);
}

ml_dataloader::~ml_dataloader()
{
	_vecSeq.clear();

	while (_preloadItemQueue.size())
	{
		_preloadItemQueue.pop();
	}
}

bool ml_dataloader::suffleSequence(const uint64_t seed)
{
	std::shuffle(_vecSeq.begin(), _vecSeq.end(), std::default_random_engine(seed));
	return true;
}
void ml_dataloader::activePreloaderThread(const bool doShuffle)
{
	initLastGetIdx();

	std::lock_guard lock(_preloaderMutex);
	if (!_isPreloadActive)
	{
		// do threadPreloader();
		BOOST_LOG_TRIVIAL(info) << "Activate preloader thread.";
		_isPreloadActive = true;

		if (doShuffle)
		{
			suffleSequence();
		}

		//_def_token = std::stop_token();
		//_tPreloader = std::jthread(&ml_dataloader::threadPreloader, this, std::stop_token());
		//_tPreloader = std::jthread(f, 5);
		_tPreloader = std::make_shared<std::jthread>(std::bind_front(&ml_dataloader::threadPreloader, this));
	}
	else
	{
		BOOST_LOG_TRIVIAL(warning) << "Preloader is already activated.";
	}
}

// WARNING:: The function returns a nullptr if preloader thread ended.
std::shared_ptr<ml_item> ml_dataloader::getNextItemBatch()
{
	//////////////////////////////
	// Without Thread
	if (!_isPreloadActive)
	{
		if(isEnd())
		{
			return std::make_shared<ml_item>();
		}
		auto out = doGetItemBatch(_lastGetIdx);
		{
			std::lock_guard lock(_preloaderMutex);
			++_lastGetIdx;
		}

		return out;
	}
	//////////////////////////////

	//////////////////////////////
	// With Thread
	if (isEnd())
	{
		return std::make_shared<ml_item>();
	}
	{
		std::lock_guard lock(_preloaderMutex);
		++_lastGetIdx;
	}

	if (_preloadItemQueue.size() == 0)
	{
		if (!_readyToServeFlag.wait_and_clear())
		{
			return nullptr;
		}
	}

	{
		std::lock_guard lock(_preloaderMutex);
		if (_preloadItemQueue.size() == 0)
		{
			BOOST_LOG_TRIVIAL(error) << "Empty preload Item Queue.";
			return std::make_shared<ml_item>();
		}

		auto out = _preloadItemQueue.front();
		_preloadItemQueue.pop();
		_loadNextFlag.set();
		return out;
	}
	//////////////////////////////
}
bool ml_dataloader::close()
{
	if (_isPreloadActive)
	{
		if (_tPreloader->joinable())
		{
			BOOST_LOG_TRIVIAL(info) << "Request stop to preloader thread.";

			try
			{
				BOOST_LOG_TRIVIAL(info) << "Before request_stop to 'ml_dataloader::_tPreloader'.";
				_tPreloader->request_stop();
				BOOST_LOG_TRIVIAL(info) << "Before joining 'ml_dataloader::_tPreloader'.";
				_tPreloader->join();
				BOOST_LOG_TRIVIAL(info) << "After joining 'ml_dataloader::_tPreloader'.";
			}
			catch (...)
			{
				BOOST_LOG_TRIVIAL(error) << "'ml_dataloader::_tPreloader' thread join error.";
			}
		}
		else
		{
			BOOST_LOG_TRIVIAL(warning) << "Cannot join 'ml_dataloader::_tPreloader' thread";
		}
	}

	_loadNextFlag.clear();
	_readyToServeFlag.clear();

	while (!_preloadItemQueue.empty())
	{
		_preloadItemQueue.pop();
	}

	return true;
}
void ml_dataloader::threadPreloader(std::stop_token stoken)
{
	// Wiat next flag and load next items
	uint64_t idxEnd = _numItems / _batchSize;
	uint64_t idxNext = 0;

	while (idxNext < idxEnd)
	{
		{
			std::lock_guard lock(_preloaderMutex);
			BOOST_LOG_TRIVIAL(debug) << "Item Buffer Size: " << _itemBufferSize << ", Item Queue Size: " << _preloadItemQueue.size();
			if (_itemBufferSize > _preloadItemQueue.size())
			{
				BOOST_LOG_TRIVIAL(debug) << "Set load next flag";
				_loadNextFlag.set();
			}
		}

		while (!stoken.stop_requested())
		{
			if (_loadNextFlag.wait_and_clear(threadWaitTimeout))
			{
				break;
			}
			BOOST_LOG_TRIVIAL(debug) << "preloader::wait() - TIMEOUT";
		}

		if(stoken.stop_requested())
		{
			BOOST_LOG_TRIVIAL(info) << "Preloader thread terminated with stop_requested.";
			return;
		}

		BOOST_LOG_TRIVIAL(debug) << "Try to load item: " << idxNext;
		auto item = doGetItemBatch(idxNext++);
		{
			std::lock_guard lock(_preloaderMutex);
			_preloadItemQueue.push(item);
			_readyToServeFlag.set();
		}
	}

}
std::shared_ptr<ml_item> ml_dataloader::doGetItemBatch(const unsigned long long batch_idx)
{
	std::vector<uint64_t> itemId;
	itemId.insert(itemId.begin(), _vecSeq.begin() + (_batchSize * batch_idx), _vecSeq.begin() + (_batchSize * (batch_idx + 1)));

	auto ret = std::make_shared<ml_item>(
		_xBufferSize ? _xBufferSize * _batchSize : _batchSize, 
		_yBufferSize ? _yBufferSize * _batchSize : _batchSize);

	if (ret->x() == nullptr || ret->y() == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "Memory allocation failed in pythonML>getItem.\n"
			<< "(x memory size: " << _xBufferSize * _batchSize << ", y memory size: " << _yBufferSize * _batchSize << ")";
		return std::make_shared<ml_item>();
	}

	//////////////////////////////
	// Make X Array
	for (int i = 0; i < _batchSize; ++i)
	{
		auto idx = itemId[i];

		if (!executeGetItemQuery((char*)ret->x() + _xBufferSize * i, _xArrId, _xAttrId, idx, _xBufferSize))
		{
			BOOST_LOG_TRIVIAL(error) << "Empty result from executeGetItemQuery(). Return empty result";
			return std::make_shared<ml_item>();
		}
	}

	//////////////////////////////
	// Make Y Array
	for (int i = 0; i < _batchSize; ++i)
	{
		auto idx = itemId[i];

		if (!executeGetItemQuery((char*)ret->y() + _yBufferSize * i, _yArrId, _yAttrId, idx, _yBufferSize))
		{
			BOOST_LOG_TRIVIAL(error) << "Empty result from executeGetItemQuery(). Return empty result";
			return std::make_shared<ml_item>();
		}
	}
	
	//////////////////////////////
	// Return Ret
	return ret;
}

bool ml_dataloader::executeGetItemQuery(void* out, const core::arrayId arrId, const core::attributeId attrId, 
	const uint64_t idx, const size_t bufferSize)
{
	msdb::Context ctx;
	auto loadAfl = msdb::Between(
		msdb::Load(msdb::Array(ctx, arrId)),
		msdb::Domain(msdb::core::coordinates(1, idx), msdb::core::coordinates(1, idx + 1)));
	auto qry = msdb::Query(loadAfl);
	qry.setRawResultOut();

	auto ra = qry.execute();
	if (qry.getStatus() != msdb::Query::Status::COMPLETE)
	{
		BOOST_LOG_TRIVIAL(warning) << "PythonML::Load query failed: ";
		return false;
	}

	auto outArr = ra.getRawResult();
	auto cit = outArr->getChunkIterator(_xAttrId);

	// TODO::Batch size, return multiple chunks
	cit->moveToSeqPos(idx);
	if (!cit->isExist())
	{
		BOOST_LOG_TRIVIAL(warning) << "PythonML::No chunk: " << idx;
		return false;
	}

	auto bf = (*cit)->getBuffer();
	if (!bf->size())
	{
		BOOST_LOG_TRIVIAL(warning) << "PythonML::Empty chunk buffer: " << idx;
		return false;
	}

	// BufferSize = 0 : INFINITE
	if (bufferSize && bf->size() != bufferSize)
	{
		BOOST_LOG_TRIVIAL(warning) << "Buffer size is different. (curr: " << bf->size() << ", expect: " << bufferSize << ")";
	}

	memcpy((char*)out, bf->getReadData(), std::min(bf->size(), bufferSize));
	
	outArr->flush();

	return true;
}
}		// api_python_ml
}		// msdb