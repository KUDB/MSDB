#pragma once
#ifndef _MSDB_FIXED_HUFFMAN_CODER_H_
#define _MSDB_FIXED_HUFFMAN_CODER_H_

#include <pch.h>
#include <boost/math/distributions/normal.hpp>
#include <compression/huffmanCode.h>
#include <util/singleton.h>

namespace msdb
{
namespace core
{
class iFixedHuffmanCoder
{
public:
	virtual void encode(bstream& out, const void* in, size_t len) = 0;
	virtual size_t decode(void* outData, size_t lenOut, bstream& in) = 0;
	virtual size_t getCodeBitWidth() const = 0;
};

template <typename codeType, typename symbolType>
class fixedHuffmanCoder1ByteSymbolBase : public huffmanCoder<codeType, symbolType>, public iFixedHuffmanCoder
{
public:
	using parentType = huffmanCoder<codeType, symbolType>;
	using nodeType = huffmanCoder<codeType, symbolType>::huffmanNode;

	using symbolType_ = symbolType;
	using codeType_ = codeType;

public:
	fixedHuffmanCoder1ByteSymbolBase()
		: huffmanCoder<codeType, symbolType>()
	{
		this->initHuffmanCode();
	}

	fixedHuffmanCoder1ByteSymbolBase(size_t bits)
		: huffmanCoder<codeType, symbolType>(bits)
	{
		this->initHuffmanCode();
	}

	~fixedHuffmanCoder1ByteSymbolBase()
	{

	}

protected:
	void initHuffmanCode()
	{
		std::vector<size_t> freq(pow(2, this->bits_), 0);
		this->initFrequencies(freq);
		this->buildTree(freq);
		buildEncodeDecodeTable();

		BOOST_LOG_TRIVIAL(debug) << "Fixed Huffman Coder<" << this->bits_ << "> is initialized.";
	}

private:
	void initFrequencies(std::vector<size_t>& freq)
	{
		size_t range = pow(2, this->bits_ - 1);
		//size_t half = ceil(range / 2.0);
		size_t precision = 100000;
		double mean = 0;
		//double sdNarrow = std::max({ (size_t)this->bits_ - 1, (size_t)1 });
		double sdNarrow = (double)this->bits_ / 2.0;
		double sdWide = sdNarrow * 10;

		boost::math::normal_distribution<double, boost::math::policies::policy<>> ndNarrow(mean, sdNarrow);
		boost::math::normal_distribution<double, boost::math::policies::policy<>> ndWide(mean, sdWide);

		freq[0] = (boost::math::pdf(ndNarrow, 0.0) + boost::math::pdf(ndWide, 0.0)) * precision;
		freq[range] = 1;
		BOOST_LOG_TRIVIAL(debug) << "Freq==>narrow: " << sdNarrow << ", wide: " << sdWide << ", range: " << range;;
		for (size_t x = 1; x < range; ++x)
		{
			freq[x] = (boost::math::pdf(ndNarrow, (double)x) + boost::math::pdf(ndWide, (double)x)) * precision;
			if (freq[x] == 0)
			{
				freq[x] = 1;
			}
			freq[x + range] = freq[x];
			//BOOST_LOG_TRIVIAL(debug) << "[" << x << "]: " << boost::math::pdf(ndNarrow, (double)x) << "+" << boost::math::pdf(ndWide, (double)x) << "=" << freq[x];
		}

		this->printFreq(freq);
	}

	void printFreq(std::vector<size_t>& freq)
	{
		BOOST_LOG_TRIVIAL(debug) << "Freq=====";
		for (size_t i = 0; i < freq.size(); ++i)
		{
			BOOST_LOG_TRIVIAL(debug) << "[" << i << "]: " << freq[i];
		}
	}

	void buildEncodeDecodeTable()
	{
		if (this->root_->isLeaf())
		{
			this->root_->codeLen_ = 1;
			this->encodeLookupTable_[this->root_->symbol_] = this->root_;
			this->insertSymbolInDecodeLookupTable(this->root_);
		}
		else
		{
			std::stack<nodeType*> dfsStack;
			dfsStack.push(this->root_);
			this->root_->code_ = 0;
			this->root_->codeLen_ = 0;

			while (!dfsStack.empty())
			{
				nodeType* node = dfsStack.top();
				dfsStack.pop();

				assert(node != nullptr);

				if (node->isLeaf())
				{
					this->encodeLookupTable_[node->symbol_] = node;
					this->insertSymbolInDecodeLookupTable(node);
				}
				else
				{
					node->left_->code_ = node->code_ << 1;
					node->right_->code_ = node->code_ << 1 | (codeType)0x1;
					node->left_->codeLen_ = node->codeLen_ + (codeType)1;
					node->right_->codeLen_ = node->codeLen_ + (codeType)1;

					dfsStack.push(node->left_);
					dfsStack.push(node->right_);
				}
			}
		}
	}

public:
	void encode(bstream& out, const symbolType* in, size_t len)
	{
		for (size_t i = 0; i < len; ++i)
		{
			this->encodeSymbol(out, in[i]);
		}
	}

