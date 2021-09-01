#include <pch.h>
#include <array/blockDesc.h>

namespace msdb
{
namespace core
{
const blockSize INVALID_BLOCK_SIZE = static_cast<blockSize>(~0);
const blockId INVALID_BLOCK_ID = static_cast<blockId>(~0);

blockDesc::blockDesc()
{
	//initBlockCoor();
	// TODO::initialization
}

blockDesc::blockDesc(const blockId id, const eleType eType,
					 const dimension& dims, 
					 const coor& sp, const coor& ep, 
					 const blockSize mSize, const blockSize mOffset)
	: id_(id), eType_(eType), 
	dims_(dims), sp_(sp), ep_(ep), isp_(dims.size()), iep_(dims),
	mSize_(mSize), mOffset_(mOffset),
	blockCoor_(dims.size())
{
	initBlockCoor();
}
coor blockDesc::getSp()
{
	return this->sp_;
}
coor blockDesc::getEp()
{
	return this->ep_;
}
coor blockDesc::getIsp()
{
	return this->isp_;
}
coor blockDesc::getIep()
{
	return this->iep_;
}
void blockDesc::setSp(const coor& coor)
{
	this->sp_ = coor;
	//this->setIsp(getOutsideCoor(this->sp_, this->isp_));
}
void blockDesc::setEp(const coor& coor)
{
	this->ep_ = coor;
	//this->setIsp(getInsideCoor());
}
void blockDesc::setIsp(const coor& coor)
{
	this->isp_ = coor;
}
void blockDesc::setIep(const coor& coor)
{
	this->iep_ = coor;
}
void blockDesc::initBlockCoor()
{
	for (dimensionId d = 0; d < this->dims_.size(); ++d)
	{
		// this->ep_[d] - this->sp_[d] = block width
		this->blockCoor_[d] = this->sp_[d] / (this->ep_[d] - this->sp_[d]);
	}
}
}		// core
}		// msdb