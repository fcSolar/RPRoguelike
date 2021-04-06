#include "engine.h"

void engine::init()
{

	if(SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		m_win = SDL_CreateWindow("RPRoguelike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

		if(m_win)
		{
			m_renderer = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_ACCELERATED);

			if(m_renderer)
			{
				IMG_Init(IMG_INIT_PNG);
			}
		}
	}

	loadTexture("font4.png");
}
 
void engine::run()
{
	bool running{true};

	SDL_Event evnt;
	
	level.bindDrawFn(drawFn);

	m_entities.emplace_back(new ety_player);
	std::unique_ptr<entity>& player{m_entities[0]};

	m_SDLToInternalKeymap.insert({SDLK_UP, keyCode::KEY_UP});
	m_SDLToInternalKeymap.insert({SDLK_DOWN, keyCode::KEY_DOWN});
	m_SDLToInternalKeymap.insert({SDLK_LEFT, keyCode::KEY_LEFT});
	m_SDLToInternalKeymap.insert({SDLK_RIGHT, keyCode::KEY_RIGHT});
	m_SDLToInternalKeymap.insert({SDLK_SPACE, keyCode::KEY_SPACE});


	level.generate({{0,0}, WINDOW_WIDTH / m_tileWidth,
						WINDOW_HEIGHT / m_tileHeight});
	m_entities[0]->update(level.addPlayer());
	level.draw();
	
	SDL_RenderPresent(m_renderer);

	bool newMessage{false};

	while(running)
	{
		SDL_WaitEvent(&evnt);

		switch(evnt.type)
		{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
			{
				std::unordered_map<int, keyCode>::iterator e_KeyCodeIt = m_SDLToInternalKeymap.find(evnt.key.keysym.sym);
				keyCode e_KeyCode{keyCode::KEY_NULL};

				if(e_KeyCodeIt != m_SDLToInternalKeymap.end())
					e_KeyCode = e_KeyCodeIt->second;
				else
					break;

				std::shared_ptr<message> e_Msg(new msg_KeyPress(e_KeyCode));
				std::shared_ptr<message> r_Msg{nullptr};
				
				r_Msg = player->update(e_Msg);
				
				m_msgQueue.emplace_back(r_Msg);

				newMessage = true;

				break;

			}
			default:
				break;
		}

		if(newMessage)
			readMessages();

		newMessage = false;
	}
}

void engine::exit()
{
	IMG_Quit();
	SDL_Quit();
}


void engine::loadTexture(const char* path)
{
	SDL_Surface *surf = IMG_Load(path);

	if(surf)
	{
		m_sheet = SDL_CreateTextureFromSurface(m_renderer, surf);

		if(m_sheet)
		{
			m_tileWidth = 9;
			m_tileHeight = 16;

			m_sheetWidth = 32;
			m_sheetHeight = 8;

			SDL_FreeSurface(surf);
		}
	}
}

void engine::putTile(tileData tile)
{
	SDL_Rect srcR;
	SDL_Rect dstR;

	srcR.w = m_tileWidth;
	srcR.h = m_tileHeight;

	dstR.w = m_tileWidth;
	dstR.h = m_tileHeight;
	dstR.x = tile.coords.x * m_tileWidth;
	dstR.y = tile.coords.y * m_tileHeight;

	Colour colour = tile.colour;

	switch(tile.tileType)
	{
		case e_tileType::TILE_WALL: //'#'
			srcR.x = 3 * m_tileWidth;
			srcR.y = 1 * m_tileHeight;
			break;
		case e_tileType::TILE_FLOOR: //middle dot
			srcR.x = 25 * m_tileWidth;
			srcR.y = 7 * m_tileHeight;
			break;
		case e_tileType::TILE_PLAYER: //'@'
			srcR.x = 0;
			srcR.y = 2 * m_tileHeight;
			break;
		default:
			break;
	}

	SDL_SetTextureColorMod(m_sheet, colour.r, colour.g, colour.b);
	SDL_RenderCopy(m_renderer, m_sheet, &srcR, &dstR);
}


void engine::readMessages()
{
	while(m_msgQueue.size() > 0)
	{
		std::shared_ptr<message> m1 = m_msgQueue.back();
		m_msgQueue.pop_back();

		switch(m1->m_messageType)
		{
			case msgType::PMOVE:
			{
				std::shared_ptr<msg_playerMove> m2 = std::static_pointer_cast<msg_playerMove>(m1);
				std::shared_ptr<message> m3 = level.movePlayer(m2);
				
				if(m3->m_messageType == msgType::UPDATE_PPOS)
				{
					level.draw();
					SDL_RenderPresent(m_renderer);
					m_msgQueue.push_back(m3);
				}
				break;
			}
			case msgType::UPDATE_PPOS:
				m_entities[0]->update(m1);
				break;
			case msgType::REGEN_MAP:
				level.clear();
				level.generate({{0,0}, WINDOW_WIDTH / m_tileWidth,
									 WINDOW_HEIGHT / m_tileHeight});
				m_entities[0]->update(level.addPlayer());
				level.draw();
				SDL_RenderPresent(m_renderer);
				break;
			default:
				break;
		}
	}
}