#include "Bitmap.h"

void Bitmap::create(int width, int height, sf::Color background)
{
	image_.create(width, height, background);

	//for (int r = 0; r < 600; r++)
	//	for (int c = 0; c < 800; c++)
	//	{
	//		image_.setPixel(c, r, sf::Color(((c + r) % 5) * 64 - 1, ((c + r) % 9) * 32 - 1, ((c + r) % 17) * 16 - 1));
	//	}

	texture_.loadFromImage(image_);

	sprite_.setTexture(texture_);

	needsUpdate_ = true;
}

void Bitmap::clear(sf::Color background)
{
	sf::Vector2u size = image_.getSize();
	image_.create(size.x, size.y, background);
	needsUpdate_ = true;
}

void Bitmap::setPixel(unsigned int x, unsigned int y, sf::Color color)
{
	image_.setPixel(x, y, color);
	needsUpdate_ = true;
}

void Bitmap::draw(sf::RenderWindow& window)
{
	if (needsUpdate_)
	{
		texture_.update(image_);
		needsUpdate_ = false;
	}

	window.draw(sprite_);
}

void Bitmap::setScale(float xScale, float yScale)
{
	sprite_.setScale(xScale, yScale);
}

void Bitmap::setPosition(float xPos, float yPos)
{
	sprite_.setPosition(xPos, yPos);
}
