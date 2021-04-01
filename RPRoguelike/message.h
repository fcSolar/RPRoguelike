#pragma once
#include <utility> //for std::pair
#include "geom.h"


enum class msgType
{
	mNULL,
	SUCCESS,
	FAILURE,
	KEYPRESS,
	PMOVE,
	UPDATE_PPOS,
	REGEN_MAP,
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

struct msg_Success: public message
{
	msg_Success() { m_messageType = msgType::SUCCESS; }
};

struct msg_Failure: public message
{
	msg_Failure() { m_messageType = msgType::FAILURE; }
};

struct msg_KeyPress: public message
{	
	keyCode m_key{keyCode::KEY_NULL};

	msg_KeyPress(keyCode key) : m_key{key}
	{
		m_messageType = msgType::KEYPRESS;
	}
};

struct msg_playerMove: public message
{
	std::pair<int8_t, int8_t> m_direction;
	coord currentPos;

	msg_playerMove(int8_t x, int8_t y, coord pos): 
		m_direction{x,y}, currentPos{pos}
	{
		m_messageType = msgType::PMOVE;
	}
};

struct msg_regenMap: public message
{
	msg_regenMap()
	{
		m_messageType = msgType::REGEN_MAP;
	}
};

struct msg_playerPos: public message
{
	coord pos;

	msg_playerPos(coord npos): pos{npos}
	{
		m_messageType = msgType::UPDATE_PPOS;
	}
};