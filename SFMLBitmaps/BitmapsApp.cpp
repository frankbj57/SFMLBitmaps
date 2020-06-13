#include "BitmapsApp.h"

#include <iostream>
#include <algorithm>

using namespace std;

//The dreaded windows include file...
#define WIN32_LEAN_AND_MEAN //Reduce compile time of windows.h
#include <Windows.h>
#undef min
#undef max

BitmapsApp::BitmapsApp()
	: SFApp("SFML Bitmaps"), pAutomaton(nullptr)
{
}

void BitmapsApp::init()
{
	WCHAR parameter[1000];

	GetPrivateProfileSection(L"CellAuto", parameter, sizeof(parameter), L"CellAutoConfig.ini");

	// Game-of-life neighborhood
	NeighborHood nb = {
		{1,1,1},
		{1,0,1},
		{1,1,1}
	};

	// Game-of-life next state function
	NextStateFunction gol_nsf = {
		{0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{0, 0, 1, 1, 0, 0, 0, 0, 0 }
	};

	pAutomaton = new CellularAutomaton(200, 150, nb, gol_nsf);

	// Insert blinker
	pAutomaton->insert(1, 1,
		{
			{0, 1, 0},
			{0, 1, 0},
			{0, 1, 0}
		});

	// Insert glider
	pAutomaton->insert(25, 25,
		{
			{0, 1, 0},
			{0, 0, 1},
			{1, 1, 1}
		});

	displayCommands();
	// Continue on any key pressed
	// system("pause");

	window.create(sf::VideoMode(400, 400),
		appName);

	window.setFramerateLimit(30);

	bitmap_.create(800, 600);

	generate(bitmap_);

	window.requestFocus();
}

void BitmapsApp::run()
{
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			handleEvent(event);
		}

		handleFrame();
	}
}

void BitmapsApp::handleEvent(sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;

	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			window.close();
			break;

		case sf::Keyboard::H:
			displayCommands();
			break;

		case sf::Keyboard::R:
			zoomFactor_ = 1;
			offsetX_ = 0;
			offsetY_ = 0;
			bitmap_.setScale(zoomFactor_, zoomFactor_);
			bitmap_.setPosition(offsetX_, offsetY_);
			break;

		case sf::Keyboard::Add:
			zoomFactor_ *= 1.5;
			bitmap_.setScale(zoomFactor_, zoomFactor_);
			break;

		case sf::Keyboard::Subtract:
		case sf::Keyboard::Hyphen:
			zoomFactor_ /= 1.5;
			bitmap_.setScale(zoomFactor_, zoomFactor_);
			break;

		case sf::Keyboard::Right:
			offsetX_ -= 0.1f * window.getSize().x;
			bitmap_.setPosition(offsetX_, offsetY_);
			break;

		case sf::Keyboard::Left:
			offsetX_ += 0.1f * window.getSize().x;
			bitmap_.setPosition(offsetX_, offsetY_);
			break;

		case sf::Keyboard::Up:
			offsetY_ += 0.1f * window.getSize().y;
			bitmap_.setPosition(offsetX_, offsetY_);
			break;

		case sf::Keyboard::Down:
			offsetY_ -= 0.1f * window.getSize().y;
			bitmap_.setPosition(offsetX_, offsetY_);
			break;

		case sf::Keyboard::Space:
		case sf::Keyboard::G:
			generate(bitmap_);
			break;

		case sf::Keyboard::A:
			active = !active;
			break;
		}
		break;

	default:
		SFApp::handleEvent(event);
		break;
	}
}

void BitmapsApp::handleFrame()
{
	window.clear();

	if (active)
		generate(bitmap_);

	bitmap_.draw(window);

	window.display();
}


void BitmapsApp::cleanup()
{
	delete pAutomaton;
	pAutomaton = nullptr;
}

void BitmapsApp::displayCommands()
{
    std::cout << "=========================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "               Welcome to SFML Bitmaps demo" << std::endl;
    std::cout << std::endl;
    std::cout << "  Plays around with textures, images and bitmaps" << std::endl;
    std::cout << std::endl;
    std::cout << "=========================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "View Controls:" << std::endl;
	std::cout << "         'H' - Show this list" << std::endl;
	std::cout << "         Esc - Quit Program" << std::endl;
	std::cout << "         'R' - Reset coordinates" << std::endl;
	std::cout << "         '+' - Zoom in" << std::endl;
	std::cout << "         '-' - Zoom out" << std::endl;
	std::cout << "        '->' - Move right" << std::endl;
	std::cout << "        '<-' - Move left" << std::endl;
	std::cout << "    Up arrow - Move up" << std::endl;
	std::cout << "  Down arrow - Move down" << std::endl;
	std::cout << std::endl;
}

void BitmapsApp::generate(Bitmap& bitmap)
{
	bitmap.clear(sf::Color::Black);

	pAutomaton->update();

	for (int y = 0; y < std::min(pAutomaton->yDim(), bitmap_.yDim()); y++)
		for (int x = 0; x < std::min(pAutomaton->xDim(), bitmap_.xDim()); x++)
		{
			if ((*pAutomaton)[y][x])
				bitmap_.setPixel(x, y, sf::Color::White);
		}
}
