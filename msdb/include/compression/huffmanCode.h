#pragma once
#ifndef _MSDB_HUFFMAN_CODE_H_
#define _MSDB_HUFFMAN_CODE_H_

#include <pch.h>
#include <io/bitstream.h>
#include <stack>
#include <queue>

namespace msdb
{
namespace core
{
template <typename codeType, typename symbolType>
class huffmanCoder
{
public:
	using codeLenType = unsigned char;

	static const unsigned char bitSymbol = sizeof(symbolType) * CHAR_BIT;
	static const unsigned char bitCode = sizeof(codeType) * CHAR_BIT;
	static const codeType codeMask = (codeType)-1;
	//static const unsigned char bitCode = 20;
	//static const codeType codeMask = 0xFFFFF;
	static const unsigned char maxDecodeTableLevel = 2;
	inline static const unsigned char bitDecodeTableLevel[maxDecodeTableLevel] = { bitCode / 4, bitCode };
	//inline static const unsigned char bitDecodeTableLevel[maxDecodeTableLevel] = { 4, 20 };
	//inline static const uint16_t remainMask[5] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F };
	

public:
	huffmanCoder()
		: bits_(sizeof(symbolType) * CHAR_BIT), root_(nullptr)
	{
	}

	huffmanCoder(size_t bits)
		: bits_(bits), root_(nullptr)
	{
	}

	~huffmanCoder()
	{
		if (this->root_)
		{
			this->deleteTree();
		}
	}

public:
	struct huffmanNode
	{
		huffmanNode()
			: code_(0), symbol_(0), codeLen_(0), parent_(nullptr), left_(nullptr), right_(nullptr)
		{
			
		}
		
		huffmanNode(symbolType symbol)
			: huffmanNode()
		{
			this->symbol_ = symbol;
		}

		huffmanNode(huffmanNode* left, huffmanNode* right)
			: huffmanNode()
		{
			this->left_ = left;
			this->right_ = right;
		}

		huffmanNode(codeType code, symbolType symbol, codeLenType codeLen, huffmanNode* parent = nullptr, huffmanNode* left = nullptr, huffmanNode* right = nullptr)
			: code_(code), symbol_(symbol), codeLen_(codeLen), parent_(parent), left_(left), right_(right)
		{
		}

		bool isLeaf()
		{
			return !this->left_ && !this->right_;
		}

		codeType code_;
		symbolType symbol_;
		codeLenType codeLen_;
		huffmanNode* left_;
		huffmanNode* right_;
		huffmanNode* parent_;
	};

	struct compareNode
	{
		bool operator()(std::pair<size_t, huffmanNode*> a, std::pair<size_t, huffmanNode*> b)
		{
			return a.first > b.first;
		}
	};

	struct decodeTable
	{
		decodeTable()
			: isLeaf_(false), node_(nullptr), level_(0)
		{

		}

		decodeTable(huffmanNode node)
			: node_(node), level_(0)
		{
			if (node)
			{
				this->isLeaf_ = true;
			} else
			{
				this->isLeaf_ = false;
			}
		}

		size_t level_;
		bool isLeaf_;
		huffmanNode* node_;
		std::vector<decodeTable> childLevel_;
	};

private:
	void deleteTree()
	{
		if (this->root_ == nullptr)
		{
			return;
		}

		std::stack<huffmanNode*> dfsStack;
		dfsStack.push(this->root_);

		while (!dfsStack.empty())
		{
			auto node = dfsStack.top();
			dfsStack.pop();

			if (node->left_)
			{
				dfsStack.push(node->left_);
			}
			if(node->right_)
			{
				dfsStack.push(node->right_);
			}

			delete node;
		}
	}

public:
	void encode(bstream& out, const symbolType* in, size_t len)
	{
		std::vector<size_t> freq(std::numeric_limits<symbolType>::max() + 1, 0);
		for(size_t i = 0; i < len; ++i)
		{
			++freq[in[i]];
		}

		buildTree(freq);
		encodeTree(out);

		for(size_t i = 0; i < len; ++i)
		{
			encodeSymbol(out, in[i]);
		}
	}

