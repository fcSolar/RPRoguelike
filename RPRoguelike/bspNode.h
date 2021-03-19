#pragma once

#include <utility>
#include <algorithm>
#include "geom.h"
#include "random/include/effolkronium/random.hpp"

//TODO: tidy up class

class bspNode
{
	const int MIN_SIZE{8};

	std::pair<bspNode*, bspNode*> m_children{nullptr, nullptr};
	
	rect m_area{{-1,-1},-1,-1};
	
	bool m_hasSplit{false};

	rect m_room{{-1,-1},-1,-1};

public:
	bspNode();
	bspNode(int x, int y, int w, int h);

	bool split();

	void createRoom();

	rect getArea() { return m_area; }
	rect getRoom();
	bool getSplitStatus(){ return m_hasSplit; }

	coord getPointInRoom();

	auto getChildren() -> std::pair<bspNode*, bspNode*> { return m_children; }

};

