#pragma once

class HealthKit : public Item {
private:
	float y0, maxDy;
	bool animationUp;
public:
	int healthCount;
	HealthKit(sf::Texture& texture) : Item() {
		maxDy = 3;
		animationUp = false;
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(203, 322, 830, 685));
		sprite.setScale(0.03 / 2, 0.03 / 2);
		width = 830 * 0.03;
		height = 685 * 0.03;
		healthCount = 50;
	}
};