	void decode(symbolType* outData, size_t lenOut, bstream& in)
	{
		this->initDecodeTable(&this->decodeLookupTable_, 0);
		this->decodeTree(in);

		codeType code = 0x0;
		in >> setw(bitCode) >> code;
		for (size_t i = 0; i < lenOut; ++i)
		{
			auto result = this->decodeSymbol(code);
			outData[i] = result.first;

			codeType nextCode = 0x0;
			in >> setw(result.second) >> nextCode;
			code = ((code << result.second) & codeMask) | nextCode;
		}
	}

	void decode(symbolType* outData, size_t lenOut, symbolType* inData, size_t lenIn)
	{
		bstream bs;
		bs.resize(sizeof(symbolType) * lenIn);
		memcpy(bs.data(), (char*)inData, sizeof(symbolType) * lenIn);

		this->initDecodeTable(&this->decodeLookupTable_, 0);
		this->decodeTree(bs);

		codeType code = 0x0;
		bs >> setw(bitCode) >> code;
		for (size_t i = 0; i < lenOut; ++i)
		{
			auto result = this->decodeSymbol(code);
			outData[i] = result.first;

			codeType nextCode = 0x0;
			bs >> setw(result.second) >> nextCode;
			code = ((code << result.second) & codeMask) | nextCode;
		}
	}

protected:
	void buildTree(std::vector<size_t>& freq)
	{
		std::priority_queue<std::pair<size_t, huffmanNode*>, std::vector<std::pair<size_t, huffmanNode*>>, compareNode> qFreq;	// , compareNode
		for (size_t i = 0; i < freq.size(); ++i)
		{
			if (freq[i])
			{
				qFreq.push({ freq[i], new huffmanNode(i) });
			}
		}

		while (qFreq.size() > 1)
		{
			auto left = qFreq.top();
			qFreq.pop();
			auto right = qFreq.top();
			qFreq.pop();

			auto next = new huffmanNode(left.second, right.second);
			left.second->parent_ = next;
			right.second->parent_ = next;

			qFreq.push({ left.first + right.first, next });
		}

		this->root_ = qFreq.top().second;
		qFreq.pop();
	}

	void encodeTree(bstream& out)
	{
		if (this->root_->isLeaf())
		{
			out << setw(1) << (unsigned char)0x1;
			out << setw(bits_) << this->root_->symbol_;
			this->root_->codeLen_ = 1;
			this->encodeLookupTable_[this->root_->symbol_] = this->root_;
		}
		else
		{
			std::stack<huffmanNode*> dfsStack;
			dfsStack.push(this->root_);
			this->root_->code_ = 0;
			this->root_->codeLen_ = 0;

			while (!dfsStack.empty())
			{
				huffmanNode* node = dfsStack.top();
				dfsStack.pop();

				assert(node != nullptr);

				if (node->isLeaf())
				{
					out << setw(1) << (unsigned char)0x1;
					out << setw(bits_) << node->symbol_;
					this->encodeLookupTable_[node->symbol_] = node;
				}
				else
				{
					out << setw(1) << (unsigned char)0x0;
					node->left_->code_ = node->code_ << 1;
					node->right_->code_ = node->code_ << 1 | (codeType)0x1;
					node->left_->codeLen_ = node->codeLen_ + (codeLenType)1;
					node->right_->codeLen_ = node->codeLen_ + (codeLenType)1;

					dfsStack.push(node->left_);
					dfsStack.push(node->right_);
				}
			}
		}
	}

public:
	void encodeSymbol(bstream& out, const symbolType symbol)
	{
		// TODO:: output encode table or tree
#ifndef NDEBUG
		assert(this->encodeLookupTable_.find(symbol) != this->encodeLookupTable_.end());
#endif

		auto node = this->encodeLookupTable_[symbol];
		out << setw(node->codeLen_) << node->code_;
		//BOOST_LOG_TRIVIAL(debug) << "S: " << static_cast<uint64_t>(symbol) << "/ L: " << static_cast<uint64_t>(node->codeLen_);
	}

protected:
	void initDecodeTable(decodeTable* table, size_t level)
	{
		if (level == 0)
		{
			size_t size = pow(2, this->bitDecodeTableLevel[level]);
			table->childLevel_ = std::vector<decodeTable>(size);
		}
		else
		{
			size_t size = pow(2, this->bitDecodeTableLevel[level] - this->bitDecodeTableLevel[level-1]);
			table->childLevel_ = std::vector<decodeTable>(size);
		}
	}

