#pragma once
#ifndef _MSDB_COORDINATE_H_
#define _MSDB_COORDINATE_H_

#include <pch.h>
#include <util/element.h>
#include <util/math.h>
#include <system/exceptions.h>
#include <array/dimensionId.h>
#include <initializer_list>

namespace msdb
{
namespace core
{
typedef int64_t		position_t;

enum class iterateMode
{
	ALL,		// Iterate all possible coordinate
	EXIST		// Only exist items
};

template <typename Dty_>
class coordinate
{
public:
	//using difference_type = ptrdiff_t;
	using self_type = coordinate;
	using size_type = size_t;
	using dim_type = Dty_;
	using dim_pointer = Dty_*;
	using dim_const_pointer = const Dty_*;
	using dim_reference = Dty_&;
	using dim_const_reference = const Dty_&;

public:
	coordinate(const size_type dSize = 0)
		: dSize_(dSize)
	{
		this->coor_ = new dim_type[this->dSize_]();
#ifndef NDEBUG
		if(this->dSize_ >= 2)
		{
			this->x_ = this->coor_ + 1;
			this->y_ = this->coor_;
		}
#endif
	}

	coordinate(const size_type dSize, dim_const_pointer coor)
		: dSize_(dSize)
	{
		this->coor_ = new dim_type[this->dSize_];
		memcpy(this->coor_, coor, this->dSize_ * sizeof(dim_type));
#ifndef NDEBUG
		if (this->dSize_ >= 2)
		{
			this->x_ = this->coor_ + 1;
			this->y_ = this->coor_;
		}
#endif
	}

	coordinate(const size_type dSize, const dim_type coor)
		: dSize_(dSize)
	{
		this->coor_ = new dim_type[this->dSize_];
		for(size_type d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] = coor;
		}
#ifndef NDEBUG
		if (this->dSize_ >= 2)
		{
			this->x_ = this->coor_ + 1;
			this->y_ = this->coor_;
		}
#endif
	}

	coordinate(const std::vector<dim_type>& coorVec)
		: dSize_(coorVec.size())
	{
		this->coor_ = new dim_type[coorVec.size()];
		memcpy(this->coor_, coorVec.data(), coorVec.size() * sizeof(dim_type));
#ifndef NDEBUG
		if (this->dSize_ >= 2)
		{
			this->x_ = this->coor_ + 1;
			this->y_ = this->coor_;
		}
#endif
	}

	coordinate(const self_type& mit)
		: dSize_(mit.dSize_)
	{
		this->coor_ = new dim_type[this->dSize_];
		memcpy(this->coor_, mit.coor_, mit.dSize_ * sizeof(dim_type));
#ifndef NDEBUG
		if (this->dSize_ >= 2)
		{
			this->x_ = this->coor_ + 1;
			this->y_ = this->coor_;
		}
#endif
	}

	self_type& operator=(self_type& rhs)
	{
		this->dSize_ = rhs.dSize_;
		this->coor_ = new dim_type[this->dSize_];
		memcpy(this->coor_, rhs.coor_, rhs.dSize_ * sizeof(dim_type));
#ifndef NDEBUG
		if (this->dSize_ >= 2)
		{
			this->x_ = this->coor_ + 1;
			this->y_ = this->coor_;
		}
#endif
		return *this;
	}

	coordinate(std::initializer_list<dim_type> lst)
	{
		this->dSize_ = lst.size();
		this->coor_ = new dim_type[this->dSize_];

		auto it = lst.begin();
		for (size_type d = 0; d < this->dSize_; d++, it++)
		{
			this->coor_[d] = *it;
		}
#ifndef NDEBUG
		if (this->dSize_ >= 2)
		{
			this->x_ = this->coor_ + 1;
			this->y_ = this->coor_;
		}
#endif
	}

	~coordinate()
	{
		delete[] this->coor_;
	}

public:
	//////////////////////////////
	// Getter
	_NODISCARD std::string toString() const
	{
		std::ostringstream ss;

		ss << "(";
		for(dimensionId d = this->dSize_ - 1; d != (dimensionId)-1; --d)
		{
			if(d != this->dSize_ - 1)
			{
				ss << ", ";
			}
			ss << this->coor_[d];
		}

		ss << ")";

		return ss.str();
	}
	_NODISCARD inline constexpr size_type size() const noexcept
	{
		return this->dSize_;
	}
	_NODISCARD inline size_type area() const
	{
		size_type capacity = 1;
		for (dimensionId d = 0; d < dSize_; ++d)
		{
			capacity *= this->coor_[d];
		}
		return capacity;
	}
	dim_pointer data()
	{
		return this->coor_;
	}

