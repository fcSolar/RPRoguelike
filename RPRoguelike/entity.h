#pragma once

#include <vector>
#include <memory>
#include "message.h"

enum entityTypes
{
	ENTITY_NULL_TYPE = -1,
	ENTITY_PLAYER,
};

class entity
{
	int m_entityType{ENTITY_NULL_TYPE};

public:
	virtual std::shared_ptr<message> 
		update(const std::shared_ptr<message> &msg) = 0;
};

class player: public entity
{
	int m_entityType{ENTITY_PLAYER};

	std::shared_ptr<message> keyInput(keyCode key);

public:
	player();

	std::shared_ptr<message> 
		update(const std::shared_ptr<message> &msg) override;
};


