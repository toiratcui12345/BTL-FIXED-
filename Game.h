#pragma

#include<sdl.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<iostream>
#include <vector>

static const int FRAME_RATE = 1000 / 60;
static const int SCREEN_WIDTH = 700;
static const int SCREEN_HEIGHT = 700;
static const int GRID_WIDTH = 32;
static const int GRID_HEIGHT = 32;




class Game {

public:

	Game();


	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
    int GetScore();
    int GetSize();
	void clean();

	bool running() { return isRunning; }


private:
	bool isRunning;
	bool alive = false;
	int fps = 0;

	SDL_Window *window;
	SDL_Renderer* renderer;


    enum class Block { head, body, food, empty };
    enum class Move { up, down, left, right };

    Move last_dir = Move::up;
    Move dir = Move::up;

    struct { float x = GRID_WIDTH / 2, y = GRID_HEIGHT / 2; } pos;

    SDL_Point head = { static_cast<int>(pos.x), static_cast<int>(pos.y) };
    SDL_Point food;
    std::vector<SDL_Point> body;

    Block grid[GRID_WIDTH][GRID_HEIGHT];

    float speed = 0.25f;
    int growing = 0;
    int score = 0;
    int size = 1;

    void Food();
    void GrowBody(int quantity);
    void UpdateWindowTitle();
    void GameLoop();

    SDL_Surface* screen;
    SDL_Surface* background;
    SDL_Texture* texture;

};

