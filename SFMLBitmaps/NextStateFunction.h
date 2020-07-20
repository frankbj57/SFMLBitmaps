#pragma once
#include <initializer_list>
#include <vector>
#include "CellularAutomatonDefinitions.h"

class NextStateFunction
{
public:
	NextStateFunction(const std::initializer_list<std::initializer_list<bool>>& nextStates);
	NextStateFunction(const std::vector<std::vector<bool>>& nextStates);

	const bool* operator[](int row) const { return nextState_[row]; }

	bool* operator[](int row) { return nextState_[row]; }

	int maxCount() const { return maxCount_; }


private:
	bool nextState_[2][maxDim * maxDim];
	int maxCount_;
};
