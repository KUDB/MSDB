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
	static const int INF = 0;

private:
	bufferMgr();
	~bufferMgr();

public:
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

		friend std::ostream& operator<<(std::ostream& out, const bufferMgr::chunkKey& val)
		{
			out << "[arrId: " << val.arrId << ", attrId: " << val.attrId << ", chunkId: " << val.cid << "]";
			return out;
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
		if (isExist(key))
		{
			auto buf = _chunkBuffer.at(key);
			_refreshBufferState(key, buf);
			return buf;
		}
		return nullptr;
	}

	//////////////////////////////
	// cacheChunkBuffer
	//////////////////////////////
	// Thread safe
	inline bool cacheChunkBuffer(const arrayId arrId, const attributeId attrId, const chunkId cid, pBuffer buf)
	{
		std::lock_guard<std::recursive_mutex> lk(this->getLock());
		chunkKey key(arrId, attrId, cid);

		// Erase the existing buffer state of input buffer.
		if (isExist(key))
		{
			_eraseChunkBufferState(key);
		}
		
		// Comparison the buffer limit and the input buffer size.
		if (_bufferLimit && _bufferLimit < buf->size())
		{
			// Can't store the input buffer.
			BOOST_LOG_TRIVIAL(warning) << "Buffer Limit is too small.\n"
				<< "(Limit: " << _bufferLimit << ", request: " << buf->size() << ")";
			return false;
		}

		// Make space to store the input buffer.
		if (_bufferLimit && _curBufferSize + buf->size() > _bufferLimit)
		{
			_reduceCurBufferSizeTo(_bufferLimit - buf->size());
		}
		
		_insertChunkBuffer(key, buf);

		return true;
	}

	inline bool flushChunkBuffer(const arrayId arrId, const attributeId attrId, const chunkId cid)
	{
		std::lock_guard<std::recursive_mutex> lk(this->getLock());
		chunkKey key(arrId, attrId, cid);

		return _eraseChunkBuffer(key);
	}

	inline bool flushAll()
	{
		return _reduceCurBufferSizeTo(0);
	}

	//////////////////////////////
	// setBufferLimit
	//////////////////////////////
	// Thread safe
	// 
	// Set buffer size in bytes.
	bool setBufferLimit(const bufferSize bytes)
	{
		std::lock_guard<std::recursive_mutex> lk(this->getLock());
		// INFINATE
		if (bytes == 0)
		{
			this->_bufferLimit = 0;
			return true;
		}
		if (_reduceCurBufferSizeTo(bytes))
		{
			this->_bufferLimit = bytes;
			return true;
		}
		return false;
	}

	//////////////////////////////
	// ::Not thread safe::
	inline bool isExist(const arrayId arrId, const attributeId attrId, const chunkId cid) const
	{
		return isExist({arrId, attrId, cid});
	}
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
	inline bufferSize getCurrentBufferSize() const
	{
		return _curBufferSize;
	}

private:
	//////////////////////////////////////////////////
	// DO NOT CALL THESE FUNCTIONS WITHOUT LOCK
	//////////////////////////////////////////////////
	// ::Not thread safe::
	// The functions have no lock for buffers.
	//
	// Erase buffer state first.
	bool _insertChunkBuffer(const chunkKey& key, pBuffer buf);
	bool _eraseChunkBuffer(const chunkKey& key);
	bool _eraseChunkBufferState(const chunkKey& key);
	bool _eraseChunkBufferState(const chunkKey& key, const pBuffer buf);
	bool _insertChunkBufferState(const chunkKey& key);
	bool _insertChunkBufferState(const chunkKey& key, const pBuffer buf);
	bool _reduceCurBufferSizeTo(const bufferSize bytes);
	bool _refreshBufferState(const chunkKey& key, const pBuffer buf);
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
	bufferSize _curBufferSize;			// Default: 0 
	bufferSize _bufferLimit;			// Default: Set as 0 (INF)

	protected:
		friend singleton<bufferMgr>;
};
}		// core
}		// msdb

#endif	// _MSDB_BUFFERMGR_H_