	const dim_pointer data() const
	{
		return this->coor_;
	}
	//////////////////////////////

	//////////////////////////////
	// Operators
	// Comparison
	bool operator==(const self_type& rhs) const
	{
		return this->dSize_ == rhs.dSize_ && (memcmp(this->coor_, rhs.coor_, this->dSize_ * sizeof(dim_type)) == 0);
	}
	bool operator==(const dim_type* rhs) const
	{
		for(dimensionId d = 0; d < this->dSize_; ++d)
		{
			if(this->coor_[d] != rhs[d])
			{
				return false;
			}
		}
		return true;
	}
	bool operator!=(const self_type& rhs) const
	{
		return this->dSize_ != rhs.dSize_ || (memcmp(this->coor_, rhs.coor_, this->dSize_ * sizeof(dim_type)) != 0);
	}
	bool operator!=(const dim_type* rhs) const
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			if (this->coor_[d] != rhs[d])
			{
				return true;
			}
		}
		return false;
	}
	bool operator<(const self_type& rhs) const
	{
		if (*this == rhs)
			return false;

		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			if (this->coor_[d] > rhs[d])
			{
				return false;
			}
		}
		return true;
	}
	bool operator>(const self_type& rhs) const
	{
		if (*this == rhs)
			return false;

		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			if (this->coor_[d] < rhs[d])
			{
				return false;
			}
		}
		return true;
	}
	bool operator<=(const self_type& rhs) const
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			if (this->coor_[d] > rhs[d])
			{
				return false;
			}
		}
		return true;
	}
	bool operator>=(const self_type& rhs) const
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			if (this->coor_[d] < rhs[d])
			{
				return false;
			}
		}
		return true;
	}

	// Assign
	self_type& operator=(const self_type& mit)
	{
		delete[] this->coor_;
		this->dSize_ = mit.dSize_;
		this->coor_ = new dim_type[mit.dSize_];
		this->memcpyCoor(this->coor_, mit.coor_);

#ifndef NDEBUG
		if (this->dSize_ >= 2)
		{
			this->x_ = this->coor_ + 1;
			this->y_ = this->coor_;
		}
#endif

		return *this;
	}
	self_type& operator=(const dim_type* mit)
	{
		this->memcpyCoor(this->coor_, mit);

		return *this;
	}

	self_type& operator+= (const self_type& mit)
	{
		assert(this->dSize_ == mit.dSize_);

		for(dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] += mit[d];
		}

		return *this;
	}

	self_type& operator-= (const self_type& mit)
	{
		assert(this->dSize_ == mit.dSize_);

		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] -= mit[d];
		}

		return *this;
	}

	self_type& operator *= (const self_type& mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] *= mit[d];
		}

		return *this;
	}

	self_type& operator*= (const int64_t mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] *= mit;
		}

		return *this;
	}

	// TODO:: Change intDivCeil to intDivFloor
	//  Why the divide operator uses 'intDivCeil'???
	// - To calculate chunk, block size.

	self_type& operator/= (const self_type& mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] = intDivCeil(this->coor_[d], mit[d]);
		}

		return *this;
	}
	self_type& operator/= (const int64_t mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] /= mit;
		}

		return *this;
	}

	self_type& operator%= (const self_type& mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] %= mit[d];
		}

		return *this;
	}
	self_type& operator %= (const int64_t mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] %= mit;
		}

		return *this;
	}

	// Move
	self_type& operator++()
	{
		this->coor_[this->dSize_ - 1]++;
		return *this;
	}
	self_type operator++(int)
	{
		coordinate<Dty_> tmp(*this);
		operator++();
		return tmp;
	}

	// Random access
	_NODISCARD _CONSTEXPR17 dim_reference operator[](_In_range_(0, dSize_ - 1) size_type pos)
		noexcept /* strengthened */
	{
		assert(pos < this->dSize_);
		return this->coor_[pos];
	}
	_NODISCARD constexpr dim_const_reference operator[](_In_range_(0, dSize_ - 1) size_type pos) const
		noexcept /* strengthened */
	{
		assert(pos < this->dSize_);
		return this->coor_[pos];
	}
	_NODISCARD _CONSTEXPR17 dim_reference at(_In_range_(0, dSize_ - 1) size_type pos)
	{
		assert(pos < this->dSize_);
		return this->coor_[pos];
	}
	_NODISCARD constexpr dim_const_reference at(_In_range_(0, dSize_ - 1) size_type pos) const
	{
		assert(pos < this->dSize_);
		return this->coor_[pos];
	}
	//////////////////////////////

