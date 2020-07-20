#include "CellularAutomaton.h"
#include <algorithm>
#include <iostream>


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

	int startr, endr;
	int startc, endc;
	if (wrapAround_)
	{
		startr = std::max(0, (minActiveY_ - nbh_.yDim() + yDim_) % yDim_);
		endr = std::min(yDim_, (maxActiveY_ + nbh_.yDim() + yDim_) % yDim_);

		startc = std::max(0, (minActiveX_ - nbh_.xDim() + xDim_) % xDim_);
		endc = std::min(xDim_, (maxActiveX_ + nbh_.xDim() + xDim_) % xDim_);
	}
	else
	{
		startr = std::max(0, (minActiveY_ - nbh_.yDim()));
		endr = std::min(yDim_, (maxActiveY_ + nbh_.yDim()));

		startc = std::max(0, (minActiveX_ - nbh_.xDim()));
		endc = std::min(xDim_, (maxActiveX_ + nbh_.xDim()));
	}

	if (endr < startr)
	{
		startr = 0;
		endr = yDim_;
	}

	if (endc < startc)
	{
		startc = 0;
		endc = xDim_;
	}

	for (int r = startr;
		 r < endr; 
		 r++)
	{
		for (int c = startc; 
		     c < endc; 
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
							effR = (effR + yDim_) % yDim_;

							effC = (effC + xDim_) % xDim_;

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
			// Assignment is intentional
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
	minActiveX_ = minActiveX;
	minActiveY_ = minActiveY;
	maxActiveX_ = maxActiveX;
	maxActiveY_ = maxActiveY;
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
				if (rows_[newR][newC] = f)  // Assignment is intentional
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