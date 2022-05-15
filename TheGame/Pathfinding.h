#pragma once

#include "consts.h"
#include <vector>

vector<pair<int, int> > get_path(int originX, int originY, int targetX, int targetY, int** map) {
	for (int i = 0; i < MAP_H; i++) {
		for (int j = 0; j < MAP_W; j++) {
			if (map[i][j] != MAXINT) {
				map[i][j] = -1;
			}
		}
	}

	if (map[targetY][targetX] == MAXINT) {
		if (map[targetY + 1][targetX] != MAXINT && targetY < MAP_H - 1) {
			targetY++;
		} else if (map[targetY - 1][targetX] != MAXINT && targetY > 0) {
			targetY--;
		} else if (map[targetY][targetX - 1] != MAXINT && targetX > 0) {
			targetX--;
		} else if (map[targetY][targetX + 1] != MAXINT && targetX < MAP_W - 1) {
			targetX++;
		}else if (map[targetY + 1][targetX + 1] != MAXINT && (targetX < MAP_W - 1) && (targetY < MAP_H - 1)) {
			targetX++; targetY++;
		}else if (map[targetY - 1][targetX - 1] != MAXINT && (targetX > 0) && (targetY > 0)) {
			targetX--; targetY--;
		}else if (map[targetY - 1][targetX + 1] != MAXINT && (targetX > 0) && (targetY > MAP_H - 1)) {
			targetX++; targetY--;
		}else if (map[targetY + 1][targetX - 1] != MAXINT) {
			targetX--; targetY++;
		}
	}

	if (map[originY][originX] == MAXINT) {
		if (map[originY + 1][originX] != MAXINT) {
			originY++;
		}
		else if (map[targetY - 1][originX] != MAXINT) {
			originY--;
		}
		else if (map[originY][originX - 1] != MAXINT) {
			originX--;
		}
		else if (map[originY][originX + 1] != MAXINT) {
			originX++;
		}
	}

	vector<pair<int, int> > oldWave;
	vector<pair<int, int> > wave;
	oldWave.push_back(pair<int, int>(originX, originY));
	int nstep = 0;
	map[originY][originX] = nstep;
	const int dx[] = { 0, 1, 0, -1, 1, 1, -1, -1 };
	const int dy[] = { -1, 0, 1, 0, -1, 1, -1, 1 };
	bool find = true;
	while (oldWave.size() > 0 && find) {
		++nstep;
		wave.clear();
		for (vector<pair<int, int> >::iterator i = oldWave.begin(); i != oldWave.end(); ++i) {
			for (int d = 0; d < 8; d++) {
				int nx = i->first + dx[d];
				int ny = i->second + dy[d];
				if (map[ny][nx] == -1) {
					wave.push_back(pair<int, int>(nx, ny));
					map[ny][nx] = nstep;
					if (nx == targetX && ny == targetY) {
						find = false;
						break;
					}
				}
			}
			if (!find) break;
		}
		oldWave = wave;
	}
	wave.clear();
	wave.push_back(pair<int, int>(targetX, targetY));
	while (map[targetY][targetX] != 0) {
		for (int d = 0; d < 8; ++d) {
			int nx = targetX + dx[d];
			int ny = targetY + dy[d];
			if (map[targetY][targetX] - 1 == map[ny][nx]) {
				targetX = nx;
				targetY = ny;
				wave.push_back(pair<int, int>(targetX, targetY));
				break;
			}
		}
	}
	return wave;
}

void display(int** grid) {

	for (int i = 0; i < MAP_H; i++) {
		for (int j = 0; j < MAP_W; j++) {
			if (grid[i][j] == MAXINT) {
				cout << "*";
			}
			else if (grid[i][j] == 0) {
				cout << "@";
			}
			else if (grid[i][j] == -100) {
				cout << "#";
			}
			else {
				cout << " ";
			}
		}
		cout << endl;
	}
}

void put_solids(int** grid, vector<Object>solids) {
	for (int i = 0; i < MAP_H; i++) {
		for (int j = 0; j < MAP_W; j++) {
			float x = j * cell_w;
			float y = i * cell_h;
			sf::FloatRect cell(x, y, cell_w, cell_h);
			for (const auto& solid : solids) {
				sf::FloatRect solidRect(solid.getAABB().left, solid.getAABB().top, solid.getAABB().width, solid.getAABB().height);
				if (cell.intersects(solidRect)) {
					grid[i][j] = MAXINT;
				}
			}
		}
	}
}