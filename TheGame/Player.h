#pragma once

#include "Person.h"
#include "view.h"

using namespace sf;

class Player : public Person {
private:
	float playerSpeed = personSpeed;
	enum { left, right, up, down, stay, right_up, right_down, left_up, left_down } state;
public:
	float soundCircleRadius = 150.f;
	bool canMove;
	Player(Image& playerImage, Image& LegsImage, string Name, Level& lev, float X, float Y, float W, float H) : Person(playerImage, LegsImage, Name, lev, X, Y, W, H) {
		state = stay;
		sprite.setTextureRect(sf::IntRect(90, 8, width, height));
		obj = lev.GetAllObjects();
		//sprite.setScale(2, 2);
		//height *= 2;
		//width = height;
		canMove = true;
		killedImage.loadFromFile("images/killed_player2.png");
	}

	void control() {
		state = stay;
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				state = left; speed = playerSpeed;
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				state = right; speed = playerSpeed;
			}
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				state = up; speed = playerSpeed;
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				state = down; speed = playerSpeed;
			}
			if (Keyboard::isKeyPressed(Keyboard::S) && Keyboard::isKeyPressed(Keyboard::D)) {
				state = right_down; speed = playerSpeed;
			}
			if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::D)) {
				state = right_up; speed = playerSpeed;
			}
			if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::A)) {
				state = left_up; speed = playerSpeed;
			}
			if (Keyboard::isKeyPressed(Keyboard::S) && Keyboard::isKeyPressed(Keyboard::A)) {
				state = left_down; speed = playerSpeed;
			}
		}
	}

	void update(float time) {
		if (life && canMove) {
			control();
			switch (state)
			{
			case right: speedX = speed; speedY = 0; isMove = true; break;
			case left: speedX = -speed; speedY = 0; isMove = true; break;
			case up: speedY = -speed; speedX = 0; isMove = true; break;
			case down: speedY = speed; speedX = 0; isMove = true; break;
			case left_up: speedY = -speed; speedX = -speed; isMove = true; break;
			case left_down: speedY = speed; speedX = -speed; isMove = true; break;
			case right_up: speedY = -speed; speedX = speed; isMove = true; break;
			case right_down: speedY = speed; speedX = speed; isMove = true; break;
			case stay: speedX = speedY = speed = 0; isMove = false; break;
			}
			x += speedX * time;
			checkCollisionWithMap(speedX, 0);
			y += speedY * time;
			checkCollisionWithMap(0, speedY);
			if (!isMove) speed = 0;
			sprite.setPosition(x + width / 2, y + height / 2);
			setPlayerCoordinateForView(x, y);
			legsTime += time;
			updateSprite();
			if (isShoot || isHit) {
				shootTime += time;
			}
			updateLegs();
		}
		else {
			health = 0;
		}
	}

	string getPlayerSpriteFile(WeaponName name) {
		switch (name) {
		case Shotgun: return "images/player_shotgun2.png";
		case Pipe: return "images/player_pipe2.png";
		case Bat: return "images/player_bat2.png";
		case Rifle: return "images/player_rifle2.png";
		case Pistol: return "images/player_pistol2.png";
		}
	}

	bool checkCollisionWithMap(float Dx, float Dy) {
		for (int i = 0; i < obj.size(); i++) {
			if (getRect().intersects(sf::FloatRect(obj[i].getAABB().left, obj[i].getAABB().top, obj[i].getAABB().width, obj[i].getAABB().height)))//проверяем пересечение игрока с объектом
			{
				if (obj[i].getName() == "solid")//если встретили препятствие
				{
					tmx::FloatRect tmxRect = obj[i].getAABB();
					stopInFront(sf::FloatRect(tmxRect.left, tmxRect.top, tmxRect.width, tmxRect.height), Dx, Dy);
					return true;
				}
			}
		}
		return false;
	}
};