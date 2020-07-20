#include "Neighborhood.h"
#include <algorithm>


NeighborHood::NeighborHood(const std::initializer_list<std::initializer_list<bool>>& neighborHoodFlags)
	: neighbors_{}
{
	yDim_ = (int)neighborHoodFlags.size();

	xDim_ = 0;
	size_t x = 0;

	for (auto row : neighborHoodFlags)
	{
		xDim_ = std::max((int)row.size(), xDim_);

		size_t y = 0;
		for (bool f : row)
		{
			neighbors_[x][y] = f;
			y++;
		}
		x++;
	}
}

const bool* NeighborHood::operator[](int row) const
{
	return neighbors_[row];
}

bool* NeighborHood::operator[](int row)
{
	return neighbors_[row];
}

