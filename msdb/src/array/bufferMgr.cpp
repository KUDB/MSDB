#include <pch.h>
#include <array/bufferMgr.h>

namespace msdb
{
namespace core
{
bufferMgr::bufferMgr()
	:_bufferLimit(0), _curBufferSize(0)
{
}
bufferMgr::~bufferMgr()
{
}
bool bufferMgr::_insertChunkBuffer(const chunkKey& key, pBuffer buf)
{
	_chunkBuffer[key] = buf;
	_insertChunkBufferState(key, buf);

	return true;
}
bool bufferMgr::_eraseChunkBuffer(const chunkKey& key)
{
	auto it = _chunkBuffer.find(key);
	if (it != _chunkBuffer.end())
	{
		_eraseChunkBufferState(key, (*it).second);

		(*it).second = nullptr;
		_chunkBuffer.erase(it);

		return true;
	}
	else
	{
		_eraseChunkBufferState(key, nullptr);
	}
	return false;
}
bool bufferMgr::_eraseChunkBufferState(const chunkKey& key)
{
	auto buf = _chunkBuffer[key];
	if (buf == nullptr)
	{
		BOOST_LOG_TRIVIAL(warning)
			<< "There is no ChunkBuffer " << key << ".";
	}

	return _eraseChunkBufferState(key, buf);
}

bool  bufferMgr::_eraseChunkBufferState(const chunkKey& key, const pBuffer buf)
{
	auto it = std::find_if(
		this->_bufferQueue.begin(), _bufferQueue.end(),
		[&key](const chunkBufferState& s)
		{return s._key.arrId == key.arrId && s._key.attrId == key.attrId && s._key.cid == key.cid; });

	if (it != _bufferQueue.end())
	{
		if (buf == nullptr)
		{
			_bufferQueue.erase(it);
			return true;
		}
		_curBufferSize -= buf->size();
		_bufferQueue.erase(it);

		if (_curBufferSize < 0)
		{
			BOOST_LOG_TRIVIAL(error) << "BufferMgr Error: Current Buffer Size is below zero.";
			flushAll();
			_curBufferSize = 0;
		}

		return true;
	}
	else
	{
		BOOST_LOG_TRIVIAL(warning)
			<< "BufferQueue doesn't have a chunk buffer where " << key << ".";
		return false;
	}
}

bool bufferMgr::_insertChunkBufferState(const chunkKey& key)
{
	auto buf = _chunkBuffer[key];

	if (buf == nullptr)
	{
		BOOST_LOG_TRIVIAL(warning)
			<< "BufferQueue doesn't have a chunk buffer where " << key << ".";
		return false;
	}

	return _insertChunkBufferState(key, buf);
}
bool bufferMgr::_insertChunkBufferState(const chunkKey& key, const pBuffer buf)
{
	_bufferQueue.push_back(chunkBufferState(now(), key, buf->size()));
	_curBufferSize += buf->size();

	return true;
}
bool bufferMgr::_reduceCurBufferSizeTo(const bufferSize bytes)
{
	if (bytes < 0)
	{
		BOOST_LOG_TRIVIAL(warning) << "BufferMgr::" << __FUNCTION__ << " got negative bytes (" << bytes << ").";
		return false;
	}
	while (_curBufferSize > bytes && _bufferQueue.size())
	{
		auto& front = _bufferQueue.front();
		_eraseChunkBuffer(front._key);
	}

	return true;
}
bool bufferMgr::_refreshBufferState(const chunkKey& key, const pBuffer buf)
{
	_eraseChunkBufferState(key);
	_insertChunkBufferState(key);
	return true;
}
std::recursive_mutex& bufferMgr::getLock()
{
	static std::recursive_mutex _lock;
	return _lock;
}
}		// core
}		// msdb