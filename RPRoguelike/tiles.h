#pragma once
#include <type_traits>
#include "geom.h"

enum class e_tileType
{
	TILE_NULL,
	TILE_WALL,
	TILE_FLOOR,
	TILE_PLAYER,
};

enum tileFlags
{
	TFLAG_NULL = 0b0000'0000,
	TFLAG_TOUCHED = 0b0000'0001,
	TFLAG_PASSABLE = 0b0000'0010,
	TILE_FLAG_2 = 0b0000'0100,
	TILE_FLAG_3 = 0b0000'1000,
	TILE_FLAG_4 = 0b0001'0000,
	TILE_FLAG_5 = 0b0010'0000,
	TILE_FLAG_6 = 0b0100'0000,
	TILE_FLAG_7 = 0b1000'0000,
};

inline tileFlags operator& (tileFlags& lhs, tileFlags rhs)
{
	return static_cast<tileFlags>(static_cast<int>(lhs)
								  & static_cast<int>(rhs));
}

inline tileFlags operator|= (tileFlags& lhs, tileFlags rhs)
{
	return lhs = static_cast<tileFlags>(static_cast<int>(lhs)
										| static_cast<int>(rhs));
}

inline tileFlags operator|= (tileFlags& lhs, int rhs)
{
	return lhs = static_cast<tileFlags>(static_cast<int>(lhs)
										| rhs);
}


inline tileFlags operator&= (tileFlags& lhs, tileFlags rhs)
{
	return lhs = static_cast<tileFlags>(static_cast<int>(lhs)
										& static_cast<int>(rhs));
}

inline tileFlags operator~(tileFlags rhs)
{
	return static_cast<tileFlags>(~static_cast<int>(rhs));
}



struct Colour
{
	uint8_t r{255};
	uint8_t g{255};
	uint8_t b{255};
};																		

const Colour cGrey{0xCC,0xCC,0xCC};
const Colour cLightBlue{0x00,0xCC,0xFF};


struct tileData
{
	coord coords;
	e_tileType tileType;

	tileFlags flags{TFLAG_NULL};

	Colour colour;

	tileData(e_tileType type, int x, int y, Colour setColour, tileFlags flagSet = TFLAG_NULL)
		: coords{x,y}, tileType{type}, colour{setColour}
	{
		flags |= flagSet;
	}

};