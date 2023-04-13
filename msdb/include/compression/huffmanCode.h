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

template <typename symbolType, typename codeType>
class huffmanCoderLonger
{
public:
	using codeLenType = unsigned char;

	static const unsigned char bitSymbol = sizeof(symbolType) * CHAR_BIT;
	static const unsigned char maxCodeLength = sizeof(codeType) * CHAR_BIT;
	static const codeType codeMask = (codeType)-1;

	// User defined bit length for each level nodes
	// The last bit length value will be used for remaining higher level nodes
	// Optimal bit length should be optimized
	// The size of bitLength should be 'bitLength.size() > 1'.
	inline static const std::vector<unsigned char> bitLength = { 4, 4, 4, 2 };

public:
	huffmanCoderLonger()
		: symbolBits_(sizeof(symbolType)* CHAR_BIT), root_(nullptr)
	{
	}

	huffmanCoderLonger(size_t bits)
		: symbolBits_(bits), root_(nullptr)
	{
	}

	~huffmanCoderLonger()
	{
		if (this->root_)
		{
			this->deleteHuffmanTree();
		}
	}

public:
	struct huffmanNode
	{
		huffmanNode()
			: code_(0), symbol_(0), codeLen_(0), _parent(nullptr), left_(nullptr), right_(nullptr), depth_(1)
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

			left->_parent = this;
			right->_parent = this;

			//++left->depth_;
			//++right->depth_;
		}

		huffmanNode(codeType code, symbolType symbol, codeLenType codeLen, huffmanNode* parent = nullptr,
			huffmanNode* left = nullptr, huffmanNode* right = nullptr)
			: code_(code), symbol_(symbol), codeLen_(codeLen), _parent(parent),
			left_(left), right_(right), depth_(parent == nullptr ? 1 : parent->depth_)
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
		huffmanNode* _parent;
		unsigned char depth_;
	};

	struct compareHuffmanNode
	{
		bool operator()(std::pair<size_t, huffmanNode*> a, std::pair<size_t, huffmanNode*> b)
		{
			return a.first > b.first;
		}
	};

	struct decodeTreeNode
	{
		decodeTreeNode()
			: _isLeaf(false), _node(nullptr), _level(0), _parent(nullptr)
		{

		}

		decodeTreeNode(huffmanNode node)
			: _node(node), _level(0)
		{
			if (node)
			{
				this->_isLeaf = true;
			}
			else
			{
				this->_isLeaf = false;
			}
		}

		size_t _level;
		bool _isLeaf;
		huffmanNode* _node;
		std::vector<decodeTreeNode> _childNodes;
		decodeTreeNode* _parent;
	};

