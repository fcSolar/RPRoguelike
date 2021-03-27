#include "map.h"


void gameMap::generate(std::pair<coord, coord> mapSize)
{
	m_levelWidth = mapSize.second.x - mapSize.first.x;
	m_levelHeight = mapSize.second.y - mapSize.first.y;

	for(int i = 0; i < m_levelWidth * m_levelHeight; i++)
	{
		m_level.push_back(0);
		m_levelTouched.push_back(true);
	}

	bspNode *root = new bspNode(0, 0, 178, 57);

	std::vector<bspNode*> tree;

	tree.push_back(root);

	std::vector<bspNode*> temp;

	for(int i = 0; i < 8; i++)
	{
		for(auto e : tree)
		{
			if(e->split())
			{
				temp.push_back(e->getChildren().first);
				temp.push_back(e->getChildren().second);
			}
		}

		for(auto e : temp)
		{
			tree.push_back(e);
		}

		temp.clear();
	}


	std::vector<coord> pts;

	for(auto e : tree)
	{
		if(!e->getSplitStatus())
		{
			e->createRoom();
			pts.push_back(e->getPointInRoom());
		}
	}

	for(auto e : tree)
	{
		if(!e->getSplitStatus())
		{
			drawRoom(e->getRoom());
		}
	}

	auto drawHalls{[this, mapSize](coord pt1, coord pt2)
	{
		int xDrawLevel{-1};
		int yDrawLevel{-1};

		if(pt1.y > pt2.y)
		{
			if(pt1.x > pt2.x)
			{
				xDrawLevel = pt1.y;
				yDrawLevel = pt2.x;
			}
			else
			{
				xDrawLevel = pt2.y;
				yDrawLevel = pt1.x;
			}

		}
		else
		{
			if(pt1.x > pt2.x)
			{
				xDrawLevel = pt2.y;
				yDrawLevel = pt1.x;
			}
			else
			{
				xDrawLevel = pt1.y;
				yDrawLevel = pt2.x;
			}
		}


		for(int i = std::min(pt1.x, pt2.x); i <= std::max(pt1.x, pt2.x); i++)
		{
			m_level[ind2d(i,xDrawLevel)] = 1;
			m_levelTouched[ind2d(i,xDrawLevel)] = true;
		}

		for(int i = std::min(pt1.y, pt2.y); i <= std::max(pt1.y, pt2.y); i++)
		{
			m_level[ind2d(yDrawLevel,i)] = 1;
			m_levelTouched[ind2d(yDrawLevel,i)] = true;
		}
	}};

	coord p1{-1,-1};
	coord p2{-1,-1};
	int it{0};

	for(auto &e : pts)
	{
		
		if(it == 0)
		{
			p2 = e;
		}
		else
		{
			p1 = p2;
			p2 = e;
			
			drawHalls(p1, p2);
		}

		it++;
	}

	for(auto e : tree)
	{
		delete e;
	}
}

void gameMap::draw()
{
	int ind{0};

	for(auto e : m_level)
	{
		
			
		if(m_levelTouched[ind])
		{
			m_drawFn(e, ind % m_levelWidth, ind / m_levelWidth);
			m_levelTouched[ind] = false;
		}

		ind++;
	}
}

void gameMap::clear()
{
	m_level.clear();
	m_levelTouched.clear();
}


void gameMap::drawRoom(rect rm)
{
	for(int i = rm.pos.y; i < rm.pos.y + rm.height; i++)
	{
		for(int j = rm.pos.x; j < rm.pos.x + rm.width; j++)
		{
			m_level[ind2d(j,i)] = 1;
			m_levelTouched[ind2d(j, i)] = true;
		}
	}
}

bool gameMap::movePlayer(int x, int y)
{
	coord oldPos{m_importantTiles.at(0).coords};
	coord newPos{oldPos.x + x,oldPos.y + y};
	
	if(m_level[ind2d(newPos.x, newPos.y)] != 0)
	{
		m_importantTiles[0].coords = {oldPos.x + x, oldPos.y + y};

		m_level[ind2d(oldPos.x,oldPos.y)] = 1;
		m_level[ind2d(newPos.x, newPos.y)] = 2;

		m_levelTouched[ind2d(oldPos.x, oldPos.y)] = true;
		m_levelTouched[ind2d(newPos.x, newPos.y)] = true;

		return true;
	}
	else
		return false;
}

coord gameMap::addPlayer()
{
	using random = effolkronium::random_static;

	std::vector<coord> validPos;
	int ind{0};

	for(auto e : m_level)
	{
		if(e == 1)
			validPos.push_back({ind%m_levelWidth,ind/m_levelWidth});
		
		ind++;

	};
	
	coord playerPos{*random::get(validPos)};

	m_level[ind2d(playerPos.x, playerPos.y)] = 2;
	m_levelTouched[ind2d(playerPos.x, playerPos.y)] = true;
	m_importantTiles.push_back({{playerPos.x, playerPos.y}, tileType::TILE_PLAYER});

	return playerPos;
}

void gameMap::clearImportantTiles()
{
	m_importantTiles.clear();
}