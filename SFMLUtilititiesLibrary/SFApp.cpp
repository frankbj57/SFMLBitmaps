#include "SFApp.h"

SFApp::SFApp(const std::string& name)
	: appName_(name)
{

}

void SFApp::run()
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

void SFApp::handleEvent(sf::Event &event)
{

}

