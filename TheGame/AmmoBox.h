#pragma once

class AmmoBox {
private:
	float y0, maxDy;
	bool animationUp;
	float x, y;
	float width, height;
public:
	sf::Sprite sprite;
	int ammoCount;
	AmmoBox(sf::Texture &texture) {
		ammoCount = 10;
		maxDy = 3;
		animationUp = false;
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(100, 49, 431, 381));
		sprite.setScale(0.05, 0.05);
		width = 431 * 0.05;
		height = 381 * 0.05;
	}

	void update(float time) {
		float dy = 0.007;
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