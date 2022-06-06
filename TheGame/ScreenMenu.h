#pragma once

#include <iostream>
#include "cScreen.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>

class ScreenMenu : public cScreen {
private:
	sf::Image backgroundImage, aimImage;
	sf::Texture backgroundTexture, aimTexture;
	sf::Sprite backgroundSprite, aimSprite;
	std::string background[102] = { 
		"images/background/frame_000_delay-0.06s.png",
		"images/background/frame_001_delay-0.06s.png",
		"images/background/frame_002_delay-0.06s.png",
		"images/background/frame_003_delay-0.06s.png",
		"images/background/frame_004_delay-0.06s.png",
		"images/background/frame_005_delay-0.06s.png",
		"images/background/frame_006_delay-0.06s.png",
		"images/background/frame_007_delay-0.06s.png",
		"images/background/frame_008_delay-0.06s.png",
		"images/background/frame_009_delay-0.06s.png",
		"images/background/frame_010_delay-0.06s.png",
		"images/background/frame_011_delay-0.06s.png",
		"images/background/frame_012_delay-0.06s.png",
		"images/background/frame_013_delay-0.06s.png",
		"images/background/frame_014_delay-0.06s.png",
		"images/background/frame_015_delay-0.06s.png",
		"images/background/frame_016_delay-0.06s.png",
		"images/background/frame_017_delay-0.06s.png",
		"images/background/frame_018_delay-0.06s.png",
		"images/background/frame_019_delay-0.06s.png",
		"images/background/frame_020_delay-0.06s.png",
		"images/background/frame_021_delay-0.06s.png",
		"images/background/frame_022_delay-0.06s.png",
		"images/background/frame_023_delay-0.06s.png",
		"images/background/frame_024_delay-0.06s.png",
		"images/background/frame_025_delay-0.06s.png",
		"images/background/frame_026_delay-0.06s.png",
		"images/background/frame_027_delay-0.06s.png",
		"images/background/frame_028_delay-0.06s.png",
		"images/background/frame_029_delay-0.06s.png",
		"images/background/frame_030_delay-0.06s.png",
		"images/background/frame_031_delay-0.06s.png",
		"images/background/frame_032_delay-0.06s.png",
		"images/background/frame_033_delay-0.06s.png",
		"images/background/frame_034_delay-0.06s.png",
		"images/background/frame_035_delay-0.06s.png",
		"images/background/frame_036_delay-0.06s.png",
		"images/background/frame_037_delay-0.06s.png",
		"images/background/frame_038_delay-0.06s.png",
		"images/background/frame_039_delay-0.06s.png",
		"images/background/frame_040_delay-0.06s.png",
		"images/background/frame_041_delay-0.06s.png",
		"images/background/frame_042_delay-0.06s.png",
		"images/background/frame_043_delay-0.06s.png",
		"images/background/frame_044_delay-0.06s.png",
		"images/background/frame_045_delay-0.06s.png",
		"images/background/frame_046_delay-0.06s.png",
		"images/background/frame_047_delay-0.06s.png",
		"images/background/frame_048_delay-0.06s.png",
		"images/background/frame_049_delay-0.06s.png",
		"images/background/frame_050_delay-0.06s.png",
		"images/background/frame_051_delay-0.06s.png",
		"images/background/frame_052_delay-0.06s.png",
		"images/background/frame_053_delay-0.06s.png",
		"images/background/frame_054_delay-0.06s.png",
		"images/background/frame_055_delay-0.06s.png",
		"images/background/frame_056_delay-0.06s.png",
		"images/background/frame_057_delay-0.06s.png",
		"images/background/frame_058_delay-0.06s.png",
		"images/background/frame_059_delay-0.06s.png",
		"images/background/frame_060_delay-0.06s.png",
		"images/background/frame_061_delay-0.06s.png",
		"images/background/frame_062_delay-0.06s.png",
		"images/background/frame_063_delay-0.06s.png",
		"images/background/frame_064_delay-0.06s.png",
		"images/background/frame_065_delay-0.06s.png",
		"images/background/frame_066_delay-0.06s.png",
		"images/background/frame_067_delay-0.06s.png",
		"images/background/frame_068_delay-0.06s.png",
		"images/background/frame_069_delay-0.06s.png",
		"images/background/frame_070_delay-0.06s.png",
		"images/background/frame_071_delay-0.06s.png",
		"images/background/frame_072_delay-0.06s.png",
		"images/background/frame_073_delay-0.06s.png",
		"images/background/frame_074_delay-0.06s.png",
		"images/background/frame_075_delay-0.06s.png",
		"images/background/frame_076_delay-0.06s.png",
		"images/background/frame_077_delay-0.06s.png",
		"images/background/frame_078_delay-0.06s.png",
		"images/background/frame_079_delay-0.06s.png",
		"images/background/frame_080_delay-0.06s.png",
		"images/background/frame_081_delay-0.06s.png",
		"images/background/frame_082_delay-0.06s.png",
		"images/background/frame_083_delay-0.06s.png",
		"images/background/frame_084_delay-0.06s.png",
		"images/background/frame_085_delay-0.06s.png",
		"images/background/frame_086_delay-0.06s.png",
		"images/background/frame_087_delay-0.06s.png",
		"images/background/frame_088_delay-0.06s.png",
		"images/background/frame_089_delay-0.06s.png",
		"images/background/frame_090_delay-0.06s.png",
		"images/background/frame_091_delay-0.06s.png",
		"images/background/frame_092_delay-0.06s.png",
		"images/background/frame_093_delay-0.06s.png",
		"images/background/frame_094_delay-0.06s.png",
		"images/background/frame_095_delay-0.06s.png",
		"images/background/frame_096_delay-0.06s.png",
		"images/background/frame_097_delay-0.06s.png",
		"images/background/frame_098_delay-0.06s.png",
		"images/background/frame_099_delay-0.06s.png",
		"images/background/frame_100_delay-0.06s.png",
		"images/background/frame_101_delay-0.06s.png",
	};
	sf::Texture backgroundTextures[102];
	int background_i;
	sf::Music backgroundMusic;
	vector <string> maps;
	int levelCount;
public:
	ScreenMenu(vector<string> maps) {
		backgroundImage.loadFromFile("images/menu/menu_background2.jpg");
		backgroundTexture.loadFromImage(backgroundImage);
		for (int i = 0; i < 102; i++) {
			sf::Image image;
			image.loadFromFile(background[i]);
			backgroundTexture.loadFromImage(image);
			backgroundTextures[i] = backgroundTexture;
		}
		background_i = 0;

		backgroundMusic.openFromFile("audio/Bio-Unit-Ambient-Warning.wav");
		backgroundMusic.setLoop(true);
		this->maps = maps;
	}
	int Run(sf::RenderWindow& window, int &map_i) {
		int windowX = 1380;
		int windowY = 720;

		window.setView(window.getDefaultView());

		bool showMenu = true;
		backgroundSprite.setScale(2,2);
		backgroundSprite.setPosition(0, 0);
		aimImage.loadFromFile("images/menu/gun_sight_12.png");
		aimTexture.loadFromImage(aimImage);
		aimSprite.setTexture(aimTexture);
		aimSprite.setTextureRect(sf::IntRect(61, 39, 250, 249));
		aimSprite.setScale(0.2, 0.2);

		sf::Font font;
		font.loadFromFile("fonts/GUNPLAY_.ttf");

		sf::Text menuTexts[3];
		for (int i = 0; i < 3; i++) {\
			sf::Text menuText;
			menuText.setFont(font);
			menuText.setCharacterSize(100);
			menuText.setPosition(windowX / 2, windowY / 2 + i * 120);
			menuText.setFillColor(sf::Color(255, 100, 20));
			menuText.setOutlineThickness(3);
			menuText.setOutlineColor(sf::Color(0, 0, 0));
			menuTexts[i] = menuText;
		}

		menuTexts[0].setString("START GAME");
		menuTexts[1].setString("SELECT LEVEL");
		menuTexts[2].setString("EXIT");

		for (int i = 0; i < 3; i++) {
			menuTexts[i].setOrigin(menuTexts[i].getGlobalBounds().width / 2, menuTexts[i].getGlobalBounds().height / 2);
		}

		levelCount = maps.size() + 1;
		sf::Text selectLevelTexts[5];
		for (int i = 0; i < levelCount; i++) {
			sf::Text menuText;
			menuText.setFont(font);
			menuText.setCharacterSize(70);
			menuText.setPosition(windowX / 2, windowY / 2 + - 200 + i * 100);
			menuText.setFillColor(sf::Color(255, 100, 20));
			menuText.setOutlineThickness(3);
			menuText.setOutlineColor(sf::Color(0, 0, 0));
			selectLevelTexts[i] = menuText;
		}

		selectLevelTexts[0].setString("LEVEL 1");
		selectLevelTexts[1].setString("LEVEL 2");
		selectLevelTexts[2].setString("LEVEL 3");
		selectLevelTexts[3].setString("LEVEL 4");
		selectLevelTexts[4].setString("BACK");

		for (int i = 0; i < levelCount; i++) {
			selectLevelTexts[i].setOrigin(selectLevelTexts[i].getGlobalBounds().width / 2, selectLevelTexts[i].getGlobalBounds().height / 2);
		}

		sf::Vector2i pixelPos;
		sf::Vector2f pos;
		Clock clock;
		float backgroundTime = 0;

		backgroundMusic.play();

		bool showSelectLevel = false;
		while (showMenu) {
			float time = clock.getElapsedTime().asMicroseconds();
			clock.restart();
			time = time / 800;
			backgroundTime += time;
			pixelPos = sf::Mouse::getPosition(window);
			pos = window.mapPixelToCoords(pixelPos);
			aimSprite.setPosition(pos.x, pos.y);

			if (!showSelectLevel) {
				for (int i = 0; i < 3; i++) {
					if (menuTexts[i].getGlobalBounds().contains(pos.x, pos.y)) {
						menuTexts[i].setFillColor(sf::Color(255, 0, 0));
						aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
					}
					else {
						menuTexts[i].setFillColor(sf::Color(255, 100, 20));
						aimSprite.setTextureRect(sf::IntRect(61, 39, 250, 249));
					}
				}
			}
			else {
				for (int i = 0; i < levelCount; i++) {
					if (selectLevelTexts[i].getGlobalBounds().contains(pos.x, pos.y)) {
						selectLevelTexts[i].setFillColor(sf::Color(255, 0, 0));
						aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
					}
					else {
						selectLevelTexts[i].setFillColor(sf::Color(255, 100, 20));
						aimSprite.setTextureRect(sf::IntRect(61, 39, 250, 249));
					}
				}
			}

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::MouseButtonPressed) {
					if (event.key.code == sf::Mouse::Left) {
						if (!showSelectLevel) {
							if (menuTexts[0].getGlobalBounds().contains(pos.x, pos.y)) {
								aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
								showMenu = false;
								backgroundMusic.stop();

								// Загрузка сохранения
								int lvl = getSaveLevel();
								if (lvl != -1) {
									map_i = lvl;
								}
								else {
									saveLevel(0, "default");
								}
								return 1;
							}
							else if (menuTexts[1].getGlobalBounds().contains(pos.x, pos.y)) {
								showSelectLevel = true;
							}
							else if (menuTexts[2].getGlobalBounds().contains(pos.x, pos.y)) {
								aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
								showMenu = false;
								window.close();
							}
						}
						else {
							for (int i = 0; i < levelCount - 1; i++) {
								if (selectLevelTexts[i].getGlobalBounds().contains(pos.x, pos.y)) {
									aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
									showMenu = false;
									showSelectLevel = false;
									backgroundMusic.stop();
									map_i = i;
									saveLevel(i, "default");
									return 1;
								}
							}
							if (selectLevelTexts[levelCount - 1].getGlobalBounds().contains(pos.x, pos.y)) {
								aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
								showSelectLevel = false;
							}
						}
					}
				}
			}

			window.clear();
			backgroundSprite.setTexture(backgroundTextures[background_i]);
			window.draw(backgroundSprite);

			if (!showSelectLevel) {
				for (int i = 0; i < 3; i++) {
					window.draw(menuTexts[i]);
				}
			}
			else {
				for (int i = 0; i < levelCount; i++) {
					window.draw(selectLevelTexts[i]);
				}
			}

			window.draw(aimSprite);
			window.display();

			if (backgroundTime > 50) {
				background_i++;
				backgroundTime = 0;
			}
			if (background_i == 102) {
				background_i = 0;
			}
		}
		return -1;
	}
};