public:
	coordinate<Dty_> divCoor(const coordinate<Dty_>& right)
	{
		coordinate<Dty_> output(*this);
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			output.coor_[d] /= right.coor_[d];
		}

		return output;
	}

protected:
	void memcpyCoor(dim_type* dest, const dim_type* src)
	{
		memcpy(dest, src, this->dSize_ * sizeof(dim_type));
	}

protected:
	size_type dSize_;
	dim_pointer coor_;
#ifndef NDEBUG
	dim_pointer x_;
	dim_pointer y_;
#endif
};

template <typename Dty_>
coordinate<Dty_> operator+ (const coordinate<Dty_>& left, const coordinate<Dty_>& right)
{
	coordinate<Dty_> output(left);
	output += right;
	return output;
}

template <typename Dty_>
coordinate<Dty_> operator+ (const coordinate<Dty_>& left, const int right)
{
	coordinate<Dty_> output(left);
	output += right;
	return output;
}

template <typename Dty_>
coordinate<Dty_> operator- (const coordinate<Dty_>& left, const coordinate<Dty_>& right)
{
	coordinate<Dty_> output(left);
	output -= right;
	return output;
}

template <typename Dty_>
coordinate<Dty_> operator- (const coordinate<Dty_>& left, const int right)
{
	coordinate<Dty_> output(left);
	output -= right;
	return output;
}

template <typename Dty_>
coordinate<Dty_> operator* (const coordinate<Dty_>& left, const coordinate<Dty_>& right)
{
	coordinate<Dty_> output(left);
	output *= right;
	return output;
}

template <typename Dty_>
coordinate<Dty_> operator* (const coordinate<Dty_>& left, const int right)
{
	coordinate<Dty_> output(left);
	output *= right;
	return output;
}

template <typename Dty_>
coordinate<Dty_> operator/ (const coordinate<Dty_>& left, const coordinate<Dty_>& right)
{
	coordinate<Dty_> output(left);
	output /= right;
	return output;
}

template <typename Dty_>
coordinate<Dty_> operator/ (const coordinate<Dty_>& left, const int right)
{
	coordinate<Dty_> output(left);
	output /= right;
	return output;
}

template <typename Dty_>
coordinate<Dty_> operator% (const coordinate<Dty_>& left, const coordinate<Dty_>& right)
{
	coordinate<Dty_> output(left);
	output %= right;
	return output;
}

template <typename Dty_>
coordinate<Dty_> operator% (const coordinate<Dty_>& left, const int right)
{
	coordinate<Dty_> output(left);
	output %= right;
	return output;
}


template <typename Dty_>
class coordinateRange
{
public:
	using self_type = coordinateRange;
	using size_type = size_t;
	using dim_type = Dty_;
	using dim_pointer = Dty_*;
	using dim_const_pointer = const Dty_*;
	using dim_reference = Dty_&;
	using dim_const_reference = const Dty_&;

public:
	coordinateRange(const size_type dSize = 0)
		: dSize_(dSize), sP_(dSize), eP_(dSize)
	{
	}

	coordinateRange(const std::vector<dim_type>& sP, const std::vector<dim_type>& eP)
		: dSize_(sP.size()), sP_(sP), eP_(eP)
	{
		assert(sP_.size() == eP_.size());
	}

	coordinateRange(std::initializer_list<dim_type> sP, std::initializer_list<dim_type> eP)
		: dSize_(sP.size()), sP_(sP), eP_(eP)
	{
	}

	coordinateRange(const coordinate<Dty_>& sP, const coordinate<Dty_>& eP)
		: dSize_(sP.size()), sP_(sP), eP_(eP)
	{
		assert(sP.size() == eP.size());
	}

	coordinateRange(const coordinate<Dty_>& eP)
		: dSize_(eP.size()), sP_(coordinate<Dty_>(eP.size())), eP_(eP)
	{

	}

	coordinateRange(const self_type& mit)
		: dSize_(mit.dSize_), sP_(mit.sP_), eP_(mit.eP_)
	{
	}

	~coordinateRange()
	{
	}

public:
	//////////////////////////////
	// Getter
	_NODISCARD inline constexpr size_type size() const noexcept
	{
		return this->dSize_;
	}

	_NODISCARD std::string toString() const
	{
		std::ostringstream ss;

		ss << this->sP_.toString() << "~" << this->eP_.toString();

		return ss.str();
	}

	coordinate<Dty_> getSp() const
	{
		return this->sP_;
	}
	coordinate<Dty_> getEp() const
	{
		return this->eP_;
	}

	coordinate<Dty_>& getSp()
	{
		return this->sP_;
	}
	coordinate<Dty_>& getEp()
	{
		return this->eP_;
	}

