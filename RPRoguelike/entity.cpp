#include "entity.h"

std::shared_ptr<message> ety_player::keyInput(keyCode key)
{
	switch(key)
	{
		case keyCode::KEY_UP:
			return std::shared_ptr<msg_playerMove>(new msg_playerMove(0,-1));
		case keyCode::KEY_DOWN:
			return std::shared_ptr<msg_playerMove>(new msg_playerMove(0, 1));
		case keyCode::KEY_LEFT:
			return std::shared_ptr<msg_playerMove>(new msg_playerMove(-1, 0));
		case keyCode::KEY_RIGHT:
			return std::shared_ptr<msg_playerMove>(new msg_playerMove(1, 0));
		case keyCode::KEY_SPACE:
			return std::shared_ptr<msg_regenMap>(new msg_regenMap());
		default:
			return nullptr;
	}
}
 
ety_player::ety_player()
{
	m_entityType = entityTypes::ENTITY_PLAYER;
}

std::shared_ptr<message> ety_player::update(const std::shared_ptr<message> &msg)
{
	switch(msg->m_messageType)
	{
		case msgType::MESSAGE_TYPE_KEYPRESS:
			return keyInput(std::static_pointer_cast
							<msg_KeyPress>(msg)->m_key);
		default:
			return nullptr;
	}
}
