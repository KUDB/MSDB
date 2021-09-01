#pragma once
#ifndef _MSDB_ADAPTIVE_HUFFMAN_CODING_H_
#define _MSDB_ADAPTIVE_HUFFMAN_CODING_H_

#include <pch.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
template <typename codeType, typename symbolType>
class aHuffmanCoder
{
public:
	using pathType = std::pair<codeType, size_t>;

public:
	aHuffmanCoder(size_t bits)
		: bits_(bits), nytNode(nullptr), root(new huffmanNode(-1, 0, pow(2, sizeof(codeType) * CHAR_BIT + 1), nullptr, nullptr, nullptr, true))
	{
		assert(sizeof(codeType) * CHAR_BIT >= bits);
		nytNode = this->root;
	}
	~aHuffmanCoder()
	{
		this->deleteTree(this->root);
	}

public:
	void updateTreeModel(symbolType symbol)
	{
		huffmanNode* leftToIncrement = nullptr;
		//huffmanNode* symbolNode = this->getSymbolNode(symbol, this->root);
		huffmanNode* symbolNode = this->getSymbolNode(symbol);

		if (symbolNode == nullptr)
		{
			nytNode->isNYT = false;
			auto curMinOrder = nytNode->order;
			nytNode->left = new huffmanNode(-1, 0, curMinOrder - 2, nullptr, nullptr, nytNode, true);
			nytNode->right = new huffmanNode(symbol, 0, curMinOrder - 1, nullptr, nullptr, nytNode, false);
			symbolNode = nytNode->right;
			nytNode = nytNode->left;

			this->table_[symbol] = symbolNode;
		}

		this->slideAndIncrement(symbolNode);
	}

	void encode(bstream& out, symbolType symbol)
	{
		//huffmanNode* symbolNode = this->getSymbolNode(symbol, this->root);
		huffmanNode* symbolNode = this->getSymbolNode(symbol);
		if (symbolNode)
		{
			//auto result = this->getPathToSymbolNode(root, symbolNode, this->makePath(0, 0));
			auto result = this->getPathToSymbolNode(symbolNode);
			out << setw(result.second) << result.first;
			this->updateTreeModel(symbol);
			return;
		}

		//auto result = getPathToSymbolNode(root, nytNode, this->makePath(0, 0));
		auto result = this->getPathToNYT();
		this->updateTreeModel(symbol);

		if (result.second)
		{
			out << setw(result.second) << result.first;
		}
		out << setw(this->bits_) << symbol;
	}

	symbolType decode(bstream& in)
	{
		symbolType result;
		huffmanNode* cur = this->root;
		while (true)
		{
			if (cur->isNYT)
			{
				symbolType result = 0;
				in >> setw(this->bits_) >> result;
				this->updateTreeModel(result);
				return result;
			} else if (cur->left == nullptr && cur->right == nullptr)
			{
				symbolType result = cur->value;
				this->updateTreeModel(cur->value);
				return result;
			} else
			{
				char b;
				in >> setw(1) >> b;

				if (b == 0)
				{
					cur = cur->left;
				} else
				{
					cur = cur->right;
				}
			}
		}

		return result;
	}

private:
	struct huffmanNode
	{
		huffmanNode()
			: value(0), path({0, 0}), weight(0), order(0), left(nullptr), right(nullptr), parent(nullptr), isNYT(false)
		{
		}

		huffmanNode(symbolType value, size_t weight, size_t order, huffmanNode* left, huffmanNode* right,
					huffmanNode* parent, bool isNYT = false)
			: value(value), path({0, 0}), weight(weight), order(order), left(left), right(right), parent(parent), isNYT(isNYT)
		{
		}

		huffmanNode(bool isNYT, symbolType value)
			: huffmanNode()
		{
			this->isNYT = isNYT;
			this->value = value;
		}

		void initPath()
		{
			this->path = { 0, 0 };
			if(this->left)
			{
				this->left->initPath();
			}
			if(this->right)
			{
				this->right->initPath();
			}
		}

		void initPath(pathType parentPath, bool isLeft)
		{
			if (isLeft)
			{
				this->path = { parentPath.first << 1, parentPath.second + 1 };
			} else
			{
				this->path = { parentPath.first << 1 | 1, parentPath.second + 1 };
			}
		}

		symbolType value;
		pathType path;
		size_t weight;
		size_t order;
		huffmanNode* left;
		huffmanNode* right;
		huffmanNode* parent;
		bool isNYT;
	};

	huffmanNode* getSymbolNode(symbolType symbol) const
	{
		if(this->table_.find(symbol) != this->table_.end())
		{
			return this->table_.at(symbol);
		}

		return nullptr;
	}

	//huffmanNode* getSymbolNode(symbolType symbol, huffmanNode* cur) const
	//{
	//	if (cur == nullptr || cur->value == symbol)
	//	{
	//		return cur;
	//	}

	//	huffmanNode* leftResult = this->getSymbolNode(symbol, cur->left);
	//	if (leftResult != nullptr)
	//	{
	//		return leftResult;
	//	}

	//	return this->getSymbolNode(symbol, cur->right);
	//}
	//void findBlockLeader(huffmanNode* cur, huffmanNode*& curMax) const
	//{
	//	if (cur == nullptr || curMax == root)
	//		return;

