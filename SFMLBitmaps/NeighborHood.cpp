#include "NeighborHood.h"
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

	// Set hotpoints default
	xHot_ = xDim_ / 2;
	yHot_ = yDim_ / 2;
}

void NeighborHood::setHotPoint(int xHot, int yHot)
{
	xHot_ = (xHot >= 0 && xHot < xDim_) ? xHot : 0;
	yHot_ = (yHot >= 0 && yHot < yDim_) ? yHot : 0;
}


void HorizontallyAlternatingNeighborHood::stepHotPoint()
{
	xHot_ = xDim() / 2 + currentStep_;
	currentStep_ = (1 - currentStep_);
}
