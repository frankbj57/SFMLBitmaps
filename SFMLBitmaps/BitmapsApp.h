#pragma once
#include "SFApp.h"
#include "Bitmap.h"
#include "CellularAutomaton.h"

class BitmapsApp :
	public SFApp
{
public:
	BitmapsApp();

	virtual void init();

	virtual void run();

	virtual void cleanup();

protected:
	void handleEvent(sf::Event& event);

	void handleFrame();

private:
	void displayCommands();
	void render(const CellularAutomaton &ca, Bitmap& bitmap);

	void Center();
	void Fit();

	Bitmap bitmap_[2];
	unsigned int currentBitmap_ = 0;

	float zoomFactor_ = 1.0;
	float offsetX_ = 0.0;
	float offsetY_ = 0.0;

	CellularAutomaton *pAutomaton;
	bool active_ = false;

	double fadein_ = 1.00;

	sf::Clock clock_;

	sf::Int32 fadeTime_ = 2000; // In milliseconds
};

