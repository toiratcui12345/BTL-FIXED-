#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>
#include "defines.h"

#include "graphics.h"

MainWindow::MainWindow() {
    this-> window = NULL; // Cửa sổ hiển thị
    this-> renderer = NULL; // Khu vực cửa sổ
}

MainWindow::~MainWindow() {
    SDL_DestroyRenderer(renderer); // Destruction renderer
    SDL_DestroyWindow(window); // Destruction window
}

void MainWindow::init(const char *name, int width, int height) {
    // Init window & randerer
    if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) < 0) {
        std::cout << "SDL ERROR WHILE CREATING A RENDERER : " << SDL_GetError();
        return;
    }
    SDL_SetWindowTitle(window, name);
}

SDL_Renderer *MainWindow::getRenderer() {
    return this-> renderer;
}

void rendererReset(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void printRectOnRenderer(SDL_Rect rect, SDL_Renderer* renderer, int r, int g, int b) {
    rect.x = (rect.x + 1) * TILE_SIZE;
    rect.y = (rect.y + 1) * TILE_SIZE;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void printImgOnRenderer(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Rect pos, int angle) {
    pos.x = (pos.x + 1) * TILE_SIZE;
    pos.y = (pos.y + 1) * TILE_SIZE;
    pos.w = TILE_SIZE;
    pos.h = TILE_SIZE;
    SDL_RenderCopyEx(renderer, texture, NULL, &pos, angle, NULL, SDL_FLIP_NONE);
}

SDL_Texture* loadSDLImg(const char* file, SDL_Renderer* renderer) {
    SDL_Surface* img = IMG_Load(file);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
    return texture;
}


Playground::Playground(SDL_Renderer* newRenderer) {
    this-> renderer = newRenderer;
    this-> shield = loadSDLImg("shield.png", this-> renderer);
    eraseAndWalls();
}

Playground::~Playground() {
    SDL_DestroyTexture(this-> shield);
}

void Playground::eraseAndWalls() {
    rendererReset(this-> renderer); // Clear the window
    SDL_SetRenderDrawColor(this-> renderer, 20, WALL_COLOR);
    for (int i = 0; i < GRID_WIDTH+1; i++) {
        for (int j = 0; j < GRID_HEIGHT+1; j++) {
            if ((i+j)%2 == 0) printRectOnRenderer({i, j}, this-> renderer, FLOOR_COLOR1);
            else printRectOnRenderer({i, j}, this-> renderer, FLOOR_COLOR2);
        }
    }
    SDL_SetRenderDrawColor(this-> renderer, WALL_COLOR, 255);
    for (int i = 0; i < TILE_SIZE; i++) {
        SDL_Rect rect = {i, i, TILE_SIZE*(3+GRID_WIDTH)-(i-1)*2-1, TILE_SIZE*(3+GRID_HEIGHT)-(i-1)*2-1};
        SDL_RenderDrawRect(this-> renderer, &rect);
    }
}

void Playground::printShieldIndicator() {
    printImgOnRenderer(this-> shield, this-> renderer, {GRID_WIDTH, GRID_HEIGHT+3});
}

void Playground::drawScore(int score) {
	int numDigits = 0;
	for (int n = score; n; n /= 10)	numDigits++;

	while (numDigits) {
		numDigits--;
		drawDigit( score % 10, TILE_SIZE + numDigits * 4 * DIGIT_PIXEL_SIZE, (GRID_HEIGHT+4)*TILE_SIZE );
		score /= 10;
	}
}

void Playground::drawDigit(int digit, int xp, int yp) {
	SDL_Rect rect = { 0, 0, DIGIT_PIXEL_SIZE, DIGIT_PIXEL_SIZE };

	/// Loop if overflow to prevent wrong memory access.
	digit = digit % 10;

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			if ( (*(digits[digit]))[y * 3 + x] == 'x' )
			{
				// Draw shadow.
				SDL_SetRenderDrawColor( this->renderer, 0, 0, 0, 255 );
				rect.x = xp + x * DIGIT_PIXEL_SIZE + DIGIT_PIXEL_SIZE;
				rect.y = yp + y * DIGIT_PIXEL_SIZE + DIGIT_PIXEL_SIZE;
				SDL_RenderFillRect( this->renderer, &rect );

				// Draw white square.
				SDL_SetRenderDrawColor( this->renderer, 255, 255, 255, 255 );
				rect.x -= DIGIT_PIXEL_SIZE;
				rect.y -= DIGIT_PIXEL_SIZE;
				SDL_RenderFillRect( this->renderer, &rect );
			}
		}
	}
}
