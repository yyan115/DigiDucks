#pragma once

#include <cstddef>
#include <vector>

namespace CustomerQueuePolicy
{
	struct Status
	{
		bool waitingForOrder = false;
		bool walking = false;
		bool orderTaken = false;
		bool angryLeaving = false;
		std::size_t currentTargetIndex = 0;
		std::size_t queueTargetCount = 0;
	};

	inline bool OccupiesCounterPath(const Status& status) noexcept
	{
		return status.waitingForOrder ||
			(status.walking &&
			 !status.orderTaken &&
			 !status.angryLeaving &&
			 status.currentTargetIndex < status.queueTargetCount);
	}

	inline bool IsCounterPathOccupied(
		const std::vector<Status>& customers) noexcept
	{
		for (const Status& customer : customers)
		{
			if (OccupiesCounterPath(customer))
			{
				return true;
			}
		}
		return false;
	}
}
