#include <SDL2/SDL.h>
#include <iostream>
#include "constants.h"

struct Player: public GameEntity {
  Player();
  void update(Uint32 delta);
};

Player::Player() {
  dstrect.w = 64;
  dstrect.h = 64;

  x = SCREEN_WIDTH / 2;
  y = SCREEN_HEIGHT - dstrect.h;
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
  if (x + dstrect.w > SCREEN_WIDTH) {
    x = SCREEN_WIDTH - dstrect.w;
  }
  GameEntity::update(delta);
}
