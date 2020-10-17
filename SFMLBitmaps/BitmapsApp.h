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

	Bitmap bitmap_;
	Bitmap oldBitmap_;

	float zoomFactor_ = 1.0;
	float offsetX_ = 0.0;
	float offsetY_ = 0.0;

	CellularAutomaton *pAutomaton;
	bool active_ = false;

	double fadein_ = 1.00;

	sf::Clock clock_;

	sf::Int32 fadeTime_ = 5000; // In milliseconds
};

