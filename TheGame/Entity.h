#pragma once

#include <SFML/Graphics.hpp>
#include <tmxlite/ObjectGroup.hpp>

class Entity {
private:

public:
	sf::Image image;
	sf::Texture texture;
	bool life;
	std::vector<tmx::Object> obj;
	float speed, speedX, speedY;
	float width, height;
	float x, y;
	bool isMove;
	sf::Sprite sprite;
	std::string name;
	Entity() {}
	Entity(sf::Image& image, std::string Name, float X, float Y, float W, float H) {
		x = X, y = Y;
		width = W, height = H;
		life = true;
		name = Name;
		speed = 0, speedX = 0, speedY = 0;
		isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setPosition(x, y);
		sprite.setOrigin(width / 2, height / 2);
	}



	void setPosition(float X, float Y) {
		x = X, y = Y;
		sprite.setPosition(x, y);
	}

	void rotate(float X, float Y) {
		float dX = X - x;
		float dY = Y - y;
		float rotation = (atan2(dY, dX)) * 180 / 3.14159265;
		sprite.setRotation(rotation);
	}

	sf::FloatRect getRect() {
		sf::FloatRect rect(x, y, width, height);
		return rect;
	}

	virtual void update(float time) = 0;
};