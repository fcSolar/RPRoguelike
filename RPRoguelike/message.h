#pragma once
#include <utility>


enum class msgType
{
	MESSAGE_TYPE_NULL,
	MESSAGE_TYPE_KEYPRESS,
	MESSAGE_TYPE_PMOVE,
	MESSAGE_TYPE_REGEN_MAP
};
 
enum class keyCode
{
	KEY_NULL,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_SPACE
};

struct message
{
	msgType m_messageType;
};

struct msg_KeyPress: public message
{	
	keyCode m_key{keyCode::KEY_NULL};

	msg_KeyPress(keyCode key) : m_key{key}
	{
		m_messageType = msgType::MESSAGE_TYPE_KEYPRESS;
	}
};

struct msg_playerMove: public message
{
	std::pair<int8_t, int8_t> m_direction;

	msg_playerMove(int8_t x, int8_t y) : m_direction{x,y}
	{
		m_messageType = msgType::MESSAGE_TYPE_PMOVE;
	}
};

struct msg_regenMap: public message
{
	msg_regenMap()
	{
		m_messageType = msgType::MESSAGE_TYPE_REGEN_MAP;
	}
};
