#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include "view.h"
#include <cmath>

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/LayerGroup.hpp>
#include "parse.h"
#include "SFMLOrthogonalLayer.h"

#include "Level.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

using namespace std;
using namespace sf;
using namespace tmx;

class Aim : public Entity {
private:
public:
	Aim(Image& image, string Name, float X, float Y, float W, float H) : Entity(image, Name, X, Y, W, H) {
		image.createMaskFromColor(Color(255, 255, 255));
	}

	void update(float time) {}
};


void updateList(std::list<Entity*>::iterator it, std::list<Entity*> &entities, float time) {
	for (it = entities.begin(); it != entities.end();) {
		Entity* b = *it;
		b->update(time);
		if (b->life == false) {
			it = entities.erase(it);
			delete b;
		}
		else it++;
	}
}

void drawList(std::list<Entity*>::iterator it, std::list<Entity*> entities, RenderWindow &window) {
	for (it = entities.begin(); it != entities.end(); it++) { window.draw((*it)->sprite); }
}

float getDistance(float tempX, float tempY, float x, float y) {
	float distance = sqrt((tempX - x) * (tempX - x) + (tempY - y) * (tempY - y));
	return distance;
}

int main() {
	parse("map.tmx");
	RenderWindow window(VideoMode(1380, 720), "Game");
	window.setMouseCursorVisible(false);
	view.reset(sf::FloatRect(0, 0, 1380, 720));

	Level lvl("map.tmx");
	MapLayer layer0(lvl.map, 0);
	MapLayer layer1(lvl.map, 1);
	MapLayer layer2(lvl.map, 2);

	std::list<Entity*> enemies;
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;
	std::list<Entity*> bullets;

	Image aimImage;
	aimImage.loadFromFile("images/aim.png");
	Aim aim(aimImage, "Aim", 0, 0, 52, 52);

	sf::Sound shootSound;
	sf::SoundBuffer soundBuffer;
	soundBuffer.loadFromFile("audio/shoot.ogg");
	shootSound.setBuffer(soundBuffer);
	

	Image playerImage, legsImage;
	playerImage.loadFromFile("images/player.png");
	legsImage.loadFromFile("images/legs.png");
	Object playerObj = lvl.GetObject("player");
	Player player(playerImage, legsImage, "Player", lvl, playerObj.getAABB().left, playerObj.getAABB().top, 32, 18);

	Image bulletImage;
	bulletImage.loadFromFile("images/bullet_tr2.png");
	
	vector <Object> enemiesObj = lvl.GetObjects("enemy");
	for (int i = 0; i < enemiesObj.size(); i++) {
		enemies.push_back(new Enemy(playerImage, legsImage, "Enemy", lvl, enemiesObj[i].getAABB().left, enemiesObj[i].getAABB().top, 32, 18, i));
	}
	
	sf::Vector2i pixelPos;
	sf::Vector2f pos;
	float aimPosX, aimPosY;
	Clock clock;
	float dt = 70;
	const float VISION_ANGLE = 120; 
	float angle = -(VISION_ANGLE / 2), countDontSeePlayer = 0, countDontSeeSolid = 0;
	bool seeYa = false, barrier = false;
	int dVec = 10;
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		pixelPos = Mouse::getPosition(window);
		pos = window.mapPixelToCoords(pixelPos);
		aim.setPosition(pos.x, pos.y);
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left && player.life) {
					if (!player.isShoot) {
						bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, aim.x, aim.y, -1));
						player.shoot();
						shootSound.play();
					}
				}
			}
		}
		sf::FloatRect rect;
		sf::RectangleShape sh;
		int i = 0;
		for (it = enemies.begin(); it != enemies.end(); it++) {
			Enemy* enemy = (Enemy*)(*it);
			if (enemy->life) {
				for (it2 = bullets.begin(); it2 != bullets.end(); it2++) {
					Bullet* bullet = (Bullet*)(*it2);
					// Убийство ВРАГА
					if (enemy->getRect().intersects(bullet->getRect())) {
						if (enemy->num != bullet->num) {
							enemy->life = false;
							bullet->life = false;
							enemy->setDeadSprite(bullet->sprite.getRotation());
						}
					}
					// Убийство ИГРОКА
					if (player.getRect().intersects(bullet->getRect())) {
						if (bullet->num != -1) {
							player.life = false;
							bullet->life = false;
							player.setDeadSprite(bullet->sprite.getRotation());
						}
					}
				}
				if (enemy->getRect().intersects(player.getRect()) && player.life) {
					player.stopInFront(enemy->getRect(), player.speedX, player.speedY);
				}

				// Зрение врага

				rect.left = enemy->x; rect.top = enemy->y; 
				rect.width = 500;
				rect.height = 2;
				sh.setFillColor(Color(255, 255, 255));
				sh.setSize(sf::Vector2f(rect.width, rect.height));
				sh.setPosition(rect.left + 10, rect.top);
				sh.setRotation(enemy->sprite.getRotation() + angle);
				/*
				// Пересечение вектора взгляда с игроком
				/*bool rase = true;
				float vec_w = dVec;
				while(rase && vec_w < 500) {
					rect.width = vec_w;
					sh.setSize(sf::Vector2f(rect.width, rect.height));
					if (sh.getGlobalBounds().intersects(player.getRect())) {
						if (barrier) {
							seeYa = true;
							rase = false;
						}
						else {
							seeYa = true;
						}
					}
					// Проходим по солидам
					for (const auto& solid : lvl.GetObjects("solid")) {
						sf::FloatRect solidRect(solid.getAABB().left, solid.getAABB().top, solid.getAABB().width, solid.getAABB().height);
						// Пересечение вектора взгляда с препятствием
						if (sh.getGlobalBounds().intersects(solidRect)) {
							if (seeYa) {
								barrier = false;
								rase = false;
							}
							else {
								barrier = true;
								rase = false;
							}
						}
					}
					vec_w += dVec;
				}*/
				
				/*
				float destToPlayer = 0, minSolidDistance = 1000;
				if (sh.getGlobalBounds().intersects(player.getRect()) && player.life) {
					seeYa = true; 
					// Проходим по всем солидам
					for (const auto& solid : lvl.GetObjects("solid")) {
						sf::FloatRect solidRect(solid.getAABB().left, solid.getAABB().top, solid.getAABB().width, solid.getAABB().height);
						// Пересечение вектора взгляда с препятствием
						if (sh.getGlobalBounds().intersects(solidRect)) {
							float destToSolid = getDistance(enemy->x, enemy->y, solid.getAABB().left, solid.getAABB().top);
							if (destToSolid < minSolidDistance)
								minSolidDistance = destToSolid;
						}
					}
					destToPlayer = getDistance(enemy->x, enemy->y, player.x, player.y);
					if (minSolidDistance < destToPlayer) {
						barrier = true;
					}
					else {
						countDontSeeSolid++;
					}
				}
				else {
					countDontSeePlayer++;
				}
				if (countDontSeePlayer == VISION_ANGLE) { 
					seeYa = false; 
					countDontSeePlayer = 0; 
					barrier = false;
				}
				if (countDontSeeSolid == VISION_ANGLE) { countDontSeeSolid = 0; barrier = false; }
				if (!seeYa) barrier = false;
				//if (seeYa)
				//	cout << seeYa << " " << barrier << " " << destToPlayer << " " << minSolidDistance << endl;
				*/
				
				if (sh.getGlobalBounds().intersects(player.getRect())) {
					sf::FloatRect particle; particle.width = 5; particle.height = 5;
					particle.left = sh.getPosition().x;
					particle.top = sh.getPosition().y;
					float distance = sqrt((player.x - particle.left) * (player.x - particle.left) + (player.y - particle.top) * (player.y - particle.top));
					float vx = (player.x - particle.left) / distance;
					float vy = (player.y - particle.top) / distance;
					barrier = false;
					seeYa = true;
					while (!particle.intersects(player.getRect())) {
						particle.left += vx * 5;
						particle.top += vy * 5;

						for (const auto& solid : lvl.GetObjects("solid")) {
							sf::FloatRect solidRect(solid.getAABB().left, solid.getAABB().top, solid.getAABB().width, solid.getAABB().height);
							// Пересечение вектора взгляда с препятствием
							if (particle.intersects(solidRect)) {
								barrier = true;
							}
						}
					}
				} else {
					countDontSeePlayer++;
				}
				if (countDontSeePlayer == VISION_ANGLE) {
					seeYa = false;
					countDontSeePlayer = 0;
					barrier = false;
				}
				cout << seeYa << " " << barrier << endl;
				// Противник идет на игрока
				if (!barrier && seeYa && player.life) {
					enemy->isMove = true;
					enemy->goToCoords(player.x, player.y, time);
					if (!enemy->isShoot) {
						//bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, enemy->x, enemy->y, 16, 16, player.x, player.y, i));
						//enemy->shoot();
						//shootSound.play();
					}
				}
				// Противник идет по заданному пути
				if (!seeYa || barrier || !player.life) {
					enemy->isMove = true;
					enemy->goThePath(time);
				}
				if (angle >= 60) { angle = -60; countDontSeePlayer = 0; }
				angle++;
			}
			i++;
		}

		for (it = enemies.begin(); it != enemies.end(); it++) {
			(*it)->update(time);
		}
		updateList(it, bullets, time);
		

		player.rotate(aim.x, aim.y);
		player.update(time);

		window.setView(view);
		window.clear(Color(130, 137, 150));
		window.draw(layer0);
		window.draw(layer1);
		window.draw(layer2);
		
		for (it = enemies.begin(); it != enemies.end(); it++) {
			Enemy* enemy = (Enemy*)*it;
			if(enemy->life)
				window.draw(enemy->legsSprite);
			window.draw(enemy->sprite);
			
			window.draw(sh);
		}
		if (player.life)
			window.draw(player.legsSprite);
		window.draw(player.sprite);
		drawList(it, bullets, window);
		window.draw(aim.sprite);
		window.display();
		//Sleep(5);
	}
	return 0;
}