	void decodeTree(bstream& in)
	{
		if (this->root_)
		{
			this->deleteTree();
		}
		
		this->root_ = new huffmanNode();
		this->root_->code_ = 0;
		this->root_->codeLen_ = 0;

		std::stack<huffmanNode*> dfsStack;
		dfsStack.push(this->root_);

		char isLeaf = 0;
		while (!dfsStack.empty())
		{
			huffmanNode* node = dfsStack.top();
			dfsStack.pop();

			assert(node != nullptr);

			in >> setw(1) >> isLeaf;

			if (isLeaf)
			{
				in >> setw(bits_) >> node->symbol_;

				if (node == this->root_)
				{
					node->codeLen_ = 1;
				}
				this->insertSymbolInDecodeLookupTable(node);
				// insert in decode table
				// insert 0~current code
			}
			else
			{
				auto left = new huffmanNode();
				auto right = new huffmanNode();

				left->code_ = node->code_ << 1;
				right->code_ = node->code_ << 1 | (codeType)0x1;

				left->codeLen_ = node->codeLen_ + (codeLenType)1;
				right->codeLen_ = node->codeLen_ + (codeLenType)1;

				left->parent_ = node;
				right->parent_ = node;

				node->left_ = left;
				node->right_ = right;
				
				dfsStack.push(left);
				dfsStack.push(right);
			}
		}
	}

public:
	std::pair<symbolType, size_t> decodeSymbol(codeType code)
	{
		decodeTable* curTable = &this->decodeLookupTable_;
		for (size_t level = 0; level < this->maxDecodeTableLevel; ++level)
		{
			// Warning for unsigned data type
			codeType curCode = code;
			if (level)
			{
				curCode <<= this->bitDecodeTableLevel[level - 1];
				curCode &= codeMask;
				curCode >>= this->bitDecodeTableLevel[level - 1];
			}
			curCode >>= (bitCode - this->bitDecodeTableLevel[level]);
			
			assert(curTable->childLevel_.size() > curCode);

			curTable = &curTable->childLevel_[curCode];
			if (curTable->isLeaf_)
			{
				return {curTable->node_->symbol_, curTable->node_->codeLen_};
			}
		}

		return { 0, 0 };
	}

protected:
	void insertSymbolInDecodeLookupTable(huffmanNode* node)
	{
		decodeTable* cur = &this->decodeLookupTable_;
		codeType baseCode = node->code_ << bitCode - node->codeLen_;
		for (size_t l = 0; l < this->maxDecodeTableLevel; ++l)
		{
			if (!cur->childLevel_.size())
			{
				this->initDecodeTable(cur, l);
			}

			codeType curCode = baseCode;
			if (l)
			{
				curCode <<= this->bitDecodeTableLevel[l - 1];
				curCode &= codeMask;
				curCode >>= this->bitDecodeTableLevel[l - 1];
			}
			curCode >>= bitCode - this->bitDecodeTableLevel[l];

			if (node->codeLen_ <= this->bitDecodeTableLevel[l])
			{
				size_t gap = pow(2, this->bitDecodeTableLevel[l] - node->codeLen_);
				for (size_t i = 0; i < gap; ++i)
				{
					auto seq = curCode + i;
					auto t = cur->childLevel_[curCode + i];
					cur->childLevel_[curCode + i].isLeaf_ = true;
					cur->childLevel_[curCode + i].node_ = node;
				}
				return;
			}
			else
			{
				cur = &(cur->childLevel_[curCode]);
			}
		}
	}

public:
	void printEncodeTable()
	{
		for (symbolType i = 0; i < (symbolType)-1; ++i)
		{
			auto node = this->encodeLookupTable_[i];
			if (node)
			{
				BOOST_LOG_TRIVIAL(debug) << static_cast<uint64_t>(i) << ": " << static_cast<uint64_t>(node->code_) << "(" << static_cast<uint64_t>(node->codeLen_) << ")";
			}
		}
	}

protected:
	size_t bits_;
	std::map<symbolType, huffmanNode*> encodeLookupTable_;
	decodeTable decodeLookupTable_;

public:
	huffmanNode* root_;
};


template <typename codeType, typename symbolType>
class huffmanCoderLonger
{
public:
	using codeLenType = unsigned char;

