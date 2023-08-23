#pragma once
#ifndef _MSDB_BUFFERMGR_H_
#define _MSDB_BUFFERMGR_H_

#include <pch.h>
#include <util/singleton.h>
#include <array/buffer.h>
#include <array/arrayId.h>
#include <array/chunkId.h>
#include <array/attributeId.h>
#include <queue>
#include <map>
#include <chrono>
#include <deque>
#include <mutex>

namespace msdb
{
namespace core
{
class bufferMgr : public singleton<bufferMgr>
{
private:
	using time_point = std::chrono::system_clock::time_point;

private:
	bufferMgr();
	~bufferMgr();

private:
	class chunkKey
	{
	public:
		arrayId arrId;
		attributeId attrId;
		chunkId cid;

	public:
		chunkKey(const arrayId arrId, const attributeId attrId, const chunkId cid)
			: arrId(arrId), attrId(attrId), cid(cid)
		{}

	public:
		bool operator <(const chunkKey& rhs) const
		{
			return (arrId < rhs.arrId) ||
				(arrId == rhs.arrId && attrId < rhs.attrId) ||
				(arrId == rhs.arrId && attrId == rhs.attrId && cid < rhs.cid);
		}

		bool operator ==(const chunkKey& rhs) const
		{
			return arrId == rhs.arrId && attrId == rhs.attrId && cid == rhs.cid;
		}
	};

	class chunkBufferState
	{
	public:
		time_point _lastTs;
		chunkKey _key;
		bufferSize _size;

	public:
		chunkBufferState(const time_point timestamp, const chunkKey& chunkKey, const bufferSize size)
			: _lastTs(timestamp), _key(chunkKey), _size(size)
		{}

		bool operator <(const chunkBufferState& rhs) const
		{
			return _lastTs < rhs._lastTs;
		}

		bool operator == (const chunkBufferState& rhs) const
		{
			return _lastTs == rhs._lastTs;
		}
	};

public:
	//////////////////////////////
	// getChunkBuffer
	//////////////////////////////
	// Thread safe
	// 
	// Return:
	// - If exist: pBuffer
	// - Not exist: nullptr
	inline pBuffer getChunkBuffer(const arrayId arrId, const attributeId attrId, const chunkId cid)
	{
		std::lock_guard<std::recursive_mutex> lk(this->getLock());
		chunkKey key(arrId, attrId, cid);
		if (this->isExist(key))
		{
			auto out = _chunkBuffer.at(key);
			removeChunkBufferState(key);
			_bufferQueue.push_back(chunkBufferState(now(), key, out->size()));

			return out;
		}
		return nullptr;
	}

	//////////////////////////////
	// setChunkBuffer
	//////////////////////////////
	// Thread safe
	inline pBuffer setChunkBuffer(const arrayId arrId, const attributeId attrId, const chunkId cid, pBuffer buf)
	{
		std::lock_guard<std::recursive_mutex> lk(this->getLock());
		chunkKey key(arrId, attrId, cid);
		if (this->isExist(key))
		{
			removeChunkBufferState(key);
		}
		_chunkBuffer[key] = buf;
		_bufferQueue.push_back(chunkBufferState(now(), key, buf->size()));
	}

	//////////////////////////////
	// ::Not thread safe::
	inline bool isExist(const chunkKey& key) const
	{
		return this->_chunkBuffer.count(key);
	}
	//////////////////////////////
	// ::Not thread safe::
	inline bufferSize getBufferLimit() const
	{
		return this->_bufferLimit;
	}
	//////////////////////////////
	// setBufferLimit
	//////////////////////////////
	// Thread safe
	// 
	// Set buffer size in bytes.
	void setBufferLimit(const bufferSize bytes)
	{
		std::lock_guard<std::recursive_mutex> lk(this->getLock());
		// INFINATE
		if (bytes == 0)
		{
			this->_bufferLimit = 0;
			return;
		}

		while (_curBufferSize > bytes && _bufferQueue.size())
		{
			auto& front = _bufferQueue.front();
			flushChunkBuffer(front._key);
			_bufferQueue.pop_front();
		}

		this->_bufferLimit = bytes;
	}

private:
	//////////////////////////////////////////////////
	// DO NOT CALL THESE FUNCTIONS WITHOUT LOCK
	//////////////////////////////////////////////////
	// ::Not thread safe::
	// The functions have no lock for buffers.
	//
	bool cacheChunkBuffer(const chunkKey& key, pBuffer buf);
	bool flushChunkBuffer(const chunkKey& key);
	bool removeChunkBufferState(const chunkKey& key);
	//////////////////////////////////////////////////

private:
	inline time_point now() const
	{
		return std::chrono::system_clock::now();
	}
	static std::recursive_mutex& getLock();

private:
	std::deque<chunkBufferState> _bufferQueue;
	std::map<chunkKey, pBuffer> _chunkBuffer;
	bufferSize _curBufferSize;
	bufferSize _bufferLimit;
};
}		// core
}		// msdb

#endif	// _MSDB_BUFFERMGR_H_