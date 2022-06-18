#pragma once
#include "SFMLApp.hpp"
#include "Bitmap.h"
#include "CellularAutomaton.h"

class BitmapsApp :
	public SFMLApp
{
public:
	BitmapsApp();

	virtual void init();

	virtual void cleanup();

protected:
	void handleEvent(sf::Event& event);

	void handleFrame(double elapsedSeconds) override;

private:
	void displayCommands();
	void render(const CellularAutomaton &ca, Bitmap& bitmap);

	void Center();
	void Fit();

	Bitmap bitmap_[2];
	unsigned int currentBitmap_ = 0;

	double zoomFactor_ = 1.0;
	float offsetX_ = 0.0;
	float offsetY_ = 0.0;

	CellularAutomaton *pAutomaton;
	bool active_ = false;

	bool continous_fit = false;
	bool continous_center = false;

	double fadein_ = 1.00;

	double fadeTime_ = 2.0; // In seconds
	double elapsedTime_ = 0.0; // In seconds
};

