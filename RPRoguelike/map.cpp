#include "map.h"


void gameMap::generate(rect mapSize)
{
	m_levelWidth = mapSize.width;
	m_levelHeight = mapSize.height;

	for(int i = 0; i < m_levelWidth * m_levelHeight; i++)
	{
		m_level.push_back(tileData(e_tileType::TILE_WALL, 
								   i % m_levelWidth, 
								   i / m_levelWidth, 
								   cGrey,
								   TFLAG_TOUCHED));
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
			m_level[ind2d(i,xDrawLevel)].tileType = e_tileType::TILE_FLOOR;
			m_level[ind2d(i,xDrawLevel)].flags |= TFLAG_PASSABLE;
		}

		for(int i = std::min(pt1.y, pt2.y); i <= std::max(pt1.y, pt2.y); i++)
		{
			m_level[ind2d(yDrawLevel, i)].tileType = e_tileType::TILE_FLOOR;
			m_level[ind2d(yDrawLevel, i)].flags |= TFLAG_PASSABLE;
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
	for(auto &e : m_level)
	{
		if(e.flags & TFLAG_TOUCHED)
		{
			m_drawFn(e);
			e.flags &= ~TFLAG_TOUCHED;
		}
	}
}

void gameMap::clear()
{
	m_level.clear();
}


void gameMap::drawRoom(rect rm)
{
	for(int i = rm.pos.y; i < rm.pos.y + rm.height; i++)
	{
		for(int j = rm.pos.x; j < rm.pos.x + rm.width; j++)
		{
			m_level[ind2d(j,i)].tileType = e_tileType::TILE_FLOOR;
			m_level[ind2d(j, i)].flags |= (TFLAG_TOUCHED | TFLAG_PASSABLE);
		}
	}
}

std::shared_ptr<message> gameMap::movePlayer(const std::shared_ptr<msg_playerMove> &msg)
{
	coord oldPos{msg->currentPos};
	coord newPos{oldPos.x + msg->m_direction.first, oldPos.y + msg->m_direction.second};
	
	if(m_level[ind2d(newPos.x, newPos.y)].flags & tileFlags::TFLAG_PASSABLE)
	{


		m_level[ind2d(oldPos.x, oldPos.y)].tileType = e_tileType::TILE_FLOOR;
		m_level[ind2d(oldPos.x, oldPos.y)].colour = cGrey;
		m_level[ind2d(newPos.x, newPos.y)].tileType = e_tileType::TILE_PLAYER;
		m_level[ind2d(newPos.x, newPos.y)].colour = cLightBlue;
		
		m_level[ind2d(oldPos.x, oldPos.y)].flags |= TFLAG_TOUCHED;
		m_level[ind2d(newPos.x, newPos.y)].flags |= TFLAG_TOUCHED;


		return std::shared_ptr<msg_playerPos>(new msg_playerPos(newPos));
	}
	else
		return std::shared_ptr<msg_Failure>(new msg_Failure);
}

std::shared_ptr<msg_playerPos> gameMap::addPlayer()
{
	using random = effolkronium::random_static;

	std::vector<coord> validPos;
	int ind{0};

	for(auto &e : m_level)
	{
		if(e.flags & TFLAG_PASSABLE)
			validPos.push_back({ind%m_levelWidth,ind/m_levelWidth});
		ind++;
	};
	
	coord playerPos{*random::get(validPos)};

	m_level[ind2d(playerPos.x, playerPos.y)].tileType = e_tileType::TILE_PLAYER;
	m_level[ind2d(playerPos.x, playerPos.y)].flags |= TFLAG_TOUCHED;
	m_level[ind2d(playerPos.x, playerPos.y)].colour = cLightBlue;

	return std::shared_ptr<msg_playerPos>(new msg_playerPos(playerPos));
}