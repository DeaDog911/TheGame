#pragma once

#include "Entity.h"
#include "Level.h"
#include "Weapon.h"

class Person : public Entity {
protected:
	float shootTime;
	float legsTime;
	const float personSpeed = 0.2;
private:
	sf::Texture legsTexture;
	float dt;
	bool hitted = false;
	
	float shootX, shootY;
public:
	bool attack = false;
	sf::Sprite legsSprite;
	int ammoCount;
	Weapon weapon;
	bool isShoot, isHit;
	Person(sf::Image& personImage, sf::Image& LegsImage, std::string Name, Level& lev, float X, float Y, float W, float H) : Entity(personImage, Name, X, Y, W, H) {
		legsTime = 0;
		shootTime = 0;
		dt = 75;
		isShoot = false;
		isHit = false;
		legsTexture.loadFromImage(LegsImage);
		legsSprite.setTexture(legsTexture);
		legsSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		legsSprite.setScale(2, 2);
		legsSprite.setOrigin(16, 16);
		ammoCount = 10;
	}

	void takeWeapon(Weapon weapon) {
		this->weapon = weapon;
		if (weapon == Melee) {
			Image image;
			image.loadFromFile("images/player_melee.png");
			texture.loadFromImage(image);
			sprite.setTexture(texture);
			sprite.setTextureRect(sf::IntRect(82, 23, 16, 31));
			sprite.setOrigin(width / 2 / 1.5, height / 2 / 1.5);
		}
		else if (weapon == Gun) {
			Image image;
			image.loadFromFile("images/player.png");
			texture.loadFromImage(image);
			sprite.setTexture(texture);
			sprite.setTextureRect(sf::IntRect(90, 8, 32, 18));
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

	void checkCollisionWithMap(float Dx, float Dy) {
		for (int i = 0; i < obj.size(); i++) {
			if (getRect().intersects(sf::FloatRect(obj[i].getAABB().left, obj[i].getAABB().top, obj[i].getAABB().width, obj[i].getAABB().height)))//проверяем пересечение игрока с объектом
			{
				if (obj[i].getName() == "solid")//если встретили препятствие
				{
					tmx::FloatRect tmxRect = obj[i].getAABB();
					stopInFront(sf::FloatRect(tmxRect.left, tmxRect.top, tmxRect.width, tmxRect.height), Dx, Dy);
				}
			}
		}
	}

	void move(float tempX, float tempY, float time) {
		if (isMove) {
			float distance = sqrt((tempX - x) * (tempX - x) + (tempY - y) * (tempY - y));
			float tempSpeedX, tempSpeedY;
			if (distance > 2) {
				tempSpeedX = 0.1 * (tempX - x) / distance;
				tempSpeedY = 0.1 * (tempY - y) / distance;
				x += tempSpeedX * time;
				checkCollisionWithMap(tempSpeedX, 0);
				y += tempSpeedY * time;
				checkCollisionWithMap(0, tempSpeedY);
			}
			else { isMove = false; }
		}
	}

	void setDeadSprite(float angle) {
		image.loadFromFile("images/killed_person.png");
		texture.loadFromImage(image);
		isMove = false;
		sprite.setTexture(texture);
		sprite.setPosition(x, y);
		width = 60 * 2;
		height = 32 * 2;
		sprite.setOrigin(width / 2, height / 2);
		sprite.setRotation(angle);
		sprite.setTextureRect(sf::IntRect(0, 0, 60, 32));
		for (int i = 0; i < obj.size(); i++) {
			if (obj[i].getName() == "solid") {
				if (getRect().intersects(sf::FloatRect(obj[i].getAABB().left, obj[i].getAABB().top, obj[i].getAABB().width, obj[i].getAABB().height))) {
					cout << "solid" << endl;
					tmx::FloatRect tmxRect = obj[i].getAABB();
					stopInFront(sf::FloatRect(tmxRect.left, tmxRect.top, tmxRect.width, tmxRect.height), speedX, speedY);
				}
			}
		}
	}

	void updateSprite() {
		float shootDt = dt + 5;
		float hitDt = dt + 15;
		if (isShoot) {
			int i = int(shootTime / shootDt);
			if (shootTime >= shootDt * 12) { shootTime = 0; isShoot = false; return; };
			if (i < 2) {
				sprite.setTextureRect(sf::IntRect(2 + i * 42, 8, 42, 18));
			}
			else {
				sprite.setTextureRect(sf::IntRect(91 + (12 + 32) * (i - 2), 8, 32, 18));
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
			}
			sprite.setTextureRect(rects[i]);
		}
	}

	sf::FloatRect getAttackRect() {
		sf::RectangleShape rectSh(sf::Vector2f(40, 60));
		rectSh.setPosition(sf::Vector2f(x, y));
		rectSh.setOrigin(0, 30);
		rectSh.setRotation(sprite.getRotation());
		return rectSh.getGlobalBounds();
	}

	void shoot() {
		isShoot = true;
		shootTime = 0;
		shootX = x;
		shootY = y;
		ammoCount--;
	}

	void hit() {
		isHit = true;
		shootTime = 0;
	}

};