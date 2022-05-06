#pragma once

enum WeaponType { Melee, Gun, None };
enum WeaponMelee {};
enum WeaponGun {};


class Weapon {
private:
	int x, y;
	int width, height;
	sf::Image image;
	sf::Texture texture;
public:
	bool free;
	int ammoCount;
	sf::Sprite sprite;
	WeaponType type;
	Weapon() { free = true; this->type = None; }
	Weapon(WeaponType type) {
		this->type = type;
		free = true;
		image.loadFromFile("images/weapons.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		if (type == Gun) {
			ammoCount = 10;
			width = 27;
			height = 9;
			sprite.setTextureRect(sf::IntRect(74, 22, width, height));
		}
		else if (type == Melee) {
			ammoCount = 0;
			width = 27;
			height = 4;
			sprite.setTextureRect(sf::IntRect(134, 21, width, height));

		}
		sprite.setScale(2, 2);
		width *= 2;
		height *= 2;
	}

	void setPosition(int x, int y) {
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