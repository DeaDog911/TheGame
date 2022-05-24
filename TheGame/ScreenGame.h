#pragma once

class Aim : public Entity {
private:
public:
	Aim(Image& image, string Name, float X, float Y, float W, float H) : Entity(image, Name, X, Y, W, H) {
		image.createMaskFromColor(Color(255, 255, 255));
		sprite.setOrigin(width / 2, height / 2);
		sprite.setScale(0.5, 0.5);
	}

	void update(float time) {}
};


class ScreenGame : public cScreen {
private:
	void updateList(std::list<Entity*>::iterator it, std::list<Entity*>& entities, float time) {
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

	void drawList(std::list<Entity*>::iterator it, std::list<Entity*> entities, RenderWindow& window) {
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
	
	sf::Text escapeMenuTexts[3];
	sf::Text levelNum;
	sf::RectangleShape escapeRect;
	sf::RectangleShape escapeBackgroundRect;
	string lastWeapon;

	void initializeEescapeMenu(sf::Font &font) {
		escapeRect.setSize(sf::Vector2f(500 / zoom_k, 300 / zoom_k));
		escapeRect.setFillColor(sf::Color(64, 64, 64));
		escapeRect.setOutlineColor(sf::Color(255, 255, 255));
		escapeRect.setOutlineThickness(10 / zoom_k);
		escapeRect.setOrigin(escapeRect.getGlobalBounds().width / 2, escapeRect.getGlobalBounds().height / 2);
		escapeBackgroundRect.setFillColor(sf::Color(96, 96, 96, 150));

		for (int i = 0; i < 3; i++) {
			sf::Text escapeMenuText;
			escapeMenuText.setFont(font);
			escapeMenuText.setCharacterSize(40 / zoom_k);
			escapeMenuText.setFillColor(sf::Color(255, 255, 255));
			escapeMenuText.setOutlineColor(sf::Color(0, 0, 0));
			escapeMenuText.setOutlineThickness(1.5);
			escapeMenuTexts[i] = escapeMenuText;
		}
		escapeMenuTexts[0].setString("NEXT LEVEL");
		escapeMenuTexts[1].setString("RESTART LEVEL");
		escapeMenuTexts[2].setString("MAIN MENU");

		for (int i = 0; i < 3; i++) {
			escapeMenuTexts[i].setOrigin(escapeMenuTexts[i].getGlobalBounds().width / 2, escapeMenuTexts[i].getGlobalBounds().height / 2);
		}
	}

	sf::Music backgroundMusic;
	vector<string> maps;
	const float zoom_k = 2;
public:

	ScreenGame(vector<string> maps) {
		backgroundMusic.openFromFile("audio/Bio-Unit-Level.wav");
		this->maps = maps;
		lastWeapon = "default"; 
	}

	int Run(sf::RenderWindow& window, int& map_i) {
		backgroundMusic.play();

		int res;
		bool next = true;
		const int lvlCount = 4;
		for (int i = map_i; i < lvlCount;) {
			if (i == lvlCount - 1) next = false;
			else next = true;
			res = startGame(window, maps[i], next, map_i);
			while (res == -10) {
				saveLevel(i, "default");
				res = startGame(window, maps[i], next, map_i);
			}
			if (res == 10) {
				i++;
				saveLevel(i, lastWeapon);
				if (i == lvlCount - 1) next = false;
			}
			else {
				backgroundMusic.stop();
				return res;
			}
		}
		backgroundMusic.stop();
		return res;
	}

	int startGame(RenderWindow& window, string map_file, bool next, int map_i) {
		string weapon = getSaveWeapon();
		cout << "weapon: " << weapon << endl;
		if (map_file == "level_1_1.tmx" || map_file == "level_2_1.tmx" ) {
			MAP_H = 75;
			MAP_W = 75;
		}
		int** grid = new int* [MAP_H];
		for (int i = 0; i < MAP_H; i++) {
			grid[i] = new int[MAP_W];
		}

		for (int i = 0; i < MAP_H; i++) {
			for (int j = 0; j < MAP_W; j++) {
				grid[i][j] = -1;
			}
		}
		
		view.reset(sf::FloatRect(0, 0, 1380, 720));
		
		view.zoom(1 / zoom_k);

		Level lvl(map_file);

		MapLayer layer0(lvl.map, 0);
		MapLayer layer1(lvl.map, 1);
		MapLayer layer2(lvl.map, 2);
		MapLayer layer3(lvl.map, 3);

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
		ammoText.setCharacterSize(63 / zoom_k);
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
		float scale_k = 0.3 / zoom_k;
		healthSprite.setScale(scale_k, scale_k);

		sf::RectangleShape healthRect;
		healthRect.setFillColor(Color(64, 64, 64));
		healthRect.setSize(sf::Vector2f(847 * scale_k , 65 * scale_k));
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

		Image playerImage, enemyLegsImage, playerLegsImage;
		playerImage.loadFromFile("images/player_shotgun.png");
		enemyLegsImage.loadFromFile("images/legs.png");
		playerLegsImage.loadFromFile("images/legs_player.png");
		Object playerObj = lvl.GetObject("player");
		Player player(playerImage, playerLegsImage, "Player", lvl, playerObj.getAABB().left, playerObj.getAABB().top, 32, 18);

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

		string weaponString = playerObj.getProperties()[0].getStringValue();
		
		string weaponStr = getSaveWeapon();
		if (weapon == "default") {
			Weapon wp(weaponsTexture, getWeaponType(weaponString), getEnumIndex(weaponString));
			player.takeWeapon(wp);
		}
		else {
			Weapon wp(weaponsTexture, getWeaponType(weaponStr), getEnumIndex(weaponStr));
			player.takeWeapon(wp);
		}
		

		Image bulletImage;
		bulletImage.loadFromFile("images/bullet_tr2.png");

		vector <Object> enemiesObj = lvl.GetObjectsWithType("enemy");
		for (int i = 0; i < enemiesObj.size(); i++) {
			Enemy* enemy = new Enemy(playerImage, enemyLegsImage, "Enemy", enemiesObj[i].getName(), lvl, enemiesObj[i].getAABB().left, enemiesObj[i].getAABB().top, 32, 18, i);
			string weaponString = enemiesObj[i].getProperties()[0].getStringValue();
			Weapon wp(weaponsTexture, getWeaponType(weaponString), getEnumIndex(weaponString));
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
		bool showEndGame = false;
		bool showExit = false;
		bool goNextRoom = false;

		sf::Text youDiedText;
		youDiedText.setFont(ammoFont);
		youDiedText.setFillColor(sf::Color(255, 0, 0));
		youDiedText.setOutlineColor(sf::Color(0, 0, 0));
		youDiedText.setOutlineThickness(2 / zoom_k);
		youDiedText.setCharacterSize(50 / zoom_k);
		youDiedText.setString("YOU DIED");
		youDiedText.setOrigin(youDiedText.getGlobalBounds().width / 2, youDiedText.getGlobalBounds().height / 2);

		sf::Text endGameText;
		endGameText.setFont(ammoFont);
		endGameText.setFillColor(sf::Color(91, 76, 255));
		endGameText.setOutlineColor(sf::Color(0, 0, 0));
		endGameText.setOutlineThickness(2 / zoom_k);
		endGameText.setCharacterSize(50 / zoom_k);
		endGameText.setString("YOU WIN");
		endGameText.setOrigin(endGameText.getGlobalBounds().width / 2, endGameText.getGlobalBounds().height / 2);
		
		bool endGameExist;
		tmx::Object endGameObj, nextRoomObj;
		tmx::FloatRect endGame, nextRoom;

		if (lvl.objectExist("endGame")) {
			endGameExist = true;
			endGameObj = lvl.GetObject("endGame");
			endGame = endGameObj.getAABB();
		}
		else {
			endGameExist = false;
			nextRoomObj = lvl.GetObject("nextRoom");
			nextRoom = nextRoomObj.getAABB();
		}

		sf::Image exitImage;
		sf::Texture exitTexture;
		sf::Sprite exitSprite;

		if (endGameExist)
			exitImage.loadFromFile("images/exit.png");
		else 
			exitImage.loadFromFile("images/go.png");

		exitTexture.loadFromImage(exitImage);
		exitSprite.setTexture(exitTexture);
		exitSprite.setScale(0.2 / zoom_k, 0.2 / zoom_k);
		exitSprite.setOrigin(170, 0);

		if (endGameExist) {
			switch (atoi(endGameObj.getType().c_str())) {
			case 1:
				exitSprite.setPosition(endGame.left, endGame.top + endGame.height / 2);
				exitSprite.setRotation(90);
				break;
			case 2:
				exitSprite.setPosition(endGame.left + endGame.width / 2, endGame.top);
				exitSprite.setRotation(180);
				break;
			case 3:
				exitSprite.setPosition(endGame.left + endGame.width, endGame.top + endGame.height / 2);
				exitSprite.setRotation(-90);
				break;
			case 4:
				exitSprite.setPosition(endGame.left + endGame.width / 2, endGame.top + endGame.height);
				break;
			}
		}
		else {
			switch (atoi(nextRoomObj.getType().c_str())) {
			case 1:
				exitSprite.setPosition(nextRoom.left, nextRoom.top + nextRoom.height / 2);
				exitSprite.setRotation(90);
				break;
			case 2:
				exitSprite.setPosition(nextRoom.left + nextRoom.width / 2, nextRoom.top);
				exitSprite.setRotation(180);
				break;
			case 3:
				exitSprite.setPosition(nextRoom.left + nextRoom.width, nextRoom.top + nextRoom.height / 2);
				exitSprite.setRotation(-90);
				break;
			case 4:
				exitSprite.setPosition(nextRoom.left + nextRoom.width / 2, nextRoom.top + nextRoom.height);
				break;
			}
		}

		levelNum.setFont(ammoFont);
		levelNum.setFillColor(sf::Color(255, 255, 255));
		levelNum.setOutlineColor(sf::Color(0, 0, 0));
		levelNum.setOutlineThickness(2 / zoom_k);
		levelNum.setCharacterSize(50 / zoom_k);
		string str = "Level: " + to_string(map_i + 1);
		levelNum.setString(str);
		levelNum.setOrigin(levelNum.getGlobalBounds().width / 2, levelNum.getGlobalBounds().height / 2);

		sf::Image eButtonImage, controlsImage;
		sf::Texture eButtonTexture, controlsTexture;
		sf::Sprite eButtonSprite, controlsSprite;
		eButtonImage.loadFromFile("images/E_button.png");
		eButtonTexture.loadFromImage(eButtonImage);
		eButtonSprite.setTexture(eButtonTexture);

		controlsImage.loadFromFile("images/WASD.png");
		controlsTexture.loadFromImage(controlsImage);
		controlsSprite.setTexture(controlsTexture);
		controlsSprite.setScale(0.5 / zoom_k, 0.5 / zoom_k);

		bool showEButton = false;
		bool showControls = false;

		initializeEescapeMenu(ammoFont);

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
					ammoSprite.setPosition(view.getCenter().x + 600 / zoom_k, view.getCenter().y + 270 / zoom_k);
					ammoSprite.setScale(0.15 / zoom_k, 0.15 / zoom_k);
					break;
				case Pistol:
					ammoSprite.setTextureRect(sf::IntRect(36, 686, 101, 246));
					ammoSprite.setPosition(view.getCenter().x + 600 / zoom_k, view.getCenter().y + 260 / zoom_k);
					ammoSprite.setScale(0.3 / zoom_k, 0.3 / zoom_k);
					break;
				case Rifle:
					ammoSprite.setTextureRect(sf::IntRect(448, 224, 121, 707));
					ammoSprite.setPosition(view.getCenter().x + 600 / zoom_k, view.getCenter().y + 225 / zoom_k);
					ammoSprite.setScale(0.15 / zoom_k, 0.15 / zoom_k);
					break;
				}
			}

			ammoText.setPosition(view.getCenter().x + 510 / zoom_k, view.getCenter().y + 260 / zoom_k);

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
					if (showEscape) {
						if (escapeMenuTexts[0].getGlobalBounds().contains(pos.x, pos.y)) {
							lastWeapon = getWeaponNameStr(player.weapon.name);
							return 10;
						}
						if (escapeMenuTexts[1].getGlobalBounds().contains(pos.x, pos.y)) {
							return -10;
						}
						if (escapeMenuTexts[2].getGlobalBounds().contains(pos.x, pos.y)) {
							return 0;
						}
						
					}
					if (event.key.code == Mouse::Left && player.life) {
						leftMousePressed = true;
						if (!player.isShoot && player.weapon.type == Gun) {
							stepSound.stop();
							enemyStepSound.stop();
							if (player.weapon.ammoCount > 0) {
								bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, aim.x, aim.y, -1));
								if (player.weapon.name == Shotgun) {
									bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, aim.x - 10, aim.y - 10, -1));
									bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, aim.x + 10, aim.y + 10, -1));
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
									player.weapon.setPosition(weapon.getPosition().x, weapon.getPosition().y);
									player.weapon.active = false;
									weapons.erase(it_w);
									weapons.push_back(player.weapon);
									weapon.active = true;
									player.takeWeapon(weapon);
									
