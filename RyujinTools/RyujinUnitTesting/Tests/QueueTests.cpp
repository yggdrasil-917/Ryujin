

#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"

#include "RyujinCore/Containers/Queue.hpp"

namespace Ryujin
{
	TEST_CASE(QueueConstruction, "Test for queue ctors")
	{
		Queue<int32> q;
		q.Enqueue(1);
		q.Enqueue(2);
		q.Enqueue(3);

		Queue<int32> q1(q);
		Queue<int32> q11 = q;
		REQUIRE(q1.Capacity() == q.Capacity());
		REQUIRE(q1.PlacementIndex() == q.PlacementIndex());

		int32 item = 0;
		int32 item1 = 0;
		int32 i = 0;
		while (q1.PeekAt(item1, i) && q11.PeekAt(item, i))
		{
			REQUIRE(item == item1);
			++i;
		}

		Queue<int32> q2(Move(q1));
		Queue<int32> q22 = Move(q11);
		REQUIRE(q1.IsEmpty() && q11.IsEmpty())
			i = 0;
		while (q2.PeekAt(item1, i) && q22.PeekAt(item, i))
		{
			REQUIRE(item == item1);
			++i;
		}
	}

	TEST_CASE(QueueEnqueueDequeueTest, "Test for adding and removing elements")
	{
		Queue<int32> q;
		REQUIRE(q.Enqueue(1));
		int32 item = 0;
		q.Enqueue(2);
		REQUIRE(q.Dequeue(item) && item == 1);
	}
}
