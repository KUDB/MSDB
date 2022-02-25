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
	ALL,		// Iterate all possible coordinates
	EXIST		// Only exist items
};

////////////////////////////////////////
// coordinates
//
class coordinates
{
public:
	//using difference_type = ptrdiff_t;
	using self_type = coordinates;
	using size_type = size_t;
	using dim_type = position_t;
	using dim_pointer = position_t*;
	using dim_const_pointer = const position_t*;
	using dim_reference = position_t&;
	using dim_const_reference = const position_t&;

public:
	coordinates(const size_type dSize = 0);
	coordinates(const size_type dSize, dim_const_pointer coor);
	coordinates(const size_type dSize, const dim_type coor);
	coordinates(const std::vector<dim_type>& coorVec);
	coordinates(const self_type& mit);
	coordinates(std::initializer_list<dim_type> lst);

	self_type& operator=(self_type& rhs);

	virtual ~coordinates();

public:
	//////////////////////////////
	// Getter
	_NODISCARD std::string toString() const;
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
	inline dim_pointer data()
	{
		return this->coor_;
	}
	inline const dim_pointer data() const
	{
		return this->coor_;
	}
	//////////////////////////////

	//////////////////////////////
	// Operators
	// Comparison
	inline bool operator==(const self_type& rhs) const
	{
		return this->dSize_ == rhs.dSize_ && (memcmp(this->coor_, rhs.coor_, this->dSize_ * sizeof(dim_type)) == 0);
	}
	inline bool operator==(const dim_type* rhs) const
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
	inline bool operator!=(const self_type& rhs) const
	{
		return this->dSize_ != rhs.dSize_ || (memcmp(this->coor_, rhs.coor_, this->dSize_ * sizeof(dim_type)) != 0);
	}
	inline bool operator!=(const dim_type* rhs) const
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
	inline bool operator<(const self_type& rhs) const
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
	inline bool operator>(const self_type& rhs) const
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
	inline bool operator<=(const self_type& rhs) const
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
	inline bool operator>=(const self_type& rhs) const
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
	inline self_type& operator=(const self_type& mit)
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
	inline self_type& operator=(const dim_type* mit)
	{
		this->memcpyCoor(this->coor_, mit);

		return *this;
	}

	inline self_type& operator+= (const self_type& mit)
	{
		assert(this->dSize_ == mit.dSize_);

		for(dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] += mit[d];
		}

		return *this;
	}
	inline self_type& operator-= (const self_type& mit)
	{
		assert(this->dSize_ == mit.dSize_);

		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] -= mit[d];
		}

		return *this;
	}
	inline self_type& operator*= (const self_type& mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] *= mit[d];
		}

		return *this;
	}
	inline self_type& operator*= (const int64_t mit)
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

	inline self_type& operator/= (const self_type& mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] = intDivCeil(this->coor_[d], mit[d]);
		}

		return *this;
	}
	inline self_type& operator/= (const int64_t mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] /= mit;
		}

		return *this;
	}

	inline self_type& operator%= (const self_type& mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] %= mit[d];
		}

		return *this;
	}
	inline self_type& operator%= (const int64_t mit)
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			this->coor_[d] %= mit;
		}

		return *this;
	}

	// Move
	inline self_type& operator++()
	{
		this->coor_[this->dSize_ - 1]++;
		return *this;
	}
	inline self_type operator++(int)
	{
		coordinates tmp(*this);
		operator++();
		return tmp;
	}

	// Random access
	_NODISCARD inline _CONSTEXPR17 dim_reference operator[](_In_range_(0, dSize_ - 1) size_type pos)
		noexcept /* strengthened */
	{
		assert(pos < this->dSize_);
		return this->coor_[pos];
	}
	_NODISCARD inline constexpr dim_const_reference operator[](_In_range_(0, dSize_ - 1) size_type pos) const
		noexcept /* strengthened */
	{
		assert(pos < this->dSize_);
		return this->coor_[pos];
	}
	_NODISCARD inline _CONSTEXPR17 dim_reference at(_In_range_(0, dSize_ - 1) size_type pos)
	{
		assert(pos < this->dSize_);
		return this->coor_[pos];
	}
	_NODISCARD inline constexpr dim_const_reference at(_In_range_(0, dSize_ - 1) size_type pos) const
	{
		assert(pos < this->dSize_);
		return this->coor_[pos];
	}
	//////////////////////////////

public:
	inline coordinates divCoor(const coordinates& right)
	{
		coordinates output(*this);
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			output.coor_[d] /= right.coor_[d];
		}

		return output;
	}

