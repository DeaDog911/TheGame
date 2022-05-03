#pragma once
#include <iostream>

#include "Person.h"
#include "Pathfinding.h"

class Enemy : public Person {
private:
	vector <tmx::Object> tracks;
	vector <pair<int, int> > path;
	vector <pair<int, int> >::iterator path_i;
	float path_time;
	float trackId;
	bool isCollised;
public:
	bool go;
	int num;
	Enemy(sf::Image & image, sf::Image& LegsImage, std::string Name, Level& lvl, float X, float Y, int W, int H, int Num) : Person(image, LegsImage, Name, lvl, X, Y, W, H) {
		path_time = 2000;
		bool isCollised = false;
		go = true;
		num = Num;
		isMove = false;
		trackId = 0;
		obj = lvl.GetObjects("solid");
		tracks = lvl.GetObjectsWithType("track");
		if (name == "Enemy") {
			sprite.setTextureRect(sf::IntRect(90, 8, width, height));
			sprite.setScale(2, 2);
		}
		//width *= 1.5;
		height *= 1.5;
		width = height;

	}

	void goThePath(int ** grid, float time) {
		if (tracks.size() > 0 && go) {
			float tempX = tracks[trackId].getPosition().x;
			float tempY = tracks[trackId].getPosition().y;
			goToCoords(tempX, tempY, grid, time);
			if (getRect().intersects(sf::FloatRect(tempX, tempY, 1, 1))) {
				trackId++;
			}
			if (trackId == tracks.size()) { 
				trackId = 0;
				//go = false; isMove = false; 
			}
		}
	}

	void goStright(float X, float Y, float time) {
		move(X, Y, time);
		rotate(X, Y);
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

	void goToCoords(float X, float Y, int ** grid, float time) {
		path_time += time;
		if (path_time >= 2000) {
			path = get_path(x / cell_w, y / cell_h, X / cell_w, Y / cell_h, grid);
			reverse(path.begin(), path.end());
			path_i = path.begin();
			path_time = 0;
		}

		isMove = true;
		int path_x = path_i->first * cell_w;
		int path_y = path_i->second * cell_h;
		isCollised = false;
		goStright(path_x, path_y, time);

		if (isCollised) {
			if (grid[path_i->second + 1][path_i->first] != MAXINT) {
				goStright(path_x, path_y + cell_h, time);
			}
			else if (grid[path_i->second - 1][path_i->first] != MAXINT) {
				goStright(path_x, path_y - cell_h, time);
			}
			else if (grid[path_i->second + 1][path_i->first + 1] != MAXINT) {
				goStright(path_x + cell_w, path_y + cell_h, time);
			}
			else if (grid[path_i->second - 1][path_i->first - 1] != MAXINT) {
				goStright(path_x - cell_w, path_y - cell_h, time);
			}
			else if (grid[path_i->second ][path_i->first + 1] != MAXINT) {
				goStright(path_x + cell_w, path_y, time);
			}
			else if (grid[path_i->second][path_i->first - 1] != MAXINT) {
				goStright(path_x - cell_w, path_y - cell_h, time);
			}
		}
		if (getRect().intersects(sf::FloatRect(path_x + 8, path_y + 8, 1, 1))) {
			if (path_i + 1 != path.end())
				path_i++;
		}
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
				//checkCollisionWithMap(speedX, speedY);
			}
		}
		else {
			isMove = false;
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

	void stopInFront(sf::FloatRect obj, float Dx, float Dy) {
		if (Dy > 0) { y = obj.top - height; speedY = 0; }
		if (Dy < 0) { y = obj.top + obj.height; speedY = 0; }
		if (Dx > 0) { x = obj.left - width; }
		if (Dx < 0) { x = obj.left + obj.width; }
		isCollised = true;
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