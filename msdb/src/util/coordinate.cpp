#include <pch.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
////////////////////////////////////////
// coordinates
//
coordinates::coordinates()
	: dSize_(0), coor_(nullptr)
{
#ifndef NDEBUG
	this->x_ = 0;
	this->y_ = 0;
#endif
}

coordinates::coordinates(const size_type dSize)
	: dSize_(dSize)
{
	this->coor_ = new dim_type[this->dSize_]();
#ifndef NDEBUG
	this->setXY();
#endif
}

coordinates::coordinates(const size_type dSize, dim_const_pointer coor)
	: dSize_(dSize)
{
	this->coor_ = new dim_type[this->dSize_];
	memcpy(this->coor_, coor, this->dSize_ * sizeof(dim_type));
#ifndef NDEBUG
	this->setXY();
#endif
}

coordinates::coordinates(const size_type dSize, const dim_type coor)
	: dSize_(dSize)
{
	this->coor_ = new dim_type[this->dSize_];
	for (size_type d = 0; d < this->dSize_; ++d)
	{
		this->coor_[d] = coor;
	}
#ifndef NDEBUG
	this->setXY();
#endif
}

coordinates::coordinates(const std::vector<dim_type>& coorVec)
	: dSize_(coorVec.size())
{
	this->coor_ = new dim_type[coorVec.size()];
	memcpy(this->coor_, coorVec.data(), coorVec.size() * sizeof(dim_type));
#ifndef NDEBUG
	this->setXY();
#endif
}

coordinates::coordinates(const coordinates& src)
	: coordinates(src.dSize_)
{
	memcpy(this->coor_, src.coor_, src.dSize_ * sizeof(dim_type));
#ifndef NDEBUG
	this->setXY();
#endif
}

coordinates::coordinates(coordinates&& src) noexcept
	: coordinates()
{
	swap(*this, src);
}

coordinates::coordinates(std::initializer_list<dim_type> lst)
{
	this->dSize_ = lst.size();
	this->coor_ = new dim_type[this->dSize_];

	auto it = lst.begin();
	for (size_type d = 0; d < this->dSize_; d++, it++)
	{
		this->coor_[d] = *it;
	}
#ifndef NDEBUG
	this->setXY();
#endif
}

//coordinates::self_type& coordinates::operator=(self_type& rhs)
//{
//	this->dSize_ = rhs.dSize_;
//	this->coor_ = new dim_type[this->dSize_];
//	memcpy(this->coor_, rhs.coor_, rhs.dSize_ * sizeof(dim_type));
//#ifndef NDEBUG
//	if (this->dSize_ >= 2)
//	{
//		this->x_ = this->coor_ + 1;
//		this->y_ = this->coor_;
//	}
//#endif
//	return *this;
//}

coordinates::~coordinates()
{
	this->dSize_ = 0;

#ifndef NDEBUG
	this->x_ = this->y_ = 0;
#endif

	if (this->coor_)
	{
		delete[] this->coor_;
	}
}

 std::string coordinates::toString() const
{
	std::ostringstream ss;

	ss << "(";
	for (dimensionId d = this->dSize_ - 1; d != (dimensionId)-1; --d)
	{
		if (d != this->dSize_ - 1)
		{
			ss << ", ";
		}
		ss << this->coor_[d];
	}

	ss << ")";

	return ss.str();
}

 // ***************************
 // Assign
 coordinates& coordinates::operator=(const coordinates& src)
 {
	 if (this == &src)
	 {
		 return *this;
	 }

	 coordinates temp(src);
	 swap(*this, temp);
	 return *this;
 }

 coordinates& coordinates::operator=(coordinates&& src) noexcept
 {
	 coordinates temp(std::move(src));
	 swap(*this, temp);
	 return *this;
 }
#ifndef NDEBUG
 void coordinates::setXY()
 {
	 if (this->dSize_ >= 2)
	 {
		 this->x_ = this->coor_ + 1;
		 this->y_ = this->coor_;
	 }
	 else
	 {
		 this->x_ = 0;
		 this->y_ = 0;
	 }
 }
