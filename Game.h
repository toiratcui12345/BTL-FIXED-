#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>

class Game {

public:
    Game();
    ~Game();

    void init(const char* title, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; }

    static SDL_Renderer *renderer;
    static SDL_Event event;

private:
    int cnt = 0;
    bool isRunning = false;
    SDL_Window *window;
};

#endif // GAME_HPP_INCLUDED
