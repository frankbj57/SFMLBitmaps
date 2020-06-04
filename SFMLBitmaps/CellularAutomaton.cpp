#include "CellularAutomaton.h"
#include <algorithm>
#include <iostream>


NeighborHood::NeighborHood(const std::initializer_list<std::initializer_list<bool>> &neighborHoodFlags)
	: neighbors_{}
{
	yDim_ = (int) neighborHoodFlags.size();

	xDim_ = 0;
	size_t x = 0;

	for (auto row : neighborHoodFlags)
	{
		xDim_ = std::max((int) row.size(), xDim_);

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

CellularAutomaton::CellularAutomaton(int xDim, int yDim, const NeighborHood& nbh, const NextStateFunction& nsf)
	: nbh_(nbh), nsf_(nsf), xDim_(xDim), yDim_(yDim)
{
	rows_ = new bool*[yDim_];
	newrows_ = new bool*[yDim_];

	for (int r = 0; r < yDim_; r++)
	{
		rows_[r] = new bool [xDim_] {};
		newrows_[r] = new bool [xDim_] {};
	}
}


void CellularAutomaton::update()
{
	int hotspotX = nbh_.xDim() / 2;
	int hotspotY = nbh_.yDim() / 2;

	for (int r = hotspotY; r < yDim_ - hotspotY; r++)
	{
		for (int c = hotspotX; c < xDim_ - hotspotX; c++)
		{
			int count = 0;

			for (int nr = 0; nr < nbh_.yDim(); nr++)
			{
				for (int nc = 0; nc < nbh_.xDim(); nc++)
				{
					if (nbh_[nr][nc])
					{
						if (rows_[r - hotspotY + nr][c - hotspotX + nc])
							count++;
					}
				}
			}

			newrows_[r][c] = nsf_[rows_[r][c]][count];
		}
	}

	std::swap(newrows_, rows_);
}

CellularAutomaton::~CellularAutomaton()
{
	for (int r = 0; r < yDim_; r++)
	{
		delete [] rows_[r];
		delete [] newrows_[r];
	}

	delete [] rows_;
	delete [] newrows_;
}

void CellularAutomaton::insert(int xPos, int yPos, const std::initializer_list<std::initializer_list<bool>>& newStates)
{
	int r = 0;
	for (auto row : newStates)
	{
		int c = 0;
		for (bool f : row)
		{
			rows_[r+yPos][c+xPos] = f;
			c++;
		}
		r++;
	}
}

NextStateFunction::NextStateFunction(const std::initializer_list<std::initializer_list<bool>>& nextStates)
	: nextState_ {}
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
		maxCount_ = (int) std::max(first->size(), second->size());

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

void CellularAutomaton::print() const
{
	for (int c = 0; c < xDim_; c++)
		std::cout << "-";
	std::cout << std::endl;

	for (int r = 0; r < yDim_; r++)
	{
		for (int c = 0; c < xDim_; c++)
		{
			std::cout << rows_[r][c];
		}
		std::cout << std::endl;
	}

	for (int c = 0; c < xDim_; c++)
		std::cout << "-";
	std::cout << std::endl;
}

void CellularAutomaton::clear()
{
	for (int r = 0; r < yDim_; r++)
	{
		for (int c = 0; c < xDim_; c++)
		{
			rows_[r][c] = false;
		}
	}
}