protected:
	inline void memcpyCoor(dim_type* dest, const dim_type* src)
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

coordinates operator+ (const coordinates& left, const coordinates& right);
coordinates operator+ (const coordinates& left, const int right);

coordinates operator- (const coordinates& left, const coordinates& right);
coordinates operator- (const coordinates& left, const int right);

coordinates operator* (const coordinates& left, const coordinates& right);
coordinates operator* (const coordinates& left, const int right);

coordinates operator/ (const coordinates& left, const coordinates& right);
coordinates operator/ (const coordinates& left, const int right);

coordinates operator% (const coordinates& left, const coordinates& right);
coordinates operator% (const coordinates& left, const int right);

class coordinatesRange
{
public:
	using self_type = coordinatesRange;
	using size_type = size_t;
	using dim_type = coordinates::dim_type;
	using dim_pointer = coordinates::dim_pointer;
	using dim_const_pointer = coordinates::dim_const_pointer;
	using dim_reference = coordinates::dim_reference;
	using dim_const_reference = coordinates::dim_const_reference;

public:
	coordinatesRange(const size_type dSize = 0);
	coordinatesRange(const std::vector<dim_type>& sP, const std::vector<dim_type>& eP);
	coordinatesRange(std::initializer_list<dim_type> sP, std::initializer_list<dim_type> eP);

	coordinatesRange(const coordinates& sP, const coordinates& eP);
	coordinatesRange(const coordinates& eP);
	coordinatesRange(const self_type& mit);

	virtual ~coordinatesRange();

public:
	//////////////////////////////
	// Getter
	_NODISCARD inline constexpr size_type size() const noexcept
	{
		return this->dSize_;
	}

	_NODISCARD inline std::string toString() const
	{
		std::ostringstream ss;

		ss << this->sP_.toString() << "~" << this->eP_.toString();

		return ss.str();
	}

	inline coordinates getSp() const
	{
		return this->sP_;
	}
	inline coordinates getEp() const
	{
		return this->eP_;
	}

	inline coordinates& getSp()
	{
		return this->sP_;
	}
	inline coordinates& getEp()
	{
		return this->eP_;
	}

	inline coordinates width() const
	{
		coordinates width(this->eP_);
		width -= sP_;

		return width;
	}

	inline void move(const coordinates& offset)
	{
		this->sP_ += offset;
		this->eP_ += offset;
	}

	inline void moveTo(const coordinates& sP)
	{
		coordinates offset(sP);
		offset -= this->sP_;

		this->move(offset);
	}

	inline bool isIntersect(const self_type& rhs) const
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

	inline bool isFullyInside(const self_type& rhs) const
	{
		if (rhs.sP_ <= this->sP_ && this->eP_ <= rhs.eP_)
			return true;

		return false;
	}

public:
	inline bool operator==(const self_type& rhs) const
	{
		return this->dSize_ == rhs.dSize_ && sP_ == rhs.sP_ && eP_ == rhs.eP_;
	}

	inline bool operator!=(const self_type& rhs) const
	{
		return !(*this == rhs);
	}

	inline self_type& operator+=(const coordinates& rhs)
	{
		this->sP_ += rhs;
		this->eP_ += rhs;

		return *this;
	}

	inline self_type& operator-=(const coordinates& rhs)
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
	coordinates sP_;
	coordinates eP_;
};

coordinatesRange operator+ (const coordinatesRange& left, const coordinates& right);
coordinatesRange operator- (const coordinatesRange& left, const coordinates& right);

////////////////////////////////////////
// coordinatesIterator
//
class coordinatesIterator
{
public:
	using self_type = coordinatesIterator;
	using size_type = size_t;
	using dim_type = coordinates::dim_type;
	using dim_pointer = coordinates::dim_pointer;
	using dim_const_pointer = coordinates::dim_const_pointer;
	using dim_reference = coordinates::dim_reference;
	using dim_const_reference = coordinates::dim_const_reference;

public:
	coordinatesIterator(const size_type dSize, dim_const_pointer dims);
	coordinatesIterator(const std::vector<dim_type>& lst);
	coordinatesIterator(const coordinates space);
	coordinatesIterator(std::initializer_list<dim_type> lst);
	coordinatesIterator(const self_type& mit);

	virtual ~coordinatesIterator();

