#pragma once

void escape(RenderWindow& window) {
	bool showMenu = true;
	while (showMenu) {
		window.clear(Color(137, 134, 150));

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left) {

				}
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape) {
					showMenu = false;
				}
			}
		}

		window.display();
	}
}

void menu(RenderWindow& window) {
	bool showMenu = true;
	sf::Image backgroundImage, aimImage;
	sf::Texture backgroundTexture, aimTexture;
	sf::Sprite backgroundSprite, aimSprite;
	backgroundImage.loadFromFile("images/menu/menu_background2.jpg");
	backgroundTexture.loadFromImage(backgroundImage);
	backgroundSprite.setTexture(backgroundTexture);
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

	startGameText.setFillColor(Color(255, 100, 20));
	startGameText.setOutlineThickness(3);
	startGameText.setOutlineColor(Color(0, 0, 0));

	exitText.setFillColor(Color(255, 100, 20));
	exitText.setOutlineThickness(3);
	exitText.setOutlineColor(Color(0, 0, 0));

	sf::Vector2i pixelPos;
	sf::Vector2f pos;
	while (showMenu) {
		pixelPos = Mouse::getPosition(window);
		pos = window.mapPixelToCoords(pixelPos);
		aimSprite.setPosition(pos.x, pos.y);

		if (startGameText.getGlobalBounds().contains(pos.x, pos.y)) {
			startGameText.setFillColor(Color(255, 0, 0));
		}
		else if (exitText.getGlobalBounds().contains(pos.x, pos.y)) {
			exitText.setFillColor(Color(255, 0, 0));
		}
		else {
			startGameText.setFillColor(Color(255, 100, 20));
			exitText.setFillColor(Color(255, 100, 20));
		}
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left) {

					if (startGameText.getGlobalBounds().contains(pos.x, pos.y)) {
						aimSprite.setTextureRect(sf::IntRect(335, 33, 256, 259));
						showMenu = false;
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
}