	static const unsigned char bitSymbol = sizeof(symbolType) * CHAR_BIT;
	//static const unsigned char bitCode = sizeof(codeType) * CHAR_BIT;
	//static const codeType codeMask = (codeType)-1;
	static const unsigned char bitCode = 20;
	static const codeType codeMask = 0xFFFFF;
	static const unsigned char maxDecodeTableLevel = 2;
	inline static const unsigned char bitDecodeTableLevel[maxDecodeTableLevel] = { bitCode / 4, bitCode };
	//inline static const unsigned char bitDecodeTableLevel[maxDecodeTableLevel] = { 4, 20 };
	//inline static const uint16_t remainMask[5] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F };


public:
	huffmanCoderLonger()
		: bits_(sizeof(symbolType)* CHAR_BIT), root_(nullptr)
	{
	}

	huffmanCoderLonger(size_t bits)
		: bits_(bits), root_(nullptr)
	{
	}

	~huffmanCoderLonger()
	{
		if (this->root_)
		{
			this->deleteTree();
		}
	}

public:
	struct huffmanNode
	{
		huffmanNode()
			: code_(0), symbol_(0), codeLen_(0), parent_(nullptr), left_(nullptr), right_(nullptr)
		{

		}

		huffmanNode(symbolType symbol)
			: huffmanNode()
		{
			this->symbol_ = symbol;
		}

		huffmanNode(huffmanNode* left, huffmanNode* right)
			: huffmanNode()
		{
			this->left_ = left;
			this->right_ = right;
		}

		huffmanNode(codeType code, symbolType symbol, codeLenType codeLen, huffmanNode* parent = nullptr, huffmanNode* left = nullptr, huffmanNode* right = nullptr)
			: code_(code), symbol_(symbol), codeLen_(codeLen), parent_(parent), left_(left), right_(right)
		{
		}

		bool isLeaf()
		{
			return !this->left_ && !this->right_;
		}

		codeType code_;
		symbolType symbol_;
		codeLenType codeLen_;
		huffmanNode* left_;
		huffmanNode* right_;
		huffmanNode* parent_;
	};

	struct compareNode
	{
		bool operator()(std::pair<size_t, huffmanNode*> a, std::pair<size_t, huffmanNode*> b)
		{
			return a.first > b.first;
		}
	};

	struct decodeTable
	{
		decodeTable()
			: isLeaf_(false), node_(nullptr), level_(0)
		{

		}

		decodeTable(huffmanNode node)
			: node_(node), level_(0)
		{
			if (node)
			{
				this->isLeaf_ = true;
			}
			else
			{
				this->isLeaf_ = false;
			}
		}

		size_t level_;
		bool isLeaf_;
		huffmanNode* node_;
		std::vector<decodeTable> childLevel_;
	};

private:
	void deleteTree()
	{
		if (this->root_ == nullptr)
		{
			return;
		}

		std::stack<huffmanNode*> dfsStack;
		dfsStack.push(this->root_);

		while (!dfsStack.empty())
		{
			auto node = dfsStack.top();
			dfsStack.pop();

			if (node->left_)
			{
				dfsStack.push(node->left_);
			}
			if (node->right_)
			{
				dfsStack.push(node->right_);
			}

			delete node;
		}
	}

public:
	void encode(bstream& out, const symbolType* in, size_t len)
	{
		std::vector<size_t> freq(std::numeric_limits<symbolType>::max() + 1, 0);
		for (size_t i = 0; i < len; ++i)
		{
			++freq[in[i]];
		}

		buildTree(freq);
		encodeTree(out);

		for (size_t i = 0; i < len; ++i)
		{
			encodeSymbol(out, in[i]);
		}
	}

	void decode(symbolType* outData, size_t lenOut, bstream& in)
	{
		this->initDecodeTable(&this->decodeLookupTable_, 0);
		this->decodeTree(in);

		codeType code = 0x0;
		in >> setw(bitCode) >> code;
		for (size_t i = 0; i < lenOut; ++i)
		{
			auto result = this->decodeSymbol(code);
			outData[i] = result.first;

			codeType nextCode = 0x0;
			in >> setw(result.second) >> nextCode;
			code = ((code << result.second) & codeMask) | nextCode;
		}
	}