									stepSound.stop();
									raiseItemSound.play();
									break;
								}
							}
						}
					}
					if (Keyboard::isKeyPressed(Keyboard::Escape)) {
						if (showEscape) {
							showEscape = false;
							showControls = false;
						}
						else {
							showEscape = true;
							showControls = true;
						}
					}
				}
				if (event.type == Event::MouseButtonReleased) {
					if (event.key.code == Mouse::Left) {
						leftMousePressed = false;
					}
				}
			}

			showEButton = false;
			for (it_w = weapons.begin(); it_w != weapons.end(); it_w++) {
				Weapon weapon = *it_w;
				if (!weapon.active) {
					if (weapon.getRect().intersects(player.getRect())) {
						showEButton = true;
					}
				}
			}

			sf::FloatRect rect;
			sf::RectangleShape sh;
			int i;
			for (it = enemies.begin(), i = 0; it != enemies.end(); it++, i++) {
				Enemy* enemy = (Enemy*)(*it);
				if (enemy->life) {
					if (bullets.size() > 0) {
						for (it2 = bullets.begin(); it2 != bullets.end(); it2++) {
							Bullet* bullet = (Bullet*)(*it2);
							// Убийство ВРАГА
							if (enemy->getRect().intersects(bullet->getRect()) && bullet->life && enemy->life) {
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
							if (player.getRect().intersects(bullet->getRect()) && bullet->life && player.life) {
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
										showEscape = true;
									}
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
								showEscape = true;
							}
						}
					}

					// Игрок останавливается перед врагами
					/*
					if (enemy->getRect().intersects(player.getRect()) && player.life) {
						player.stopInFront(enemy->getRect(), player.speedX, player.speedY);
					}
					*/

					// Зрение врага
					rect.left = enemy->x; rect.top = enemy->y;
					rect.width = 200;
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
								bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, enemy->x, enemy->y, 16, 16, player.x, player.y, i));
								if (enemy->weapon.name == Shotgun) {
									bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, enemy->x, enemy->y, 16, 16, player.x - 10, player.y - 10, i));
									bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, enemy->x, enemy->y, 16, 16, player.x + 10, player.y + 10, i));
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

			int livinEnemyAmount = 0;

			for (it = enemies.begin(); it != enemies.end(); it++) {
				(*it)->update(time);
				if ((*it)->life)
					livinEnemyAmount++;
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

			if (livinEnemyAmount == 0) {
				showExit = true;
				if (endGameExist) {
					if (player.getRect().intersects(sf::FloatRect(endGame.left, endGame.top, endGame.width, endGame.height))) {
						showEscape = true;
						showEndGame = true;
					}
				}
				else {
					if (player.getRect().intersects(sf::FloatRect(nextRoom.left, nextRoom.top, nextRoom.width, nextRoom.height))) {
						return 10;
					}
					goNextRoom = true;
				}
			}

			healthSprite.setPosition(view.getCenter().x - 650 / zoom_k, view.getCenter().y - 330 / zoom_k);
			healthRect.setPosition(view.getCenter().x - 645 / zoom_k + 849 * scale_k, view.getCenter().y - 325 / zoom_k);
			healthRect.setSize(sf::Vector2f(((849 * scale_k) / 100) * (100 - player.health), 65 * scale_k));

			updateList(it, bullets, time);

			player.rotate(aim.x, aim.y);
			player.update(time);
			if (player.isMove) {
				if (stepSound.getStatus() != sf::SoundSource::Status::Playing) {
					stepSound.setPitch(1.1f);
					stepSound.play();
				}
			}

			for (int i = 0; i < 3; i++) {
				if (escapeMenuTexts[i].getGlobalBounds().contains(pos.x, pos.y)) {
					escapeMenuTexts[i].setFillColor(sf::Color(194, 194, 194));
				}
				else {
					escapeMenuTexts[i].setFillColor(sf::Color(255, 255, 255));
				}
			}

			window.setView(view);
			window.clear(Color(130, 137, 150));

			window.draw(layer0);
			window.draw(layer1);
			window.draw(layer2);
			window.draw(layer3);

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


			// Рисуем оружие, лежащее на земле
			for (it_w = weapons.begin(); it_w != weapons.end(); it_w++) {
				it_w->update(time);
				if (!(it_w->active)) {
					window.draw(it_w->sprite);
				}
			}

			for (it_ab = ammoBoxes.begin(); it_ab != ammoBoxes.end(); it_ab++) {
				it_ab->update(time);
				window.draw(it_ab->sprite);
			}

			for (it_hk = healthKits.begin(); it_hk != healthKits.end(); it_hk++) {
				it_hk->update(time);
				window.draw(it_hk->sprite);
			}

			if (showExit) {
				window.draw(exitSprite);
			}

			if (goNextRoom) {
				window.draw(exitSprite);
			}

			// Рисуем живого игрока
			if (player.life) {
				window.draw(player.legsSprite);
				window.draw(player.sprite);
			}

			// Рисуем пули
			/*
			for (it = bullets.begin(); it != bullets.end(); it++) {
				sf::RectangleShape sh(sf::Vector2f((*it)->getRect().width, (*it)->getRect().height));
				sh.setPosition(sf::Vector2f((*it)->getRect().left  , (*it)->getRect().top + (*it)->getRect().height / 2));
				sh.setRotation((*it)->sprite.getRotation());
				window.draw(sh);
			}
			*/

			drawList(it, bullets, window);

			if (player.weapon.type == Gun) {
				window.draw(ammoSprite);
				ammoText.setString(std::to_string(player.weapon.ammoCount));
				window.draw(ammoText);
			}

			window.draw(healthSprite);
			window.draw(healthRect);

			if (showEButton) {
				eButtonSprite.setPosition(view.getCenter().x - 600 / zoom_k, view.getCenter().y + 200 / zoom_k);
				eButtonSprite.setScale(0.7 / zoom_k, 0.7 / zoom_k);
				window.draw(eButtonSprite);
			}

			if (showEscape) {
				escapeBackgroundRect.setSize(sf::Vector2f(view.getSize().x, view.getSize().y));
				escapeBackgroundRect.setPosition(view.getCenter().x, view.getCenter().y);
				escapeBackgroundRect.setOrigin(view.getSize().x / 2, view.getSize().y / 2);
				escapeRect.setPosition(view.getCenter().x, view.getCenter().y);

				window.draw(escapeBackgroundRect);
				window.draw(escapeRect);

				for (int i = 0; i < 3; i++) {
					if ((i == 0 && !showEndGame) || (i == 0 && !next))
						continue;
					escapeMenuTexts[i].setPosition(view.getCenter().x, view.getCenter().y - 70 / zoom_k + i * 60 / zoom_k);
					window.draw(escapeMenuTexts[i]);
				}

				levelNum.setPosition(view.getCenter().x - 250, view.getCenter().y - 150);
				window.draw(levelNum);
			}
			
			if (showControls) {
				controlsSprite.setPosition(view.getCenter().x + 350 / zoom_k, view.getCenter().y - 200 / zoom_k);
				window.draw(controlsSprite);
			}

			if (showEndGame) {
				endGameText.setPosition(escapeRect.getPosition().x, escapeRect.getPosition().y - 250 / zoom_k);
				player.canMove = false;
				window.draw(endGameText);
			}else if (!player.life) {
				youDiedText.setPosition(escapeRect.getPosition().x, escapeRect.getPosition().y - 250 / zoom_k);
				window.draw(youDiedText);
			}
			window.draw(aim.sprite);

			window.display();

		}

		for (int i = 0; i < MAP_H; i++) {
			delete[] grid[i];
		}
		delete[] grid;

		return 0;
	}
};