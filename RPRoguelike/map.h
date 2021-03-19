#pragma once

#include <functional> //for std::function
#include <vector> //for std::vector
#include <utility> //for std::pair
#include "geom.h"
#include "bspNode.h"
#include "SDL.h"
#include "random/include/effolkronium/random.hpp"


struct tileData
{
	coord coords;
	uint8_t tileCode;
};


class gameMap
{

	std::vector<std::vector<uint8_t>> m_level;
	std::vector<std::vector<bool>> m_levelTouched;

	std::function<void(uint8_t, int, int)> m_drawFn;

	std::vector<tileData> m_importantTiles;


public:
	void generate(std::pair<coord, coord> mapSize); //TODO: change to take rect
	void draw();
	void clear();

	void bindDrawFn(std::function<void(uint8_t, int, int)> fn) { m_drawFn = fn; };

	void drawRoom(rect rm);

	bool movePlayer(int x, int y);

	coord addPlayer();


};