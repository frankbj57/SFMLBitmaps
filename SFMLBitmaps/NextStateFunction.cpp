#include "NextStateFunction.h"
#include <algorithm>

NextStateFunction::NextStateFunction(const std::initializer_list<std::initializer_list<bool>>& nextStates)
	: nextState_{}
{
	maxCount_ = 0;
	if (nextStates.size() == 1)
	{
		// Assume same count function for both non-active and active cell
		auto first = nextStates.begin();
		maxCount_ = 0;

		for (bool nextState : *first)
		{
			nextState_[0][maxCount_] = nextState_[1][maxCount_] = nextState;
			maxCount_++;
		}
	}
	else
	{
		auto first = nextStates.begin();
		auto second = first + 1;
		maxCount_ = (int)std::max(first->size(), second->size());

		int c = 0;
		for (bool nextState : *first)
		{
			nextState_[0][c] = nextState;
			c++;
		}

		c = 0;
		for (bool nextState : *second)
		{
			nextState_[1][c] = nextState;
			c++;
		}
	}
}

