#pragma once

#include "Person.h"

class Enemy : public Person {
private:
	vector <tmx::Object> tracks;
	float trackId;
public:
	int num;
	Enemy(sf::Image & image, sf::Image& LegsImage, std::string Name, Level& lvl, float X, float Y, int W, int H, int Num) : Person(image, LegsImage, Name, lvl, X, Y, W, H) {
		num = Num;
		isMove = false;
		trackId = 0;
		obj = lvl.GetObjects("solid");
		tracks = lvl.GetObjectsWithType("track");
		if (name == "Enemy") {
			sprite.setTextureRect(sf::IntRect(90, 8, width, height));
			sprite.setScale(2, 2);
		}
	}

	void go(float time) {
		float tempX = tracks[trackId].getPosition().x;
		float tempY = tracks[trackId].getPosition().y;
		move(tempX, tempY, time);
		rotate(tempX, tempY);
		if (sqrt((tempX - x) * (tempX - x) + (tempY - y) * (tempY - y)) <= 2) {
			trackId++;
		}
		if (trackId == tracks.size()) trackId = 0;
	}

	void goToCoords(float X, float Y, float time) {
		move(X, Y, time);
		rotate(X, Y);
	}

	void update(float time)
	{
		if (life) {
			if (name == "Enemy") {
				legsTime += time;
				if (isShoot)
					shootTime += time;
				updateLegs();
				updateSprite();
				sprite.setPosition(x + width / 2, y + height / 2);
				checkCollisionWithMap(speedX, speedY);
			}
		}
	}

	sf::RectangleShape getEnemyView() {
		sf::FloatRect rect(x + 20, y, width + 300, height + 150);
		sf::RectangleShape sh(sf::Vector2f(rect.width, rect.height));
		sh.setFillColor(sf::Color(255, 255, 255));
		sh.setOrigin(0, rect.height / 2);
		sh.setPosition(rect.left, rect.top);
		sh.setRotation(sprite.getRotation());
		return sh;
	}
};