	coordinate<Dty_> width() const
	{
		coordinate<Dty_> width(this->eP_);
		width -= sP_;

		return width;
	}

	void move(const coordinate<Dty_>& offset)
	{
		this->sP_ += offset;
		this->eP_ += offset;
	}

	void moveTo(const coordinate<Dty_>& sP)
	{
		coordinate<Dty_> offset(sP);
		offset -= this->sP_;

		this->move(offset);
	}

	bool isIntersect(const self_type& rhs) const
	{
		bool first;
		bool second;

		dimensionId d;
		for (d = 0; d < rhs.dSize_; ++d)
		{
			if (this->sP_[d] >= rhs.eP_[d])
			{
				break;
			}
		}
		if (d == rhs.dSize_)
			first = true;
		else
			first = false;

		for (d = 0; d < rhs.dSize_; ++d)
		{
			if (rhs.sP_[d] >= this->eP_[d])
			{
				break;
			}
		}
		if (d == rhs.dSize_)
			second = true;
		else
			second = false;

		return first && second;
	}

	bool isFullyInside(const self_type& rhs) const
	{
		if (rhs.sP_ <= this->sP_ && this->eP_ <= rhs.eP_)
			return true;

		return false;
	}

public:
	bool operator==(const self_type& rhs) const
	{
		return this->dSize_ == rhs.dSize_ && sP_ == rhs.sP_ && eP_ == rhs.eP_;
	}

	bool operator!=(const self_type& rhs) const
	{
		return !(*this == rhs);
	}

	self_type& operator+=(const coordinate<Dty_>& rhs)
	{
		this->sP_ += rhs;
		this->eP_ += rhs;

		return *this;
	}

	self_type& operator-=(const coordinate<Dty_>& rhs)
	{
		this->sP_ -= rhs;
		this->eP_ -= rhs;

		return *this;
	}

	//bool operator<(const self_type& rhs) const
	//{
	//	assert(this->dSize_ == rhs.dSize_);
	//	if (*this == rhs)
	//		return false;

	//	if(!(this->sP_ < rhs.sP_))
	//	{
	//		return false;
	//	}

	//	if(!(this->eP_ > rhs.eP_))
	//	{
	//		return false;
	//	}

	//	return true;
	//}

	//bool operator>(const self_type& rhs) const
	//{
	//	assert(this->dSize_ == rhs.dSize_);
	//	if (*this == rhs)
	//		return false;

	//	for (dimensionId d = 0; d < this->dSize_; ++d)
	//	{
	//		if (this->sP_[d] > rhs.sP_[d] || this->eP_[d] < rhs.eP_[d])
	//			return false;
	//	}
	//	return true;
	//}

protected:
	size_type dSize_;
	coordinate<Dty_> sP_;
	coordinate<Dty_> eP_;
};

template <typename Dty_>
coordinateRange<Dty_> operator+ (const coordinateRange<Dty_>& left, const coordinate<Dty_>& right)
{
	coordinateRange<Dty_> output(left);
	output += right;
	return output;
}

template <typename Dty_>
coordinateRange<Dty_> operator- (const coordinateRange<Dty_>& left, const coordinate<Dty_>& right)
{
	coordinateRange<Dty_> output(left);
	output -= right;
	return output;
}

template <typename Dty_>
class coordinateIterator
{
public:
	using self_type = coordinateIterator<Dty_>;
	using size_type = size_t;
	using dim_type = Dty_;
	using dim_pointer = Dty_*;
	using dim_const_pointer = const Dty_*;
	using dim_reference = Dty_&;
	using dim_const_reference = const Dty_&;

	using coordinate_type = coordinate<Dty_>;

public:
	coordinateIterator(const size_type dSize, dim_const_pointer dims)
		: coor_(dSize), dSize_(dSize), end_(false), basisDimOffset_(1), seqPos_(0), 
		dims_(dSize, dims), sP_(dSize), eP_(dSize, dims)
	{
		this->basisDim_ = (dimensionId)(this->dSize() - 1);
		this->initSeqCapacity();
	}

	coordinateIterator(const std::vector<dim_type>& lst)
		: coor_(lst.size()), dSize_(lst.size()), end_(false), basisDimOffset_(1), seqPos_(0),
		dims_(lst), sP_(lst.size()), eP_(lst)
	{
		this->basisDim_ = (dimensionId)(this->dSize() - 1);
		this->initSeqCapacity();
	}

