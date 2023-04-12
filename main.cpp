#include <SDL.h>
#undef main
#include "Game.h"

int main()
{

	Game game;


	game.init("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 700, false);

	while (game.running())
	{
		game.handleEvents();
		game.update();
		game.render();


	}


	game.clean();



	return 0;
}