#endif

 void swap(coordinates& first, coordinates& second) noexcept
 {
	 using std::swap;

	 swap(first.dSize_, second.dSize_);
	 swap(first.coor_, second.coor_);

#ifndef NDEBUG
	 swap(first.x_, second.x_);
	 swap(first.y_, second.y_);
#endif
 }

 coordinates operator+ (const coordinates& left, const coordinates& right)
{
	coordinates output(left);
	output += right;
	return output;
}


coordinates operator+ (const coordinates& left, const int right)
{
	coordinates output(left);
	output += right;
	return output;
}


coordinates operator- (const coordinates& left, const coordinates& right)
{
	coordinates output(left);
	output -= right;
	return output;
}


coordinates operator- (const coordinates& left, const int right)
{
	coordinates output(left);
	output -= right;
	return output;
}


coordinates operator* (const coordinates& left, const coordinates& right)
{
	coordinates output(left);
	output *= right;
	return output;
}


coordinates operator* (const coordinates& left, const int right)
{
	coordinates output(left);
	output *= right;
	return output;
}


coordinates operator/ (const coordinates& left, const coordinates& right)
{
	coordinates output(left);
	output /= right;
	return output;
}


coordinates operator/ (const coordinates& left, const int right)
{
	coordinates output(left);
	output /= right;
	return output;
}


coordinates operator% (const coordinates& left, const coordinates& right)
{
	coordinates output(left);
	output %= right;
	return output;
}


coordinates operator% (const coordinates& left, const int right)
{
	coordinates output(left);
	output %= right;
	return output;
}

////////////////////////////////////////
// range class
////////////////////////////////////////
range::range()
	: dSize_(0), sP_(), eP_()
{
}

range::range(const size_type dSize)
	: dSize_(dSize), sP_(dSize), eP_(dSize)
{
}

range::range(const std::vector<dim_type>& sP, const std::vector<dim_type>& eP)
	: dSize_(sP.size()), sP_(sP), eP_(eP)
{
	assert(sP_.size() == eP_.size());
}

range::range(std::initializer_list<dim_type> sP, std::initializer_list<dim_type> eP)
	: dSize_(sP.size()), sP_(sP), eP_(eP)
{
}

range::range(const coordinates& sP, const coordinates& eP)
	: dSize_(sP.size()), sP_(sP), eP_(eP)
{
	assert(sP.size() == eP.size());
}

range::range(const coordinates& eP)
	: dSize_(eP.size()), sP_(coordinates(eP.size())), eP_(eP)
{

}

range::range(const range& src)
	: dSize_(src.dSize_), sP_(src.sP_), eP_(src.eP_)
{

}

range::range(range&& src) noexcept
{
	swap(*this, src);
}

range::~range()
{
}

range& range::operator=(const range& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	range temp(rhs);
	swap(*this, temp);
	return *this;
}

range& range::operator=(range&& rhs) noexcept
{
	if (this == &rhs)
	{
		return *this;
	}

	range temp(std::move(rhs));
	swap(*this, temp);
	return *this;
}

// ***************************
// swap for range
// ***************************
void swap(range& first, range& second) noexcept
{
	using std::swap;

	swap(first.dSize_, second.dSize_);
	swap(first.sP_, second.sP_);
	swap(first.eP_, second.eP_);
}

range operator+ (const range& left, const coordinates& right)
{
	range output(left);
	output += right;
	return output;
}

range operator- (const range& left, const coordinates& right)
{
	range output(left);
	output -= right;
	return output;
}

////////////////////////////////////////
// multiDimIterator
////////////////////////////////////////
multiDimIterator::multiDimIterator(const size_type dSize, dim_const_pointer dims)
	: coor_(dSize), dSize_(dSize), end_(false), basisDimOffset_(1), seqPos_(0),
	dims_(dSize, dims), sP_(dSize), eP_(dSize, dims)
{
	this->basisDim_ = (dimensionId)(this->dSize() - 1);
	this->initSeqCapacity();
}

multiDimIterator::multiDimIterator(const size_type dSize, dim_const_pointer dims, 
								   dim_const_pointer startCoor, dim_const_pointer endCoor)
	: coor_(dSize), dSize_(dSize), end_(false), basisDimOffset_(1), seqPos_(0),
	dims_(dSize, dims), sP_(dSize, startCoor), eP_(dSize, endCoor)
{
	this->basisDim_ = (dimensionId)(this->dSize() - 1);
	this->initSeqCapacity();

	assert(this->initCheckSpEp());
	this->moveToStart();
}

