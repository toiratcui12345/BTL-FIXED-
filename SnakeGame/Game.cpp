#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defines.h"

#include "game.h"
#include "objects.h"
#include "graphics.h"

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
}


Application::~Application(){
  delete this-> Nico;
  delete this-> Apple;
  delete this-> Room;
  delete this-> main_window;
  IMG_Quit();
  SDL_Quit();
}

void Application::appLoop() {
  /// Main loop
  int lastDir = this-> dir; // Last direction (we can't go backward)
  bool interrupt = false;

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
            if (this-> event.key.keysym.sym == SDLK_UP && lastDir%2 == 0) dir = DIR_UP; // Up arrow
            if (this-> event.key.keysym.sym == SDLK_DOWN && lastDir%2 == 0) dir = DIR_DOWN; // Down arrow
            if (this-> event.key.keysym.sym == SDLK_LEFT && lastDir%2 == 1) dir = DIR_LEFT; // Left arrow
            if (this-> event.key.keysym.sym == SDLK_RIGHT && lastDir%2 == 1) dir = DIR_RIGHT; // Right arrow
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
        this-> Nico->eat(this-> Apple);
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
