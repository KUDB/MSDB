#include <pch.h>
#include <array/bufferMgr.h>

namespace msdb
{
namespace core
{
bufferMgr::bufferMgr()
{
}
bufferMgr::~bufferMgr()
{
}
bool bufferMgr::cacheChunkBuffer(const chunkKey& key, pBuffer buf)
{
	_chunkBuffer[key] = buf;
	_bufferQueue.push_back(
		chunkBufferState(now(), key, buf->size()));
	_curBufferSize += buf->size();

	return true;
}
bool bufferMgr::flushChunkBuffer(const chunkKey& key)
{
	auto it = _chunkBuffer.find(key);
	if (it != _chunkBuffer.end())
	{
		_curBufferSize -= (*it).second->size();
		(*it).second = nullptr;
		_chunkBuffer.erase(it);
		return true;
	}
	return false;
}
bool bufferMgr::removeChunkBufferState(const chunkKey& key)
{
	auto it = std::find_if(
		this->_bufferQueue.begin(), _bufferQueue.end(),
		[&key](const chunkBufferState& s)
		{return s._key.arrId == key.arrId && s._key.attrId == key.attrId && s._key.cid == key.cid; });
	if (it != _bufferQueue.end())
	{
		_bufferQueue.erase(it);
		return true;
	}
	else
	{
		BOOST_LOG_TRIVIAL(warning)
			<< "BufferQueue doesn't have a chunk buffer where arrId=" << key.arrId << ", attrId=" << key.attrId << ", and chunkId=" << key.cid << ".";
		return false;
	}
}
std::recursive_mutex& bufferMgr::getLock()
{
	static std::recursive_mutex _lock;
	return _lock;
}
}		// core
}		// msdb