#pragma once
#include "Neighborhood.h"
#include "NextStateFunction.h"
#include <string>

class CellularAutomaton
{
public:
	CellularAutomaton(int xDim, int yDim, const NeighborHood& nbh, const NextStateFunction& nsf);
	virtual void update();

	virtual ~CellularAutomaton();

	const bool* operator[](int row) const { return rows_[row];  }

	bool* operator[](int row) { return rows_[row]; }

	int xDim() const { return xDim_; }
	int yDim() const { return yDim_; }


	void insert(int xPos, int YPos, const std::initializer_list<std::initializer_list<bool>>& newStates);
	void insert(int xPos, int YPos, const std::initializer_list<std::string>& newStates);

	void clear();

	void print() const;

	void wrapAround(bool wrap) { wrapAround_ = wrap; }
	bool wrapAround() const { return wrapAround_;  }

	void activeArea(int& minX, int& maxX, int& minY, int& maxY)
	{
		minX = minActiveX_;
		maxX = maxActiveX_;
		minY = minActiveY_;
		maxY = maxActiveY_;
	}

private:
	NeighborHood nbh_;
	NextStateFunction nsf_;

	int xDim_, yDim_;

	bool** rows_;
	bool** newrows_;

	int minActiveX_, minActiveY_, maxActiveX_, maxActiveY_;

	bool wrapAround_;
};
