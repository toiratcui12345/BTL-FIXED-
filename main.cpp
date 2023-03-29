#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Game.h"
#include "Game.cpp"
#include "GameObject.h"

using namespace std;

Game *game = nullptr;

int main(int argc, char** args)
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    game -> init("MyGame", 800, 640, false);

    while (game -> running()){



        frameStart = SDL_GetTicks();

        game->handleEvents();
        game -> update();
        game -> render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game -> clean();
    return 0;
}
