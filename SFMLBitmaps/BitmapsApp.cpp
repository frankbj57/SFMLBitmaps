#include "BitmapsApp.h"

#include <iostream>

using namespace std;

BitmapsApp::BitmapsApp()
	: SFApp("SFML Bitmaps")
{
}

void BitmapsApp::init()
{
	displayCommands();
	// Continue on any key pressed
	// system("pause");

	window.create(sf::VideoMode(400, 400),
		appName);

	window.setFramerateLimit(30);

	bitmap_.create(800, 600);

	circle.setFillColor(sf::Color::Green);

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
			zoomFactor_ += 0.5;
			bitmap_.setScale(zoomFactor_, zoomFactor_);
			break;

		case sf::Keyboard::Subtract:
		case sf::Keyboard::Hyphen:
			zoomFactor_ -= 0.5;
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

		case sf::Keyboard::G:
			generate(bitmap_);
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
	static sf::Uint8 pixels[]
		=
	{
		255, 255, 255, 255,
		0, 0, 0, 255,
		0, 0, 0, 255,
		255, 255, 255, 255
	};

	window.clear();

	sf::Texture tex2x2;

	tex2x2.create(2, 2);

	tex2x2.update(pixels);

	// tex2x2.setRepeated(true);

	sf::Sprite spr;

	spr.setTexture(tex2x2);


	// spr.setTextureRect(sf::IntRect(0, 0, 100, 100));
	// set texture to be repeated to fill the new rectangle size with copies

	spr.setScale(50,50);
	// this will stretch the texture along with the size of the rectangle of the sprite
	// For sprites, stretching overrides repeat

	sf::Vector2f pos = circle.getPosition();

	circle.setPosition(pos.x + 1, pos.y + 1);

	circle.setTexture(&tex2x2);

	// For cirles, apparently size will stretch the texture

	window.draw(circle);

	window.draw(spr);

	bitmap_.draw(window);

	window.display();
}


void BitmapsApp::cleanup()
{
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

	for (int y = 0; y < 600; y++)
		for (int x = 0; x < 800; x++)
		{
			float fx = -15.0f + (87.0f / 800) * x;
			float fy = -20.0f + (87.0f / 600) * y;
			float sqr = fx * fx + fy * fy;
			int isqr = (int)sqr;
			if (isqr % 2)
				bitmap.setPixel(x, y, sf::Color::White);
		}
}
