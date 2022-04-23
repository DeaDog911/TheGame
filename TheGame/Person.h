#pragma once

#include "Entity.h"
#include "Level.h"

class Person : public Entity {
private:
	sf::Texture legsTexture;
	float dt;
public:
	float shootTime;
	float isShoot;
	float legsTime;
	float shootX, shootY;
	const float personSpeed = 0.2;
	sf::Sprite legsSprite;
	Person(sf::Image& personImage, sf::Image& LegsImage, std::string Name, Level& lev, float X, float Y, float W, float H) : Entity(personImage, Name, X, Y, W, H) {

		legsTime = 0;
		shootTime = 0;
		dt = 75;
		isShoot = false;
		legsTexture.loadFromImage(LegsImage);
		legsSprite.setTexture(legsTexture);
		legsSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		legsSprite.setScale(2, 2);
		legsSprite.setOrigin(16, 16);
	}

	void updateLegs() {
		if (isMove) {
			if (legsTime >= dt * 16 || isMove == false) legsTime = 0;
			legsSprite.setTextureRect(sf::IntRect(32 * (int(legsTime / dt)) + 1, 0, 32, 32));
		}
		else {
			legsSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
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
		width = 60;
		height = 32;
		sprite.setOrigin(width / 2, height / 2);
		sprite.setRotation(angle);
		sprite.setTextureRect(sf::IntRect(0, 0, 60, 32));
	}

	void updateSprite() {
		float shootDt = dt + 5;
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
	}

	void shoot() {
		isShoot = true;
		shootTime = 0;
		shootX = x;
		shootY = y;
	}

};