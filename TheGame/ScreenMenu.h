#pragma once

#include <iostream>
#include "cScreen.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ScreenMenu : public cScreen {
private:
	sf::Image backgroundImage, aimImage;
	sf::Texture backgroundTexture, aimTexture;
	sf::Sprite backgroundSprite, aimSprite;
public:
	ScreenMenu() {
		backgroundImage.loadFromFile("images/menu/menu_background2.jpg");
		backgroundTexture.loadFromImage(backgroundImage);
		backgroundSprite.setTexture(backgroundTexture);
	}
	int Run(sf::RenderWindow& window) {
		window.setView(window.getDefaultView());
		bool showMenu = true;
		backgroundSprite.setScale(0.2, 0.2);
		aimImage.loadFromFile("images/menu/gun_sight_12.png");
		aimTexture.loadFromImage(aimImage);
		aimSprite.setTexture(aimTexture);
		aimSprite.setTextureRect(sf::IntRect(61, 39, 250, 249));
		aimSprite.setScale(0.2, 0.2);

		sf::Font font;
		font.loadFromFile("fonts/GUNPLAY_.ttf");
		sf::Text startGameText;
		startGameText.setFont(font);
		startGameText.setCharacterSize(100);
		startGameText.setString("START GAME");
		startGameText.setOrigin(startGameText.getGlobalBounds().width / 2, startGameText.getGlobalBounds().height / 2);
		startGameText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

		sf::Text exitText;
		exitText.setFont(font);
		exitText.setCharacterSize(100);
		exitText.setString("EXIT");
		exitText.setOrigin(exitText.getGlobalBounds().width / 2, exitText.getGlobalBounds().height / 2);
		exitText.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 150);

		startGameText.setFillColor(sf::Color(255, 100, 20));
		startGameText.setOutlineThickness(3);
		startGameText.setOutlineColor(sf::Color(0, 0, 0));

		exitText.setFillColor(sf::Color(255, 100, 20));
		exitText.setOutlineThickness(3);
		exitText.setOutlineColor(sf::Color(0, 0, 0));

		sf::Vector2i pixelPos;
		sf::Vector2f pos;
		while (showMenu) {
			pixelPos = sf::Mouse::getPosition(window);
			pos = window.mapPixelToCoords(pixelPos);
			aimSprite.setPosition(pos.x, pos.y);

			if (startGameText.getGlobalBounds().contains(pos.x, pos.y)) {
				startGameText.setFillColor(sf::Color(255, 0, 0));
				aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
			}
			else if (exitText.getGlobalBounds().contains(pos.x, pos.y)) {
				exitText.setFillColor(sf::Color(255, 0, 0));
				aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
			}
			else {
				startGameText.setFillColor(sf::Color(255, 100, 20));
				exitText.setFillColor(sf::Color(255, 100, 20));
				aimSprite.setTextureRect(sf::IntRect(61, 39, 250, 249));
			}
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::MouseButtonPressed) {
					if (event.key.code == sf::Mouse::Left) {

						if (startGameText.getGlobalBounds().contains(pos.x, pos.y)) {
							aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
							showMenu = false;
							return 1;
						}
						else if (exitText.getGlobalBounds().contains(pos.x, pos.y)) {
							aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
							showMenu = false;
							window.close();
						}
					}
				}
			}

			window.clear();
			window.draw(backgroundSprite);
			window.draw(startGameText);
			window.draw(exitText);
			window.draw(aimSprite);
			window.display();
		}
		return -1;
	}
};