multiDimIterator::multiDimIterator(const std::vector<dim_type>& dims)
	: coor_(dims.size()), dSize_(dims.size()), end_(false), basisDimOffset_(1), seqPos_(0),
	dims_(dims), sP_(dims.size()), eP_(dims)
{
	this->basisDim_ = (dimensionId)(this->dSize() - 1);
	this->initSeqCapacity();
}

multiDimIterator::multiDimIterator(const std::vector<dim_type>& dims, 
								   const std::vector<dim_type>& startCoor, const std::vector<dim_type>& endCoor)
	: coor_(dims.size()), dSize_(dims.size()), end_(false), basisDimOffset_(1), seqPos_(0),
	dims_(dims), sP_(startCoor), eP_(endCoor)
{
	assert(dims.size() == startCoor.size() && dims.size() == endCoor.size());

	this->basisDim_ = (dimensionId)(this->dSize() - 1);
	this->initSeqCapacity();

	assert(this->initCheckSpEp());
	this->moveToStart();
}

multiDimIterator::multiDimIterator(const coordinates& space)
	: coor_(space.size()), dSize_(space.size()), end_(false), basisDimOffset_(1), seqPos_(0),
	dims_(space), sP_(space.size()), eP_(space)
{
	this->basisDim_ = (dimensionId)(this->dSize() - 1);
	this->initSeqCapacity();
}

multiDimIterator::multiDimIterator(const coordinates& space, const range& itRange)
	: coor_(space.size()), dSize_(space.size()), end_(false), basisDimOffset_(1), seqPos_(0),
	dims_(space), sP_(itRange.getSp()), eP_(itRange.getEp())
{
	this->basisDim_ = (dimensionId)(this->dSize() - 1);
	this->initSeqCapacity();

	assert(this->initCheckSpEp());
	this->moveToStart();
}

multiDimIterator::multiDimIterator(const std::initializer_list<dim_type>& dims)
	: coor_(dims.size()), dSize_(dims.size()), end_(false), basisDimOffset_(1), seqPos_(0),
	dims_(dims.size()), sP_(dims.size()), eP_(dims.size())
{
	auto dimsIt = dims.begin();

	for (int d = 0; d < dims.size(); ++d)
	{
		this->dims_[d] = *dimsIt++;
		this->eP_[d] = *dimsIt++;
	}

	this->basisDim_ = (dimensionId)(this->dSize() - 1);
	this->initSeqCapacity();
}

multiDimIterator::multiDimIterator(const std::initializer_list<dim_type>& dims,
								   const std::initializer_list<dim_type>& startCoor, 
								   const std::initializer_list<dim_type>& endCoor)
{
	auto dimsIt = dims.begin();
	auto spIt = startCoor.begin();
	auto epIt = endCoor.begin();

	for (int d = 0; d < dims.size(); ++d)
	{
		this->dims_[d] = *dimsIt++;
		this->sP_[d] = *spIt++;
		this->eP_[d] = *epIt++;
	}

	this->basisDim_ = (dimensionId)(this->dSize() - 1);
	this->initSeqCapacity();

	assert(this->initCheckSpEp());
	this->moveToStart();
}

multiDimIterator::multiDimIterator(const self_type& mit)
	: coor_(mit.coor_), basisDim_(mit.basisDim_), dSize_(mit.dSize_),
	front_(mit.front_), end_(mit.end_), basisDimOffset_(mit.basisDimOffset_),
	seqPos_(mit.seqPos_), seqCapacity_(mit.seqCapacity_),
	dims_(mit.dims_), sP_(mit.sP_), eP_(mit.eP_)
{
}

multiDimIterator::multiDimIterator(self_type&& src) noexcept
	: multiDimIterator(src.dSize_, src.dims_)
{
	swap(*this, src);
}

multiDimIterator::~multiDimIterator()
{

}

// ***************************
// Operators
// ***************************
multiDimIterator::self_type& multiDimIterator::operator=(const self_type& src)
{
	if (this == &src)
	{
		return *this;
	}

	multiDimIterator temp(src);
	swap(*this, temp);
	return *this;
}

