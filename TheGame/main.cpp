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

#include "Screens.h"


using namespace std;
using namespace sf;
using namespace tmx;

int main() {
	sf::RenderWindow window(VideoMode(1380, 720), "Game");
	window.setMouseCursorVisible(false);
	
	std::vector<cScreen*> Screens;
	int screen = 0;

	ScreenMenu screenMenu;
	Screens.push_back(&screenMenu);
	ScreenGame screenGame;
	Screens.push_back(&screenGame);


	while(screen >= 0) {
		screen = Screens[screen]->Run(window);
	}

	return EXIT_SUCCESS;
}	
