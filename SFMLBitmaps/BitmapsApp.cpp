#include "BitmapsApp.h"

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

//The dreaded windows include file...
#include <Windows.h>
#undef min
#undef max

#include <commdlg.h>

bool GetSaveFileName(string& fileName, const vector<string> &extensions);

BitmapsApp::BitmapsApp()
	: SFApp("SFML Bitmaps"), pAutomaton(nullptr)
{
}

void BitmapsApp::init()
{
	WCHAR parameter[1000];

	// GetPrivateProfileSection(L"CellAuto", parameter, sizeof(parameter), L"CellAutoConfig.ini");

	// Game-of-life neighborhood
	NeighborHood gol_nb = {
		{1,1,1},
		{1,0,1},
		{1,1,1}
	};

	// Game-of-life next state function
	NextStateFunction gol_nsf = {
		{0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{0, 0, 1, 1, 0, 0, 0, 0, 0 }
	};

	 pAutomaton = new CellularAutomaton(103, 103, gol_nb, gol_nsf);

	// Insert blinker
	//pAutomaton->insert(1, 1,
	//	{
	//		{0, 1, 0},
	//		{0, 1, 0},
	//		{0, 1, 0}
	//	});

	// Insert glider
	pAutomaton->insert(25, 25,
		{
			{0, 1, 0},
			{0, 0, 1},
			{1, 1, 1}
		});

	//NeighborHood fredkin_nb =
	//{
	//	{0, 1, 0},
	//	{1, 0, 1},
	//	{0, 1, 0},
	//};

	//NextStateFunction fredkin_nsf = {
	//	{0, 1, 0, 1, 0, 1, 0, 1, 0, 1}
	//};

	//pAutomaton = new CellularAutomaton(10000, 10000, fredkin_nb, fredkin_nsf);
	//pAutomaton->setWrapAround(false);

	//pAutomaton->insert(99, 99,
	//	{
	//		{1, 0, 1},
	//		{0, 0, 0},
	//		{1, 0, 1}
	//	});


	// Hotspot alternating between (3,2) og (3,3)
	NeighborHood mayan = {
		{ 0, 1, 1, 1, 1, 0},
		{ 1, 1, 0, 0, 1, 1},
		{ 1, 0, 1, 1, 0, 1},
		{ 0, 1, 0, 0, 1, 0},
		{ 1, 0, 1, 1, 0, 1},
		{ 1, 1, 0, 0, 1, 1},
		{ 0, 1, 1, 1, 1, 0},
	};

	// Insert mayan start pattern
	pAutomaton->insert(0, 0,
		{""});

	displayCommands();
	// Continue on any key pressed
	// system("pause");

	window_.create(sf::VideoMode(400, 400),
		appName_);

	window_.setFramerateLimit(30);

	bitmap_.create(800, 600);

	render(*pAutomaton, bitmap_);

	window_.requestFocus();
}

void BitmapsApp::run()
{
	while (window_.isOpen())
	{
		sf::Event event;
		while (window_.pollEvent(event))
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
		window_.close();
		break;

	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			window_.close();
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
			offsetX_ -= 0.1f * window_.getSize().x;
			bitmap_.setPosition(offsetX_, offsetY_);
			break;

		case sf::Keyboard::Left:
			offsetX_ += 0.1f * window_.getSize().x;
			bitmap_.setPosition(offsetX_, offsetY_);
			break;

		case sf::Keyboard::Up:
			offsetY_ += 0.1f * window_.getSize().y;
			bitmap_.setPosition(offsetX_, offsetY_);
			break;

		case sf::Keyboard::Down:
			offsetY_ -= 0.1f * window_.getSize().y;
			bitmap_.setPosition(offsetX_, offsetY_);
			break;

		case sf::Keyboard::Space:
		case sf::Keyboard::G:
			pAutomaton->update();
			render(*pAutomaton, bitmap_);
			break;

		case sf::Keyboard::A:
			active = !active;
			break;

		case sf::Keyboard::S:
			{
				string fileName;
				if (GetSaveFileName(fileName, { "PNG", "*.png", "Bitmap", "*.bmp", "Targa", "*.tga"} ))
				{
					cout << "Saving to file: " << fileName << endl;
					cout << "Saving " << (bitmap_.saveToFile(fileName) ? "succeeded" : "did not succeed") << endl;
				}
			}
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
	window_.clear();

	if (active)
	{
		pAutomaton->update();
		render(*pAutomaton, bitmap_);
	}

	bitmap_.draw(window_);

	window_.display();
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

void BitmapsApp::render(const CellularAutomaton&ca, Bitmap& bitmap)
{
	bitmap.clear(sf::Color::White);

	for (int y = 0; y < std::min(ca.yDim(), bitmap_.yDim()); y++)
		for (int x = 0; x < std::min(ca.xDim(), bitmap_.xDim()); x++)
		{
			if (ca[y][x])
				bitmap_.setPixel(x, y, sf::Color::Black);
		}
}

bool GetSaveFileName(string& fileName, const vector<string>& extensions)
{
	OPENFILENAMEA opf;
	char strResult[500];

	vector<char> extensionlist;

	for (auto extension : extensions)
	{
		for (auto c : extension)
			extensionlist.push_back(c);

		extensionlist.push_back('\0');
	}
	extensionlist.push_back('\0');

	strcpy_s(strResult, sizeof(strResult), fileName.c_str());

	ZeroMemory(&opf, sizeof opf);
	opf.lStructSize = sizeof opf;
	opf.lpstrFile = strResult;
	opf.nMaxFile = sizeof strResult;


	opf.lpstrFilter = &extensionlist[0];
	opf.nFilterIndex = 1;

	opf.Flags = OFN_OVERWRITEPROMPT;

	if (GetSaveFileNameA(&opf))
	{
		fileName = strResult;
		if (opf.nFileExtension == 0)
		{
			// No extension in file name
			if (opf.nFilterIndex != 0 && (opf.nFilterIndex) * 2 <= extensions.size())
			{
				// User used on of the filters as file type
				// Use the corresponding extension
				string ext = extensions[(opf.nFilterIndex - 1) * 2 + 1];
				int i = 0;
				while (i < ext.size() && ext[i] != '.')
					i++;

				if (i < ext.size())
				{
					while (i < ext.size())
					{
						fileName.push_back(ext[i]);
						i++;
					}
				}
			}
		}
		return true;
	}

	return false;
}