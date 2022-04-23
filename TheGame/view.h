#pragma once


#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;

void setPlayerCoordinateForView(float x, float y) { //функция для считывания координат игрока
	float tempX = x, tempY = y;
	//if (x < 320) tempX = 320;
	//if (y < 240) tempY = 240;
	view.setCenter(tempX + 100, tempY);
}