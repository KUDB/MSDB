#pragma once
#ifndef _MSDB_WAVELETITERATOR_H_
#define _MSDB_WAVELETITERATOR_H_

#include <util/coordinate.h>
#include <system/exceptions.h>
#include <algorithm>

namespace msdb
{
namespace core
{
	template <typename Dty_, typename Ty_>
	class waveletIterator : public coorIterator<Dty_, Ty_>
	{
	public:
		using self_type = waveletIterator<Dty_, Ty_>;
		using size_type = size_t;
		using dim_type = Dty_;
		using dim_pointer = Dty_*;
		using dim_const_pointer = const Dty_*;
		using dim_reference = Dty_&;
		using dim_const_reference = const Dty_&;

		using value_type = Ty_;
		using value_pointer = Ty_*;
		using value_const_pointer = const Ty_*;
		using value_reference = Ty_&;
		using value_const_reference = const Ty_&;

	public:
		waveletIterator(value_pointer x, const size_type dSize, dim_const_pointer boundary, size_type maxLevel = 0) :
			coorIterator<Dty_, Ty_>(x, dSize, boundary)
		{
			this->ptrBegin_ = x;
			this->curLevel_ = 0;
			this->maxLevel_ = 0;
			/* curBandNum_
			 *     0  1
			 *   ┌──┬──┐
			 * 0 │ 0│ 1│
			 *   ├──┼──┤
			 * 1 │ 2│ 3│
			 *   └──┴──┘
			 */
			this->curBand_ = 0;

			// NOTE::BandDims and Band Size will be reassigned in setLevel function
			this->bandDims_ = new dim_type[1];
			this->bandSize_ = new size_type[1];

			this->bsP_ = new Dty_[this->dSize_];
			this->beP_ = new Dty_[this->dSize_];
			memset(this->bsP_, 0, sizeof(dim_type) * this->dSize_);
			memcpy(this->beP_, this->bandDims_, sizeof(dim_type) * this->dSize_);

			this->setLevel(maxLevel);
		}

		waveletIterator(const self_type& mit) : coorIterator<Dty_, Ty_>(mit)
		{
			memcpy(this->bandDims_, mit.bandDims_, sizeof(dim_type) * this->dSize_);
			memcpy(this->bsP_, mit.bsP_, sizeof(dim_type) * this->dSize_);
			memcpy(this->beP_, mit.beP_, sizeof(dim_type) * this->dSize_);
			memcpy(this->bandSize_, mit.bandSize_, sizeof(size_type) * (this->maxLevel_ + 1));

			this->maxLevel_ = mit.maxLevel_;

			this->curLevel_ = mit.curLevel_;
			this->curBand_ = mit.curBand_;

			this->ptrBegin_ = mit.ptrBegin_;
		}

		~waveletIterator()
		{
			delete[] this->bandDims_;
			delete[] this->bandSize_;
			delete[] this->bsP_;
			delete[] this->beP_;
		}

	public:
		virtual void next(const size_type dim)
		{
			const Dty_ offset = static_cast<Dty_>(this->getDimOffset(dim));
			if (this->coor_[dim] + 1 < std::min((dim_type)this->beP_[dim], (dim_type)this->eP_[dim]))
			{
				this->coor_[dim]++;
				this->ptr_ += offset;
			} else
			{
				if (dim > 0)
				{
					if (this->basisDim_ == dim - 1)
					{
						this->moveToNextBand();
						return;
					}
					this->next(dim - 1);
				} else
				{
					if (this->basisDim_ == this->dSize_ - 1)
					{
						this->moveToNextBand();
						return;
					}
					this->next(static_cast<unsigned int>(this->dSize_ - 1));
				}

				this->moveDimCoor(dim, std::max((dim_type)this->bsP_[dim], (dim_type)this->sP_[dim]), offset);

				/////
				//if ((dim > 0 && this->basisDim_ == dim - 1) || (dim == 0 && this->basisDim_ == this->dSize_ - 1))
				//{
				//	// If reach to the end of the current band
				//	// Then move to next band
				//	this->moveToNextBand();
				//} else
				//{
				//	if (dim > 0)
				//	{
				//		this->next(dim - 1);
				//	} else
				//	{
				//		this->next(this->dSize_ - 1);
				//	}
				//	//this->ptr_++;
				//	this->coor_[dim] = std::max(this->bsP_[dim], this->sP_[dim]);
				//}
			}
		}

