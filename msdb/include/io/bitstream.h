#pragma once
#ifndef _MSDB_BITSTRINGSTREAM_H_
#define _MSDB_BITSTRINGSTREAM_H_

#include <pch.h>

namespace msdb
{
namespace core
{
	union bitchar
	{
		std::bitset<8> b;
		char c;
	};

	class iobs_base
	{
		using size_type = size_t;

	public:
		iobs_base() {}

		////////////////////////////////////////
		// Set options
		void width(size_type w)
		{
			this->bitWidth = w;
		}

		void initWidth()
		{
			this->bitWidth = 0;
		}

	protected:
		_NODISCARD inline size_type getWidth(const size_type length) const
		{
			return this->bitWidth ? std::min(length, this->bitWidth) : length;
		}

	protected:
		static const unsigned char mask[9];
		static const unsigned char rmask[9];

	protected:
		size_t bitWidth = 0;	// default: 0
	};

	template <class _Block, class _Traits>
	class vector_iobs : public iobs_base
	{
	public:
		using container_type = std::vector<_Block>;
		using size_type = size_t;
		using block_type = _Block;

		using traits_type = _Traits;
		using int_type = typename _Traits::int_type;
		using pos_type = typename _Traits::pos_type;

		// off_type?
		// using pos_type = std::conditional_t < sizeof(_Block) < 31, unsigned char, unsigned int > ;

		vector_iobs(container_type* mem) : _container(mem), iobs_base() {};

	public:
		virtual void flush()
		{
			if(this->_container)
			{
				this->_container->clear();
			}
		}

	private:
		container_type* _container;		// memory space to store the data

	protected:
		void init(container_type* mem)
		{
			this->_container = mem;
		}

		vector_iobs() : _container(nullptr) {};
	};

	template <class _Block, class _Traits, size_t _BlockBytes = sizeof(_Block), size_t _BlockBits = sizeof(_Block)* CHAR_BIT>
	class vector_obitstream : virtual public vector_iobs<_Block, _Traits>
	{
	public:
		using _myBase = vector_iobs<_Block, _Traits>;

		using container_type = std::vector<_Block>;
		using block_bitset_type = std::bitset<sizeof(_Block) * CHAR_BIT>;
		using size_type = size_t;
		using block_type = _Block;

		using traits_type = _Traits;
		using int_type = typename _Traits::int_type;
		using pos_type = typename _Traits::pos_type;

	public:
		vector_obitstream(container_type* mem) : _container(mem)
		{
			_myBase::init(mem);
		}

		// This function has endian problem (byte order) with numbers
		size_type fillArray(const unsigned char* c, const size_type length)
		{
			size_type i = 0, remain = this->getWidth(length);

			// Head Bit + Byte Encoding
			while (remain > CHAR_BIT)
			{
				remain -= this->fill((static_cast<unsigned __int16>(*(c + i)) >> ((remain - 8) % CHAR_BIT)) & 0xFF, CHAR_BIT);
				i = (length - remain) / CHAR_BIT;
			}

			// Tail Bit Encoding
			while (remain > 0)
			{
				remain -= this->fill(static_cast<unsigned char>(*(c + i)) >> (CHAR_BIT - remain), remain);
			}

			return length - remain;
		}

		size_type fillChar(const unsigned char c, const size_type length = CHAR_BIT)
		{
			size_type remain = this->getWidth(length);

			while (remain > 0)
			{
				remain -= this->fill(c & this->mask[remain], remain);
			}

			return length - remain;
		}

		size_type fillLongLong(const unsigned long long c, const size_type length = CHAR_BIT * sizeof(long long))
		{
			size_type remain = this->getWidth(length);

			// Head Bit + Byte Encoding
			while (remain >= CHAR_BIT)
			{
				remain -= this->fill(static_cast<unsigned char>((c >> (remain - CHAR_BIT)) & 0xFF), CHAR_BIT);
			}

			// Tail Bit Encoding
			while (remain > 0)
			{
				remain -= this->fill(c & this->mask[remain], remain);
			}

			return length - remain;
		}

		// Return total number of used bits
		_NODISCARD size_type sizebits() const noexcept
		{
			if (this->bitPos)
			{
				return (this->_container->size() - 1) * _BlockBits * CHAR_BIT + this->bitPos;
			}
			return this->_container->size() * _BlockBits;
		}

		// Return total number of bytes capacity
		_NODISCARD size_type capacity() const noexcept
		{
			return this->_container->size() * _BlockBytes;
		}

