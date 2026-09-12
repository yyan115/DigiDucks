#include "CustomerQueuePolicy.h"

#include <iostream>
#include <vector>

namespace
{
	bool Expect(bool condition, const char* message)
	{
		if (!condition)
		{
			std::cerr << "FAILED: " << message << std::endl;
		}
		return condition;
	}
}

int main()
{
	using CustomerQueuePolicy::IsCounterPathOccupied;
	using CustomerQueuePolicy::Status;

	bool passed = true;
	passed &= Expect(!IsCounterPathOccupied({}),
		"an empty customer list must leave the counter path clear");

	passed &= Expect(IsCounterPathOccupied({ Status{ true } }),
		"customer zero waiting to order must block the next spawn");

	Status approaching;
	approaching.walking = true;
	approaching.currentTargetIndex = 0;
	approaching.queueTargetCount = 3;
	passed &= Expect(IsCounterPathOccupied({ approaching }),
		"customer zero approaching the counter must block the next spawn");

	Status orderTaken = approaching;
	orderTaken.orderTaken = true;
	passed &= Expect(!IsCounterPathOccupied({ orderTaken }),
		"a customer walking after ordering must not block the counter path");

	Status angryLeaving = approaching;
	angryLeaving.angryLeaving = true;
	passed &= Expect(!IsCounterPathOccupied({ angryLeaving }),
		"an angry departing customer must not block the counter path");

	Status finishedApproach = approaching;
	finishedApproach.currentTargetIndex = finishedApproach.queueTargetCount;
	passed &= Expect(!IsCounterPathOccupied({ finishedApproach }),
		"a customer past the counter route must not block the next spawn");

	passed &= Expect(IsCounterPathOccupied({ Status{}, approaching }),
		"every customer must be checked, not only the first one");

	return passed ? 0 : 1;
}
