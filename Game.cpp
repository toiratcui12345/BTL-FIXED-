#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defines.h"

#include "game.h"
#include "objects.h"
#include "graphics.h"

using namespace std;

Application::Application() {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) { // SDL init error ?
    printf("SDL initialization error: %s\nn",SDL_GetError()); // Print error
    exit(1); // Quit to avoid more problems
  }
  IMG_Init(IMG_INIT_PNG);
  srand(time(0)); // rand() init
  this-> frame_rate = 30; // Frame rate (fps)
  this-> snake_rate = 8; // Tiles per second (tps)
  this-> paused = false;
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
  {
      cout << "Error: " << Mix_GetError() << endl;
  }

  Mix_Chunk* gMusic = Mix_LoadWAV("beat.wav");

  Mix_PlayChannel(-1,gMusic,1000);
}


Application::~Application(){
  delete this-> Nico;
  delete this-> Apple;
  delete this-> Room;
  delete this-> main_window;
  IMG_Quit();
  SDL_Quit();
}
bool mouseInside(SDL_Point mouse, SDL_Rect area){
    if(mouse.x >= area.x && mouse.y <= area.x + area.w &&
        mouse.y >= area.y && mouse.y <= area.y + area.h)
            return true;
    return false;
}
void Application::appLoop() {

    /// Main loop
    int lastDir = this-> dir; // Last direction (we can't go backward)
    bool interrupt = false;
    bool status = false, mouseOver = 0;

    while(!interrupt) {
        SDL_Texture *gameBg, *startG[2];
        SDL_Rect rectGame, rectSt;
        gameBg = loadSDLImg("Bg.png", this-> renderer);
        rectGame.x = 0;
        rectGame.y = 0;
        rectGame.w = 1280;
        rectGame.h = 750;

        rectSt.x = 640 - 150;
        rectSt.y = 490;
        rectSt.w = 300;
        rectSt.h = 150;

        startG[0] = loadSDLImg("startGame.png", this-> renderer);
        startG[1] = loadSDLImg("startGame1.png", this-> renderer);

        SDL_RenderCopy(this-> renderer, gameBg, NULL, &rectGame);


        while (SDL_PollEvent(&this-> event)){
            if(this-> event.type == SDL_QUIT){
                interrupt = true;
            }
            if(this-> event.type == SDL_MOUSEMOTION || this-> event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point mouse;
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if(mouseInside(mouse, rectSt)) {
                    if(this-> event.type == SDL_MOUSEBUTTONDOWN)
                    {
                        status = true;
                        interrupt = true;
                    }
                    mouseOver = 1;
                }
                else
                {
                    mouseOver = 0;
                }
            }

            SDL_RenderClear(this-> renderer);
            SDL_RenderCopy(this-> renderer, gameBg, NULL, &rectGame);
            SDL_RenderCopy(this-> renderer, startG[mouseOver], NULL, &rectSt);
            SDL_RenderPresent(this-> renderer);
        }
        SDL_DestroyTexture(gameBg);
        SDL_DestroyTexture(startG[0]);
        SDL_DestroyTexture(startG[1]);
    }
    if(!status) return;
    interrupt = false;
    while (!interrupt) {
    count++;
    Uint32 tTime = SDL_GetTicks(); // For frame rate
    this-> Room-> eraseAndWalls(); // Clear the window and draw floor and walls

    /// Events
    while (SDL_PollEvent(&this-> event)) { // While there is keyboard/mouse event in the queue
      switch (this-> event.type) {
        case SDL_QUIT: // In case we hit the red X
          interrupt = true;
          break;
        case SDL_KEYDOWN: // We pressed a key on the keyboard
            if (this-> event.key.keysym.sym == SDLK_UP && lastDir%2 == 0 || this-> event.key.keysym.sym == SDLK_w && lastDir%2 == 0 )
                {dir = DIR_UP;} // Up arrow
            if (this-> event.key.keysym.sym == SDLK_DOWN && lastDir%2 == 0 || this-> event.key.keysym.sym == SDLK_s && lastDir%2 == 0)
                {dir = DIR_DOWN;} // Down arrow
            if (this-> event.key.keysym.sym == SDLK_LEFT && lastDir%2 == 1 || this-> event.key.keysym.sym == SDLK_a && lastDir%2 == 1)
                {dir = DIR_LEFT;} // Left arrow
            if (this-> event.key.keysym.sym == SDLK_RIGHT && lastDir%2 == 1 || this-> event.key.keysym.sym == SDLK_d && lastDir%2 == 1)
                {dir = DIR_RIGHT;} // Right arrow
            if (this-> event.key.keysym.sym == SDLK_p || this-> event.key.keysym.sym == SDLK_ESCAPE)
            {
                this-> paused = !this-> paused; // Pause the game!
            }
          break;
      }
    }
    if (!paused && count%(frame_rate/snake_rate) == 0 && !interrupt) {
      Nico-> move(dir); // Snake moves forward
      if (this-> Nico-> hitAWallOrHimself()) {
        if (Nico-> shield == true) {
          Nico-> Head-> x = (Nico-> Head-> x + GRID_WIDTH)%GRID_WIDTH;
          Nico-> Head-> y = (Nico-> Head-> y + GRID_HEIGHT)%GRID_HEIGHT;
          Nico-> shield = false;
        }
        else interrupt = true;
      }
      lastDir = this-> dir;

      // If the head is on a fruit, we eat it
        if (this->Apple->x == this->Nico->Head->x && this->Apple->y == this->Nico->Head->y)
            {Mix_Music* sMusic = Mix_LoadMUS("snake_bite.wav");
             Mix_PlayMusic(sMusic, 1);
             this-> Nico->eat(this-> Apple);}
    }

    /// Drawing
    // In the Playground
    this-> Nico-> printEntireSnake(); // Display snake
    this-> Apple-> print(this-> renderer); // Display fruit
    // In the Score bar
    Room-> drawScore(this-> Nico-> score);
    if (Nico-> shield == true) Room-> printShieldIndicator();
    SDL_RenderPresent(this-> renderer); // Update the window (print all at once)

    /// Frame rate
    tTime = SDL_GetTicks() - tTime; // tTime is now time elapsed in execution
    // If execution time is less than the frame rate (no lag), we wait next frame
    if (1000 / this-> frame_rate - tTime > 0) SDL_Delay(1000 / this-> frame_rate - tTime);
  }
}

void Application::appInit() {
  /// Graphical init
  this-> main_window = new MainWindow();
  this-> main_window-> init("Snake!", SCREEN_WIDTH, SCREEN_HEIGHT);
  this-> renderer = main_window-> getRenderer(); // Get renderer
  this-> Room = new Playground(this-> renderer);

  /// Variables init
  this-> Nico = new Snake(GRID_WIDTH/3, GRID_HEIGHT/2, 0, this-> renderer); // Snake
  this-> Apple = new Fruit(this-> renderer); // Fruit
  this-> Nico-> eat(this-> Apple);
}