	void decode(symbolType* outData, size_t lenOut, symbolType* inData, size_t lenIn)
	{
		bstream bs;
		bs.resize(sizeof(symbolType) * lenIn);
		memcpy(bs.data(), (char*)inData, sizeof(symbolType) * lenIn);

		this->initDecodeTable(&this->decodeLookupTable_, 0);
		this->decodeTree(bs);

		codeType code = 0x0;
		bs >> setw(bitCode) >> code;
		for (size_t i = 0; i < lenOut; ++i)
		{
			auto result = this->decodeSymbol(code);
			outData[i] = result.first;

			codeType nextCode = 0x0;
			bs >> setw(result.second) >> nextCode;
			code = ((code << result.second) & codeMask) | nextCode;
		}
	}

protected:
	void buildTree(std::vector<size_t>& freq)
	{
		std::priority_queue<std::pair<size_t, huffmanNode*>, std::vector<std::pair<size_t, huffmanNode*>>, compareNode> qFreq;	// , compareNode
		for (size_t i = 0; i < freq.size(); ++i)
		{
			if (freq[i])
			{
				qFreq.push({ freq[i], new huffmanNode(i) });
			}
		}

		while (qFreq.size() > 1)
		{
			auto left = qFreq.top();
			qFreq.pop();
			auto right = qFreq.top();
			qFreq.pop();

			auto next = new huffmanNode(left.second, right.second);
			left.second->parent_ = next;
			right.second->parent_ = next;

			qFreq.push({ left.first + right.first, next });
		}

		this->root_ = qFreq.top().second;
		qFreq.pop();
	}

	void encodeTree(bstream& out)
	{
		if (this->root_->isLeaf())
		{
			out << setw(1) << (unsigned char)0x1;
			out << setw(bits_) << this->root_->symbol_;
			this->root_->codeLen_ = 1;
			this->encodeLookupTable_[this->root_->symbol_] = this->root_;
		}
		else
		{
			std::stack<huffmanNode*> dfsStack;
			dfsStack.push(this->root_);
			this->root_->code_ = 0;
			this->root_->codeLen_ = 0;

			while (!dfsStack.empty())
			{
				huffmanNode* node = dfsStack.top();
				dfsStack.pop();

				assert(node != nullptr);

				if (node->isLeaf())
				{
					out << setw(1) << (unsigned char)0x1;
					out << setw(bits_) << node->symbol_;
					this->encodeLookupTable_[node->symbol_] = node;
				}
				else
				{
					out << setw(1) << (unsigned char)0x0;
					node->left_->code_ = node->code_ << 1;
					node->right_->code_ = node->code_ << 1 | (codeType)0x1;
					node->left_->codeLen_ = node->codeLen_ + (codeLenType)1;
					node->right_->codeLen_ = node->codeLen_ + (codeLenType)1;

					dfsStack.push(node->left_);
					dfsStack.push(node->right_);
				}
			}
		}
	}

public:
	void encodeSymbol(bstream& out, const symbolType symbol)
	{
		// TODO:: output encode table or tree
#ifndef NDEBUG
		assert(this->encodeLookupTable_.find(symbol) != this->encodeLookupTable_.end());
#endif

		auto node = this->encodeLookupTable_[symbol];
		out << setw(node->codeLen_) << node->code_;
		//BOOST_LOG_TRIVIAL(debug) << "S: " << static_cast<uint64_t>(symbol) << "/ L: " << static_cast<uint64_t>(node->codeLen_);
	}

protected:
	void initDecodeTable(decodeTable* table, size_t level)
	{
		if (level == 0)
		{
			size_t size = pow(2, this->bitDecodeTableLevel[level]);
			table->childLevel_ = std::vector<decodeTable>(size);
		}
		else
		{
			size_t size = pow(2, this->bitDecodeTableLevel[level] - this->bitDecodeTableLevel[level - 1]);
			table->childLevel_ = std::vector<decodeTable>(size);
		}
	}

