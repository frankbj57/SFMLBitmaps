#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class SFApp
{
public:
	SFApp(const std::string &name);

	virtual void init() = 0;

	virtual void run();

	virtual void cleanup() = 0;

	virtual void handleEvent(sf::Event& event) = 0;

	virtual void handleFrame() = 0;


protected:
	std::string appName_;
	sf::RenderWindow window_;

private:



};

