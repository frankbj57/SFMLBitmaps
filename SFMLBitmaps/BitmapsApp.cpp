#include "BitmapsApp.h"

#include <iostream>
#include <algorithm>
#include <vector>

#ifdef _WIN32

//The dreaded windows include file...
#include <Windows.h>
#undef min
#undef max

#include <commdlg.h>

#endif

using namespace std;

bool GetSaveFileName(string& fileName, const vector<string>& extensions);

BitmapsApp::BitmapsApp()
	: SFMLApp("SFML Bitmaps"), pAutomaton(nullptr)
{
}

void BitmapsApp::init()
{
	// WCHAR parameter[1000];

	// GetPrivateProfileSection(L"CellAuto", parameter, sizeof(parameter), L"CellAutoConfig.ini");

	// Game-of-life neighborhood
	//NeighborHood gol_nb = {
	//	{1,1,1},
	//	{1,0,1},
	//	{1,1,1}
	//};

	//// Game-of-life next state function
	//NextStateFunction gol_nsf = {
	//	{0, 0, 0, 1, 0, 0, 0, 0, 0 },
	//	{0, 0, 1, 1, 0, 0, 0, 0, 0 }
	//};

	// pAutomaton = new CellularAutomaton(800, 600, gol_nb, gol_nsf);

	// Insert blinker
	//pAutomaton->insert(1, 1,
	//	{
	//		{0, 1, 0},
	//		{0, 1, 0},
	//		{0, 1, 0}
	//	});

	// Insert glider
	//pAutomaton->insert(25, 25,
	//	{
	//		{0, 1, 0},
	//		{0, 0, 1},
	//		{1, 1, 1}
	//	});

	//NeighborHood fredkin_nb =
	//{
	//	{0, 1, 0},
	//	{1, 0, 1},
	//	{0, 1, 0},
	//};

	NextStateFunction fredkin_nsf = {
		{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}
	};

	//pAutomaton = new CellularAutomaton(10000, 10000, fredkin_nb, fredkin_nsf);
	//pAutomaton->setWrapAround(false);

	//pAutomaton->insert(99, 99,
	//	{
	//		{1, 0, 1},
	//		{0, 0, 0},
	//		{1, 0, 1}
	//	});


	// Hotspot alternating between (3,2) og (3,3)
	HorizontallyAlternatingNeighborHood mayan = {
		{ 0, 1, 1, 1, 1, 0},
		{ 1, 1, 0, 0, 1, 1},
		{ 1, 0, 1, 1, 0, 1},
		{ 0, 1, 0, 0, 1, 0},
		{ 1, 0, 1, 1, 0, 1},
		{ 1, 1, 0, 0, 1, 1},
		{ 0, 1, 1, 1, 1, 0},
	};

	pAutomaton = new CellularAutomaton(800, 600, mayan, fredkin_nsf);

	//// Insert mayan start pattern
	pAutomaton->insert(400, 300,
		{
			string("101010101"),
			string{ "010101010" },
			string{ "101010101" },
			string{ "010101010" }
		}
	);

	displayCommands();
	// Continue on any key pressed
	// system("pause");

	window_.create(sf::VideoMode(1000, 800),
		appName_);

	window_.setFramerateLimit(30);

	bitmap_[currentBitmap_].create(800, 600);
	render(*pAutomaton, bitmap_[currentBitmap_]);

	bitmap_[1 - currentBitmap_].create(800, 600);

	Fit();

	window_.requestFocus();
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
			cout << "Zoom set to " << zoomFactor_ << endl;
			cout << "Offsets set to (" << offsetX_ << ", " << offsetY_ << ")" << endl;
			break;

		case sf::Keyboard::Add:
			zoomFactor_ *= 1.5;
			cout << "Zoom set to " << zoomFactor_ << endl;
			break;

		case sf::Keyboard::Subtract:
		case sf::Keyboard::Hyphen:
			zoomFactor_ /= 1.5;
			cout << "Zoom set to " << zoomFactor_ << endl;
			break;

		case sf::Keyboard::Right:
			offsetX_ -= 0.1f * window_.getSize().x;
			cout << "Offsets set to (" << offsetX_ << ", " << offsetY_ << ")" << endl;
			break;

		case sf::Keyboard::Left:
			offsetX_ += 0.1f * window_.getSize().x;
			cout << "Offsets set to (" << offsetX_ << ", " << offsetY_ << ")" << endl;
			break;

		case sf::Keyboard::Up:
			offsetY_ += 0.1f * window_.getSize().y;
			cout << "Offsets set to (" << offsetX_ << ", " << offsetY_ << ")" << endl;
			break;

		case sf::Keyboard::Down:
			offsetY_ -= 0.1f * window_.getSize().y;
			cout << "Offsets set to (" << offsetX_ << ", " << offsetY_ << ")" << endl;
			break;

		case sf::Keyboard::Space:
		case sf::Keyboard::G:
		{
			currentBitmap_ = 1 - currentBitmap_;
			pAutomaton->update();
			render(*pAutomaton, bitmap_[currentBitmap_]);
			fadein_ = 0.0;
			elapsedTime_ = 0.0;
		}
		break;

		case sf::Keyboard::A:
			active_ = !active_;
			break;

		case sf::Keyboard::S:
		{
			string fileName;
#ifdef _WIN32
			if (GetSaveFileName(fileName, { "PNG", "*.png", "Bitmap", "*.bmp", "Targa", "*.tga" }))
			{
				cout << "Saving to file: " << fileName << endl;
				cout << "Saving " << (bitmap_[currentBitmap_].saveToFile(fileName) ? "succeeded" : "did not succeed") << endl;
			}
#else
			cout << "Saving to file not implemented!" << endl;
#endif
		}
		break;

		// Center
		case sf::Keyboard::C:
		{
			if (event.key.shift)
			{
				if (continous_center = !continous_center)  // Assignment is intentional
					Center();
			}
			else
				Center();
		}
		break;

		// Fit
		case sf::Keyboard::F:
		{
			if (event.key.shift)
			{
				if (continous_fit = !continous_fit)  // Assignment is intentional
					Fit();
			}
			else
				Fit();
		}
		break;

		}
		break;

	default:
		SFMLApp::handleEvent(event);
		break;
	}
}