	inline self_type& operator=(const self_type& rhs)
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

public:
	virtual void setBasisDim(const unsigned int dim);

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
	_NODISCARD inline coordinates& coor() { return this->coor_; }
	_NODISCARD inline coordinates dims() const { return this->dims_; }
	_NODISCARD inline bool isEnd() const { return this->end_; }
	_NODISCARD inline bool isFront() const { return this->front_; }
	inline coordinates getSp() const
	{
		return this->sP_;
	}
	inline coordinates getEp() const
	{
		return this->eP_;
	}

	inline coordinates& getSp()
	{
		return this->sP_;
	}
	inline coordinates& getEp()
	{
		return this->eP_;
	}

	inline size_type getCapacity()
	{
		return this->seqCapacity_;
	}
	//////////////////////////////

	//////////////////////////////
	// Converting
	//////////////////////////////
	// pos: basis dim pos
	inline size_type posToSeq(const size_type pos)
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

		for (int d = (int)(this->dSize() - 1); d > (int)this->basisDim_; d--)
		{
			seq += (left % this->dims_[d]) * offset;
			left = left / this->dims_[d];
			offset *= this->dims_[d];
		}

		return seq;
	}
	inline size_type coorToSeq(const coordinates coor)
	{
		size_type seq = 0, offset = 1;
		for (dimensionId d = this->dSize() - 1; d != (dimensionId)-1; d--)
		{
			seq += coor[d] * offset;
			offset *= this->dims_[d];
		}
		return seq;
	}
	inline coordinates seqToCoor(const size_type seq)
	{
		//assert(seq <= this->getCapacity());

		size_type remain = seq;
		coordinates outCoor(this->dSize_);
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
	virtual void next();
	virtual void prev();

	virtual void moveTo(const coordinates& coor);

	//virtual void moveTo(const size_type seqPos)
	//{
	//	//this->seqPos
	//}

	virtual void moveToStart();
	virtual void moveToLast();
	//////////////////////////////

	//////////////////////////////
	// Operators
	//////////////////////////////
	// forward
	inline self_type& operator++()
	{
		this->next();
		return *this;
	}
	inline self_type operator++(int)
	{
		self_type tmp(*this);
		operator++();
		return tmp;
	}
	inline self_type& operator+=(const int& rhs)
	{
		for (int i = 0; i < rhs; i++)
		{
			operator++();
		}
		return *this;
	}

	// backward
	inline self_type& operator--()
	{
		this->prev();
		return *this;
	}
	inline self_type operator--(int)
	{
		self_type tmp(*this);
		operator--();
		return tmp;
	}
	inline self_type& operator-=(const int& rhs)
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
	inline void memcpyDim(dim_type* dest, const dim_type* src)
	{
		memcpy(dest, src, this->dSize() * sizeof(dim_type));
	}

	inline size_type initSeqCapacity()
	{
		this->seqCapacity_ = this->dims_[0];
		for (int i = 1; i < this->dSize(); i++)
		{
			this->seqCapacity_ *= this->dims_[i];
		}
		return this->seqCapacity_;
	}

	inline void setFrontEnd()
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
	inline size_t getDimOffset(const unsigned int dim)
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
	coordinates coor_;		// coordinates
	dimensionId basisDim_;		// current dimension

	coordinates dims_;		// original array dimension size
	coordinates sP_;		// range start point
	coordinates eP_;		// range end point
	size_type dSize_;			// number of dimensions

	bool end_;					// is iterator at the end
	bool front_;				// is iterator at the front

	size_type seqPos_;
	size_type seqCapacity_;
	size_type basisDimOffset_;

	// TODO:: replace sP_ to zero
	// static std::vector<dim_type> zeroP_;
};

template <typename Ty_ = element>
class itemIterator : virtual public coordinatesIterator
{
public:
	using self_type = itemIterator<Ty_>;
	using base_type = coordinatesIterator;

	using size_type = size_t;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	itemIterator(Ty_* ptr, const size_type dSize, dim_const_pointer dims)
		: base_type(dSize, dims), ptr_(ptr)
	{
	}

	itemIterator(Ty_* ptr, const coordinates dims)
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
	inline bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_ && this->seqPos_ == rhs.seqPos_; }
	inline bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_ || this->seqPos_ != rhs.seqPos_; }

	// Pointer
	virtual Ty_& operator*() { return *(ptr_ + this->seqPos_); }
	//element operator->() { return element((void*)(ptr_ + this->seqPos_ * this->eSize_), this->eType_); }
	//////////////////////////////

protected:
	Ty_* ptr_;					// pointer to element
								// set pointer type as char, to move one byte for each seqPos_
};

template <>
class itemIterator<element> : virtual public coordinatesIterator
{
public:
	using self_type = itemIterator<element>;
	using base_type = coordinatesIterator;