	void decode(symbolType* outData, size_t lenOut, bstream& in)
	{
		codeType code = 0x0;
		in >> setw(parentType::bitCode) >> code;
		for (size_t i = 0; i < lenOut; ++i)
		{
			auto result = this->decodeSymbol(code);
			outData[i] = result.first;

			codeType nextCode = 0x0;
			in >> setw(result.second) >> nextCode;
			code = (code << result.second) | nextCode;
		}
	}

	virtual void encode(bstream& out, const void* in, size_t len)
	{
		for (size_t i = 0; i < len; ++i)
		{
			this->encodeSymbol(out, static_cast<const symbolType*>(in)[i]);
		}
	}

	virtual size_t decode(void* outData, size_t lenOut, bstream& in)
	{
		size_t readBits = 0;
		codeType code = 0x0;
		in >> setw(parentType::bitCode) >> code;
		for (size_t i = 0; i < lenOut; ++i)
		{
			auto result = this->decodeSymbol(code);
			static_cast<symbolType*>(outData)[i] = result.first;

			codeType nextCode = 0x0;
			in >> setw(result.second) >> nextCode;
			code = (code << result.second) | nextCode;

			readBits += result.second;
			//BOOST_LOG_TRIVIAL(debug) << "S: " << static_cast<uint64_t>(result.first) << "/ L: " << static_cast<uint64_t>(result.second);
		}

		return readBits;
	}