void BitmapsApp::handleFrame(double elapsedSeconds)
{
	elapsedTime_ += elapsedSeconds;

	window_.clear();

	sf::Transform transform;
	transform.translate(offsetX_, offsetY_);
	transform.scale(zoomFactor_, zoomFactor_);

	if (active_ && fadein_ >= 1.00)
	{
		currentBitmap_ = 1 - currentBitmap_;
		pAutomaton->update();
		render(*pAutomaton, bitmap_[currentBitmap_]);

		fadein_ = 0.0;
		elapsedTime_ = 0.0;
	}


	if (fadein_ >= 1.00)
	{
		sf::RenderStates renderState(transform);
		bitmap_[currentBitmap_].draw(window_, renderState);
	}
	else
	{
		if (elapsedTime_ >= fadeTime_)
			fadein_ = 1.00;
		else
		{
			fadein_ = elapsedTime_ / fadeTime_;
		}

		// Use a rectangle to fade the whole area, hitting all pixels
		sf::RectangleShape fullscreen_rect;
		fullscreen_rect.setPosition(0.0f, 0.0f);
		fullscreen_rect.setSize(sf::Vector2f(window_.getSize()));

		// Setup the RenderState with a blend
		sf::Uint8 fadeInAmount = (sf::Uint8)(fadein_ * 255);
		sf::Uint8 fadeOutAmount = (sf::Uint8)((1.0 - fadein_) * 255);
		cout << "Fadeout: " << (int) fadeOutAmount << " Fadein: " << (int) fadeInAmount << endl;
		fullscreen_rect.setFillColor(sf::Color(0, 0, 0, fadeOutAmount));

		//                    Src (rectangle)     Dst (old)                Operation
		sf::BlendMode fadeOut(
			sf::BlendMode::One, sf::BlendMode::One, sf::BlendMode::Add, // For RGB
			sf::BlendMode::SrcAlpha, sf::BlendMode::Zero, sf::BlendMode::Add); // For alpha
		sf::RenderStates renderBlur(fadeOut);
		renderBlur.transform = transform;

		// Now it should be the previous bitmap colors but with fadeout factor as alpha
		// So we keep this alpha as a possible factor when adding the new bitmap

		// Show old, faded out
		bitmap_[1 - currentBitmap_].draw(window_, transform);
		// bitmap_[1 - currentBitmap_].draw(window_, renderBlur);
		window_.draw(fullscreen_rect, renderBlur);

		// Now add the new with a weighting from the destination alpha
		renderBlur.blendMode = sf::BlendMode(
			sf::BlendMode::OneMinusDstAlpha, sf::BlendMode::DstAlpha, sf::BlendMode::Add, // Mix the colors
			sf::BlendMode::One, sf::BlendMode::Zero, sf::BlendMode::Add); // Ending with an alpha of 1

		bitmap_[currentBitmap_].draw(window_, renderBlur);
	}

	window_.display();

	if (fadein_ >= 1.0 && continous_fit)
	{
		Fit();
	}
	else if (fadein_ >= 1.0 && continous_center)
	{
		Center();
	}
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
	std::cout << "         'S' - Save picture" << std::endl;
	std::cout << "         'C' - Center active area (Shift+ for continuous)" << std::endl;
	std::cout << "         'F' - Fit active area (Shift+ for continuous)" << std::endl;
	std::cout << "       Space - One step" << std::endl;
	std::cout << "         'G' - One step" << std::endl;
	std::cout << "         'A' - Run steps automatically" << std::endl;
	std::cout << "        '<-' - Move left" << std::endl;
	std::cout << std::endl;
}

