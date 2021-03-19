#include "map.h"


void gameMap::generate(std::pair<coord, coord> mapSize)
{

	for(int i{0}; i < (mapSize.second.y - mapSize.first.y); i++)
	{
		std::vector<uint8_t> temp;
		std::vector<bool> temp2;

		for(int j{0}; j < (mapSize.second.x - mapSize.first.x); j++)
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

	auto drawHallHoriz{[this](int first, int second, int third) 
					   { 
						   for(int i = first; i < first + second; i++) 
						   { 
							   m_level[third][i] = 1;
							   m_levelTouched[third][i] = true;
						   } 
					   }};

	auto drawHallVert{[this](int first, int second, int third) 
					  { 
						  for(int i = first; i < first + second; i++) 
						  { 
							  m_level[i][third] = 1; 
							  m_levelTouched[i][third] = true;
						  } 
					  }};

	coord p1{-1,-1};
	coord p2{-1,-1};

	for(auto &e : pts)
	{
		using random = effolkronium::random_static;

		if(p1.x == -1 && p2.x == -1)
		{
			p1 = e;
		}
		else if(p1.x != -1 && p2.x == -1)
		{
			
			p2 = e;

			int w{p2.x - p1.x};
			int h{p2.y - p1.y};

			if(w < 0)
			{
				if(h < 0)
				{
					if(random::get<bool>())
					{
						drawHallHoriz(p2.x, abs(w), p1.y);
						drawHallVert(p2.y, abs(h), p2.x);
					}
					else
					{
						drawHallHoriz(p2.x, abs(w), p2.y);
						drawHallVert(p2.y, abs(h), p2.x);
					}
				}
				else if(h > 0)
				{
					if(random::get<bool>())
					{
						drawHallHoriz(p2.x, abs(w), p1.y);
						drawHallVert(p1.y, abs(h), p2.x);
					}
					else
					{
						drawHallHoriz(p2.x, abs(w), p2.y);
						drawHallVert(p1.y, abs(h), p1.x);
					}
				}
				else
				{
					drawHallHoriz(p2.x, abs(w), p2.y);
				}
			}
			else if(w > 0)
			{
				if(h < 0)
				{
					if(random::get<bool>())
					{
						drawHallHoriz(p1.x, abs(w), p2.y);
						drawHallVert(p2.y, abs(h), p1.x);
					}
					else
					{
						drawHallHoriz(p1.x, abs(w), p1.y);
						drawHallVert(p2.y, abs(h), p2.x);
					}
				}
				else if(h > 0)
				{
					if(p1.y + h >= mapSize.second.y)
					{
						drawHallHoriz(p1.x, abs(w), p1.y);
						drawHallVert(p2.y, abs(h), p2.x);
					}
					else
					{
						drawHallHoriz(p1.x, abs(w), p2.y);
						drawHallVert(p1.y, abs(h), p1.y);
					}
				}
				else
				{
					drawHallHoriz(p1.x, abs(w), p1.y);
				}
			}
			else
			{
				if(h < 0)
				{
					drawHallVert(p2.y, abs(h), p2.x);
				}
				else
				{
					drawHallVert(p1.y, abs(h), p1.x);
				}
			}
		}
		else
		{
			p1 = p2;
			p2 = e;

			int w{p2.x - p1.x};
			int h{p2.y - p1.y};

			if(w < 0)
			{
				if(h < 0)
				{
					if(random::get<bool>())
					{
						drawHallHoriz(p2.x, abs(w), p1.y);
						drawHallVert(p2.y, abs(h), p2.x);
					}
					else
					{
						drawHallHoriz(p2.x, abs(w), p2.y);
						drawHallVert(p2.y, abs(h), p2.x);
					}
				}
				else if(h > 0)
				{
					if(random::get<bool>())
					{
						drawHallHoriz(p2.x, abs(w), p1.y);
						drawHallVert(p1.y, abs(h), p2.x);
					}
					else
					{
						drawHallHoriz(p2.x, abs(w), p2.y);
						drawHallVert(p1.y, abs(h), p1.x);
					}
				}
				else
				{
					drawHallHoriz(p2.x, abs(w), p2.y);
				}
			}
			else if(w > 0)
			{
				if(h < 0)
				{
					if(random::get<bool>())
					{
						drawHallHoriz(p1.x, abs(w), p2.y);
						drawHallVert(p2.y, abs(h), p1.x);
					}
					else
					{
						drawHallHoriz(p1.x, abs(w), p1.y);
						drawHallVert(p2.y, abs(h), p2.x);
					}
				}
				else if(h > 0)
				{
					if(p1.y + h >= mapSize.second.y)
					{
						drawHallHoriz(p1.x, abs(w), p1.y);
						drawHallVert(p2.y, abs(h), p2.x);
					}
					else
					{
						drawHallHoriz(p1.x, abs(w), p2.y);
						drawHallVert(p1.y, abs(h), p1.x);
					}
				}
				else
				{
					drawHallHoriz(p1.x, abs(w), p1.y);
				}
			}
			else
			{
				if(h < 0)
				{
					drawHallVert(p2.y, abs(h), p2.x);
				}
				else
				{
					drawHallVert(p1.y, abs(h), p1.x);
				}
			}
		}
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
		m_importantTiles.at(0).coords = {oldPos.x + x, oldPos.y + y};

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
	m_importantTiles.push_back({{playerPos.x, playerPos.y}, 2});

	return playerPos;
}
