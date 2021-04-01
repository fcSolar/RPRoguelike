#pragma once

#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include "SDL.h"
#include "SDL_image.h"
#include "map.h"
#include "geom.h"
#include "entity.h"
#include "message.h"
 

//TODO: Split render stuff out into new class. (tilesheet, render fucntions, etc.)

class engine
{
	const int WINDOW_WIDTH{1602};
	const int WINDOW_HEIGHT{912};
	
	SDL_Texture *m_sheet{nullptr};

	int m_tileWidth{-1};   //in px
	int m_tileHeight{-1};  //in px

	int m_sheetWidth{-1};  //in tiles
	int m_sheetHeight{-1}; //in tiles

	SDL_Window *m_win{nullptr};
	SDL_Renderer *m_renderer{nullptr};

	gameMap level;

	std::vector<std::unique_ptr<entity>> m_entities;

	std::unordered_map<int, keyCode> m_SDLToInternalKeymap;

	std::vector<std::shared_ptr<message>> m_msgQueue;

	std::function<void(tileData tile)> 
		drawFn = [&](tileData tile) { putTile(tile); };

public:
	void init();
	void run();
	void exit();

private:
	void loadTexture(const char* path);
	void putTile(tileData tile);
	void readMessages();
};

