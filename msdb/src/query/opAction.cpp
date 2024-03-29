﻿#include <pch.h>
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

	BOOST_LOG_TRIVIAL(info) << "==============================";
	for (auto attr : *arr->getDesc()->attrDescs_)
	{
		auto cit = arr->getChunkIterator(attr->id_, iterateMode::EXIST);
		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				++numChunks;
				serialSize += (*cit)->getSerializedSize();
				auto bit = (*cit)->getBlockIterator(iterateMode::EXIST);
				while (!bit->isEnd())
				{
					if (bit->isExist())
					{
						++numBlocks;
					}

					++(*bit);
				}
			}

			++(*cit);
		}

		BOOST_LOG_TRIVIAL(info) << "------------------------------" << std::endl <<
			"Attribute: " << attr->id_ << std::endl << "- Num chunks: " << numChunks << std::endl << "- Num blocks: " << numBlocks << std::endl << "- Serialized size: " << serialSize << " bytes" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "------------------------------";
	}
	BOOST_LOG_TRIVIAL(info) << "==============================";
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
void opAction::threadCreate()
{
	this->threadCreate(this->getDefaultThreadNum());
}
void opAction::threadStop()
{
	/*
	 * This will stop the io_service_ processing loop.
	 * Any tasks you add behind this point will not execute
	 */
	this->io_service_->stop();
}
void opAction::threadJoin()
{
	/*
	 * Will wait till all the threads in the thread pool are finished 
	 * with their assigned tasks and 'join' them.
	 */
	this->threadpool_.join_all();
}
const int opAction::getDefaultThreadNum()
{
	////////////////////////////////////////
	// TODO:: Change Default Thread Num for opAction
	return 6;
	////////////////////////////////////////
}
}		// core
}		// msdb