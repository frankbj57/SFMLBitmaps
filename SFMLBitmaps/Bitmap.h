#pragma once
#include <SFML/Graphics.hpp>

class Bitmap
{
public:
	void create(int width, int height, sf::Color background = sf::Color::Black);
	void clear(sf::Color background = sf::Color::Black);
	void setPixel(unsigned int x, unsigned int y, sf::Color color) {
		image_.setPixel(x, y, color);
		needsUpdate_ = true;
	}


	void draw(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window, sf::RenderStates renderStates);

	void setScale(float xScale, float yScale);
	void setPosition(float xPos, float yPos);

	int xDim() const { return image_.getSize().x; }
	int yDim() const { return image_.getSize().y; }

	bool saveToFile(const std::string& fileName);

private:
	void CheckAndUpdate();

	bool needsUpdate_ = true;
	sf::Texture texture_;
	sf::Image image_;
	sf::Sprite sprite_;
};

