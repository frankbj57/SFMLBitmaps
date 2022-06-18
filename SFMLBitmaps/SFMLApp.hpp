#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class SFMLApp
{
public:
	SFMLApp(const std::string& name) : appName_(name)
	{
		timer_.restart();
		prevElapsed = 0.0;
	}

	virtual void init()
	{
		window_.create(sf::VideoMode(1000, 800),
			appName_);

		window_.requestFocus();
	}

	virtual void run() final
	{
		while (window_.isOpen())
		{
			sf::Event event;
			while (window_.pollEvent(event))
			{
				handleEvent(event);
			}

			double elapsed = timer_.getElapsedTime().asMicroseconds() * 1.0e-6;
			handleFrame(elapsed-prevElapsed);
			prevElapsed = elapsed;
		}
	}


	virtual void cleanup()
	{

	}

	virtual void handleEvent(sf::Event& event)
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
			}
		}
	}


	virtual void handleFrame(double elapsedSeconds)
	{

	}


protected:
	std::string appName_;
	sf::RenderWindow window_;
	sf::Clock timer_;
	double prevElapsed;

private:

};
