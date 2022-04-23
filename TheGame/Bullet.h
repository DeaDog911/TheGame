#pragma once

#include "Entity.h"
#include "Level.h"

class Bullet : public Entity {
private:
	float bullet_speed;
	float targetX, targetY, distance;
	float vx, vy;
public:
	int num;
	float fromX, fromY;
	Bullet(sf::Image& image, std::string Name, Level& lvl, float X, float Y, float W, float H, float tX, float tY, int Num) : Entity(image, Name, X, Y, W, H) {
		num = Num;
		life = true;
		obj = lvl.GetObjects("solid");
		sprite.setTextureRect(sf::IntRect(0, 0, width, height));
		bullet_speed = 2;
		targetX = tX;
		targetY = tY;
		fromX = X, fromY = Y;
		distance = sqrt((targetX - x) * (targetX - x) + (targetY - y) * (targetY - y));
		vx = (targetX - x) / distance;
		vy = (targetY - y) / distance;
		rotate(targetX, targetY);
	}

	void update(float time)
	{
		x += vx * time * bullet_speed;
		y += vy * time * bullet_speed;
		if (x <= 0) x = 1;
		if (y <= 0) y = 1;
		for (int i = 0; i < obj.size(); i++) {
			if (getRect().intersects(sf::FloatRect(obj[i].getAABB().left, obj[i].getAABB().top, obj[i].getAABB().width, obj[i].getAABB().height)))
			{
				life = false;
			}
		}
		sprite.setPosition(x + width / 2, y + height / 2);
	}
};