	using coordinates = coordinates;
	using size_type = size_t;
	using dim_type = coordinatesIterator::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	itemIterator(void* ptr, const eleType eType, const size_type dSize, dim_const_pointer dims)
		: base_type(dSize, dims), eType_(eType), eSize_(getEleSize(eType))
	{
		this->ptr_ = reinterpret_cast<char*>(ptr);
	}

	itemIterator(void* ptr, const eleType eType, const coordinates dims)
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

	inline eleType getEtype()
	{
		return this->eType_;
	}
	//////////////////////////////

	//////////////////////////////
	// Operators
	//////////////////////////////
	// Comparison
	inline bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_ && this->seqPos_ == rhs.seqPos_; }
	inline bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_ || this->seqPos_ != rhs.seqPos_; }

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

template <typename Ty_ = element>
class itemRangeIterator : public itemIterator<Ty_>
{
public:
	using self_type = itemRangeIterator<Ty_>;
	using base_type = itemIterator<Ty_>;

	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	itemRangeIterator(void* ptr, const size_type dSize, dim_const_pointer dim,
					  dim_const_pointer sP, dim_const_pointer eP)
		: base_type(ptr, dSize, dim), coordinatesIterator(dSize, dim)
	{
		assert(this->initCheckSpEp());
		this->sP_ = coordinates(dSize, sP);
		this->eP_ = coordinates(dSize, eP);
		this->moveToStart();
	}

	itemRangeIterator(void* ptr, eleType eType, const size_type dSize, dim_const_pointer dim,
					  const coordinatesRange& range)
		: base_type(ptr, eType, dSize, dim), coordinatesIterator(dSize, dim)
	{
		assert(this->initCheckSpEp());
		this->sP_ = range.getSp();
		this->eP_ = range.getEp();
		this->moveToStart();
	}

	itemRangeIterator(const self_type& mit) : base_type(mit), coordinatesIterator(mit)
	{
		this->sP_ = mit.sP_;
		this->eP_ = mit.eP_;
	}

	~itemRangeIterator()
	{
	}

protected:
	// Check if sP_ and eP_ has proper coordinates.
	inline bool initCheckSpEp()
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

template <>
class itemRangeIterator<element> : public itemIterator<element>
{
public:
	using self_type = itemRangeIterator<element>;
	using base_type = itemIterator<element>;

	using typename base_type::coordinates;
	using typename base_type::size_type;
	using typename base_type::dim_type;
	using typename base_type::dim_pointer;
	using typename base_type::dim_const_pointer;
	using typename base_type::dim_reference;
	using typename base_type::dim_const_reference;

public:
	itemRangeIterator(void* ptr, 
					  const eleType eType,
					  const size_type dSize, 
					  dim_const_pointer dim,
					  dim_const_pointer sP,
					  dim_const_pointer eP)
		: base_type(ptr, eType, dSize, dim), coordinatesIterator(dSize, dim)
	{
		assert(this->initCheckSpEp());
		this->sP_ = coordinates(dSize, sP);
		this->eP_ = coordinates(dSize, eP);
		this->moveToStart();
	}

	itemRangeIterator(void* ptr, 
					  const eleType eType,
					  const coordinates dim,
					  const coordinatesRange& range)
		: base_type(ptr, eType, dim), coordinatesIterator(dim)
	{
		assert(this->initCheckSpEp());
		this->sP_ = range.getSp();
		this->eP_ = range.getEp();
		this->moveToStart();
	}

	itemRangeIterator(const self_type& mit) 
		: base_type(mit), coordinatesIterator(mit)
	{
		this->sP_ = mit.sP_;
		this->eP_ = mit.eP_;
	}

	~itemRangeIterator()
	{
	}

protected:
	// Check if sP_ and eP_ has proper coordinates.
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

// Pic less coordinates value on each dimension
coordinates getInsideCoor(const coordinates& c1, const coordinates& c2);

// Pic greater coordiante value on each dimension
coordinates getOutsideCoor(const coordinates& c1, const coordinates& c2);

using coor = coordinates;
using pCoor = std::shared_ptr<coor>;

using coorItr = coordinatesIterator;
using pCoorItr = std::shared_ptr<coorItr>;

using coorRange = coordinatesRange;
using pCoorRange = std::shared_ptr<coorRange>;

using itemItr = itemIterator<element>;
using pItemItr = std::shared_ptr<itemItr>;

using itemRangeItr = itemRangeIterator<element>;
using pItemRangeItr = std::shared_ptr<itemRangeItr>;
}	// core
}	// msdb
#endif