	coordinateIterator(const coordinate<position_t> space)
		: coor_(space.size()), dSize_(space.size()), end_(false), basisDimOffset_(1), seqPos_(0),
		dims_(space), sP_(space.size()), eP_(space)
	{
		this->basisDim_ = (dimensionId)(this->dSize() - 1);
		this->initSeqCapacity();
	}

	coordinateIterator(std::initializer_list<dim_type> lst)
		: coor_(lst.size()), dSize_(lst.size()), end_(false), basisDimOffset_(1), seqPos_(0),
		dims_(lst.size()), sP_(lst.size()), eP_(lst.size())
	{
		size_type d = 0;
		for(auto value : lst)
		{
			this->dims_[d] = value;
			this->eP_[d] = value;
		}
		this->basisDim_ = (dimensionId)(this->dSize() - 1);
		this->initSeqCapacity();
	}

	coordinateIterator(const self_type& mit)
		: coor_(mit.coor_), basisDim_(mit.basisDim_), dSize_(mit.dSize_),
		front_(mit.front_), end_(mit.end_), basisDimOffset_(mit.basisDimOffset_),
		seqPos_(mit.seqPos_), seqCapacity_(mit.seqCapacity_),
		dims_(mit.dims_), sP_(mit.sP_), eP_(mit.eP_)
	{
	}
	self_type& operator=(const self_type& rhs)
	{
		this->coor_ = rhs.coor_;
		this->basisDim_ = rhs.basisDim_;
		this->dSize_ = rhs.dSize_;
		this->front_ = rhs.front_;
		this->end_ = rhs.end_;
		this->basisDimOffset_ = rhs.basisDimOffset_;
		this->seqPos_ = rhs.seqPos_;
		this->seqCapacity_ = rhs.seqCapacity_;
		this->dims_ = rhs.dims_;
		this->sP_ = rhs.sP_;
		this->eP_ = rhs.eP_;

		return *this;
	}