void BitmapsApp::render(const CellularAutomaton& ca, Bitmap& bitmap)
{
	bitmap.clear(sf::Color::White);

	for (int y = 0; y < std::min(ca.yDim(), bitmap.yDim()); y++)
		for (int x = 0; x < std::min(ca.xDim(), bitmap.xDim()); x++)
		{
			if (ca[y][x])
				bitmap.setPixel(x, y, sf::Color::Black);
		}
}

void BitmapsApp::Center()
{
	// Center
	int minX, minY, maxX, maxY;
	pAutomaton->activeArea(minX, maxX, minY, maxY);

	int offsetX, offsetY;
	double scale = zoomFactor_;

	if (maxX < minX || maxY < minY)
	{
		offsetX = 0;
		offsetY = 0;
	}
	else
	{
		if (scale < 1.0)
		{
			offsetX = minX / 2;
			offsetY = minY / 2;
		}
		else
		{
			offsetX = (1.0 - 0.5 / scale) * scale * minX;
			offsetY = (1.0 - 0.5 / scale) * scale * minY;
		}
	}

	// Cellular automation 0,0 is mapped to bitmap 0,0
	// Then the bitmap is rendered according to its scale and offset
	// Therefore we need to calculate the bitmap rendering
	if (offsetX_ != -offsetX || offsetY_ != -offsetY)
	{
		offsetX_ = -offsetX;
		offsetY_ = -offsetY;

		cout << "Offsets set to (" << offsetX_ << ", " << offsetY_ << ")" << endl;
	}
}

void BitmapsApp::Fit()
{
	int minX, minY, maxX, maxY;
	pAutomaton->activeArea(minX, maxX, minY, maxY);

	double scaleX, scaleY, scale;

	if (maxX < minX || maxY < minY)
	{
		scaleX = scaleY = scale = 1.0;
	}
	else
	{
		// Scale to window
		sf::Vector2u ws = window_.getSize();
		scaleX = (ws.x) / ((double)(maxX + 1 - minX)) / 2;
		scaleY = (ws.y) / ((double)(maxY + 1 - minY)) / 2;
		scale = std::min(scaleX, scaleY);
	}

	// Cellular automation 0,0 is mapped to bitmap 0,0
	// Then the bitmap is rendered according to its scale and offset
	// Therefore we need to calculate the bitmap rendering
	if (zoomFactor_ != scale)
	{
		zoomFactor_ = scale;
		cout << "Zoom set to " << zoomFactor_ << endl;

		Center();
	}
}

#ifdef _WIN32

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

#endif