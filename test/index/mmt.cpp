#include <pch_test.h>
#include <index/mmtNode.h>

using namespace msdb::core;

namespace msdb
{
namespace test
{
template <typename Ty_>
pMmtNode getMmtNode(Ty_ realMin, Ty_ realMax, 
					bit_cnt_type bMinDelta, bit_cnt_type bMaxDelta, 
					bit_cnt_type order, bit_cnt_type childOrder, bool finished = false)
{
	pMmtNode curNode = std::make_shared<mmtNode>();

	curNode->realMin_ = realMin;
	curNode->realMax_ = realMax;
	curNode->bMinDelta_ = bMinDelta;
	curNode->bMaxDelta_ = bMaxDelta;
	curNode->order_ = order;
	curNode->childOrder_ = childOrder;
	curNode->finished_ = finished;

	curNode->bMin_ = msb<Ty_>(abs_(realMin), order) * SIGN(realMin);
	curNode->bMax_ = msb<Ty_>(abs_(realMax), order) * SIGN(realMax);

	curNode->min_ = getMinBoundary<Ty_>(realMin, curNode->order_, curNode->bMin_);
	curNode->max_ = getMaxBoundary<Ty_>(realMax, curNode->order_, curNode->bMax_);

	curNode->bits_ = std::max({ abs_(msb<Ty_>(realMin, order)), abs_(msb<Ty_>(realMax, order)) });;

	return curNode;
}

template <typename Ty_>
pMmtNode getMmtNode(Ty_ realMin, Ty_ realMax, Ty_ prevMin, Ty_ prevMax, 
					bit_cnt_type bMinDelta, bit_cnt_type bMaxDelta, 
					bit_cnt_type order, bit_cnt_type childOrder, bool finished = false)
{
	pMmtNode curNode = getMmtNode(realMin, realMax, bMinDelta, bMaxDelta, order, childOrder, finished);

	curNode->min_ = getMinBoundary<Ty_>(prevMin, curNode->order_, curNode->bMin_);
	curNode->max_ = getMaxBoundary<Ty_>(prevMax, curNode->order_, curNode->bMax_);

	return curNode;
}

TEST(mmt, mmt_update_WhenChildOrderChanged)
{
	pMmtNode node_n124_n77 = getMmtNode<char>(-124, -77, -127, 127, 0, 14, 1, 2);
	nodeUpdateWhenChildOrderChanged<char>(node_n124_n77, 0, -124);
	EXPECT_EQ(node_n124_n77->getMin<char>(), -127);
	EXPECT_EQ(node_n124_n77->getMax<char>(), -64);

	nodeUpdateWhenChildOrderChanged<char>(node_n124_n77, 0, -77);
	EXPECT_EQ(node_n124_n77->getMin<char>(), -127);
	EXPECT_EQ(node_n124_n77->getMax<char>(), -64);
}

TEST(mmt, mmt_update_ChildNodeOrder)
{
	pMmtNode node_64_64 = getMmtNode<char>(64, 64, -127, 127, 14, 0, 1, 2);
	EXPECT_EQ(updateChildNodeOrder<char>(node_64_64), 6);

	pMmtNode node_0_0 = getMmtNode<char>(0, 0 - 127, 127, 7, 7, 1, 0);
	EXPECT_EQ(updateChildNodeOrder<char>(node_0_0), 0);

	pMmtNode node_n35_n35 = getMmtNode<char>(-35, -35, 1, 13, 1, 0, false);
	EXPECT_EQ(updateChildNodeOrder<char>(node_n35_n35), 5);
}

TEST(mmt, mmt_serialize_jumpBits)
{
	bstream bs;
	bit_cnt_type jumpBits = 0;
	char jumpValue = 0;

	pMmtNode node_0_0 = getMmtNode<char>(0, 0, -127, 127, 7, 7, 1, 0);
	node_0_0->outJumpedBits<char>(bs, 0, 0);
	node_0_0->inJumpedBits<char>(bs, jumpBits, jumpValue);
}
}
}