	~coordinateIterator()
	{
		//delete[] this->dims_;
		//delete[] this->sP_;
	}

public:
	virtual void setBasisDim(const unsigned int dim)
	{
		if (this->dSize() <= dim)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_LOGIC_ERROR, MSDB_ER_OUT_OF_RANGE, "setBasisDim - too large dim"));
		}

		this->basisDimOffset_ = this->getDimOffset(dim);
		this->basisDim_ = dim;
	}

	//////////////////////////////
	// Getter
	//////////////////////////////
	_NODISCARD inline size_type seqPos()
	{
		return this->seqPos_;
	}
	_NODISCARD inline size_type dSize() const
	{
		// this->coor_.size();
		return this->dSize_;
	}
	_NODISCARD inline coordinate_type& coor() { return this->coor_; }
	_NODISCARD inline coordinate_type dims() const { return this->dims_; }
	_NODISCARD inline bool isEnd() const { return this->end_; }
	_NODISCARD inline bool isFront() const { return this->front_; }
	coordinate<Dty_> getSp() const
	{
		return this->sP_;
	}
	coordinate<Dty_> getEp() const
	{
		return this->eP_;
	}

	coordinate<Dty_>& getSp()
	{
		return this->sP_;
	}
	coordinate<Dty_>& getEp()
	{
		return this->eP_;
	}

	size_type getCapacity()
	{
		return this->seqCapacity_;
	}
	//////////////////////////////

	//////////////////////////////
	// Converting
	//////////////////////////////
	// pos: basis dim pos
	virtual size_type posToSeq(const size_type pos)
	{
		//size_type absolutePos = this->coor_[this->basisDim_] + pos;
		//if(absolutePos < this->sP_[this->basisDim_] || this->eP_[this->basisDim_] <= absolutePos)
		//{
		//	_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_LOGIC_ERROR, MSDB_ER_OUT_OF_RANGE, "itemIterator getAtDimPos out of range"));
		//}

		size_type left = pos * this->basisDimOffset_;
		size_type seq = 0;
		size_type offset = 1;
		// Do not change int -> dimensionId, 
		// Error happened when d == -1
		for (int d = this->basisDim_; d >= 0; d--)
		{
			seq += (left % this->dims_[d]) * offset;
			left = left / this->dims_[d];
			offset *= this->dims_[d];
		}

		for (int d = (int)(this->dSize() - 1); d > this->basisDim_; d--)
		{
			seq += (left % this->dims_[d]) * offset;
			left = left / this->dims_[d];
			offset *= this->dims_[d];
		}

		return seq;
	}
	size_type coorToSeq(const coordinate_type coor)
	{
		size_type seq = 0, offset = 1;
		for (dimensionId d = this->dSize() - 1; d != (dimensionId)-1; d--)
		{
			seq += coor[d] * offset;
			offset *= this->dims_[d];
		}
		return seq;
	}
	coordinate<Dty_> seqToCoor(const size_type seq)
	{
		//assert(seq <= this->getCapacity());

		size_type remain = seq;
		coordinate<Dty_> outCoor(this->dSize_);
		size_type offset = this->getCapacity();

		for (dimensionId d = 0; d < this->dSize(); ++d)
		{
			offset /= this->dims_[d];
			outCoor[d] = remain / offset;
			remain %= offset;
		}
		return outCoor;
	}

	//////////////////////////////

	//////////////////////////////
	// Iterating
	//////////////////////////////
	virtual void next()
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

			// move current dimension coordinate to sP_
			this->seqPos_ -= ((dim_type)this->coor_[curDim] - (dim_type)this->sP_[curDim]) * offset;
			this->coor_[curDim] = this->sP_[curDim];

			if (curDim != 0)		--curDim;
			else				curDim = (dimensionId)(this->dSize() - 1);

		} while (curDim != this->basisDim_);

		this->end_ = true;
	}
	virtual void prev()
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

			// move current dimension coordinate to sP_
			this->seqPos_ += ((dim_type)this->eP_[curDim] - (dim_type)1 - (dim_type)this->coor_[curDim]) * offset;
			this->coor_[curDim] = this->eP_[curDim] - 1;

			curDim = (++curDim) % this->dSize();
		} while (curDim != this->basisDim_);

		this->front_ = true;
	}

	virtual void moveTo(const coordinate_type& coor)
	{
		assert(this->dSize() == coor.size());

		size_type offset = 1;
		for (dimensionId d = this->dSize() - 1; d != (dimensionId)-1; d--)
		{
			this->seqPos_ += (coor[d] - this->coor_[d]) * offset;
			this->coor_[d] = coor[d];
			offset *= this->dims_[d];
		}

		this->setFrontEnd();
	}

	virtual void moveTo(const size_type seqPos)
	{
		//this->seqPos
	}

	virtual void moveToStart()
	{
		this->moveTo(this->sP_);
	}
	virtual void moveToLast()
	{
		this->moveTo(this->eP_ - 1);
	}

	//////////////////////////////

	//////////////////////////////
	// Operators
	//////////////////////////////
	// forward
	self_type& operator++()
	{
		this->next();
		return *this;
	}
	self_type operator++(int)
	{
		self_type tmp(*this);
		operator++();
		return tmp;
	}
	self_type& operator+=(const int& rhs)
	{
		for (int i = 0; i < rhs; i++)
		{
			operator++();
		}
		return *this;
	}

	// backward
	self_type& operator--()
	{
		this->prev();
		return *this;
	}
	self_type operator--(int)
	{
		self_type tmp(*this);
		operator--();
		return tmp;
	}
	self_type& operator-=(const int& rhs)
	{
		for (int i = 0; i < rhs; i++)
		{
			operator--();
		}
		return *this;
	}
	//////////////////////////////

protected:
	// copy dimension value using memcpy
	void memcpyDim(dim_type* dest, const dim_type* src)
	{
		memcpy(dest, src, this->dSize() * sizeof(dim_type));
	}

	size_type initSeqCapacity()
	{
		this->seqCapacity_ = this->dims_[0];
		for (int i = 1; i < this->dSize(); i++)
		{
			this->seqCapacity_ *= this->dims_[i];
		}
		return this->seqCapacity_;
	}

	void setFrontEnd()
	{
		if (this->coor_ == this->sP_)
		{
			this->front_ = true;
		} else
		{
			this->front_ = false;
		}
		if (this->coor_ == this->eP_)
		{
			this->end_ = true;
		} else
		{
			this->end_ = false;
		}
	}

	// call this function before you change a basisDim
	size_t getDimOffset(const unsigned int dim)
	{
		if (dim == this->basisDim_)
		{
			return this->basisDimOffset_;
		}

		size_t offset = 1;
		for (size_type i = this->dSize() - 1; i > dim; i--)
		{
			offset *= this->dims_[i];
		}

		//std::cout << "dim : " << dim << ", getOffset: " << offset << std::endl;
		return offset;
	}

protected:
	coordinate_type coor_;		// coordinate
	dimensionId basisDim_;		// current dimension

	coordinate_type dims_;		// original array dimension size
	coordinate_type sP_;		// range start point
	coordinate_type eP_;		// range end point
	size_type dSize_;			// number of dimensions

	bool end_;					// is iterator at the end
	bool front_;				// is iterator at the front

	size_type seqPos_;
	size_type seqCapacity_;
	size_type basisDimOffset_;

	// TODO:: replace sP_ to zero
	// static std::vector<dim_type> zeroP_;
};

