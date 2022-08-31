#include <pch_test.h>

#include <util/coordinate.h>

namespace msdb
{
namespace test
{
TEST(multiDimIterator, move_in_4d_space)
{
	core::multiDimIterator it({8, 3, 16, 32});
	core::coordinates target({ 7, 2, 12, 0 });

	it.moveTo(target);
}
}		// test
}		// msdb