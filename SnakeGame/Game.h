#pragma once
#include <SDL.h>
#include "graphics.h"
#include "objects.h"

class Application{
public:
    Application();
    ~Application();
    void appLoop();
    void appInit();
private:
    bool paused;
    MainWindow* main_window;
    SDL_Renderer* renderer;
    Playground* Room;
    SDL_Event event; // Bind keyboard/mouse events
    Snake* Nico;
    Fruit* Apple;
    Uint32 frame_rate;
    Uint32 snake_rate;
    int dir = 0; // Snake next direction
    int count = 0; // How many cases the snake has moved forward
};
