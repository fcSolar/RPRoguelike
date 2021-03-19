#include "SDL.h"
#include "SDL_image.h"
#include "engine.h"

int main(int argc, char *argv[])
{
	engine rlEngine;

	rlEngine.init();
	rlEngine.run();
	rlEngine.exit();

	return 0;
}