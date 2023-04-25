#pragma once

class Fragment {
public:
    int x;  // Vị trí ngang
    int y; // Vị trí dọc
    Fragment(int newX, int newY, SDL_Renderer* newRenderer);
    ~Fragment();
    void move(int newX, int newY);
    void createFragment(int newX, int newY);
    void printAndNext(int angle);
    void printHeadSkin(int angle);
    bool checkCollision(int newX, int newY);
    Fragment* next; // Next fragment
private:
    SDL_Texture* headTexture;
    SDL_Texture* bodyTexture;
    SDL_Texture* L_bodyTexture;
    SDL_Texture* tailTexture;
    SDL_Renderer* renderer;
};

class Fruit {
public:
    Fruit(SDL_Renderer* renderer);
    ~Fruit();
    int x;
    int y;
    int type;
    void relocate(Fragment* Head, Fragment* Tail);
    void print(SDL_Renderer* renderer);
private:
    SDL_Texture* appleTexture;
    SDL_Texture* jamTexture;
    SDL_Texture* shieldTexture;
    SDL_Texture* poopTexture;
};


class Snake {
public:
    Snake(int newX, int newY, int dir, SDL_Renderer* newRenderer);
    ~Snake();
    void changeDir(int dir);
    void move(int dir);
    void eat(Fruit* whatever);
    void printEntireSnake();
    bool hitAWallOrHimself();
    Fragment* Head;
    bool shield;
    int score;
private:
    Fragment* Tail;
    int dirX; // Direction horizonale
    int dirY; // Direction vertiacale
    int actualLenght;
    SDL_Renderer* renderer;
};