		virtual void moveTo(const coordinate<Dty_>& coor)
		{
			if (this->dSize_ != coor.size())
			{
				throw std::exception("moveTo - different dimension size");
			}

			// Find which level and band
			size_type level = this->findLevel(coor);
			size_type band = this->findBand(coor, level);

			// Set current level, band, ptr.
			this->setCurLevel(level, false);
			this->setCurBand(band, false);
			this->ptr_ = this->ptrBegin_;
			this->ptr_ += this->getBandSize(level) * this->curBand_;
			this->ptr_ += this->posToSeq(coor);

			this->setCurCoor(coor);
		}

		void setLevel(size_type maxLevel)
		{
			this->maxLevel_ = maxLevel;

			this->calcBandDims();
			this->calcBandSize();
		}

		void setCurLevel(size_type level, bool adjustCoor = false)
		{
#ifndef NDEBUG
			assert(level <= this->maxLevel_);
#endif // !NDEBUG

			this->curLevel_ = level;

			if (adjustCoor)
			{
				if (level == this->maxLevel_)
				{
					this->setCurBand(0);
				} else
				{
					// band 0 exists in a maxLevel only.
					this->setCurBand(1);
				}
			}
		}

		// adjustCoor: make true if you want to move the coordinate it have.
		void setCurBand(size_type band, bool adjustCoor = false)
		{
#ifndef NDEBUG
			assert(band != 0 || (band == 0 && this->curLevel_ == this->maxLevel_));
			assert(band < pow(2, this->dSize_));
#endif // !NDEBUG

			this->curBand_ = band;
			this->calcBandBoundary();

			if (adjustCoor && !isInside(this->coor(), this->bsP_, this->beP_))
			{
				this->moveTo(coordinate<Dty_>(this->dSize_, this->bsP_));
			}
		}

		void setCurCoor(const coordinate<Dty_>& coor)
		{
			for (dim_type d = 0; d < this->dSize_; d++)
			{
				this->coor_[d] = coor[d];
			}
		}

		_NODISCARD size_type curBand()
		{
			return this->curBand_;
		}

		_NODISCARD size_type curLevel()
		{
			return this->curLevel_;
		}

		bool isInside(const coordinate<Dty_>& coor, dim_type* sP, dim_type* eP)
		{
			for (size_type d = 0; d < coor.size(); d++)
			{
				if (coor[d] < sP[d] || eP[d] <= coor[d])
				{
					return false;
				}
			}

			return true;
		}

		// Move to first child of the current coordinate
		void moveToChild()
		{
			assert(this->curLevel_ > 0);
			assert(this->curBand_ != 0 || (this->curBand_ == 0 && this->curLevel_ == this->maxLevel_));

			coordinate<Dty_> newCoor(this);
			if (this->curBand_ == 0)
			{
				size_type odd = 0;
				for (size_type d = this->dSize_ - 1; d + 1 > 0; d--)
				{
					newCoor[d] = ((int)this->coor[d] / 2) * 2;
					if (this->coor[d] & 0x1)
					{
						newCoor[d] += this->bandSize_[d];
						odd++;
					}
				}
				assert(odd != 0);	// some coor[d] is odd
			} else
			{
				// Other level bands
				for (size_type d = this->dSize_ - 1; d + 1 > 0; d--)
				{
					newCoor[d] = this->coor[d] * 2;
				}
			}

			this->moveTo(newCoor);
		}

		void moveToNextSibling()
		{
			size_type cur = 0;
			for (size_type d = 0; d < this->dSize_; d++)
			{
				cur += (this->coor[d] % 2);

				if (d != this->dSize_ - 1)
					cur *= 2;
			}
			size_type next = cur + 1;
			assert(next != pow(2, this->dSize_));

			coordinate<Dty_> newCoor(this);
			for (size_type d = this->dSize_ - 1; d + 1 > 0; d--)
			{
				newCoor[d] = ((next % 2) - (cur % 2));
				newCoor[d] += this->coor[d];

				next /= 2;
				cur /= 2;
			}

			this->moveTo(newCoor);
		}

		void moveToChildBand()
		{
			assert(this->curBand_ != 0);
			assert(this->curLevel_ > 0);

			this->setCurLevel(this->curLevel_ - 1, false);
			this->setCurBand(this->curBand_, true);
		}