		_NODISCARD const _Block* data() const noexcept
		{
			return this->_container->data();
		}

		_NODISCARD size_type getOutBitPos() const noexcept
		{
			return (this->_container->size() - 1) * _BlockBytes * CHAR_BIT + this->bitPos;
		}

		_NODISCARD size_type getOutBitPosInBlock() const noexcept
		{
			return this->bitPos;
		}

		_NODISCARD size_type getOutBlockPos() const noexcept
		{
			return (this->_container->size() - 1);
		}

		_NODISCARD uint64_t getOutLastBlock() const
		{
			return static_cast<uint64_t>(this->endBlock->to_ulong() & 0xFF);
		}

		virtual void flush()
		{
			_myBase::flush();
			this->bitPos = 0;
			this->bitWidth = 0;
		}

	protected:
		/*
		* Here is an example of a bitstream with 4 bits block.
		*
		*
		* ?뚢?????????????
		* ??pos]      ??
		* ?귘뼹 ->		  ??
		* ?쒋???р???р???р???ㅲ뵆???р???р???р????
		* ??3??2??1??0?귘봻 3??2??1??0??bitset<4>
		* ?쒋???닳???닳???닳???ㅲ뵜???닳???닳???닳????
		* ??[0]	      ?귘봻 [1]	   ??vector<bitset<4>>
		* ?붴????????????섃뵒?????????????
		* ?뚢???р???р???р???먥뵆???р???р???р????
		* ??0??1??2??3?귘봻 4??5??6??7??global bit order
		* ?붴???닳???닳???닳???섃뵒???닳???닳???닳????
		*/
		pos_type fill(const unsigned char c, pos_type length = CHAR_BIT)
		{
			assert(0 < length && length <= CHAR_BIT);

			//if (this->bitPos == 0)
			//{
			//	this->addNewBlock();
			//}

			if (this->bitPos % CHAR_BIT != 0 || length % CHAR_BIT != 0)
			{
				return this->fillBits(c, length);
			} else
			{
				return this->fillByte(c);
			}
		}

	protected:
		void addNewBlock()
		{
			this->_container->push_back(0x0);
			this->endBlock = reinterpret_cast<block_bitset_type*>(&this->_container->back());
		}

	private:
		// out: number of filled bits
		unsigned char fillBits(const unsigned char c, const pos_type length)
		{
			pos_type i = length - 1;
			pos_type last = std::max((pos_type)(length - (CHAR_BIT - (char)(this->bitPos % CHAR_BIT))), (pos_type)0);

			for (; i != (pos_type)(-1) && i >= last; i--, this->bitPos++)
			{
				if ((c >> i) & 0x1)
				{
					(this->endBlock)->set(_BlockBits - this->bitPos - 1);
				}
			}
			this->bitPos %= _BlockBits;

			if (this->bitPos == 0)
			{
				this->addNewBlock();
			}

			return (unsigned char)(length - last);
		}

		// out: number of filled bits
		unsigned char fillByte(const unsigned char c)
		{
			(*this->endBlock) |= (static_cast<size_type>(c) << (_BlockBits - this->bitPos - CHAR_BIT));
			//this->bitPos = this->bitPos + CHAR_BIT;
			this->bitPos = (this->bitPos + CHAR_BIT) % _BlockBits;

			if (this->bitPos == 0)
			{
				this->addNewBlock();
			}

			return CHAR_BIT;
		}

	protected:
		pos_type bitPos = 0;			// current bit in a block
		block_bitset_type* endBlock = nullptr;

	private:
		container_type* _container;
	};

	template <class _Block, class _Traits, size_t _BlockBytes = sizeof(_Block), size_t _BlockBits = sizeof(_Block)* CHAR_BIT>
	class vector_ibitstream : virtual public vector_iobs<_Block, _Traits>
	{
	public:
		using _myBase = vector_iobs<_Block, _Traits>;

		using container_type = std::vector<_Block>;
		using block_bitset_type = std::bitset<sizeof(_Block) * CHAR_BIT>;
		using size_type = size_t;
		using block_type = _Block;

		using traits_type = _Traits;
		using int_type = typename _Traits::int_type;
		using pos_type = typename _Traits::pos_type;

		vector_ibitstream(container_type* mem) : _container(mem)
		{
			_myBase::init(mem);
		};

