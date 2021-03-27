#pragma once

#include <functional>
#include <vector>
#include <utility> //for std::pair
#include <cmath>
#include <bitset>
#include "geom.h"
#include "bspNode.h"
#include "SDL.h"
#include "random/include/effolkronium/random.hpp"
 
enum class tileType
{
	TILE_NULL,
	TILE_WALL,
	TILE_PLAYER,
};

struct tileData
{
	coord coords;
	tileType tileCode;
	
};


class gameMap
{
	//std::vector<std::vector<uint8_t>> m_level;
	//std::vector<std::vector<bool>> m_levelTouched;
	std::vector<uint8_t> m_level;
	std::vector<bool> m_levelTouched;

	int m_levelWidth;
	int m_levelHeight;

	std::function<void(uint8_t, int, int)> m_drawFn;

	std::vector<tileData> m_importantTiles;

	int ind2d(int x, int y) { return y * m_levelWidth + x; }


public:
	void generate(std::pair<coord, coord> mapSize); //TODO: change to take rect
	void draw();
	void clear();

	void bindDrawFn(std::function<void(uint8_t, int, int)> fn) { m_drawFn = fn; };

	void drawRoom(rect rm);

	bool movePlayer(int x, int y);

	coord addPlayer();

	void clearImportantTiles();
};