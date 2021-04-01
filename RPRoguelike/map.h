#pragma once

#include <functional>
#include <vector>
#include <utility> //for std::pair
#include <cmath>
#include "message.h"
#include "tiles.h"
#include "geom.h"
#include "bspNode.h"
#include "SDL.h"
#include "random/include/effolkronium/random.hpp"

class gameMap
{
	

	std::vector<tileData> m_level;

	int m_levelWidth{-1};
	int m_levelHeight{-1};

	std::function<void(tileData tile)> m_drawFn;

	//std::vector<tileData&> m_importantTiles;

	int ind2d(int x, int y) { return y * m_levelWidth + x; }

public:
	void generate(rect mapSize); //TODO: change to take rect
	void draw();
	void clear();

	void bindDrawFn(std::function<void(tileData tile)> fn) { m_drawFn = fn; };

	void drawRoom(rect rm);

	std::shared_ptr<message> movePlayer(
		const std::shared_ptr<msg_playerMove> &msg);

	std::shared_ptr<msg_playerPos> addPlayer();
};