template <typename Dty_, typename Ty_ = element>
class itemIterator : virtual public coordinateIterator<Dty_>
{
public:
	using self_type = itemIterator<Dty_, Ty_>;
	using base_type = coordinateIterator<Dty_>;
	using coordinate_type = coordinate<Dty_>;
	using size_type = size_t;
	using dim_type = Dty_;
	using dim_pointer = Dty_*;
	using dim_const_pointer = const Dty_*;
	using dim_reference = Dty_&;
	using dim_const_reference = const Dty_&;

public:
	itemIterator(Ty_* ptr, const size_type dSize, dim_const_pointer dims)
		: base_type(dSize, dims), ptr_(ptr)
	{
	}

	itemIterator(Ty_* ptr, const coordinate<Dty_> dims)
		: base_type(dims), ptr_(ptr)
	{
	}
	

	itemIterator(const self_type& mit)
		: base_type(mit), ptr_(mit.ptr_)
	{

	}

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	virtual Ty_& getAtDimPos(position_t pos)
	{
		return *(ptr_ + this->seqPos_ + this->posToSeq(pos));
	}

	virtual Ty_& getAtSeqPos(position_t seqPos)
	{
		return *(ptr_ + seqPos);
	}
	//////////////////////////////

	//////////////////////////////
	// Operators
	//////////////////////////////
	// Comparison
	bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_ && this->seqPos_ == rhs.seqPos_; }
	bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_ || this->seqPos_ != rhs.seqPos_; }

	// Pointer
	virtual Ty_& operator*() { return *(ptr_ + this->seqPos_); }
	//element operator->() { return element((void*)(ptr_ + this->seqPos_ * this->eSize_), this->eType_); }
	//////////////////////////////

protected:
	Ty_* ptr_;					// pointer to element
								// set pointer type as char, to move one byte for each seqPos_
};

template <typename Dty_>
class itemIterator<Dty_, element> : virtual public coordinateIterator<Dty_>
{
public:
	using self_type = itemIterator<Dty_, element>;
	using base_type = coordinateIterator<Dty_>;
	using coordinate_type = coordinate<Dty_>;
	using size_type = size_t;
	using dim_type = Dty_;
	using dim_pointer = Dty_*;
	using dim_const_pointer = const Dty_*;
	using dim_reference = Dty_&;
	using dim_const_reference = const Dty_&;

public:
	itemIterator(void* ptr, const eleType eType, const size_type dSize, dim_const_pointer dims)
		: base_type(dSize, dims), eType_(eType), eSize_(getEleSize(eType))
	{
		this->ptr_ = reinterpret_cast<char*>(ptr);
	}

	itemIterator(void* ptr, const eleType eType, const coordinate<Dty_> dims)
		: base_type(dims), eType_(eType), eSize_(getEleSize(eType))
	{
		this->ptr_ = reinterpret_cast<char*>(ptr);
	}

	itemIterator(const self_type& mit)
		: base_type(mit), ptr_(mit.ptr_), eType_(mit.eType_), eSize_(mit.eSize_)
	{

	}

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	virtual element getAtDimPos(position_t dimPos)
	{
		return element((void*)(ptr_ + (this->seqPos_ + this->posToSeq(dimPos)) * this->eSize_), this->eType_);
	}

	virtual element getAtSeqPos(position_t seqPos)
	{
		return element((void*)(ptr_ + seqPos * this->eSize_));
	}

	eleType getEtype()
	{
		return this->eType_;
	}
	//////////////////////////////

	//////////////////////////////
	// Operators
	//////////////////////////////
	// Comparison
	bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_ && this->seqPos_ == rhs.seqPos_; }
	bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_ || this->seqPos_ != rhs.seqPos_; }

	// Pointer
	virtual element operator*() { return element((void*)(ptr_ + this->seqPos_ * this->eSize_), this->eType_); }
	//element operator->() { return element((void*)(ptr_ + this->seqPos_ * this->eSize_), this->eType_); }
	//////////////////////////////

protected:
	char* ptr_;					// pointer to element
								// set pointer type as char, to move one byte for each seqPos_
	eleType eType_;				// element type
	size_type eSize_;			// element size in byte
};

