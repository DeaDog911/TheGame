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
#include "AmmoBox.h"
#include "HealthKit.h"
#include "Menu.h"

using namespace std;
using namespace sf;
using namespace tmx;

class Aim : public Entity {
private:
public:
	Aim(Image& image, string Name, float X, float Y, float W, float H) : Entity(image, Name, X, Y, W, H) {
		image.createMaskFromColor(Color(255, 255, 255));
		sprite.setOrigin(width / 2, height / 2);
		sprite.setScale(0.75, 0.75);
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
	menu(window);

	view.reset(sf::FloatRect(0, 0, 1380, 720));

	Level lvl("map.tmx");
	MapLayer layer0(lvl.map, 0);
	MapLayer layer1(lvl.map, 1);
	MapLayer layer2(lvl.map, 2);

	std::list<Entity*> enemies;
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;
	std::list<Entity*> bullets;

	list<Weapon> weapons;
	list<Weapon>::iterator it_w;

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

	sf::Font ammoFont;
	ammoFont.loadFromFile("fonts/GUNPLAY_.ttf");

	sf::Text ammoText;
	ammoText.setFont(ammoFont);
	ammoText.setCharacterSize(63);
	ammoText.setOrigin(ammoText.getGlobalBounds().width / 2, ammoText.getGlobalBounds().height / 2);
	ammoText.setFillColor(sf::Color(255, 42, 0));
	ammoText.setOutlineThickness(3);
	ammoText.setOutlineColor(sf::Color(0, 0, 0));

	sf::Image healthImage;
	sf::Texture healthTexture;
	sf::Sprite healthSprite;
	healthImage.loadFromFile("images/health.png");
	healthTexture.loadFromImage(healthImage);
	healthSprite.setTexture(healthTexture);
	healthSprite.setTextureRect(sf::IntRect(159, 319, 881, 97));
	float scale_k = 0.3;
	healthSprite.setScale(scale_k, scale_k);

	sf::RectangleShape healthRect;
	healthRect.setFillColor(Color(64, 64, 64));
	healthRect.setSize(sf::Vector2f(847 * scale_k, 65 * scale_k));
	healthRect.setScale(-1, 1);

	sf::Sound shootSound;
	sf::SoundBuffer shootBuffer, stepBuffer;
	shootBuffer.loadFromFile("audio/shoot.ogg");
	shootSound.setBuffer(shootBuffer);

	sf::Sound rifleShootSound, shotgunShootSound, pistolShotSound;
	sf::SoundBuffer rifleShootSoundBuffer, shotgunShootSoundBuffer, pistolShotSoundBuffer;
	rifleShootSoundBuffer.loadFromFile("audio/rifle_shoot2.wav");
	shotgunShootSoundBuffer.loadFromFile("audio/shotgun_shoot.wav");
	pistolShotSoundBuffer.loadFromFile("audio/pistol_shoot2.wav");
	rifleShootSound.setBuffer(rifleShootSoundBuffer);
	shotgunShootSound.setBuffer(shotgunShootSoundBuffer);
	pistolShotSound.setBuffer(pistolShotSoundBuffer);

	sf::Sound shootSounds[] = { rifleShootSound , shotgunShootSound, pistolShotSound };

	sf::Sound stepSound;
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

	sf::Sound raiseItemSound;
	sf::SoundBuffer raiseItemBuffer;
	raiseItemBuffer.loadFromFile("audio/raise_item.wav");
	raiseItemSound.setBuffer(raiseItemBuffer);

	sf::Sound healthKitSound;
	sf::SoundBuffer healthKitBuffer;
	healthKitBuffer.loadFromFile("audio/health_kit.wav");
	healthKitSound.setBuffer(healthKitBuffer);

	sf::Sound deathSound;
	sf::SoundBuffer deathSoundBuffer;
	deathSoundBuffer.loadFromFile("audio/death.wav");
	deathSound.setBuffer(deathSoundBuffer);

	sf::Sound enemyDeathSound;
	sf::SoundBuffer enemyDeathSoundBuffer;
	enemyDeathSoundBuffer.loadFromFile("audio/enemy_death.wav");
	enemyDeathSound.setBuffer(enemyDeathSoundBuffer);

	sf::Sound bulletHitSound;
	sf::SoundBuffer bulletHitSoundBuffer;
	bulletHitSoundBuffer.loadFromFile("audio/bullet_hit.wav");
	bulletHitSound.setBuffer(bulletHitSoundBuffer);

	Image playerImage, legsImage;
	playerImage.loadFromFile("images/player.png");
	legsImage.loadFromFile("images/legs.png");
	Object playerObj = lvl.GetObject("player");
	Player player(playerImage, legsImage, "Player", lvl, playerObj.getAABB().left, playerObj.getAABB().top, 32, 18);

	sf::Image weaponsImage;
	sf::Texture weaponsTexture;
	weaponsImage.loadFromFile("images/weapons.png");
	weaponsTexture.loadFromImage(weaponsImage);

	sf::Image ammoBoxImage;
	sf::Texture ammoBoxTexture;
	ammoBoxImage.loadFromFile("images/ammo_box.png");
	ammoBoxTexture.loadFromImage(ammoBoxImage);

	sf::Image healthKitImage;
	sf::Texture healthKitTexture;
	healthKitImage.loadFromFile("images/health_kit.png");
	healthKitTexture.loadFromImage(healthKitImage);

	Weapon wp(weaponsTexture, Gun, Shotgun);
	player.takeWeapon(wp);

	Image bulletImage;
	bulletImage.loadFromFile("images/bullet_tr2.png");

	vector <Object> enemiesObj = lvl.GetObjectsWithType("enemy");
	for (int i = 0; i < enemiesObj.size(); i++) {
		Enemy* enemy = new Enemy(playerImage, legsImage, "Enemy", enemiesObj[i].getName(), lvl, enemiesObj[i].getAABB().left, enemiesObj[i].getAABB().top, 32, 18, i);
		Weapon wp(weaponsTexture, Gun, Shotgun);
		enemy->takeWeapon(wp);
		enemies.push_back(enemy);
	}

	sf::Vector2i pixelPos;
	sf::Vector2f pos;
	Clock clock;
	float dt = 70;

	vector<Object> solids = lvl.GetObjects("solid");

	vector<Object> weaponsObj = lvl.GetObjectsWithType("weapon");

	for (int i = 0; i < weaponsObj.size(); i++) {
		Weapon weapon;
		if (weaponsObj[i].getProperties()[0].getStringValue() == "melee") {
			weapon = Weapon(weaponsTexture, Melee, getEnumIndex(weaponsObj[i].getName()));
		}
		else if (weaponsObj[i].getProperties()[0].getStringValue() == "gun") {
			weapon = Weapon(weaponsTexture, Gun, getEnumIndex(weaponsObj[i].getName()));
		}
		weapon.setPosition(weaponsObj[i].getAABB().left, weaponsObj[i].getAABB().top);
		weapons.push_back(weapon);
	}

	vector<Object> ammoBoxeesObj = lvl.GetObjects("ammoBox");
	list<AmmoBox> ammoBoxes;
	list<AmmoBox>::iterator it_ab;

	for (int i = 0; i < ammoBoxeesObj.size(); i++) {
		AmmoBox ammoBox(ammoBoxTexture);
		ammoBox.setPosition(ammoBoxeesObj[i].getAABB().left, ammoBoxeesObj[i].getAABB().top);
		ammoBoxes.push_back(ammoBox);
	}

	vector<Object> healthKitsObj = lvl.GetObjects("healthKit");
	list<HealthKit> healthKits;
	list<HealthKit>::iterator it_hk;

	for (int i = 0; i < healthKitsObj.size(); i++) {
		HealthKit healthKit(healthKitTexture);
		healthKit.setPosition(healthKitsObj[i].getAABB().left, healthKitsObj[i].getAABB().top);
		healthKits.push_back(healthKit);
	}

	put_solids(grid, solids);
	vector<pair<int, int>> path;
	vector<pair<int, int> >::iterator path_i;
	sf::Vector2f playerShootCoords;
	bool showEscape = false;
	bool leftMousePressed = false;
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		
		pixelPos = Mouse::getPosition(window);
		pos = window.mapPixelToCoords(pixelPos);
		aim.setPosition(pos.x, pos.y);

		if (player.weapon.type == Gun) {
			switch (player.weapon.name) {
			case Shotgun: 
				ammoSprite.setTextureRect(sf::IntRect(1139, 507, 211, 425));
				ammoSprite.setPosition(view.getCenter().x + 600, view.getCenter().y + 270);
				ammoSprite.setScale(0.15, 0.15);
				break;
			case Pistol: 
				ammoSprite.setTextureRect(sf::IntRect(36, 686, 101, 246)); 
				ammoSprite.setPosition(view.getCenter().x + 600, view.getCenter().y + 260);
				ammoSprite.setScale(0.3, 0.3);
				break;
			case Rifle: 
				ammoSprite.setTextureRect(sf::IntRect(448, 224, 121, 707));
				ammoSprite.setPosition(view.getCenter().x + 600, view.getCenter().y + 225);
				ammoSprite.setScale(0.15, 0.15);
				break;
			}
		}

		ammoText.setPosition(view.getCenter().x + 510, view.getCenter().y + 260);

		healthSprite.setPosition(view.getCenter().x - 650, view.getCenter().y - 330);
		healthRect.setPosition(view.getCenter().x - 645 + 849 * scale_k, view.getCenter().y - 325);
		healthRect.setSize(sf::Vector2f(((849 * scale_k) / 100) * (100 - player.health), 65 * scale_k));

		Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (leftMousePressed && player.life) {
				if (player.weapon.name == Rifle && !player.isShoot) {
					stepSound.stop();
					enemyStepSound.stop();
					if (player.weapon.ammoCount > 0) {
						bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, aim.x, aim.y, -1));
						player.shoot();
						shootSound.setVolume(100);
						shootSounds[getSoundId(player.weapon.name)].play();
						playerShootCoords.x = player.x;
						playerShootCoords.y = player.y;
					}
				}
			}
			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left && player.life) {
					leftMousePressed = true;
					if (!player.isShoot && player.weapon.type == Gun) {
						stepSound.stop();
						enemyStepSound.stop();
						if (player.weapon.ammoCount > 0) {
							bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, aim.x, aim.y, -1));
							if (player.weapon.name == Shotgun) {
								bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, aim.x - 15, aim.y - 15, -1));
								bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, aim.x + 15, aim.y + 15, -1));
							}
							player.shoot();
							shootSound.setVolume(100);
							shootSounds[getSoundId(player.weapon.name)].play();
							playerShootCoords.x = player.x;
							playerShootCoords.y = player.y;
						}
						if (player.weapon.ammoCount == 0)
							shootNoAmmoSound.play();
					}
					else if (!player.isHit && player.weapon.type == Melee) {
						player.hit();
						stepSound.stop();
						enemyStepSound.stop();
						meleeMissSound.play();
					}
				}
			}
			if (event.type == Event::KeyPressed) {
				if (Keyboard::isKeyPressed(Keyboard::E)) {
					for (it_w = weapons.begin(); it_w != weapons.end(); it_w++) {
						Weapon weapon = *it_w;
						if (!weapon.active) {
							if (weapon.getRect().intersects(player.getRect())) {
								weapon.active = true;
								Weapon player_weapon = player.weapon;
								player_weapon.setPosition(weapon.getPosition().x, weapon.getPosition().y);
								player_weapon.active = false;
								player.takeWeapon(weapon);
								weapons.push_back(player_weapon);
								weapons.erase(it_w);
								stepSound.stop();
								raiseItemSound.play();
								break;
							}
						}
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					
				}
			}
			if (event.type == Event::MouseButtonReleased) {
				if (event.key.code == Mouse::Left) {
					leftMousePressed = false;
				}
			}
		}
		sf::FloatRect rect;
		sf::RectangleShape sh;
		int i;
		for (it = enemies.begin(), i = 0; it != enemies.end(); it++, i++) {
			Enemy* enemy = (Enemy*)(*it);
			if (enemy->life) {
				if (bullets.size() > 0)
					for (it2 = bullets.begin(); it2 != bullets.end(); it2++) {
						Bullet* bullet = (Bullet*)(*it2);
						// Убийство ВРАГА
						if (enemy->getRect().intersects(bullet->getRect()) && bullet->life) {
							if (enemy->num != bullet->num) {
								shootSound.pause();
								bulletHitSound.play();
								shootSounds[getSoundId(player.weapon.name)].play();
								enemy->health -= bullet->damage;
								bullet->life = false;
								if (enemy->health <= 0) {
									enemy->life = false;
									enemy->setDeadSprite(bullet->sprite.getRotation());
									enemyDeathSound.play();
									enemy->weapon.active = false;
									weapons.push_back(enemy->weapon);
								}
							}
						}
						// Убийство ИГРОКА
						if (player.getRect().intersects(bullet->getRect()) && bullet->life) {
							if (bullet->num != -1) {
								shootSound.pause();
								bulletHitSound.play();
								shootSounds[getSoundId(player.weapon.name)].play();
								player.health -= bullet->damage;
								bullet->life = false;
								if (player.health <= 0) {
									player.life = false;
									player.setDeadSprite(bullet->sprite.getRotation());
									deathSound.play();
									player.weapon.active = false;
									weapons.push_back(player.weapon);
								}
							}
						}
					}

				if (player.attack && enemy->life && !player.got) {
					if (player.getAttackRect().intersects(enemy->getRect())) {
						player.got = true;
						enemy->health -= player.weapon.damage;
						stepSound.stop();
						enemyStepSound.stop();
						meleeMissSound.stop();
						meleeAttackSound.play();
						if (enemy->health <= 0) {
							enemy->life = false;
							enemy->setDeadSprite(enemy->sprite.getRotation());
							enemyDeathSound.play();
							enemy->weapon.active = false;
							weapons.push_back(enemy->weapon);
						}
					}
				}

				if (enemy->attack && player.life && !enemy->got) {
					if (enemy->getAttackRect().intersects(player.getRect())) {
						enemy->got = true;
						player.health -= enemy->weapon.damage;
						cout << "melee: " << player.health << endl;
						stepSound.stop();
						enemyStepSound.stop();
						meleeMissSound.stop();
						meleeAttackSound.play();
						if (player.health <= 0) {
							player.life = false;
							player.setDeadSprite(player.sprite.getRotation());
							deathSound.play();
							player.weapon.active = false;
							weapons.push_back(player.weapon);
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
					enemy->timeFromSeeYa = 0;
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
					enemy->timeFromSeeYa += time;
					enemy->rotate(player.x, player.y);
					if (enemy->weapon.type == Gun) {
						if (!enemy->isShoot && enemy->timeFromSeeYa >= 2000) {
							enemy->timeFromSeeYa = 0;
							bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, enemy->x, enemy->y, 16, 16, player.x, player.y, i));
							if (enemy->weapon.name == Shotgun) {
								bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, enemy->x, enemy->y, 16, 16, player.x - 15, player.y - 15, i));
								bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, enemy->x, enemy->y, 16, 16, player.x + 15, player.y + 15, i));
							}
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
							shootSounds[getSoundId(player.weapon.name)].play();
						}
					}
					else if (enemy->weapon.type == Melee) {
						if (!enemy->isHit && enemy->getAttackRect().intersects(player.getRect())) {
							enemy->hit();
							stepSound.stop();
							enemyStepSound.stop();
							meleeMissSound.play();
						}
					}

				}

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

		for (it_ab = ammoBoxes.begin(); it_ab != ammoBoxes.end(); it_ab++) {
			if (it_ab->getRect().intersects(player.getRect()) && player.weapon.type == Gun) {
				if (player.weapon.ammoCount < player.weapon.maxAmmoCount) {
					player.weapon.ammoCount += (it_ab->ammoCount);
					if (player.weapon.ammoCount > player.weapon.maxAmmoCount) player.weapon.ammoCount = player.weapon.maxAmmoCount;
					stepSound.stop();
					raiseItemSound.play();
					ammoBoxes.erase(it_ab);
					break;
				}
			}
		}

		for (it_hk = healthKits.begin(); it_hk != healthKits.end(); it_hk++) {
			if (it_hk->getRect().intersects(player.getRect()) && player.health < 100) {
				player.health += it_hk->healthCount;
				if (player.health > 100) player.health = 100;
				stepSound.stop();
				healthKitSound.play();
				healthKits.erase(it_hk);
				break;
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

		// Рисуем оружие, лежащее на земле
		for (it_w = weapons.begin(); it_w != weapons.end(); it_w++) {
			it_w->update(time);
			window.draw(it_w->sprite);
		}

		for (it_ab = ammoBoxes.begin(); it_ab != ammoBoxes.end(); it_ab++) {
			it_ab->update(time);
			window.draw(it_ab->sprite);
		}

		for (it_hk = healthKits.begin(); it_hk != healthKits.end(); it_hk++) {
			it_hk->update(time);
			window.draw(it_hk->sprite);
		}

		// Рисуем мертвого игрока
		if (!player.life) {
			window.draw(player.sprite);
		}

		// Рисуем врагов
		for (it = enemies.begin(); it != enemies.end(); it++) {
			Enemy* enemy = (Enemy*)*it;
			if (enemy->life)
				window.draw(enemy->legsSprite);
			window.draw(enemy->sprite);
		}

		// Рисуем живого игрока
		if (player.life) {
			window.draw(player.legsSprite);
			window.draw(player.sprite);
		}

		// Рисуем пули
		drawList(it, bullets, window);
		window.draw(aim.sprite);

		if (player.weapon.type == Gun) {
			window.draw(ammoSprite);
			ammoText.setString(std::to_string(player.weapon.ammoCount));
			window.draw(ammoText);
		}

		window.draw(healthSprite);
		window.draw(healthRect);

		window.display();

	}

	for (int i = 0; i < MAP_H; i++) {
		delete[] grid[i];
	}
	delete[] grid;

	return 0;
}