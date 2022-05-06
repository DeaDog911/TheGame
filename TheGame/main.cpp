#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include "view.h"
#include <cmath>
#include <vector>
#include <climits>

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
#include "consts.h"
#include "Pathfinding.h"

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
	if (entities.size() > 0) {
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
}

void drawList(std::list<Entity*>::iterator it, std::list<Entity*> entities, RenderWindow &window) {
	if (entities.size() > 0) {
		for (it = entities.begin(); it != entities.end(); it++) { window.draw((*it)->sprite); }
	}
}

float getDistance(float tempX, float tempY, float x, float y) {
	float distance = sqrt((tempX - x) * (tempX - x) + (tempY - y) * (tempY - y));
	return distance;
}


sf::CircleShape getSoundSpreadCircle(float radius, float x, float y) {
	sf::CircleShape soundCircle(radius);
	soundCircle.setOrigin(radius, radius);
	soundCircle.setPosition(sf::Vector2f(x, y));
	return soundCircle;
}

int main() {

	int** grid = new int* [MAP_H];
	for (int i = 0; i < MAP_H; i++) {
		grid[i] = new int[MAP_W];
	}

	for (int i = 0; i < MAP_H; i++) {
		for (int j = 0; j < MAP_W; j++) {
			grid[i][j] = -1;
		}
	}

	//parse("map.tmx");
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

	// Отображение количества патронов
	Image ammoImage;
	Texture ammoTexture;
	Sprite ammoSprite;
	ammoImage.loadFromFile("images/ammo.png");
	ammoImage.createMaskFromColor(Color(0, 0, 0));
	ammoTexture.loadFromImage(ammoImage);
	ammoSprite.setTexture(ammoTexture);
	ammoSprite.setTextureRect(sf::IntRect(1139, 507, 211, 425));
	ammoSprite.setOrigin(211 * 0.15f / 2, 425 * 0.15f / 2);
	ammoSprite.setScale(0.15f, 0.15f);

	sf::Font ammoFont;
	ammoFont.loadFromFile("fonts/GUNPLAY_.ttf");

	sf::Text ammoText;
	ammoText.setFont(ammoFont);
	ammoText.setCharacterSize(63);
	ammoText.setOrigin(ammoText.getGlobalBounds().width / 2, ammoText.getGlobalBounds().height / 2);
	ammoText.setFillColor(sf::Color(255, 42, 0));
	ammoText.setOutlineThickness(3);
	ammoText.setOutlineColor(sf::Color(0, 0, 0));


	sf::Listener listener;
	sf::Sound shootSound, stepSound;
	sf::SoundBuffer shootBuffer, stepBuffer;
	shootBuffer.loadFromFile("audio/shoot.ogg");
	shootSound.setBuffer(shootBuffer);

	stepBuffer.loadFromFile("audio/step3.wav");
	stepSound.setBuffer(stepBuffer);

	sf::Sound enemyStepSound;
	sf::SoundBuffer enemyStepBuffer;
	enemyStepBuffer.loadFromFile("audio/step3.wav");
	enemyStepSound.setBuffer(enemyStepBuffer);

	sf::Sound shootNoAmmoSound;
	sf::SoundBuffer shootNoAmmoBuffer;
	shootNoAmmoBuffer.loadFromFile("audio/shoot_no_ammo.wav");
	shootNoAmmoSound.setBuffer(shootNoAmmoBuffer);

	sf::Sound meleeAttackSound;
	sf::SoundBuffer meleeAttackBuffer;
	meleeAttackBuffer.loadFromFile("audio/meleeHit_cut.wav");
	meleeAttackSound.setBuffer(meleeAttackBuffer);

	sf::Sound meleeMissSound;
	sf::SoundBuffer meleeMissBuffer;
	meleeMissBuffer.loadFromFile("audio/meleeMiss.wav");
	meleeMissSound.setBuffer(meleeMissBuffer);


	Image playerImage, legsImage;
	playerImage.loadFromFile("images/player.png");
	legsImage.loadFromFile("images/legs.png");
	Object playerObj = lvl.GetObject("player");
	Player player(playerImage, legsImage, "Player", lvl, playerObj.getAABB().left, playerObj.getAABB().top, 32, 18);

	player.takeWeapon(Gun);

	Image bulletImage;
	bulletImage.loadFromFile("images/bullet_tr2.png");

	vector <Object> enemiesObj = lvl.GetObjects("enemy");
	for (int i = 0; i < enemiesObj.size(); i++) {
		Enemy* enemy = new Enemy(playerImage, legsImage, "Enemy", lvl, enemiesObj[i].getAABB().left, enemiesObj[i].getAABB().top, 32, 18, i);
		enemy->takeWeapon(Gun);
		enemies.push_back(enemy);
	}

	sf::Vector2i pixelPos;
	sf::Vector2f pos;
	Clock clock;
	float dt = 70;

	vector<Object> solids = lvl.GetObjects("solid");
	put_solids(grid, solids);
	vector<pair<int, int>> path;
	vector<pair<int, int> >::iterator path_i;

	sf::Vector2f playerShootCoords;
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		//listener.setPosition(sf::Vector3f(player.x, player.y, 0));

		pixelPos = Mouse::getPosition(window);
		pos = window.mapPixelToCoords(pixelPos);
		aim.setPosition(pos.x, pos.y);

		ammoSprite.setPosition(view.getCenter().x + 600, view.getCenter().y + 270);
		ammoText.setPosition(view.getCenter().x + 520, view.getCenter().y + 260);

		Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left && player.life) {
					if (!player.isShoot && player.weapon == Gun) {
						stepSound.stop();
						enemyStepSound.stop();
						if (player.ammoCount > 0 && player.weapon == Gun) {
							bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, aim.x, aim.y, -1));
							player.shoot();
							shootSound.setVolume(100);
							shootSound.play();
							playerShootCoords.x = player.x;
							playerShootCoords.y = player.y;
						}
						shootNoAmmoSound.play();
					}
					if (!player.isHit && player.weapon == Melee) {
						player.hit();
						stepSound.stop();
						enemyStepSound.stop();
						meleeMissSound.play();
						
					}
				}
			}
		}
		sf::FloatRect rect;
		sf::RectangleShape sh;
		for (it = enemies.begin(); it != enemies.end(); it++) {
			Enemy* enemy = (Enemy*)(*it);
			if (enemy->life) {
				if (bullets.size() > 0)
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

				if (player.attack && enemy->life) {
					if (player.getAttackRect().intersects(enemy->getRect())) {
						enemy->life = false;
						stepSound.stop();
						enemyStepSound.stop();
						meleeMissSound.stop();
						meleeAttackSound.play();
						enemy->setDeadSprite(enemy->sprite.getRotation());
					}
				}

				if (enemy->attack && player.life) {
					if (enemy->getAttackRect().intersects(player.getRect())) {
						player.life = false;
						stepSound.stop();
						enemyStepSound.stop();
						meleeMissSound.stop();
						meleeAttackSound.play();
						player.setDeadSprite(player.sprite.getRotation());
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
				sh.setRotation(enemy->sprite.getRotation() + enemy->angle);

				// Пересечение вектора взгляда с игроком
				bool intersects = false;
				if (sh.getGlobalBounds().intersects(player.getRect())) {
					intersects = true;
					sf::FloatRect particle; particle.width = 5; particle.height = 5;
					particle.left = sh.getPosition().x;
					particle.top = sh.getPosition().y;
					float distance = sqrt((player.x - particle.left) * (player.x - particle.left) + (player.y - particle.top) * (player.y - particle.top));
					float vx = (player.x - particle.left) / distance;
					float vy = (player.y - particle.top) / distance;
					enemy->barrier = false;
					enemy->seeYa = true;
					while (!particle.intersects(player.getRect())) {
						particle.left += vx * 5;
						particle.top += vy * 5;

						for (const auto& solid : lvl.GetObjects("solid")) {
							sf::FloatRect solidRect(solid.getAABB().left, solid.getAABB().top, solid.getAABB().width, solid.getAABB().height);
							// Пересечение вектора взгляда с препятствием
							if (particle.intersects(solidRect)) {
								enemy->barrier = true;
							}
						}
					}
				}
				if (enemy->barrier || !intersects) {
					enemy->countDontSeePlayer++;
				}

				if (enemy->countDontSeePlayer == enemy->VISION_ANGLE) {
					enemy->dontSeeYaTime += time;
					enemy->seeYa = false;
					enemy->countDontSeePlayer = 0;
					enemy->barrier = false;
				}

				if (!enemy->barrier && enemy->seeYa) {
					enemy->dontSeeYaTime = 0;
				}

				// Противник идет на игрока
				if (!enemy->barrier && enemy->seeYa && player.life || enemy->dontSeeYaTime <= 20 && player.life) {
					enemy->isMove = true;
					if (enemy->dontSeeYaTime <= 20)
						enemy->goToCoords(player.x, player.y, grid, time);
					else
						enemy->goStright(player.x, player.y, time);
					if (enemy->weapon == Gun) {
						if (!enemy->isShoot) {
							/*
							bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, enemy->x, enemy->y, 16, 16, player.x, player.y, i));
							float dest = getDistance(player.x, player.y, enemy->x, enemy->y);
							if (dest >= 1000) {
								shootSound.setVolume(15);
							}
							else if (dest >= 800) {
								shootSound.setVolume(25);
							}
							else if (dest >= 500) {
								shootSound.setVolume(50);
							}
							else if (dest >= 300) {
								shootSound.setVolume(75);
							}
							else {
								shootSound.setVolume(100);
							}
							enemy->shoot();
							stepSound.stop();
							enemyStepSound.stop();
							shootSound.play();
							*/
						}
					}
					else if (enemy->weapon == Melee) {
						if (!enemy->isHit && enemy->getAttackRect().intersects(player.getRect())) {
							enemy->hit();
							stepSound.stop();
							enemyStepSound.stop();
							meleeMissSound.play();
						}
					}

				}

				//cout << enemy->seeYa << " " << enemy->barrier << endl;

				// Противник идет по заданному пути
				if ((!enemy->goToShootSound && !(enemy->dontSeeYaTime <= 20)) || !player.life) {
					if (!enemy->seeYa || enemy->barrier || !player.life) {
						if (enemy->go) {
							enemy->isMove = true;
							enemy->goThePath(grid, time);
						}
					}
				}

				// Противник идет на звук выстрела, если попал в область распространения звука
				if ((!enemy->seeYa || enemy->barrier) && player.life) {
					if (enemy->goToShootSound) {
						enemy->goToCoords(playerShootCoords.x, playerShootCoords.y, grid, time);
						if (enemy->getRect().intersects(sf::FloatRect(playerShootCoords.x, playerShootCoords.y, 1.f, 1.f))) {
							enemy->goToShootSound = false;
						}
					}

					if (!enemy->goToShootSound) {
						if (getSoundSpreadCircle(player.soundCircleRadius, playerShootCoords.x, playerShootCoords.y).getGlobalBounds().intersects(enemy->getRect())) {
							if (player.isShoot) {
								enemy->goToCoords(playerShootCoords.x, playerShootCoords.y, grid, time);
								enemy->goToShootSound = true;
							}
						}
					}
				}

				if (enemy->angle >= 60) { enemy->angle = -60; enemy->countDontSeePlayer = 0; }
				enemy->angle++;
			}
		}

			for (it = enemies.begin(); it != enemies.end(); it++) {
				(*it)->update(time);
				if ((*it)->isMove) {
					if (enemyStepSound.getStatus() != sf::SoundSource::Status::Playing && stepSound.getStatus() != sf::SoundSource::Status::Playing) {
						enemyStepSound.setPitch(1.1f);
						float dest = getDistance(player.x, player.y, (*it)->x, (*it)->y);
						if (dest >= 400) {
							enemyStepSound.setVolume(0);
						}
						else if (dest >= 300) {
							enemyStepSound.setVolume(25);
						}
						else if (dest >= 200) {
							enemyStepSound.setVolume(50);
						}
						else if (dest >= 100) {
							enemyStepSound.setVolume(75);
						}
						else {
							enemyStepSound.setVolume(100);
						}
						enemyStepSound.play();
					}
				}

			}

			updateList(it, bullets, time);

			player.rotate(aim.x, aim.y);
			player.update(time);
			if (player.isMove) {
				if (stepSound.getStatus() != sf::SoundSource::Status::Playing) {
					stepSound.setPitch(1.1f);
					stepSound.play();
				}
			}

			window.setView(view);
			window.clear(Color(130, 137, 150));
			window.draw(layer0);
			window.draw(layer1);
			window.draw(layer2);

			if (!player.life) {
				window.draw(player.sprite);
			}

			for (it = enemies.begin(); it != enemies.end(); it++) {
				Enemy* enemy = (Enemy*)*it;
				if (enemy->life)
					window.draw(enemy->legsSprite);
				window.draw(enemy->sprite);
				/*
				sf::RectangleShape rectSh(sf::Vector2f(enemy->getRect().width, enemy->getRect().height));
				rectSh.setPosition(sf::Vector2f(enemy->getRect().left, enemy->getRect().top));
				rectSh.setOrigin(enemy->getRect().width / 2, enemy->getRect().height / 2);
				rectSh.setPosition(enemy->sprite.getPosition());
				//window.draw(rectSh);

				*/
				//window.draw(sh);
			}
			if (player.life) {
				window.draw(player.legsSprite);
				window.draw(player.sprite);
			}
			drawList(it, bullets, window);
			window.draw(aim.sprite);

			if (player.weapon == Gun) {
				window.draw(ammoSprite);
				ammoText.setString(std::to_string(player.ammoCount));
				window.draw(ammoText);
			}

			window.display();
			//Sleep(5);
		}

		for (int i = 0; i < MAP_H; i++) {
			delete[] grid[i];
		}
		delete[] grid;

		return 0;
}