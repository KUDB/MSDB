#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
opAction::opAction()
	: threadExist_(false), threadNums_(0)
{
}

opAction::~opAction()
{
}

const pArrayDesc msdb::core::opAction::getArrayDesc()
{
	return this->aDesc_;
}

void opAction::setParams(const parameters& params)
{
	this->params_ = params;
}

void opAction::setArrayDesc(pArrayDesc aDesc)
{
	this->aDesc_ = aDesc;
}
void opAction::setPlanInBitmap(pBitmap planBitmap)
{
	this->planInBitmap_ = planBitmap;
}
void opAction::setPlanOutBitmap(pBitmap planBitmap)
{
	this->planOutBitmap_ = planBitmap;
}
cpBitmap opAction::getPlanInChunkBitmap() const
{
	return this->planInBitmap_;
}
cpBitmap opAction::getPlanOutChunkBitmap() const
{
	return this->planOutBitmap_;
}
void opAction::getArrayStatus(pArray arr)
{
	size_t numChunks = 0, numBlocks = 0, serialSize = 0;
	std::stringstream ss;

	auto cit = arr->getChunkIterator();
	while(!cit->isEnd())
	{
		if(cit->isExist())
		{
			++numChunks;
			serialSize += (*cit)->getSerializedSize();
			ss << cit->seqPos() << "/" << (*cit)->getSerializedSize() << ", ";
			auto bit = (*cit)->getBlockIterator();
			while(!bit->isEnd())
			{
				
				if(bit->isExist())
				{
					++numBlocks;
				}

				++(*bit);
			}
		}

		++(*cit);
	}

	//BOOST_LOG_TRIVIAL(info) << "- Chunks: " << ss.str();
	BOOST_LOG_TRIVIAL(info) << "- Num chunks: " << numChunks;
	BOOST_LOG_TRIVIAL(info) << "- Num blocks: " << numBlocks;
	BOOST_LOG_TRIVIAL(info) << "- Serialized size: " << serialSize;
}

cpBitmap opAction::getPlanBlockBitmap(chunkId cid) const
{
	if(this->planInBitmap_->isTree())
	{
		return std::static_pointer_cast<bitmapTree>(this->planInBitmap_)->getChild(cid);
	}
	return nullptr;
}
void opAction::threadCreate(size_t threadNums)
{
	if(this->threadNums_ >= threadNums)
	{
		return;
	}

	this->io_service_ = std::make_shared<boost::asio::io_service>();
	this->work_ = std::make_shared<boost::asio::io_service::work>(*(this->io_service_));

	for(size_t i = 0; i < threadNums - this->threadNums_; ++i)
	{
		this->threadpool_.create_thread(boost::bind(&boost::asio::io_service::run, this->io_service_));
	}
	this->threadExist_ = true;
	this->threadNums_ = threadNums;
}
void opAction::threadStop()
{
	this->io_service_->stop();
}
void opAction::threadJoin()
{
	this->threadpool_.join_all();
}
}		// core
}		// msdb