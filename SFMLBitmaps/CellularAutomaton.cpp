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
	: nbh_(nbh), nsf_(nsf), xDim_(xDim), yDim_(yDim), wrapAround_(true)
{
	rows_ = new bool*[yDim_];
	newrows_ = new bool*[yDim_];

	for (int r = 0; r < yDim_; r++)
	{
		rows_[r] = new bool [xDim_] {};
		newrows_[r] = new bool [xDim_] {};
	}

	minActiveX_ = minActiveY_ = INT_MAX;
	maxActiveX_ = maxActiveY_ = -1;
}


void CellularAutomaton::update()
{
	int hotspotX = nbh_.xDim() / 2;
	int hotspotY = nbh_.yDim() / 2;
	int minActiveX = INT_MAX;
	int minActiveY = INT_MAX;
	int maxActiveX = -1;
	int maxActiveY = -1;


	for (int r = std::max(0, minActiveY_-nbh_.yDim()); 
		 r < std::min(yDim_, maxActiveY_+1+nbh_.yDim()); 
		 r++)
	{
		for (int c = std::max(0, minActiveX_-nbh_.xDim()); 
		     c < std::min(xDim_, maxActiveX_+1+nbh_.xDim()); 
			 c++)
		{
			int count = 0;

			// Use the neighborhood to conditionally count neighbors
			for (int nr = 0; nr < nbh_.yDim(); nr++)
			{
				for (int nc = 0; nc < nbh_.xDim(); nc++)
				{
					if (nbh_[nr][nc])
					{
						// This cell in the neighborhood should be counted
						// Calculate effective cell to check, with wrap-around
						int effR = r - hotspotY + nr;
						int effC = c - hotspotX + nc;
						if (wrapAround_)
						{
							if (effR < 0)
								effR += yDim_;
							else if (effR >= yDim_)
								effR -= yDim_;
							//  effR = (effR + yDim_) % yDim_;

							if (effC < 0)
								effC += xDim_;
							else if (effC >= xDim_)
								effC -= xDim_;

							if (rows_[effR][effC])
								count++;
						}
						else
						{
							if (effR >= 0 && effR < yDim_ && effC >= 0 && effC < xDim_)
								if (rows_[effR][effC])
									count++;
						}
					}
				}
			}

			// Assign based on next state function
			// At the same time, check active area
			if (newrows_[r][c] = nsf_[rows_[r][c]][count])
			{
				minActiveX = std::min(minActiveX, c);
				maxActiveX = std::max(maxActiveX, c);
				minActiveY = std::min(minActiveY, r);
				maxActiveY = std::max(maxActiveY, r);
			};
		}
	}

	std::swap(newrows_, rows_);
	std::swap(minActiveX, minActiveX_);
	std::swap(minActiveY, minActiveY_);
	std::swap(maxActiveX, maxActiveX_);
	std::swap(maxActiveY, maxActiveY_);
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
			int newR = r + yPos;
			int newC = c + xPos;
			if (newR >= 0 && newR < yDim_ && newC >= 0 && newC < xDim_)
				if (rows_[newR][newC] = f)
				{
					minActiveX_ = std::min(minActiveX_, newC);
					maxActiveX_ = std::max(maxActiveX_, newC);
					minActiveY_ = std::min(minActiveY_, newR);
					maxActiveY_ = std::max(maxActiveY_, newR);
				}

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