	//	if (cur->weight == curMax->weight && cur != curMax->parent && cur->order > curMax->order)
	//		curMax = cur;

	//	findBlockLeader(cur->left, curMax);
	//	findBlockLeader(cur->right, curMax);
	//}

	huffmanNode* findBlockLeaderFromTable(huffmanNode* node) const
	{
		if(this->blockLeaderTable_.size() <= node->weight)
		{
			return nullptr;
		}

		auto it = this->blockLeaderTable_[node->weight].begin();
		auto out = node;
		while (it != this->blockLeaderTable_[node->weight].end())
		{
			if ((*it) != this->root && (*it) != node->parent && (*it)->order > out->order)
			{
				out = *it;
			}

			++it;
		}

		if(out != node)
		{
			return out;
		}

		return nullptr;
	}

	void swapNodes(huffmanNode* first, huffmanNode* second)
	{
		if (first->parent == nullptr || second->parent == nullptr)
			return;

		if (first->parent == second || second->parent == first)
			return;

		huffmanNode*& firstRef = first->parent->left == first ? first->parent->left : first->parent->right;
		huffmanNode*& secondRef = second->parent->left == second ? second->parent->left : second->parent->right;

		std::swap(firstRef, secondRef);
		std::swap(firstRef->parent, secondRef->parent);
		std::swap(firstRef->order, secondRef->order);

		firstRef->initPath();
		secondRef->initPath();

		updateTable(firstRef);
		updateTable(secondRef);
	}

	void updateTable(huffmanNode* node)
	{
		if(node->left != nullptr || node->right != nullptr)
		{
			return;
		}

		this->table_[node->value] = node;
	}

	std::pair<codeType, size_t> getPathToSymbolNode(huffmanNode* symbolNode) const
	{
		if(symbolNode->path.second != 0)
		{
			return symbolNode->path;
		}

		return this->getPathToSymbolNode(this->root, symbolNode, this->makePath(0, 0));
	}

	std::pair<codeType, size_t> getPathToNYT() const
	{
		if(this->nytNode->path.second != 0)
		{
			return this->nytNode->path;
		}

		return this->getPathToSymbolNode(this->root, this->nytNode, this->makePath(0, 0));
	}

	std::pair<codeType, size_t> getPathToSymbolNode(huffmanNode* cur, huffmanNode* result, pathType curPath) const
	{
		if (cur == nullptr)
		{
			return std::make_pair<codeType, size_t>(0, 0);
		}

		cur->path = curPath;
		if (cur == result)
		{
			return curPath;
		}

		auto left = this->getPathToSymbolNode(cur->left, result, this->makePath((curPath.first << 1), curPath.second + 1));
		if (left.second != 0)
		{
			return left;
		}

		return this->getPathToSymbolNode(cur->right, result, this->makePath((curPath.first << 1) | 1, curPath.second + 1));
	}

	void slideAndIncrement(huffmanNode* node)
	{
		//huffmanNode* blockLeader = node;
		//this->findBlockLeader(this->root, blockLeader);
		//if (blockLeader != node)
		//{
		//	this->swapNodes(blockLeader, node);
		//}

		huffmanNode* tableBlockLeader = findBlockLeaderFromTable(node);
		if(tableBlockLeader != nullptr)
		{
			this->swapNodes(tableBlockLeader, node);
		}

		++(node->weight);
		this->updateBlockLeaderTable(node);
		if(node->parent)
		{
			this->slideAndIncrement(node->parent);
		}
	}

	void deleteTree(huffmanNode* node)
	{
		if (node == nullptr)
		{
			return;
		}

		this->deleteTree(node->left);
		this->deleteTree(node->right);
		this->table_[node->value] = nullptr;

		delete node;
	}

	inline aHuffmanCoder::pathType makePath(codeType code, size_t depth) const
	{
		assert(depth < sizeof(codeType) * CHAR_BIT);
		return { code, depth };
	}

	void updateBlockLeaderTable(huffmanNode* node)
	{
		if (this->blockLeaderTable_.size() <= node->weight)
		{
			int gap = node->weight - this->blockLeaderTable_.size() + 1;

			for (int i = 0; i < gap; ++i)
			{
				this->blockLeaderTable_.push_back(std::vector<huffmanNode*>());
			}
		}

		this->blockLeaderTable_[node->weight].push_back(node);

		auto prevWeight = node->weight - 1;
		if(prevWeight > 0)
		{
			auto it = this->blockLeaderTable_[prevWeight].begin();
			auto deleteIt = this->blockLeaderTable_[prevWeight].end();
			bool inserted = false;

			while (it != this->blockLeaderTable_[prevWeight].end())
			{
				if(*it == node)
				{
					this->blockLeaderTable_[prevWeight].erase(it);
					break;
				}
				++it;
			}
		}
	}

	std::map<symbolType, huffmanNode*> table_;
	std::vector<std::vector<huffmanNode*>> blockLeaderTable_;
	huffmanNode* nytNode;
	huffmanNode* root;
	size_t bits_;
};

template <typename codeType, typename symbolType>
class aHashHuffmanCoder
{
public:
	aHashHuffmanCoder()
	{

	}

	~aHashHuffmanCoder()
	{
	}

private:

};
}		// core
}		// msdb
#endif	// _MSDB_ADAPTIVE_HUFFMAN_CODING_H_