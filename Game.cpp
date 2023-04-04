#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"

using namespace std;

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
auto& player(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        cout << "Subsystems Initialised!..." << endl;

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if (window)
        {
            cout << "Window created!" << endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            cout << "Renderer created!" << endl;
        }

        isRunning = true;
    } else {
        isRunning = false;
    }
    map = new Map();

    player.addComponent<PositionComponent>();
    player.addComponent<SpriteComponent>("assets/Player.png");

}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent (&event);
    switch (event.type){
    case SDL_QUIT:
        isRunning = false;
        break;

    default:
        break;
    }
}

void Game::update()
{
    manager.refresh();
    manager.update();

    if(player.getComponent<PositionComponent>().x() > 100)
    {
        player.getComponent<SpriteComponent>().setTex("assets/Enemy.png");
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    map -> DrawMap();
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "Game Cleaned" << endl;
}
