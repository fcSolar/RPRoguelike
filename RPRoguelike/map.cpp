#include "map.h"


void gameMap::generate(std::pair<coord, coord> mapSize)
{

	for(int i = 0; i < (mapSize.second.y - mapSize.first.y); i++)
	{
		std::vector<uint8_t> temp;
		std::vector<bool> temp2;

		for(int j = 0; j < (mapSize.second.x - mapSize.first.x); j++)
		{
			temp.push_back(0);
			temp2.push_back(true);
		}

		m_level.push_back(temp);
		m_levelTouched.push_back(temp2);
	}

	bspNode *root = new bspNode(0, 0, 178, 57);

	std::vector<bspNode*> tree;

	tree.push_back(root);

	std::vector<bspNode*> temp;

	for(int i = 0; i < 7; i++)
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

	auto drawHallHoriz{[this](int start, int end, int y) 
					   { 
						   for(int i = start; i < end; i++) 
						   { 
							   m_level[y][i] = 1;
							   m_levelTouched[y][i] = true;
						   } 
					   }};

	auto drawHallVert{[this](int start, int end, int x) 
					  { 
						  for(int i = start; i < end; i++)
						  { 
							  m_level[i][x] = 1; 
							  m_levelTouched[i][x] = true;
						  } 
					  }};

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
			m_level[xDrawLevel][i] = 1;
			m_levelTouched[xDrawLevel][i] = true;
		}

		for(int i = std::min(pt1.y, pt2.y); i <= std::max(pt1.y, pt2.y); i++)
		{
			m_level[i][yDrawLevel] = 1;
			m_levelTouched[i][yDrawLevel] = true;
		}
	}
};

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
	int x{0};
	int y{0};

	for(auto &e : m_level)
	{
		for(auto &ee : e)
		{
			
			if(m_levelTouched[y][x])
			{
				m_drawFn(ee, x, y);
				m_levelTouched[y][x] = false;
			}
				

			x++;
		}

		x = 0;
		y++;
	}
}

void gameMap::clear()
{
	for(auto &e : m_level)
	{
		e.clear();
	}

	for(auto &e : m_levelTouched)
	{
		e.clear();
	}

	m_level.clear();
	m_levelTouched.clear();
}


void gameMap::drawRoom(rect rm)
{
	for(int i = rm.pos.y; i < rm.pos.y + rm.height; i++)
	{
		for(int j = rm.pos.x; j < rm.pos.x + rm.width; j++)
		{
			m_level[i][j] = 1;
			m_levelTouched[i][j] = true;
		}
	}
}

bool gameMap::movePlayer(int x, int y)
{
	coord oldPos = m_importantTiles.at(0).coords;
	
	if(m_level[oldPos.y + y][oldPos.x + x] != 0)
	{
		m_importantTiles[0].coords = {oldPos.x + x, oldPos.y + y};

		m_level[oldPos.y][oldPos.x] = 1;
		m_level[oldPos.y + y][oldPos.x + x] = 2;

		m_levelTouched[oldPos.y][oldPos.x] = true;
		m_levelTouched[oldPos.y + y][oldPos.x + x] = true;

		return true;
	}
	else
		return false;
}

coord gameMap::addPlayer()
{
	using random = effolkronium::random_static;

	std::vector<coord> validPos;
	int x{0};
	int y{0};

	for(auto &e : m_level)
	{
		for(auto &ee : e)
		{
			if(ee == 1)
				validPos.push_back({x,y});
			x++;
		}

		x = 0;
		y++;
	}
	
	coord playerPos{*random::get(validPos)};

	m_level[playerPos.y][playerPos.x] = 2;
	m_levelTouched[playerPos.y][playerPos.x] = true;
	m_importantTiles.push_back({{playerPos.x, playerPos.y}, tileType::TILE_PLAYER});

	return playerPos;
}

void gameMap::clearImportantTiles()
{
	m_importantTiles.clear();
}