	public:
		unsigned char getChar(size_type length = CHAR_BIT)
		{
			unsigned char out = 0x00;
			size_type remain = this->getWidth(length);

			while (remain > 0)
			{
				unsigned char temp = 0x00;
				size_type move = this->get(temp, remain);
				if(!move)
				{
					// EOF, return output
					return out;
				}
				remain -= move;
				out |= (temp << remain);
			}

			return out;
		}

		unsigned long long getLongLong(size_type length = CHAR_BIT * sizeof(long long))
		{
			unsigned long long out = 0x00;
			size_type remain = this->getWidth(length);

			// Head bits -> get by byte
			while (remain >= CHAR_BIT)
			{
				unsigned char temp = 0x00;
				size_type move = this->get(temp, CHAR_BIT);
				if(!move)
				{
					return out;
				}
				remain -= move;
				out |= (temp << remain);
			}

			// Tail bits -> get by bit length
			while (remain > 0)
			{
				unsigned char temp = 0x00;
				size_type move = this->get(temp, remain);
				if(!move)
				{
					return out;
				}
				remain -= move;
				out |= (temp << remain);
			}

			return out;
		}

		bool eof()
		{
			// this->bitPos == _BlockBytes ?
			if ((pos_type)this->_container->size() <= this->blockPos)
			{
				return true;
			}

			return false;
		}

		_NODISCARD size_type getInBitPos() const noexcept
		{
			return this->blockPos * _BlockBytes * CHAR_BIT + this->bitPos;
		}

		_NODISCARD size_type getInBitPosInBlock() const noexcept
		{
			return this->bitPos;
		}

		_NODISCARD size_type getInBlockPos() const noexcept
		{
			return this->blockPos;
		}

		_NODISCARD uint64_t getInFrontBlock() const
		{
			return static_cast<uint64_t>(this->frontBlock->to_ulong() & 0xFF);
		}

		_NODISCARD size_type getBlockBytes() const noexcept
		{
			return _BlockBytes;
		}

		// Return total number of byte capacity
		_NODISCARD size_type capacity() const noexcept
		{
			return this->_container->size() * _BlockBytes;
		}

		_NODISCARD const _Block* data() const noexcept
		{
			return this->_container->data();
		}

		void resize(size_t bytes)
		{
			this->_container->resize(bytes);
		}

		virtual void flush()
		{
			_myBase::flush();
			this->bitPos = 0;
			this->blockPos = 0;
			this->frontBlock = nullptr;
		}

	protected:
		pos_type get(unsigned char& out, const pos_type length = CHAR_BIT)
		{
			assert(length <= CHAR_BIT);
			if (this->eof())
			{
				return 0;
			}

			if (this->frontBlock == nullptr)
			{
				this->frontBlock = reinterpret_cast<block_bitset_type*>(&this->_container->front());
			}

			pos_type move = 0;
			if (this->bitPos % CHAR_BIT != 0 || length % CHAR_BIT != 0)
			{
				move = this->getBits(out, length);
			} else
			{
				move = this->getByte(out);
			}

			if (move && this->bitPos == 0)
			{
				this->moveToNextInputBlock();
			}

			return move;
		}

	private:
		unsigned char getBits(unsigned char& out, const pos_type length)
		{
			pos_type possible = std::min(static_cast<pos_type>(_BlockBits - this->bitPos), static_cast<pos_type>(length));

			out |= (((*this->frontBlock << this->bitPos).to_ulong() & this->rmask[possible]) >> (_BlockBits - possible));
			auto blockBits = (*this->frontBlock << this->bitPos).to_ulong();

			this->bitPos = (this->bitPos + possible) % _BlockBits;

			return (unsigned char)possible;
		}

		unsigned char getByte(unsigned char& out)
		{
			out |= ((*this->frontBlock << (_BlockBits - this->bitPos - CHAR_BIT)).to_ulong() & 0xFF);
			this->bitPos = (this->bitPos + CHAR_BIT) % _BlockBits;

			return CHAR_BIT;
		}

		void moveToNextInputBlock()
		{
			this->blockPos++;
			this->bitPos = 0;

			if (this->eof())
			{
				this->blockPos = this->_container->size();
				return;
			}

			this->frontBlock = reinterpret_cast<block_bitset_type*>(&this->_container->at(this->blockPos));
		}

