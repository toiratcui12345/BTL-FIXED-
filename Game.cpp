#include<vector>
#include <string>
#include <ctime>

#include "Game.h"

using namespace std;



Game::Game()
{

	for (int i = 0; i < GRID_WIDTH; ++i)
		for (int j = 0; j < GRID_HEIGHT; ++j)
		{
			grid[i][j] = Block::empty;
		}

	srand(static_cast<unsigned int>(time(0)));
}


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	//Initialization flag

	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "system initialised...." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window Launched !" << std::endl;
		}
		background = SDL_LoadBMP("desert.bmp");
		if (background == NULL)
		{
			SDL_ShowSimpleMessageBox(0, "Background init error", SDL_GetError(), window);
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if(renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Launched !" << std::endl;
		}
	    texture = SDL_CreateTextureFromSurface(renderer, background);
		if (texture == NULL)
		{
			SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
		}

		//Initialize SDL_mixer

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			cout << "Error: " << Mix_GetError() << std::endl;
		}

		Mix_Music* gMusic = Mix_LoadMUS("beat.wav");
		//Play the music
		Mix_PlayMusic(gMusic, 1000);



		alive = true;
		isRunning = true;
		Food();
		GameLoop();
	}
	else
	{
		isRunning = false;
	}

}



void Game::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			isRunning = false;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				if (last_dir != Move::down || size == 1)
					dir = Move::up;
				break;

			case SDLK_s:
				if (last_dir != Move::up || size == 1)
					dir = Move::down;
				break;

			case SDLK_a:
				if (last_dir != Move::right || size == 1)
					dir = Move::left;
				break;

			case SDLK_d:
				if (last_dir != Move::left || size == 1)
					dir = Move::right;
				break;
			}
		}
	}
}

void Game::update()
{
	if (!alive)
		return;

	switch (dir)
	{
	case Move::up:
		pos.y -= speed;
		pos.x = pos.x;
		break;

	case Move::down:
		pos.y += speed;
		pos.x = pos.x;
		break;

	case Move::left:
		pos.x -= speed;
		pos.y = pos.y;
		break;

	case Move::right:
		pos.x += speed;
		pos.y = pos.y;
		break;

	}

	// Wrap
	if (pos.x < 0) pos.x = GRID_WIDTH - 1;
	else if (pos.x > GRID_WIDTH - 1) pos.x = 0;

	if (pos.y < 0) pos.y = GRID_HEIGHT - 1;
	else if (pos.y > GRID_HEIGHT - 1) pos.y = 0;

	int new_x = static_cast<int>(pos.x);
	int new_y = static_cast<int>(pos.y);

	// Check if head position has changed
	if (new_x != head.x || new_y != head.y)
	{
		last_dir = dir;

		// If we are growing, just make a new neck
		if (growing > 0)
		{
			size++;
			body.push_back(head);
			growing--;
			grid[head.x][head.y] = Block::body;
		}
		else
		{
			// We need to shift the body
			SDL_Point free = head;
			std::vector<SDL_Point>::reverse_iterator rit = body.rbegin();
			for (; rit != body.rend(); ++rit)
			{
				grid[free.x][free.y] = Block::body;
				std::swap(*rit, free);
			}

			grid[free.x][free.y] = Block::empty;
		}

	}

	head.x = new_x;
	head.y = new_y;

	Block& next = grid[head.x][head.y];
	// Check if there's food over here
	if (next == Block::food)
	{
		score++;
		Food();
		GrowBody(1);
	}
	// Check if we're dead
	else if (next == Block::body)
	{
		alive = false;
	}

	next = Block::head;
}

void Game::render()
{

	SDL_Rect block;
	block.w = SCREEN_WIDTH / GRID_WIDTH;
	block.h = SCREEN_WIDTH / GRID_HEIGHT;

	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, texture, NULL, NULL);

	// Render food
	SDL_SetRenderDrawColor(renderer, 149, 53, 83, 255);
	block.x = food.x * block.w;
	block.y = food.y * block.h;
	SDL_RenderFillRect(renderer, &block);

	// Render snake's body
	SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
	for (SDL_Point& point : body)
	{
		block.x = point.x * block.w;
		block.y = point.y * block.h;
		SDL_RenderFillRect(renderer, &block);
	}

	// Render snake's head
	block.x = head.x * block.w;
	block.y = head.y * block.h;
	if (alive) SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
	else       SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &block);

	// Update Screen

	SDL_RenderPresent(renderer);
}


int Game::GetScore()
{
	return 0;
}

int Game::GetSize()
{
	return size;
}



void Game::Food()
{
	int x, y;
	while (true)
	{
		x = rand() % GRID_WIDTH;
		y = rand() % GRID_HEIGHT;

		if (grid[x][y] == Block::empty)
		{
			grid[x][y] = Block::food;
			food.x = x;
			food.y = y;
			break;
		}
	}
}

void Game::GrowBody(int quantity)
{
	growing += quantity;
}

void Game::UpdateWindowTitle()
{
	string title = "Snake        Score: " + to_string(score);
	SDL_SetWindowTitle(window, title.c_str());
}

void Game::GameLoop()
{
	Uint32 before, second = SDL_GetTicks(), after;
	int frame_time, frames = 0;

	while (isRunning)
	{
		before = SDL_GetTicks();

		handleEvents();
		update();
		render();

		frames++;
		after = SDL_GetTicks();
		frame_time = after - before;

		if (after - second >= 1000)
		{
			fps = frames;
			frames = 0;
			second = after;
			UpdateWindowTitle();
		}

		if (FRAME_RATE > frame_time)
		{
			SDL_Delay(FRAME_RATE - frame_time);
		}
	}
}

void Game::clean()
{


	SDL_DestroyWindow(window);

	SDL_FreeSurface(background);

	//Quit SDL subsystems
	Mix_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleared" << std::endl;
}