private:
	void deleteHuffmanTree()
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

		buildHuffmanTree(freq);
		encodeHuffmanTree(out);

		for (size_t i = 0; i < len; ++i)
		{
			encodeSymbol(out, in[i]);
		}
	}

	void decode(symbolType* outData, size_t lenOut, bstream& in)
	{
		this->initDecodeTable(&this->decodeTreeRoot_, 0);
		this->decodeHuffmanTree(in);

		codeType code = 0x0;
		in >> setw(maxCodeLength) >> code;
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

		this->initDecodeTable(&this->decodeTreeRoot_, 0);
		this->decodeHuffmanTree(bs);

		codeType code = 0x0;
		bs >> setw(maxCodeLength) >> code;
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
	//////////
	// Build Huffman tree for the given frequency vector (freq).
	void buildHuffmanTree(std::vector<size_t>& freq)
	{
		std::priority_queue<std::pair<size_t, huffmanNode*>, std::vector<std::pair<size_t, huffmanNode*>>, compareHuffmanNode> qFreq;	// , compareNode
		for (size_t i = 0; i < freq.size(); ++i)
		{
			if (freq[i])
			{
				// Make leaf nodes for each symbol.
				qFreq.push({ freq[i], new huffmanNode(i) });
			}
		}

		// Build the tree until you find a root node.
		while (qFreq.size() > 1)
		{
			auto left = qFreq.top();
			qFreq.pop();
			auto right = qFreq.top();
			qFreq.pop();

			auto next = new huffmanNode(left.second, right.second);
			left.second->_parent = next;
			right.second->_parent = next;

			qFreq.push({ left.first + right.first, next });
		}

		this->root_ = qFreq.top().second;
		qFreq.pop();
	}

	void encodeHuffmanTree(bstream& out)
	{
		if (this->root_->isLeaf())
		{
			out << setw(1) << (unsigned char)0x1;
			out << setw(symbolBits_) << this->root_->symbol_;
			this->root_->codeLen_ = 1;
			this->insertInEncodeTable(this->root_);
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
					out << setw(symbolBits_) << node->symbol_;
					this->insertInEncodeTable(node);
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

	void decodeHuffmanTree(bstream& in)
	{
		if (this->root_)
		{
			this->deleteHuffmanTree();
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
				in >> setw(symbolBits_) >> node->symbol_;

				if (node == this->root_)
				{
					node->codeLen_ = 1;
				}
				this->insertInDecodeTable(node);
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

				left->_parent = node;
				right->_parent = node;

				node->left_ = left;
				node->right_ = right;

				dfsStack.push(left);
				dfsStack.push(right);
			}
		}
	}

public:
	void encodeSymbol(bstream& out, const symbolType symbol)
	{
#ifndef NDEBUG
		assert(this->encodeHashTable_.find(symbol) != this->encodeHashTable_.end());
#endif

		auto node = this->encodeHashTable_[symbol];
		out << setw(node->codeLen_) << node->code_;
		//BOOST_LOG_TRIVIAL(debug) << "S: " << static_cast<uint64_t>(symbol) << "/ L: " << static_cast<uint64_t>(node->codeLen_);
	}

	std::pair<symbolType, size_t> decodeSymbol(codeType code)
	{
		decodeTreeNode* curTable = &this->decodeTreeRoot_;
		size_t level = 0, codeLength = 0;

		while (true)
		{
			// Warning for unsigned data type
			size_t curLevelCodeLength = this->getLevelCodeLength(level);
			codeType curCode = subCode(code, codeLength, curLevelCodeLength);
			codeLength += curLevelCodeLength;

			assert(curTable->_childNodes.size() > curCode);

			curTable = &curTable->_childNodes[curCode];
			if (curTable->_isLeaf)
			{
				return { curTable->_node->symbol_, curTable->_node->codeLen_ };
			}
			++level;
		}

		// Decode symbol fails
		return { 0, 0 };
	}

protected:
	void initDecodeTable(decodeTreeNode* table, const size_t level)
	{
		size_t tableSize = pow(2, getLevelCodeLength(level));
		table->_childNodes = std::vector<decodeTreeNode>(tableSize);
		for (decodeTreeNode& childTable : table->_childNodes)
		{
			childTable._parent = table;
			childTable._level = table->_level + 1;
		}
	}

protected:
	void insertInDecodeTable(huffmanNode* node)
	{
		decodeTreeNode* cur = &this->decodeTreeRoot_;
		size_t level = 0, codeLength = 0;

		while (true)
		{
			if (!cur->_childNodes.size())
			{
				this->initDecodeTable(cur, level);
			}

			size_t curLevelCodeLength = this->getLevelCodeLength(level);
			codeType curCode = subCode(node->code_, maxCodeLength - node->codeLen_ + codeLength, curLevelCodeLength);
			codeLength += curLevelCodeLength;

			if (node->codeLen_ <= codeLength)
			{
				// Now Found Leaf Decode Table
				// Set decent nodes
				size_t gap = pow(2, codeLength - node->codeLen_);
				for (size_t i = 0; i < gap; ++i)
				{
					auto seq = curCode + i;
					auto t = cur->_childNodes[curCode + i];
					cur->_childNodes[curCode + i]._isLeaf = true;
					cur->_childNodes[curCode + i]._node = node;
				}
				return;
			}
			else
			{
				cur = &(cur->_childNodes[curCode]);
			}

			++level;
		}
	}

	void insertInEncodeTable(huffmanNode* node)
	{
		this->encodeHashTable_[node->symbol_] = node;
	}

protected:
	inline size_t getLevelCodeLength(const size_t level)
	{
		return this->bitLength[level < this->bitLength.size() ? level : this->bitLength.size() - 1];
	}

	inline static codeType subCode(const codeType code, const size_t start, const size_t len)
	{
		assert(start <= maxCodeLength && "Start point cannot exceed maxCodeLength");
		assert(len <= maxCodeLength && "Length cannot excced maxCodeLength");

		codeType out = code;

		out <<= start;
		out &= codeMask;
		out >>= maxCodeLength - len;

		return out;
	}

public:
	void printEncodeTable()
	{
		BOOST_LOG_TRIVIAL(debug) << "===== Encode Table Start =====";
		for (symbolType i = 0; i < (symbolType)-1; ++i)
		{
			auto node = this->encodeHashTable_[i];
			if (node)
			{
				BOOST_LOG_TRIVIAL(debug) << static_cast<uint64_t>(i) << ": " << static_cast<uint64_t>(node->code_) << "(" << static_cast<uint64_t>(node->codeLen_) << ")";
			}
		}
		BOOST_LOG_TRIVIAL(debug) << "----- Encode Table End -----";
	}

protected:
	size_t symbolBits_;		// Size of symbol data type in bits.

	// Hash Table for encode 'symbol' to 'code'.
	// Integer with 1-byte data type has only 255 symbols.
	// Do not use with data types exceeding 2-bytes.
	std::map<symbolType, huffmanNode*> encodeHashTable_;

	// Decoding process uses 'Decode tree' instead of 'Huffman tree'
	// Each node in Decode tree provides multi-bit indexing.
	// Such tree structure would help to reduce the overall tree depth and leads to better decoding performance.
	decodeTreeNode decodeTreeRoot_;

public:
	// TODO::Move the variable to protected section
	huffmanNode* root_;
};
}		// core
}		// msdb
#endif	// _MSDB_HUFFMAN_CODE_H_