#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defines.h"

#include "game.h"
#include "objects.h"
#include "graphics.h"

// Class FRAGMENT
Fragment::Fragment(int newX, int newY, SDL_Renderer* newRenderer) {
  this-> x = newX;
  this-> y = newY;
  this-> next = NULL;
  this-> renderer = newRenderer;
  this-> headTexture = loadSDLImg("head_c.png", this-> renderer);
  this-> bodyTexture = loadSDLImg("body_c.png", this-> renderer);
  this-> L_bodyTexture = loadSDLImg("L_body_c.png", this-> renderer);
  this-> tailTexture = loadSDLImg("tail_c.png", this-> renderer);
}
Fragment::~Fragment() {
  SDL_DestroyTexture(this-> headTexture);
  SDL_DestroyTexture(this-> bodyTexture);
  SDL_DestroyTexture(this-> L_bodyTexture);
  SDL_DestroyTexture(this-> tailTexture);
  if (next != NULL) delete next;
}

void Fragment::createFragment(int newX, int newY) {
  if (next != NULL) next-> createFragment(newX, newY);
  else this-> next = new Fragment(newX, newY, this->renderer);
}

void Fragment::printAndNext(int angle) {
  if (next != NULL) {
    int nextAngle = 90*((this->x-this->next->x-1)*(this->x-this->next->x)+this->y-this->next->y);
    if (this->x==this->next->x && this->y==this->next->y)
      this-> next-> printAndNext(angle);
    else if (nextAngle != angle) {
      if ((nextAngle+360)%360 - angle == 90) angle -= 90;
      printImgOnRenderer(this-> L_bodyTexture, this->renderer, {this->x, this->y}, angle);
      this-> next-> printAndNext(nextAngle);
    } else {
      printImgOnRenderer(this-> bodyTexture,this->renderer, {this->x, this->y}, angle);
      this-> next-> printAndNext(nextAngle);
    }
  } else {
    printImgOnRenderer(this-> tailTexture, this-> renderer, {this->x, this->y}, angle);
  }
}

void Fragment::printHeadSkin(int angle) {
  printImgOnRenderer(this-> headTexture, this-> renderer, {this->x, this->y}, angle);
}

void Fragment::move(int newX, int newY) {
  if (this-> next != NULL) {
    if (this-> x == this-> next-> x && this-> y == this-> next-> y);
    else next-> move(this-> x, this-> y);
  }
  this-> x = newX;
  this-> y = newY;
}

bool Fragment::checkCollision(int newX, int newY) {
  if (this-> x == newX && this-> y == newY) return true;
  if (this-> next != NULL) return this-> next-> checkCollision(newX, newY);
  return false;
}


// Class FRUIT
Fruit::Fruit(SDL_Renderer* renderer) {
  this-> appleTexture = loadSDLImg("apple.png", renderer);
  this-> jamTexture = loadSDLImg("jam.png", renderer);
  this-> shieldTexture = loadSDLImg("shield.png", renderer);
  this-> poopTexture = loadSDLImg("poop.png", renderer);
}

Fruit::~Fruit() {
  SDL_DestroyTexture(this-> appleTexture);
  SDL_DestroyTexture(this-> jamTexture);
  SDL_DestroyTexture(this-> shieldTexture);
  SDL_DestroyTexture(this-> poopTexture);
}

void Fruit::relocate(Fragment* Head, Fragment* Tail) {
  if (rand()%10 == 0) this-> type = FRUIT_SHIELD;
  else if (rand()%5 == 0) this-> type = FRUIT_JAM;
  else this-> type = FRUIT_APPLE;

  for (int i = 0; i == 0 || (i < 10 && Head->checkCollision(this-> x, this-> y)); i++) {
    this-> x = rand() % GRID_WIDTH;
    this-> y = rand() % GRID_HEIGHT;
  } if (Head->checkCollision(this-> x, this-> y)) {
    this-> type = FRUIT_POOP;
    this-> x = Tail-> x;
    this-> y = Tail-> y;
  }
}


void Fruit::print(SDL_Renderer* renderer) {
  if (this-> type == FRUIT_APPLE)
    printImgOnRenderer(this-> appleTexture, renderer, {this->x, this->y});
  else if (this-> type == FRUIT_JAM)
    printImgOnRenderer(this-> jamTexture, renderer, {this->x, this->y});
  else if (this-> type == FRUIT_SHIELD)
    printImgOnRenderer(this-> shieldTexture, renderer, {this->x, this->y});
  else
    printImgOnRenderer(this-> poopTexture, renderer, {this->x, this->y});
}


// Class SNAKE
Snake::Snake(int newX, int newY, int dir, SDL_Renderer* newRenderer) {
  changeDir(dir);
  this-> renderer = newRenderer;
  this-> Head = new Fragment(newX, newY, this-> renderer);
  this-> Head-> createFragment(newX - this-> dirX, newY - this-> dirY);
  this-> Tail = Head-> next;
  this-> actualLenght = 2;
  this-> shield = false;
  this-> score = -10;
}

Snake::~Snake() {
  delete this-> Head;
}

void Snake::changeDir(int dir) {
  dirX = (1-dir%2)*(1-dir);
  dirY = -(dir%2)*(dir-2);
}

void Snake::move(int dir) {
  changeDir(dir);
  this-> Head-> move(this-> Head->x + this-> dirX, this-> Head->y + this-> dirY);
}

void Snake::eat(Fruit* whatever) {
  int size_gain = 1;
  this-> score += 10;
  if (whatever-> type == FRUIT_JAM) {
    size_gain = 3;
    this-> score += 20;
  } else if (whatever-> type == FRUIT_SHIELD) {
    this-> shield = true;
    this-> score += 65;
  }
  whatever-> relocate(this-> Head, this-> Tail);
  for (int i = 0; i < size_gain; i++) {
    this-> Head-> createFragment(this-> Tail-> x, this-> Tail-> y);
    this-> Tail = this-> Tail -> next;
    this-> actualLenght++;
  }
}

void Snake::printEntireSnake() {
  this-> Head -> next-> printAndNext(90*((this->Head->x-this->Head->next->x-1)*(this->Head->x-this->Head->next->x)+this->Head->y-this->Head->next->y));
  this-> Head -> printHeadSkin(90*((dirX-1)*dirX+dirY));
}

bool Snake::hitAWallOrHimself() {
  return this-> Head-> next-> checkCollision(this-> Head-> x, this-> Head->y)
        || this-> Head-> x > GRID_WIDTH || this-> Head-> x < 0
        || this-> Head-> y > GRID_HEIGHT || this-> Head-> y < 0;
}
