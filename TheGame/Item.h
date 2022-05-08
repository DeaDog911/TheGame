#pragma once


class Item {
private:
	float dy;
	float maxDy;
	
	bool animationUp;
public:
	float x, y, y0;
	float width, height;
	sf::Sprite sprite;
	Item() {
		maxDy = 3;
		dy = 0.007;
		animationUp = false;
	}

	void update(float time) {
		if ((y0 - y) < maxDy && !animationUp) {
			y -= dy * time;
		}
		else if (y != y0) {
			animationUp = true;
			y += dy * time;
		}
		else if (y - y0 < maxDy) {
			y += dy * time;
		}
		if (y - y0 >= maxDy) animationUp = false;
		sprite.setPosition(x, y);
	}

	void setPosition(int x, int y) {
		y0 = y;
		this->x = x;
		this->y = y;
		sprite.setPosition(x, y);
	}

	sf::Vector2f getPosition() {
		return sf::Vector2f(x, y);
	}

	sf::Vector2f getSize() {
		return sf::Vector2f(width, height);
	}

	sf::FloatRect getRect() {
		return sf::FloatRect(x, y, width, height);
	}
};