multiDimIterator::self_type& multiDimIterator::operator=(self_type&& src) noexcept
{
	if (this == &src)
	{
		return *this;
	}

	swap(*this, src);
}

void multiDimIterator::setBasisDim(const unsigned int dim)
{
	if (this->dSize() <= dim)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_LOGIC_ERROR, MSDB_ER_OUT_OF_RANGE, "setBasisDim - too large dim"));
	}

	this->basisDimOffset_ = this->getDimOffset(dim);
	this->basisDim_ = dim;
}

void multiDimIterator::next()
{
	if (this->end_)		return;
	this->front_ = false;

	dimensionId curDim = this->basisDim_;
	do
	{
		dim_type offset = this->getDimOffset(curDim);
		if (this->coor_[curDim] + 1 < this->eP_[curDim])
		{
			// move next and return
			this->coor_[curDim]++;
			this->seqPos_ += offset;
			return;
		}

		// move current dimension coordinates to sP_
		this->seqPos_ -= ((dim_type)this->coor_[curDim] - (dim_type)this->sP_[curDim]) * offset;
		this->coor_[curDim] = this->sP_[curDim];

		if (curDim != 0)		--curDim;
		else				curDim = (dimensionId)(this->dSize() - 1);

	} while (curDim != this->basisDim_);

	this->end_ = true;
}
void multiDimIterator::prev()
{
	if (this->front_)		return;
	this->end_ = false;

	dimensionId curDim = this->basisDim_;
	do
	{
		dim_type offset = this->getDimOffset(curDim);
		if (this->coor_[curDim] > this->sP_[curDim])
		{
			// move next and return
			this->coor_[curDim]--;
			this->seqPos_ += offset;
			return;
		}

		// move current dimension coordinates to sP_
		this->seqPos_ += ((dim_type)this->eP_[curDim] - (dim_type)1 - (dim_type)this->coor_[curDim]) * offset;
		this->coor_[curDim] = this->eP_[curDim] - 1;

		curDim = (++curDim) % this->dSize();
	} while (curDim != this->basisDim_);

	this->front_ = true;
}

void multiDimIterator::moveTo(const coordinates& coor)
{
	assert(this->dSize() == coor.size());

	size_type offset = 1;
	for (dimensionId d = (dimensionId)(this->dSize() - 1); d != (dimensionId)-1; d--)
	{
		this->seqPos_ += (coor[d] - this->coor_[d]) * offset;
		this->coor_[d] = coor[d];
		offset *= this->dims_[d];
	}

	this->setFrontEnd();
}

void multiDimIterator::moveToStart()
{
	this->moveTo(this->sP_);
}
void multiDimIterator::moveToLast()
{
	this->moveTo(this->eP_ - 1);
}

// ***************************
// swap for multiDimIterator
// ***************************
void swap(multiDimIterator& first, multiDimIterator& second) noexcept
{
	using std::swap;

	swap(first.coor_, second.coor_);
	swap(first.basisDim_, second.basisDim_);

	swap(first.dims_, second.dims_);
	swap(first.sP_, second.sP_);
	swap(first.eP_, second.eP_);
	swap(first.dSize_, second.dSize_);

	swap(first.end_, second.end_);
	swap(first.front_, second.front_);

	swap(first.seqPos_, second.seqPos_);
	swap(first.seqCapacity_, second.seqCapacity_);
	swap(first.basisDimOffset_, second.basisDimOffset_);
}
////////////////////////////////////////

////////////////////
// Pic less coordinates value on each dimension
coordinates getInsideCoor(const coordinates& c1, const coordinates& c2)
{
	coordinates newCoor(c1);
	for (dimensionId d = 0; d < c1.size(); ++d)
	{
		if (c1[d] > c2[d])
		{
			newCoor[d] = c2[d];
		}
	}
	return newCoor;
}

////////////////////
// Pic greater coordiante value on each dimension
coordinates getOutsideCoor(const coordinates& c1, const coordinates& c2)
{
	coordinates newCoor(c1);
	for (dimensionId d = 0; d < c1.size(); ++d)
	{
		if (c1[d] < c2[d])
		{
			newCoor[d] = c2[d];
		}
	}
	return newCoor;
}
}		// core
}		// msdb