		dim_const_pointer getBandStartPoint()
		{
			return this->bsP_;
		}

		dim_const_pointer getBandEndPoint()
		{
			return this->beP_;
		}

	protected:
		void moveToNextBand()
		{
			if (this->curBand_ + 1 >= pow(2, this->dSize_))
			{
				if (this->curLevel_ == 0)
				{
					return;
				}

				// Except maxLevel, curBandNum starts from 1.
				this->setCurLevel(this->curLevel_ - 1, false);
				this->setCurBand(1, false);
			} else
			{
				this->setCurBand(this->curBand_ + 1, false);
			}

			this->moveTo(coordinate<Dty_>(this->dSize_, this->bsP_));
		}

		void moveToPrevBand()
		{
			// Band num 0 only exists in maxLevel.
			// In other levels, band num starts from 1.
			if (this->curBand_ <= 1)
			{
				if (this->curLevel_ >= this->maxLevel_)
				{
					this->curBand_ = 0;
					return;
				}

				this->curBand_ = pow(2, this->dSize_) - 1;
				this->curLevel_++;
			} else
			{
				this->curBand_--;
			}

			this->moveTo(coordinate<Dty_>(this->dSize_, this->beP_));
		}

		inline void moveDimCoor(const size_type dim, const Dty_ coor, const Dty_ offset)
		{
			if (coor >= this->coor_[dim])
			{
				this->ptr_ += (coor - this->coor_[dim]) * offset;
			} else
			{
				this->ptr_ -= (this->coor_[dim] - coor) * offset;
			}
			this->coor_[dim] = coor;
		}

		size_t getDimOffset(_In_range_(0, dSize_ - 1) const unsigned int dim)
		{
			if (dim == this->basisDim_)
			{
				return this->offset_;
			}

			size_t offset = 1;
			dim_type* curBandDims = this->getBandDims(this->curLevel_);
			for (size_type i = this->dSize_ - 1; i > dim; i--)
			{
				offset *= curBandDims[i];
			}

			//std::cout << "dim : " << dim << ", getOffset: " << offset << std::endl;
			return offset;
		}

		void calcBandBoundary()
		{
			dim_pointer dims = this->getBandDims(this->curLevel_);
			memset(this->bsP_, 0, sizeof(dim_type) * this->dSize_);
			memset(this->beP_, 0, sizeof(dim_type) * this->dSize_);


			for (size_type d = 0; d < this->dSize_; d++)
			{
				if ((this->curBand_ >> (this->dSize_ - 1 - d)) & 0x1)
				{
					this->bsP_[d] = dims[d];
					this->beP_[d] = dims[d] << 1;
				} else
				{
					this->beP_[d] = dims[d];
				}
			}
		}

		// Band dimensions
		void calcBandDims()
		{
			dim_pointer temp = new dim_type[this->dSize_ * (this->maxLevel_ + 1)];

			// Band dim for level 0 is half of data dims
			for (size_type d = 0; d < this->dSize_; d++)
			{
				temp[d] = this->dims_[d] >> 1;
			}

			// Rests are half of previous band dim
			for (size_type l = 1; l <= this->maxLevel_; l++)
			{
				for (size_type d = 0; d < this->dSize_; d++)
				{
					temp[this->dSize_ * l + d] = (temp[this->dSize_ * (l - 1) + d] >> 1);
				}
			}

			// Replace bandDims
			delete[] this->bandDims_;
			this->bandDims_ = temp;
		}

		dim_pointer getBandDims(size_type level)
		{
			assert(level <= this->maxLevel_);
			return this->bandDims_ + (this->dSize_ * level);
		}

		// Number of element in a band
		void calcBandSize()
		{
			size_type* temp = new size_type[this->maxLevel_ + 1];
			for (size_type l = 0; l <= this->maxLevel_; l++)
			{
				dim_type* curBandDim = this->getBandDims(l);
				temp[l] = 1;
				for (size_type d = 0; d < this->dSize_; d++)
				{
					temp[l] *= curBandDim[d];
				}
			}

			delete[] this->bandSize_;
			this->bandSize_ = temp;
		}

		size_type getBandSize(size_type level)
		{
			assert(level <= this->maxLevel_);
			return this->bandSize_[level];
		}