	void decodeTree(bstream& in)
	{
		if (this->root_)
		{
			this->deleteTree();
		}

		this->root_ = new huffmanNode();
		this->root_->code_ = 0;
		this->root_->codeLen_ = 0;

		std::stack<huffmanNode*> dfsStack;
		dfsStack.push(this->root_);

		char isLeaf = 0;
		while (!dfsStack.empty())
		{
			huffmanNode* node = dfsStack.top();
			dfsStack.pop();

			assert(node != nullptr);

			in >> setw(1) >> isLeaf;

			if (isLeaf)
			{
				in >> setw(bits_) >> node->symbol_;

				if (node == this->root_)
				{
					node->codeLen_ = 1;
				}
				this->insertSymbolInDecodeLookupTable(node);
				// insert in decode table
				// insert 0~current code
			}
			else
			{
				auto left = new huffmanNode();
				auto right = new huffmanNode();

				left->code_ = node->code_ << 1;
				right->code_ = node->code_ << 1 | (codeType)0x1;

				left->codeLen_ = node->codeLen_ + (codeLenType)1;
				right->codeLen_ = node->codeLen_ + (codeLenType)1;

				left->parent_ = node;
				right->parent_ = node;

				node->left_ = left;
				node->right_ = right;

				dfsStack.push(left);
				dfsStack.push(right);
			}
		}
	}

public:
	std::pair<symbolType, size_t> decodeSymbol(codeType code)
	{
		decodeTable* curTable = &this->decodeLookupTable_;
		for (size_t level = 0; level < this->maxDecodeTableLevel; ++level)
		{
			// Warning for unsigned data type
			codeType curCode = code;
			if (level)
			{
				curCode <<= this->bitDecodeTableLevel[level - 1];
				curCode &= codeMask;
				curCode >>= this->bitDecodeTableLevel[level - 1];
			}
			curCode >>= (bitCode - this->bitDecodeTableLevel[level]);

			assert(curTable->childLevel_.size() > curCode);

			curTable = &curTable->childLevel_[curCode];
			if (curTable->isLeaf_)
			{
				return { curTable->node_->symbol_, curTable->node_->codeLen_ };
			}
		}

		return { 0, 0 };
	}

protected:
	void insertSymbolInDecodeLookupTable(huffmanNode* node)
	{
		decodeTable* cur = &this->decodeLookupTable_;
		codeType baseCode = node->code_ << bitCode - node->codeLen_;
		for (size_t l = 0; l < this->maxDecodeTableLevel; ++l)
		{
			if (!cur->childLevel_.size())
			{
				this->initDecodeTable(cur, l);
			}

			codeType curCode = baseCode;
			if (l)
			{
				curCode <<= this->bitDecodeTableLevel[l - 1];
				curCode &= codeMask;
				curCode >>= this->bitDecodeTableLevel[l - 1];
			}
			curCode >>= bitCode - this->bitDecodeTableLevel[l];

			if (node->codeLen_ <= this->bitDecodeTableLevel[l])
			{
				size_t gap = pow(2, this->bitDecodeTableLevel[l] - node->codeLen_);
				for (size_t i = 0; i < gap; ++i)
				{
					auto seq = curCode + i;
					auto t = cur->childLevel_[curCode + i];
					cur->childLevel_[curCode + i].isLeaf_ = true;
					cur->childLevel_[curCode + i].node_ = node;
				}
				return;
			}
			else
			{
				cur = &(cur->childLevel_[curCode]);
			}
		}
	}

public:
	void printEncodeTable()
	{
		for (symbolType i = 0; i < (symbolType)-1; ++i)
		{
			auto node = this->encodeLookupTable_[i];
			if (node)
			{
				BOOST_LOG_TRIVIAL(debug) << static_cast<uint64_t>(i) << ": " << static_cast<uint64_t>(node->code_) << "(" << static_cast<uint64_t>(node->codeLen_) << ")";
			}
		}
	}

protected:
	size_t bits_;
	std::map<symbolType, huffmanNode*> encodeLookupTable_;
	decodeTable decodeLookupTable_;

public:
	huffmanNode* root_;
};
}		// core
}		// msdb
#endif	// _MSDB_HUFFMAN_CODE_H_