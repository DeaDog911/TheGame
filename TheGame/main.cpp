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
#include "Saves.h"
#include "Screens.h"


using namespace std;
using namespace sf;
using namespace tmx;

int main() {
	sf::RenderWindow window(VideoMode(1380, 720), "Game");
	window.setMouseCursorVisible(false);

	// Растянуть на весь экран
	::ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);

	std::vector<cScreen*> Screens;
	int screen = 0;

	vector<string> maps = {
		//"map.tmx",
		//"map_empty.tmx",
		"level_1_1.tmx",
		"level_2_1.tmx",
		"level_3_1.tmx",
		"level_3_2.tmx",
		
	};

	int map_i = 0;
	ScreenMenu screenMenu(maps);
	Screens.push_back(&screenMenu);
	ScreenGame screenGame(maps);
	Screens.push_back(&screenGame);

	while(screen >= 0) {
		screen = Screens[screen]->Run(window, map_i);
	}

	return EXIT_SUCCESS;
}	