	public:
		void jumpBits(size_type numBits)
		{
			pos_type possible = std::min(static_cast<pos_type>(_BlockBits - this->bitPos), static_cast<pos_type>(numBits));
			if (this->bitPos + possible >= _BlockBits)
			{
				this->blockPos++;
			}
			this->bitPos = (this->bitPos + possible) % _BlockBits;

			numBits -= possible;
			if (numBits)
			{
				this->blockPos = std::min(this->blockPos + numBits / _BlockBits, this->_container->size());
				this->bitPos = numBits % _BlockBits;
			}

			this->frontBlock = reinterpret_cast<block_bitset_type*>(&this->_container->at(this->blockPos));
		}

	protected:
		pos_type blockPos = 0;		// current byte in a stream
		pos_type bitPos = 0;		// current bit in a byte
		block_bitset_type* frontBlock = nullptr;

	private:
		container_type* _container;
	};

	template <class _Block, class _Traits, size_t _BlockBytes = sizeof(_Block), size_t _BlockBits = sizeof(_Block)* CHAR_BIT>
	class vector_iobitstream : public vector_ibitstream<_Block, _Traits>,
		public vector_obitstream<_Block, _Traits>
	{
	public:
		using _myIs = vector_ibitstream<_Block, _Traits>;
		using _myOs = vector_obitstream<_Block, _Traits>;

		using container_type = std::vector<_Block>;
		using block_bitset_type = std::bitset<sizeof(_Block) * CHAR_BIT>;
		using size_type = size_t;
		using block_type = _Block;

		using traits_type = _Traits;
		using int_type = typename _Traits::int_type;
		using pos_type = typename _Traits::pos_type;

		vector_iobitstream()
			: _myIs(_STD addressof(this->_concreateContainer)), _myOs(_STD addressof(this->_concreateContainer))
		{
			_myOs::addNewBlock();
		}

	public:
		// Return total number of byte capacity
		_NODISCARD size_type capacity() const noexcept
		{
			return this->_concreateContainer.size() * _BlockBytes;
		}

		_NODISCARD _Block* data() noexcept
		{
			return this->_concreateContainer.data();
		}

		void resize(size_t bytes)
		{
			this->_concreateContainer.resize(bytes);
		}

		virtual void flush()
		{
			_myIs::flush();
			_myOs::flush();
		}

		void print()
		{
			std::stringstream ss;
			ss << std::hex;
			for(size_t i = 0; i < this->_concreateContainer.size(); ++i)
			{
				ss << static_cast<uint8_t>(_concreateContainer[i]);
			}
			BOOST_LOG_TRIVIAL(debug) << ss.str();
		}

	protected:
		container_type _concreateContainer;
	};

	// STRUCT TEMPLATE _BitSmanip
	// Copy from _Smanip in <iomanip>
	// iomanip requires that _Arg inherits 'ios_base'
	template <class _Arg>
	struct _BitSmanip
	{ // store function pointer and argument value
		_BitSmanip(iobs_base& (__cdecl* _Left)(iobs_base&, _Arg), _Arg _Val) : _Pfun(_Left), _Manarg(_Val) {}

		iobs_base& (__cdecl* _Pfun)(iobs_base&, _Arg); // the function pointer
		_Arg _Manarg; // the argument value
	};

