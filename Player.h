#include <SDL2/SDL.h>
#include <iostream>
#include "constants.h"

struct GameEntity {
  float x, y, velocity;
  SDL_Surface* bitmap;
  SDL_Rect rect;

  ~GameEntity() {
    SDL_FreeSurface(bitmap);
  };
  virtual void update(Uint32 delta);
};

void GameEntity::update(Uint32 delta) {
  rect.x = (int)x;
  rect.y = (int)y;
}

struct Bullet: public GameEntity {
  Bullet(float x, float y);
  void update(Uint32 delta);
};

Bullet::Bullet(float x, float y) {
  this->x = x;
  this->y = y;

  velocity = 0.4f;

  bitmap = SDL_LoadBMP("res/bullet.bmp");
  if (bitmap == NULL) {
    std::cout << "Unable to load bullet bitmap: " << SDL_GetError() << std::endl;
  }

  rect.w = bitmap->w;
  rect.h = bitmap->h;
}

void Bullet::update(Uint32 delta) {
  y -= delta * velocity;
  GameEntity::update(delta);
}

struct Player: public GameEntity {
  Player();
  void update(Uint32 delta);
};

Player::Player() {
  rect.w = 64;
  rect.h = 64;

  x = SCREEN_WIDTH / 2;
  y = SCREEN_HEIGHT - rect.h;
  velocity = 0;

  bitmap = SDL_LoadBMP("res/player.bmp");
  if (bitmap == NULL) {
    std::cout << "Unable to load player bitmap: " << SDL_GetError() << std::endl;
  }
}

void Player::update(Uint32 delta) {
  x += delta * velocity;
  if (x < 0) {
    x = 0;
  }
  if (x + rect.w > SCREEN_WIDTH) {
    x = SCREEN_WIDTH - rect.w;
  }
  GameEntity::update(delta);
}

struct Alien: public GameEntity {
  Alien(float x, float y);
  void update(Uint32 delta);
};

Alien::Alien(float x, float y) {
  this->x = x;
  this->y = y;

  // velocity = 0.1f;

  bitmap = SDL_LoadBMP("res/alien.bmp");
  if (bitmap == NULL) {
    std::cout << "Unable to load alien bitmap: " << SDL_GetError() << std::endl;
  }

  rect.w = bitmap->w * 2;
  rect.h = bitmap->h * 2;
}

void Alien::update(Uint32 delta) {
  GameEntity::update(delta);
}
