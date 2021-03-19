#pragma once

#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "map.h"
#include "geom.h"
#include <functional>
#include <cmath>
#include <memory>
#include <unordered_map>
#include "entity.h"
#include "message.h"


//TODO: Split render stuff out into new class. (tilesheet, render fucntions, etc.)

class engine
{
	const int WINDOW_WIDTH{1602};
	const int WINDOW_HEIGHT{912};
	
	SDL_Texture *m_sheet;

	int m_tileWidth;   //in px
	int m_tileHeight;  //in px

	int m_sheetWidth;  //in tiles
	int m_sheetHeight; //in tiles

	SDL_Window *m_win;
	SDL_Renderer *m_renderer;

	gameMap level;

	std::vector<std::unique_ptr<entity>> m_entities;

	std::unordered_map<int, keyCode> m_SDLToInternalKeymap;

	std::vector<std::shared_ptr<message>> m_msgQueue;

	std::function<void(uint8_t tile, int x, int y)> drawFn = [&](auto tile, auto x, auto y) { putTile(tile, x, y); };
	//TODO: map of tile aliases, key: string, value: uint8_t

public:
	void init();
	void run();
	void exit();

private:
	void loadTexture(std::string path);
	void putTile(int tileX, int tileY, int x, int y); //TODO: change to take coord struct instead.
	void putTile(uint8_t tile, int x, int y);
	void readMessages();
};