		// TODO::erase posToSeq func (here and parent)
		virtual size_t posToSeq(const size_type pos)
		{
			size_type seq = 0;

			_MSDB_EXCEPTIONS(MSDB_EC_LOGIC_ERROR, MSDB_ER_NOT_IMPLEMENTED);

			return seq;
		}

		virtual size_type posToSeq(const coordinate<Dty_>& coor)
		{
			size_type seq = 0;
			size_type offset = 1;
			dim_type* curBandDim = this->getBandDims(this->curLevel_);

			for (size_type d = this->dSize_ - 1; d != (size_type)-1; d--)
			{
				if (size_type(coor[d]) / size_type(curBandDim[d]))
				{
					seq += offset * (size_type)(coor[d] - curBandDim[d]);
				} else
				{
					seq += offset * (size_type)coor[d];
				}

				offset *= curBandDim[d];
			}

			return seq;
		}

		virtual size_type findLevel(const coordinate<Dty_>& coor)
		{
			size_type level = 0;
			dim_type* levelBoundary;
			for (level = this->maxLevel_; level > 0; level--)
			{
				levelBoundary = this->getBandDims(level - 1);
				bool flag = true;
				for (size_type d = this->dSize_ - 1; d != (size_type)-1; d--)
				{
					if (coor[d] > levelBoundary[d])
					{
						flag = false;
						break;
					}
				}

				if (flag)
				{
					break;
				}
			}
			return level;
		}

		virtual size_type findBand(const coordinate<Dty_>& coor, size_type level)
		{
			size_type band = 0;
			dim_type* curBandDim = this->getBandDims(level);
			for (size_type d = this->dSize_ - 1; d != (size_type)-1; d--)
			{
				band |= (size_type(coor[d]) / size_type(curBandDim[d])) << (this->dSize_ - 1 - d);
			}
			return band;
		}

	protected:
		dim_pointer bandDims_;
		dim_pointer bsP_;
		dim_pointer beP_;
		size_type* bandSize_;
		size_type maxLevel_;

		size_type curLevel_;
		size_type curBand_;

		value_pointer ptrBegin_;

	private:
		//FRIEND_TEST(caIterators, caWTIteratorMoveTo);
		//FRIEND_TEST(caIterators, caWTIteratorCalcBandDim);
	};

	// Cannot over band boundaries and level boundaries
	template <typename Dty_, typename Ty_>
	class waveletRangeIterator : public waveletIterator<Dty_, Ty_>
	{
		using self_type = waveletRangeIterator<Dty_, Ty_>;
		using size_type = size_t;
		using dim_type = Dty_;
		using dim_pointer = Dty_*;
		using dim_const_pointer = const Dty_*;
		using dim_reference = Dty_&;
		using dim_const_reference = const Dty_&;

		using value_type = Ty_;
		using value_pointer = Ty_*;
		using value_const_pointer = const Ty_*;
		using value_reference = Ty_&;
		using value_const_reference = const Ty_&;

	public:
		waveletRangeIterator(value_pointer x, const size_type dSize, dim_const_pointer boundary,
							 dim_const_pointer sP, dim_const_pointer eP, size_type maxLevel = 0) :
			waveletIterator<Dty_, Ty_>(x, dSize, boundary, maxLevel)
		{
			this->eP_ = new Dty_[dSize];
			memcpy(this->sP_, sP, dSize * sizeof(dim_type));
			memcpy(this->eP_, eP, dSize * sizeof(dim_type));

			this->moveToStart();
		}

		waveletRangeIterator(const self_type& mit) : waveletIterator<Dty_, Ty_>(mit)
		{
			this->eP_ = new Dty_[mit.dSize_];
			memcpy(this->eP_, mit.eP_, mit.dSize_ * sizeof(dim_type));
		}

		~waveletRangeIterator()
		{
			delete[] this->eP_;
		}

	protected:
		// Check if sP_ and eP_ has proper range.
		bool checkSpEp()
		{
			for (int i = 0; i < this->dSize_; i++)
			{
				if (this->sP_[i] > this->eP_[i])
				{
					return false;
				}
			}

			return true;
		}

		// Check if eP_ and boundary_ has proper range.
		bool checkEpBoundary()
		{
			for (int i = 0; i < this->dSize_; i++)
			{
				if (this->dims_[i] < this->eP_[i])
				{
					return false;
				}
			}

			return false;
		}
	};
}		// core
}		// msdb
#endif