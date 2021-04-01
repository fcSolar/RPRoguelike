#pragma once

#include <vector>
#include <memory>
#include "message.h"
#include "geom.h"

enum entityTypes
{
	ENTITY_NULL_TYPE = -1,
	ENTITY_PLAYER,
};
 
class entity
{

protected:
	int m_entityType{ENTITY_NULL_TYPE};

public:
	virtual ~entity() = default;
	virtual std::shared_ptr<message> 
		update(const std::shared_ptr<message> &msg) = 0;
	
};

class ety_player : public entity
{
	std::shared_ptr<message> keyInput(keyCode key);
	coord m_pos;

public:
	ety_player();

	std::shared_ptr<message> 
		update(const std::shared_ptr<message> &msg) override;
};