	virtual size_t getCodeBitWidth() const
	{
		return parentType::bitCode;
	}
};

template <size_t _BITS, typename symbolType>
class fixedHuffmanCoder : public fixedHuffmanCoder1ByteSymbolBase<uint16_t, symbolType>, public singleton<fixedHuffmanCoder<_BITS, symbolType>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder1ByteSymbolBase<uint16_t, symbolType>(_BITS), singleton<fixedHuffmanCoder<_BITS, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<1, symbolType> : public fixedHuffmanCoder1ByteSymbolBase<uint8_t, symbolType>, public singleton<fixedHuffmanCoder<1, symbolType>>
{
public:
	using codeType_ = uint8_t;
	using symbolType_ = symbolType;

public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder1ByteSymbolBase<uint8_t, symbolType>(1), singleton<fixedHuffmanCoder<1, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<2, symbolType> : public fixedHuffmanCoder1ByteSymbolBase<uint8_t, symbolType>, public singleton<fixedHuffmanCoder<2, symbolType>>
{
public:
	using codeType_ = uint8_t;
	using symbolType_ = symbolType;

public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder1ByteSymbolBase<uint8_t, symbolType>(2), singleton<fixedHuffmanCoder<2, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<3, symbolType> : public fixedHuffmanCoder1ByteSymbolBase<uint8_t, symbolType>, public singleton<fixedHuffmanCoder<3, symbolType>>
{
public:
	using codeType_ = uint8_t;
	using symbolType_ = symbolType;

public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder1ByteSymbolBase<uint8_t, symbolType>(3), singleton<fixedHuffmanCoder<3, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<4, symbolType> : public fixedHuffmanCoder1ByteSymbolBase<uint8_t, symbolType>, public singleton<fixedHuffmanCoder<4, symbolType>>
{
public:
	using codeType_ = uint8_t;
	using symbolType_ = symbolType;

public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder1ByteSymbolBase<uint8_t, symbolType>(4), singleton<fixedHuffmanCoder<4, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<5, symbolType> : public fixedHuffmanCoder1ByteSymbolBase<uint16_t, symbolType>, public singleton<fixedHuffmanCoder<5, symbolType>>
{
public:
	using codeType_ = uint16_t;
	using symbolType_ = symbolType;

public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder1ByteSymbolBase<uint16_t, symbolType>(5), singleton<fixedHuffmanCoder<5, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<6, symbolType> : public fixedHuffmanCoder1ByteSymbolBase<uint16_t, symbolType>, public singleton<fixedHuffmanCoder<6, symbolType>>
{
public:
	using codeType_ = uint16_t;
	using symbolType_ = symbolType;

public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder1ByteSymbolBase<uint16_t, symbolType>(6), singleton<fixedHuffmanCoder<6, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<7, symbolType> : public fixedHuffmanCoder1ByteSymbolBase<uint16_t, symbolType>, public singleton<fixedHuffmanCoder<7, symbolType>>
{
public:
	using codeType_ = uint16_t;
	using symbolType_ = symbolType;

public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder1ByteSymbolBase<uint16_t, symbolType>(7), singleton<fixedHuffmanCoder<7, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<8, symbolType> : public fixedHuffmanCoder1ByteSymbolBase<uint16_t, symbolType>, public singleton<fixedHuffmanCoder<8, symbolType>>
{
public:
	using codeType_ = uint16_t;
	using symbolType_ = symbolType;

public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder1ByteSymbolBase<uint16_t, symbolType>(8), singleton<fixedHuffmanCoder<8, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <size_t FIRST_BITS_, size_t SECOND_BITS_, typename symbolType>
class fixedHuffmanCoder2ByteSymbolBase : public iFixedHuffmanCoder
{
	using parentType = iFixedHuffmanCoder;
	using firstType = fixedHuffmanCoder<FIRST_BITS_, symbolType>;
	using secondType = fixedHuffmanCoder<SECOND_BITS_, symbolType>;

public:
	fixedHuffmanCoder2ByteSymbolBase()
		: first(fixedHuffmanCoder<FIRST_BITS_, symbolType>::instance()), second(fixedHuffmanCoder<SECOND_BITS_, symbolType>::instance())
	{

	}

	~fixedHuffmanCoder2ByteSymbolBase()
	{

	}

	virtual void encode(bstream& out, const void* in, size_t len)
	{
		for (size_t i = 0; i < len; )
		{
			first->encodeSymbol(out, static_cast<const firstType::symbolType_*>(in)[i++]);
			second->encodeSymbol(out, static_cast<const secondType::symbolType_*>(in)[i++]);
		}
	}

	virtual size_t decode(void* outData, size_t lenOut, bstream& in)
	{
		size_t readBits = 0;
		typename firstType::codeType_ code = 0x0;

		assert(firstType::bitCode >= secondType::bitCode);
		size_t bitCodeDiff = firstType::bitCode - secondType::bitCode;

		in >> setw(firstType::bitCode) >> code;
		for (size_t i = 0, bi = 0; i < lenOut; ++i)
		{
			{
				auto result = first->decodeSymbol((typename firstType::codeType_)(code));
				static_cast<typename firstType::symbolType_*>(outData)[bi++] = result.first;

				typename firstType::codeType_ nextCode = 0x0;
				in >> setw(result.second) >> nextCode;
				code = (code << result.second) | nextCode;
				readBits += result.second;
			}

			{
				auto result = second->decodeSymbol((typename secondType::codeType_)(code >> bitCodeDiff));
				static_cast<typename secondType::symbolType_*>(outData)[bi++] = result.first;

				typename secondType::codeType_ nextCode = 0x0;
				in >> setw(result.second) >> nextCode;
				code = (code << result.second) | nextCode;
				readBits += result.second;
			}

			//BOOST_LOG_TRIVIAL(debug) << "S: " << static_cast<uint64_t>(result.first) << "/ L: " << static_cast<uint64_t>(result.second);
		}

		return readBits;
	}

	virtual size_t getCodeBitWidth() const
	{
		return firstType::bitCode + secondType::bitCode;
	}

private:
	fixedHuffmanCoder<FIRST_BITS_, symbolType>* const first;
	fixedHuffmanCoder<SECOND_BITS_, symbolType>* const second;
};

template <typename symbolType>
class fixedHuffmanCoder<9, symbolType> : public fixedHuffmanCoder2ByteSymbolBase<8, 1, uint8_t>, public singleton<fixedHuffmanCoder<9, symbolType>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder2ByteSymbolBase<8, 1, uint8_t>(), singleton<fixedHuffmanCoder<9, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<10, symbolType> : public fixedHuffmanCoder2ByteSymbolBase<8, 2, uint8_t>, public singleton<fixedHuffmanCoder<10, symbolType>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder2ByteSymbolBase<8, 2, uint8_t>(), singleton<fixedHuffmanCoder<10, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<11, symbolType> : public fixedHuffmanCoder2ByteSymbolBase<8, 3, uint8_t>, public singleton<fixedHuffmanCoder<11, symbolType>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder2ByteSymbolBase<8, 3, uint8_t>(), singleton<fixedHuffmanCoder<11, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<12, symbolType> : public fixedHuffmanCoder2ByteSymbolBase<8, 4, uint8_t>, public singleton<fixedHuffmanCoder<12, symbolType>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder2ByteSymbolBase<8, 4, uint8_t>(), singleton<fixedHuffmanCoder<12, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<13, symbolType> : public fixedHuffmanCoder2ByteSymbolBase<8, 5, uint8_t>, public singleton<fixedHuffmanCoder<13, symbolType>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder2ByteSymbolBase<8, 5, uint8_t>(), singleton<fixedHuffmanCoder<13, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<14, symbolType> : public fixedHuffmanCoder2ByteSymbolBase<8, 6, uint8_t>, public singleton<fixedHuffmanCoder<14, symbolType>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder2ByteSymbolBase<8, 6, uint8_t>(), singleton<fixedHuffmanCoder<14, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<15, symbolType> : public fixedHuffmanCoder2ByteSymbolBase<8, 7, uint8_t>, public singleton<fixedHuffmanCoder<15, symbolType>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder2ByteSymbolBase<8, 7, uint8_t>(), singleton<fixedHuffmanCoder<15, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};

template <typename symbolType>
class fixedHuffmanCoder<16, symbolType> : public fixedHuffmanCoder2ByteSymbolBase<8, 8, uint8_t>, public singleton<fixedHuffmanCoder<16, symbolType>>
{
public:
	fixedHuffmanCoder()
		: fixedHuffmanCoder2ByteSymbolBase<8, 8, uint8_t>(), singleton<fixedHuffmanCoder<16, symbolType>>()
	{

	}

	~fixedHuffmanCoder()
	{

	}
};
}		// core
}		// msdb
#endif	// _MSDB_FIXED_HUFFMAN_CODER_H_