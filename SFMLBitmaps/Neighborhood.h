#pragma once
#include <initializer_list>
#include "CellularAutomatonDefinitions.h"

class NeighborHood
{
public:
	NeighborHood(int xDim, int yDim);

	NeighborHood(const std::initializer_list<std::initializer_list<bool>>& neighborHoodFlags);

	const bool* operator[](int row) const;

	bool* operator[](int row);

	int xDim() const { return xDim_; }
	int yDim() const { return yDim_; }



private:
	bool neighbors_[maxDim][maxDim];

	int xDim_, yDim_;
};