	template <class  _Block, class _Traits, size_t _Bits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const std::bitset<_Bits>& _Right)
	{
		int length = _Bits;

		// Head Bit + Byte Encoding
		while (length > 8)
		{
			length -= _Ostr.fillChar((unsigned char)(_Right.to_ulong() >> (length - 8)) & 0xFF, 8);
		}

		// Tail Bit Encoding
		while (length > 0)
		{
			length -= _Ostr.fillChar((unsigned char)(_Right.to_ulong()) & 0xFF, length);
		}

		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const char _val)
	{
		_Ostr.fillChar(static_cast<const unsigned char>(_val), CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const int8_t _val)
	{
		_Ostr.fillChar(static_cast<const unsigned char>(_val), CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const uint8_t _val)
	{
		_Ostr.fillChar(_val, CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const int16_t _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(uint16_t)* CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const uint16_t _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(uint16_t)* CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const int32_t _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int32_t)* CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const uint32_t _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(uint32_t)* CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const int64_t _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(int64_t) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const uint64_t _val)
	{
		_Ostr.fillLongLong(_val, sizeof(uint64_t) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const double _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(double)* CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const float _val)
	{
		_Ostr.fillLongLong(static_cast<const unsigned long long>(_val), sizeof(float) * CHAR_BIT);
		return _Ostr;
	}

	template <class _Block, class _Traits, class _Arg>
	vector_obitstream<_Block, _Traits>& operator<<(vector_obitstream<_Block, _Traits>& _Ostr, const _BitSmanip<_Arg>& _BitManip)
	{
		(*_BitManip._Pfun)(_Ostr, _BitManip._Manarg);
		return _Ostr;
	}

	//////////////////////////////
	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, char& _val)
	{
		_val = (char)_is.getChar();
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, int8_t& _val)
	{
		_val = (char)_is.getChar();
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, unsigned char& _val)
	{
		_val = (unsigned char)_is.getChar();
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, int16_t& _val)
	{
		_val = (int16_t)_is.getLongLong(sizeof(int16_t) * CHAR_BIT);
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, uint16_t& _val)
	{
		_val = (uint16_t)_is.getLongLong(sizeof(uint16_t) * CHAR_BIT);
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, int32_t& _val)
	{
		_val = (int32_t)_is.getLongLong(sizeof(int32_t) * CHAR_BIT);
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, uint32_t& _val)
	{
		_val = (uint32_t)_is.getLongLong(sizeof(uint32_t) * CHAR_BIT);
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, int64_t& _val)
	{
		_val = (int64_t)_is.getLongLong(sizeof(int64_t) * CHAR_BIT);
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, uint64_t& _val)
	{
		_val = (uint64_t)_is.getLongLong(sizeof(uint64_t) * CHAR_BIT);
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, float& _val)
	{
		_val = (float)_is.getLongLong(sizeof(float) * CHAR_BIT);
		return _is;
	}

	template <class _Block, class _Traits>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, double& _val)
	{
		_val = (double)_is.getLongLong(sizeof(double) * CHAR_BIT);
		return _is;
	}

	// Change parameter 2
	// _BitSmanip<_Arg>& _BitManip -> _BitSmanip<_Arg> _BitManip
	// Compile error
	template <class _Block, class _Traits, class _Arg>
	vector_ibitstream<_Block, _Traits>& operator>>(vector_ibitstream<_Block, _Traits>& _is, _BitSmanip<_Arg> _BitManip)
	{
		(*_BitManip._Pfun)(_is, _BitManip._Manarg);
		return _is;
	}

	template <class _Elem, class _IntType>
	struct _BlockTraits
	{
		using char_type = _Elem;
		using int_type = _IntType;
		using pos_type = long long;

		static _CONSTEXPR17 void assign(_Elem& _Left, const _Elem& _Right) noexcept
		{
			_Left = _Right;
		}

		_NODISCARD static constexpr bool eq(const _Elem& _Left, const _Elem& _Right) noexcept
		{
			return _Left == _Right;
		}

		_NODISCARD static constexpr bool lt(const _Elem& _Left, const _Elem& _Right) noexcept
		{
			return _Left < _Right;
		}

		_NODISCARD static constexpr _Elem to_char_type(const int_type& _Meta) noexcept
		{
			return static_cast<_Elem>(_Meta);
		}

		_NODISCARD static constexpr int_type to_int_type(const _Elem& _Ch) noexcept
		{
			return static_cast<int_type>(_Ch);
		}

		_NODISCARD static constexpr bool eq_int_type(const int_type& _Left, const int_type& _Right) noexcept
		{
			return _Left == _Right;
		}
	};

	template <class _Elem>
	struct _charBlockTraits : _BlockTraits<char, unsigned long>
	{

	};

	template <>
	struct _charBlockTraits<char> : _BlockTraits<char, unsigned char> {};

	template <>
	struct _charBlockTraits<char16_t> : _BlockTraits<char, unsigned long> {};

	template <>
	struct _charBlockTraits<char32_t> : _BlockTraits<char, unsigned int> {};

	_MRTIMP2 _BitSmanip<std::streamsize> __cdecl setw(std::streamsize);
	using bstream = vector_iobitstream<char, _charBlockTraits<char>>;
	using u16bstream = vector_iobitstream<char16_t, _charBlockTraits<char16_t>>;
	using u32bstream = vector_iobitstream<char32_t, _charBlockTraits<char32_t>>;

	using pBstream = std::shared_ptr<bstream>;

	//const unsigned char iobs_base::mask[9] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };
	//const unsigned char iobs_base::rmask[9] = { 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00 };
}		// core
}		// msdb
#endif	// _MSDB_BITSTRINGSTREAM_H_