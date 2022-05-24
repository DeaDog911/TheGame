#pragma once

#include "Entity.h"
#include "Level.h"
#include "Weapon.h"

class Person : public Entity {
protected:
	float shootTime;
	float legsTime;
	const float personSpeed = 0.2;
	sf::Image killedImage;
private:
	sf::Texture legsTexture;
	float dt;
	bool hitted = false;
	float shootX, shootY;
public:
	bool got;
	bool attack = false;
	sf::Sprite legsSprite;
	Weapon weapon;
	float health;
	bool isShoot, isHit;
	Person() {}
	Person(sf::Image& personImage, sf::Image& LegsImage, std::string Name, Level& lev, float X, float Y, float W, float H) : Entity(personImage, Name, X, Y, W, H) {
		legsTime = 0;
		shootTime = 0;
		dt = 75;
		isShoot = false;
		isHit = false;
		legsTexture.loadFromImage(LegsImage);
		legsSprite.setTexture(legsTexture);
		legsSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		legsSprite.setOrigin(16, 16);
		health = 100;
		got = false;
	}

	void takeWeapon(Weapon Weapon) {

		weapon = Weapon;
		weapon.active = true;
		if (weapon.type == Melee) {
			sf::Image image;
			image.loadFromFile(getPlayerSpriteFile(weapon.name));
			texture.loadFromImage(image);
			sprite.setTextureRect(getPlayerTextureRect(weapon.name));
			sprite.setOrigin(8, 15);
		}
		else if (weapon.type == Gun) {
			sf::Image image;
			image.loadFromFile(getPlayerSpriteFile(weapon.name));
			texture.loadFromImage(image);
			sprite.setTextureRect(getPlayerTextureRect(weapon.name));
			sprite.setOrigin(16, 8);
		}
	}

	void updateLegs() {
		if (isMove) {
			if (legsTime >= dt * 16 || isMove == false) legsTime = 0;
			legsSprite.setTextureRect(sf::IntRect(32 * (int(legsTime / dt)) + 1, 0, 32, 32));
		}
		else {
			legsSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		}
		if (attack) {
			legsSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
			legsTime = 0;
		}
		legsSprite.setPosition(x + width / 2, y + height / 2);
	}

	void rotate(float X, float Y) {
		if (life) {
			float dX = X - x;
			float dY = Y - y;
			float rotation = (atan2(dY, dX)) * 180 / 3.14159265;
			sprite.setRotation(rotation);
			legsSprite.setRotation(rotation);
		}
	}

	void stopInFront(sf::FloatRect obj, float Dx, float Dy) {
		if (Dy > 0) { y = obj.top - height; speedY = 0; }
		if (Dy < 0) { y = obj.top + obj.height; speedY = 0; }
		if (Dx > 0) { x = obj.left - width; }
		if (Dx < 0) { x = obj.left + obj.width; }
		legsTime = 0;
	}

	void setDeadSprite(float angle) {
		texture.loadFromImage(killedImage);
		isMove = false;
		sprite.setTexture(texture);
		sprite.setOrigin(38, 16);
		sprite.setPosition(x, y);
		width = 60 * 2;
		height = 32 * 2;
		sprite.setRotation(angle);
		sprite.setTextureRect(sf::IntRect(0, 0, 60, 32));
		for (int i = 0; i < obj.size(); i++) {
			if (obj[i].getName() == "solid") {
				if (getRect().intersects(sf::FloatRect(obj[i].getAABB().left, obj[i].getAABB().top, obj[i].getAABB().width, obj[i].getAABB().height))) {
					tmx::FloatRect tmxRect = obj[i].getAABB();
					stopInFront(sf::FloatRect(tmxRect.left, tmxRect.top, tmxRect.width, tmxRect.height), speedX, speedY);
				}
			}
		}
		weapon.setPosition(x, y);
		weapon.active = false;
	}

	void updateSprite() {
		float shootDt = dt + 5;
		float hitDt = 60;
		if (isShoot) {
			if (weapon.name == Shotgun) {
				int i = int(shootTime / shootDt);
				if (shootTime >= shootDt * 12) { shootTime = 0; isShoot = false; return; };
				if (i < 2) {
					sprite.setTextureRect(sf::IntRect(2 + i * 42, 8, 42, 18));
				}
				else {
					sprite.setTextureRect(sf::IntRect(91 + (12 + 32) * (i - 2), 8, 32, 18));
				}
			}
			else if (weapon.name == Pistol) {
				sf::IntRect rects[3] = {
				sf::IntRect(0, 6, 42, 13),
				sf::IntRect(45, 5, 43, 14),
				sf::IntRect(88, 6, 36, 13),
				};
				int i = int(shootTime / shootDt);
				if (i >= 3) {
					shootTime = 0;
					isShoot = false;
					return;
				}
				sprite.setTextureRect(rects[i]);
			}
			else if (weapon.name == Rifle) {
				sf::IntRect rects[2] = {
				sf::IntRect(4, 9, 36, 16),
				sf::IntRect(49, 8, 29, 17),
				};
				int i = int(shootTime / shootDt);
				if (i >= 2) {
					shootTime = 0;
					isShoot = false;
					return;
				}
				sprite.setTextureRect(rects[i]);
			}
		}
		else if (isHit) {
			sf::IntRect rects[9] = {
				sf::IntRect(24, 5, 17, 33),
				sf::IntRect(82, 23, 16, 31),
				sf::IntRect(135, 23, 23, 18),
				sf::IntRect(195, 23, 23, 18),
				sf::IntRect(262, 23, 16, 31),
				sf::IntRect(324, 22, 36, 22),
				sf::IntRect(384, 13, 33, 29),
				sf::IntRect(444, 5, 23, 34),
				sf::IntRect(504, 5, 20, 33),
			};
			
			int i = int(shootTime / hitDt) + 1;

			if (i >= 10) {
				isHit = false;
				shootTime = false;
				return;
			}
			if (i >= 9) {
				i = 1;
			} else if (i >= 8) {
				i = 0;
			}
			if (i >= 4) { attack = true; }
			if (i == 0) {
				attack = false;
				got = false;
			}
			sprite.setTextureRect(rects[i]);
		}
	}

	sf::FloatRect getAttackRect() {
		sf::RectangleShape rectSh(sf::Vector2f(25, 30));
		rectSh.setPosition(sf::Vector2f(x + width / 2, y + height / 2));
		rectSh.setOrigin(0, 15);
		rectSh.setRotation(sprite.getRotation());
		return rectSh.getGlobalBounds();
	}

	void shoot() {
		isShoot = true;
		shootTime = 0;
		shootX = x;
		shootY = y;
		if (weapon.name == Shotgun) {
			weapon.ammoCount -= 3;
		}
		else {
			weapon.ammoCount--;
		}
		if (weapon.ammoCount < 0) weapon.ammoCount = 0;
	}

	void hit() {
		isHit = true;
		shootTime = 0;
	}

	/*
	sf::FloatRect getRect() {
		sf::RectangleShape sh(sf::Vector2f(width, height));
		sh.setPosition(sf::Vector2f(x + width / 2, y + height / 2));
		sh.setRotation(sprite.getRotation());
		sh.setOrigin(sh.getSize().x / 2, sh.getSize().y / 2);
		return sh.getGlobalBounds();
	}
	*/

	virtual std::string getPlayerSpriteFile(WeaponName name) = 0;
	bool virtual checkCollisionWithMap(float dx, float dy) = 0;
};