#pragma once

class AmmoBox : public Item{
private:
	float y0, maxDy;
	bool animationUp;
public:
	int ammoCount;
	AmmoBox(sf::Texture &texture) : Item() {
		ammoCount = 10;
		maxDy = 3;
		animationUp = false;
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(100, 49, 431, 381));
		sprite.setScale(0.05 / 2, 0.05 / 2);
		width = 431 * 0.05;
		height = 381 * 0.05;
	}
};