template <typename Dty_, typename Ty_ = element>
class itemRangeIterator : public itemIterator<Dty_, Ty_>
{
public:
	using self_type = itemRangeIterator<Dty_, Ty_>;
	using base_type = itemIterator<Dty_, Ty_>;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	itemRangeIterator(void* ptr, const size_type dSize, dim_const_pointer dim,
					  dim_const_pointer sP, dim_const_pointer eP)
		: base_type(ptr, dSize, dim), coordinateIterator<Dty_>(dSize, dim)
	{
		assert(this->initCheckSpEp());
		this->sP_ = coordinate<Dty_>(dSize, sP);
		this->eP_ = coordinate<Dty_>(dSize, eP);
		this->moveToStart();
	}

	itemRangeIterator(void* ptr, eleType eType, const size_type dSize, dim_const_pointer dim,
					  const coordinateRange<Dty_>& range)
		: base_type(ptr, eType, dSize, dim), coordinateIterator<Dty_>(dSize, dim)
	{
		assert(this->initCheckSpEp());
		this->sP_ = range.getSp();
		this->eP_ = range.getEp();
		this->moveToStart();
	}

	itemRangeIterator(const self_type& mit) : base_type(mit), coordinateIterator<Dty_>(mit)
	{
		this->sP_ = mit.sP_;
		this->eP_ = mit.eP_;
	}

	~itemRangeIterator()
	{
	}

protected:
	// Check if sP_ and eP_ has proper coordinate.
	bool initCheckSpEp()
	{
		for (dimensionId d = 0; d < this->dSize(); d++)
		{
			if (this->sP_[d] > this->eP_[d] || this->dims_[d] < this->eP_[d])
			{
				return false;
			}
		}

		return true;
	}
};

template <typename Dty_>
class itemRangeIterator<Dty_, element> : public itemIterator<Dty_, element>
{
public:
	using self_type = itemRangeIterator<Dty_, element>;
	using base_type = itemIterator<Dty_, element>;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	itemRangeIterator(void* ptr, 
					  const eleType eType,
					  const size_type dSize, 
					  dim_const_pointer dim,
					  dim_const_pointer sP,
					  dim_const_pointer eP)
		: base_type(ptr, eType, dSize, dim), coordinateIterator<Dty_>(dSize, dim)
	{
		assert(this->initCheckSpEp());
		this->sP_ = coordinate<Dty_>(dSize, sP);
		this->eP_ = coordinate<Dty_>(dSize, eP);
		this->moveToStart();
	}

	itemRangeIterator(void* ptr, 
					  const eleType eType,
					  const coordinate<Dty_> dim,
					  const coordinateRange<Dty_>& range)
		: base_type(ptr, eType, dim), coordinateIterator<Dty_>(dim)
	{
		assert(this->initCheckSpEp());
		this->sP_ = range.getSp();
		this->eP_ = range.getEp();
		this->moveToStart();
	}

	itemRangeIterator(const self_type& mit) 
		: base_type(mit), coordinateIterator<Dty_>(mit)
	{
		this->sP_ = mit.sP_;
		this->eP_ = mit.eP_;
	}

	~itemRangeIterator()
	{
	}

protected:
	// Check if sP_ and eP_ has proper coordinate.
	bool initCheckSpEp()
	{
		for (dimensionId d = 0; d < this->dSize(); d++)
		{
			if (this->sP_[d] > this->eP_[d] || this->dims_[d] < this->eP_[d])
			{
				return false;
			}
		}

		return true;
	}
};

// Pic less coordinate value on each dimension
template <typename Dty_>
coordinate<Dty_> getInsideCoor(const coordinate<Dty_>& c1, const coordinate<Dty_>& c2)
{
	coordinate<Dty_> newCoor(c1);
	for(dimensionId d = 0; d < c1.size(); ++d)
	{
		if(c1[d] > c2[d])
		{
			newCoor[d] = c2[d];
		}
	}
	return newCoor;
}

// Pic greater coordiante value on each dimension
template <typename Dty_>
coordinate<Dty_> getOutsideCoor(const coordinate<Dty_>& c1, const coordinate<Dty_>& c2)
{
	coordinate<Dty_> newCoor(c1);
	for (dimensionId d = 0; d < c1.size(); ++d)
	{
		if (c1[d] < c2[d])
		{
			newCoor[d] = c2[d];
		}
	}
	return newCoor;
}

using coor = coordinate<position_t>;
using pCoor = std::shared_ptr<coor>;

using coorItr = coordinateIterator<position_t>;
using pCoorItr = std::shared_ptr<coorItr>;

using coorRange = coordinateRange<position_t>;
using pCoorRange = std::shared_ptr<coorRange>;

using itemItr = itemIterator<position_t>;
using pItemItr = std::shared_ptr<itemItr>;

using itemRangeItr = itemRangeIterator<position_t>;
using pItemRangeItr = std::shared_ptr<itemRangeItr>;
}	// core
}	// msdb
#endif