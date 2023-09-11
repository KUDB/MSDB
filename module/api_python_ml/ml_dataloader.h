#pragma once
#ifndef _MSDB_PYTHON_ML_DATALOADER_
#define _MSDB_PYTHON_ML_DATALOADER_

#include <api_python_ml.h>
#include <array/arrayId.h>
#include <array/attributeId.h>
#include <vector>
#include <chrono>
#include <util/threadUtil.h>

namespace msdb
{
namespace api_python_ml
{
class ml_item
{
public:
	ml_item()
		: _x(nullptr), _y(nullptr)	{}

	ml_item(size_t xSize, size_t ySize)
	{
		if (xSize == 0 || ySize == 0)
		{
			BOOST_LOG_TRIVIAL(error) << "Cannot construct ml_item instance: xSize and ySize should be larger than zero.";
			return;
		}

		_x = new char[xSize];
		_y = new char[ySize];
	}

	~ml_item()
	{
		clear();
	}

	bool assign(size_t xSize, size_t ySize)
	{
		if (xSize == 0 || ySize == 0)
		{
			BOOST_LOG_TRIVIAL(error) << "Cannot construct ml_item instance: xSize and ySize should be larger than zero.";
			return false;
		}

		clear();

		_x = new char[xSize];
		_y = new char[ySize];

		return true;
	}

	void clear()
	{
		if (_x)
			delete[] _x;

		if (_y)
			delete[] _y;
	}

	inline void* x() { return _x; }
	inline void* y() { return _y; }

private:
	void* _x;
	void* _y;
};

class ml_dataloader
{
public:
	ml_dataloader(const core::arrayId xArrId, const core::arrayId yArrId, 
		const size_t batchSize, const size_t xBufferSize, const size_t yBufferSize);
	~ml_dataloader();

public:
	bool suffleSequence(const uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count());
	void activePreloaderThread();
	std::shared_ptr<ml_item> getItem(const unsigned long long idx);

private:
	void threadPreloader(const bool doShuffle = false);
	std::shared_ptr<ml_item> doGetItem(const unsigned long long batch_idx);
	bool executeGetItemQuery(void* out, const core::arrayId arrId, const core::attributeId attrId, 
		const uint64_t idx, const size_t bufferSize);

private:
	std::vector<uint64_t> _vecSeq;

	core::arrayId _xArrId;
	core::arrayId _yArrId;

	core::attributeId _xAttrId;
	core::attributeId _yAttrId;

	size_t _numItems;
	size_t _batchSize;
	size_t _xBufferSize;
	size_t _yBufferSize;

	std::mutex _preloaderMutex;
	bool _isPreloadActive;
	core::ThreadFlag _loadNextFlag;
	std::vector<core::ThreadFlag> _itemFlags;
};
}		// api_python
}		// msdb
#endif	// _MSDB_PYTHON_ML_DATALOADER_