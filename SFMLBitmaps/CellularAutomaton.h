#pragma once
#include <initializer_list>

const int maxDim = 7;


class NeighborHood
{
public:
	NeighborHood(int xDim, int yDim);

	NeighborHood(const std::initializer_list<std::initializer_list<bool>> &neighborHoodFlags);

	const bool* operator[](int row) const;

	bool* operator[](int row);

	int xDim() const { return xDim_; }
	int yDim() const { return yDim_; }



private:
	bool neighbors_[maxDim][maxDim];

	int xDim_, yDim_;

};

class NextStateFunction
{
public:
	NextStateFunction(const std::initializer_list<std::initializer_list<bool>>& nextStates);

	const bool* operator[](int row) const { return nextState_[row]; }

	bool* operator[](int row) { return nextState_[row]; }

	int maxCount() const { return maxCount_; }


private:
	bool nextState_[2][maxDim * maxDim];
	int maxCount_;
};


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

	void clear();

	void print() const;

private:
	NeighborHood nbh_;
	NextStateFunction nsf_;

	int xDim_, yDim_;

	bool** rows_;
	bool** newrows_;
};


