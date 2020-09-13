#pragma once
#include <initializer_list>
#include "CellularAutomatonDefinitions.h"

class NeighborHood
{
public:
	NeighborHood(const std::initializer_list<std::initializer_list<bool>>& neighborHoodFlags);

	const bool* operator[](int row) const
	{
		return neighbors_[row];
	}

	bool* operator[](int row)
	{
		return neighbors_[row];
	}

	int xDim() const { return xDim_; }
	int yDim() const { return yDim_; }

	void setHotPoint(int xHot, int yHot);
	
	void getHotPoint(int& xHot, int& yHot) const
	{
		xHot = xHot_;
		yHot = yHot_;
	}

	virtual void stepHotPoint() {}


protected:
	int xHot_, yHot_;

private:
	bool neighbors_[maxDim][maxDim];

	int xDim_, yDim_;
};

class HorizontallyAlternatingNeighborHood : public NeighborHood
{
public:
	// Inherit the constructors
	using NeighborHood::NeighborHood;

	void stepHotPoint() override;

private:
	// Doing in-class initialization means that it can still inherit constructors from base class
	int currentStep_ = 0;
};