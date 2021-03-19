#include "bspNode.h"
#include <stdlib.h>

bspNode::bspNode()
{

}

bspNode::bspNode(int x, int y, int w, int h)
{
	m_area.pos.x = x;
	m_area.pos.y = y;

	m_area.width = w;
	m_area.height = h;

	m_hasSplit = false;
}

bool bspNode::split()
{
	using random = effolkronium::random_static;

	if(m_hasSplit)
		return false;

	bool vert{random::get<bool>()};

	if(m_area.width > m_area.height && m_area.width / m_area.height >= 1.25)
		vert = true;
	else if(m_area.height > m_area.width && m_area.height / m_area.width)
		vert = false;
	
	int maxSize{(vert ? m_area.width : m_area.height) - MIN_SIZE};
	
	if(maxSize <= MIN_SIZE)
		return false;

	int split{random::get(MIN_SIZE, maxSize)};

	if(vert)
	{
		m_children.first = new bspNode(m_area.pos.x, m_area.pos.y, split, m_area.height);
		m_children.second = new bspNode(m_area.pos.x + split, m_area.pos.y, m_area.width - split, m_area.height);
	}
	else
	{
		m_children.first = new bspNode(m_area.pos.x, m_area.pos.y, m_area.width, split);
		m_children.second = new bspNode(m_area.pos.x, m_area.pos.y + split, m_area.width, m_area.height - split);
	}

	m_hasSplit = true;

	return true;
}

void bspNode::createRoom()
{
	using random = effolkronium::random_static;

	m_room.width = random::get(5, m_area.width - 2 > 15 ? 15 : m_area.width - 2);
	m_room.height = random::get(5, m_area.height - 2 > 15 ? 15 : m_area.height - 2);

	m_room.pos.x = random::get(1, m_area.width - m_room.width - 1) + m_area.pos.x;
	m_room.pos.y = random::get(1, m_area.height - m_room.height - 1) + m_area.pos.y;
}

//void bspNode::createHall(rect r1, rect r2)
//{
//	using random = effolkronium::random_static;
//
//	coord p1{random::get(r1.pos.x + 1, r1.pos.x + r1.width - 2), 
//			 random::get(r1.pos.y + 1, r1.pos.y + r1.height - 2)};
//
//	coord p2{random::get(r2.pos.x + 1, r2.pos.x + r2.width - 2),
//			 random::get(r2.pos.y + 1, r2.pos.y + r2.height - 2)};
//
//	int w{p2.x - p1.x};
//	int h{p2.y - p1.y};
//
//
//	if(w < 0)
//	{
//		if(h < 0)
//		{
//			if(random::get<bool>())
//			{
//				m_halls.push_back(new rect{{p2.x, p1.y}, abs(w), 1});
//				m_halls.push_back(new rect{{p2.x, p2.y}, 1, abs(h)});
//			}
//			else
//			{
//				m_halls.push_back(new rect{{p2.x, p2.y}, abs(w), 1});
//				m_halls.push_back(new rect{{p2.x, p2.y}, 1, abs(h)});
//			}
//		}
//		else if(h > 0)
//		{
//			if(random::get<bool>())
//			{
//				m_halls.push_back(new rect{{p2.x, p1.y}, abs(w), 1});
//				m_halls.push_back(new rect{{p2.x, p1.y}, 1, abs(h)});
//			}
//			else
//			{
//				m_halls.push_back(new rect{{p2.x, p2.y}, abs(w), 1});
//				m_halls.push_back(new rect{{p1.x, p1.y}, 1, abs(h)});
//			}
//		}
//		else
//		{
//			m_halls.push_back(new rect{{p2.x, p2.y}, abs(w), 1});
//		}
//	}
//	else if(w > 0)
//	{
//		if(h < 0)
//		{
//			if(random::get<bool>())
//			{
//				m_halls.push_back(new rect{{p1.x, p2.y}, abs(w), 1});
//				m_halls.push_back(new rect{{p1.x, p2.y}, 1, abs(h)});
//			}
//			else
//			{
//				m_halls.push_back(new rect{{p1.x, p1.y}, abs(w), 1});
//				m_halls.push_back(new rect{{p2.x, p2.y}, 1, abs(h)});
//			}
//		}
//		else if(h > 0)
//		{
//			if(random::get<bool>())
//			{
//				m_halls.push_back(new rect{{p1.x, p1.y}, abs(w), 1});
//				m_halls.push_back(new rect{{p2.x, p2.y}, 1, abs(h)});
//			}
//			else
//			{
//				m_halls.push_back(new rect{{p1.x, p2.y}, abs(w), 1});
//				m_halls.push_back(new rect{{p1.x, p1.y}, 1, abs(h)});
//			}
//		}
//		else
//		{
//			m_halls.push_back(new rect{{p1.x, p1.y}, abs(w), 1});
//		}
//	}
//	else
//	{
//		if(h < 0)
//		{
//			m_halls.push_back(new rect{{p2.x, p2.y}, 1, abs(h)});
//		}
//		else
//		{
//			m_halls.push_back(new rect{{p1.x, p1.y}, 1, abs(h)});
//		}
//	}
//
//}

rect bspNode::getRoom()
{
	return m_room;
}

coord bspNode::getPointInRoom()
{
	using random = effolkronium::random_static;
	coord pt{random::get(m_room.pos.x + 1, m_room.pos.x + m_room.width - 2),
			 random::get(m_room.pos.y + 1, m_room.pos.y + m_room.height - 2)};

	return pt;
}
