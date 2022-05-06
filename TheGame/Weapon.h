#pragma once

enum WeaponType { Melee, Gun, None };
enum WeaponMelee {};
enum WeaponGun {};


class Weapon {
private:
	bool animationUp;
	float dy, maxDy;
	float y0;
	float x, y;
	float width, height;
public:
	float damage;
	bool active;
	int ammoCount;
	sf::Sprite sprite;
	WeaponType type;
	Weapon() { active = false; this->type = None; }
	Weapon(Texture & texture, WeaponType type) {
		this->type = type;
		active = false;
		sprite.setTexture(texture);
		if (type == Gun) {
			ammoCount = 10;
			width = 27;
			height = 9;
			damage = 50;
			sprite.setTextureRect(sf::IntRect(74, 22, width, height));
		}
		else if (type == Melee) {
			damage = 25;
			ammoCount = 0;
			width = 27;
			height = 4;
			sprite.setTextureRect(sf::IntRect(134, 21, width, height));
		}
		sprite.setScale(2, 2);
		width *= 2;
		height *= 2;
		maxDy = 3;
	}

	void update(float time) {
		dy = 0.007;
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